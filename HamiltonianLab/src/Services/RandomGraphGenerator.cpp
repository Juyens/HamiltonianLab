#include "RandomGraphGenerator.h"

namespace HamiltonianLab::Services
{
    RandomGraphGenerator::RandomGraphGenerator(GraphCanvas^ canvas)
        : m_canvas(canvas)
        , m_random(gcnew Random())
    {
    }

    void RandomGraphGenerator::SetRandomSeed(int seed)
    {
        m_random = gcnew Random(seed);
    }

    void RandomGraphGenerator::ClearRandomSeed()
    {
        m_random = gcnew Random();
    }

    void RandomGraphGenerator::Generate(GraphDocument^ doc, int nodeCount)
    {
        if (!doc)
            return;

        nodeCount = ClampNodeCount(nodeCount);
        ClearDocument(doc);

        System::Drawing::Rectangle client = m_canvas ? m_canvas->ClientRectangle : System::Drawing::Rectangle(0, 0, 1280, 720);
        const int margin = 60;

        client = EnsureValidClientArea(client, margin);

        float defaultRadius = doc->Visual ? doc->Visual->DefaultNodeRadius : 24.0f;

        PopulateNodes(doc, nodeCount, client, margin, defaultRadius);

        auto visual = doc->Visual;
        if (!visual || !visual->Nodes)
            return;

        const int minDegree = 2;
        const int maxDegree = Math::Max(minDegree + 1, Math::Min(nodeCount - 1, 6));
        const int minWeight = 1;
        const int maxWeight = 50;

        BuildEdges(doc, minDegree, maxDegree, minWeight, maxWeight);
    }

    void RandomGraphGenerator::ClearDocument(GraphDocument^ doc)
    {
        if (!doc)
            return;

        if (doc->Selection)
            doc->Selection->Clear();

        auto visual = doc->Visual;
        if (!visual || !visual->Nodes)
            return;

        auto logicalIds = gcnew List<int>();
        for each (auto node in visual->Nodes)
        {
            logicalIds->Add(node->LogicalId);
        }

        for each (int id in logicalIds)
        {
            doc->RemoveNodeByLogicalId(id);
        }

        if (visual)
            visual->Clear();

        doc->ClearModified();
    }

    int RandomGraphGenerator::ClampNodeCount(int nodeCount)
    {
        const int minNodes = 8;
        const int maxNodes = 16;

        if (nodeCount < minNodes)
            return minNodes;
        if (nodeCount > maxNodes)
            return maxNodes;

        return nodeCount;
    }

    System::Drawing::Rectangle RandomGraphGenerator::EnsureValidClientArea(System::Drawing::Rectangle clientArea, int margin)
    {
        if (clientArea.Width < margin * 2 || clientArea.Height < margin * 2)
        {
            int width = Math::Max(clientArea.Width, margin * 2);
            int height = Math::Max(clientArea.Height, margin * 2);
            clientArea = System::Drawing::Rectangle(clientArea.X, clientArea.Y, width, height);
        }

        return clientArea;
    }

    void RandomGraphGenerator::PopulateNodes(GraphDocument^ doc, int nodeCount, System::Drawing::Rectangle clientArea, int margin, float defaultRadius)
    {
        if (!doc)
            return;

        float usableWidth = static_cast<float>(Math::Max(clientArea.Width - margin * 2, 1));
        float usableHeight = static_cast<float>(Math::Max(clientArea.Height - margin * 2, 1));

        float circleRadius = (usableWidth < usableHeight ? usableWidth : usableHeight) / 2.0f;
        float minimumRadius = defaultRadius * 3.0f;
        if (circleRadius < minimumRadius)
            circleRadius = minimumRadius;

        float jitterRadius = circleRadius * 0.08f;

        for (int i = 0; i < nodeCount; ++i)
        {
            PointF position = CreateNodePosition(clientArea, circleRadius, i, nodeCount, jitterRadius);
            float minX = static_cast<float>(clientArea.Left + margin);
            float maxX = static_cast<float>(clientArea.Right - margin);
            float minY = static_cast<float>(clientArea.Top + margin);
            float maxY = static_cast<float>(clientArea.Bottom - margin);

            float clampedX = position.X;
            if (clampedX < minX)
                clampedX = minX;
            else if (clampedX > maxX)
                clampedX = maxX;

            float clampedY = position.Y;
            if (clampedY < minY)
                clampedY = minY;
            else if (clampedY > maxY)
                clampedY = maxY;

            doc->AddNode(PointF(clampedX, clampedY), defaultRadius, nullptr);
        }
    }

    void RandomGraphGenerator::BuildEdges(GraphDocument^ doc, int minDegree, int maxDegree, int minWeight, int maxWeight)
    {
        if (!doc || !doc->Visual || !doc->Visual->Nodes)
            return;

        auto nodes = doc->Visual->Nodes;
        if (!nodes || nodes->Count < 2)
            return;

        auto logicalIds = gcnew List<int>(nodes->Count);
        for each (auto node in nodes)
        {
            logicalIds->Add(node->LogicalId);
        }

        int count = logicalIds->Count;
        auto adjacency = gcnew cli::array<bool, 2>(count, count);
        auto degrees = gcnew cli::array<int>(count);

        auto order = gcnew List<int>(count);
        for (int i = 0; i < count; ++i)
            order->Add(i);
        ShuffleList(order);

        int edgeCount = 0;
        for (int i = 1; i < order->Count; ++i)
        {
            int current = order[i];

            auto candidates = gcnew List<int>();
            for (int j = 0; j < i; ++j)
            {
                int candidate = order[j];
                if (degrees[candidate] < maxDegree)
                    candidates->Add(candidate);
            }

            if (candidates->Count == 0)
            {
                for (int j = 0; j < i; ++j)
                    candidates->Add(order[j]);
            }

            ShuffleList(candidates);

            for each (int candidate in candidates)
            {
                if (TryAddEdge(doc, logicalIds, adjacency, degrees, current, candidate, maxDegree, minWeight, maxWeight))
                {
                    ++edgeCount;
                    break;
                }
            }
        }

        auto candidatePairs = gcnew List<Tuple<int, int>^>();
        for (int i = 0; i < count; ++i)
        {
            for (int j = i + 1; j < count; ++j)
            {
                candidatePairs->Add(Tuple::Create(i, j));
            }
        }

        ShuffleList(candidatePairs);

        while (HasNodeBelowMinimumDegree(degrees, minDegree))
        {
            bool addedEdge = false;
            for each (auto pair in candidatePairs)
            {
                int u = pair->Item1;
                int v = pair->Item2;

                if (degrees[u] >= maxDegree || degrees[v] >= maxDegree)
                    continue;

                if (degrees[u] >= minDegree && degrees[v] >= minDegree)
                    continue;

                if (TryAddEdge(doc, logicalIds, adjacency, degrees, u, v, maxDegree, minWeight, maxWeight))
                {
                    ++edgeCount;
                    addedEdge = true;
                }

                if (!HasNodeBelowMinimumDegree(degrees, minDegree))
                    break;
            }

            if (!addedEdge)
                break;

            ShuffleList(candidatePairs);
        }

        int minEdges = (minDegree * count + 1) / 2;
        int maxEdges = (maxDegree * count) / 2;
        int currentEdges = edgeCount;
        int additionalCapacity = Math::Max(0, maxEdges - currentEdges);
        int additionalTarget = additionalCapacity > 0 ? m_random->Next(0, Math::Max(1, additionalCapacity / 2 + 1)) : 0;
        int desiredEdges = Math::Min(maxEdges, currentEdges + additionalTarget);

        ShuffleList(candidatePairs);

        for each (auto pair in candidatePairs)
        {
            if (currentEdges >= desiredEdges)
                break;

            int u = pair->Item1;
            int v = pair->Item2;

            if (degrees[u] >= maxDegree || degrees[v] >= maxDegree)
                continue;

            if (TryAddEdge(doc, logicalIds, adjacency, degrees, u, v, maxDegree, minWeight, maxWeight))
            {
                ++currentEdges;
            }
        }
    }

    PointF RandomGraphGenerator::CreateNodePosition(System::Drawing::Rectangle clientArea, float circleRadius, int index, int totalCount, float jitterRadius)
    {
        double baseAngle = (index % totalCount) * (2.0 * Math::PI / totalCount);

        double angleOffset = (m_random->NextDouble() - 0.5) * (Math::PI / totalCount);
        double radiusOffset = (m_random->NextDouble() - 0.5) * 2.0 * jitterRadius;

        double angle = baseAngle + angleOffset;
        double radius = Math::Max(0.0, circleRadius + radiusOffset);

        float centerX = static_cast<float>(clientArea.Left) + static_cast<float>(clientArea.Width) / 2.0f;
        float centerY = static_cast<float>(clientArea.Top) + static_cast<float>(clientArea.Height) / 2.0f;

        float x = static_cast<float>(centerX + static_cast<float>(radius * Math::Cos(angle)));
        float y = static_cast<float>(centerY + static_cast<float>(radius * Math::Sin(angle)));

        return PointF(x, y);
    }

    bool RandomGraphGenerator::TryAddEdge(GraphDocument^ doc, List<int>^ logicalIds, cli::array<bool, 2>^ adjacency, cli::array<int>^ degrees, int uIndex, int vIndex, int maxDegree, int minWeight, int maxWeight)
    {
        if (!doc || !logicalIds || !adjacency || !degrees)
            return false;

        if (uIndex == vIndex)
            return false;

        if (uIndex < 0 || uIndex >= logicalIds->Count || vIndex < 0 || vIndex >= logicalIds->Count)
            return false;

        if (adjacency[uIndex, vIndex])
            return false;

        if (degrees[uIndex] >= maxDegree || degrees[vIndex] >= maxDegree)
            return false;

        int weight = m_random->Next(minWeight, maxWeight + 1);

        int uLogicalId = logicalIds[uIndex];
        int vLogicalId = logicalIds[vIndex];

        doc->AddEdgeByLogicalIds(uLogicalId, vLogicalId, weight);

        adjacency[uIndex, vIndex] = true;
        adjacency[vIndex, uIndex] = true;
        degrees[uIndex] += 1;
        degrees[vIndex] += 1;

        return true;
    }

    bool RandomGraphGenerator::HasNodeBelowMinimumDegree(cli::array<int>^ degrees, int minDegree)
    {
        if (!degrees)
            return false;

        for (int i = 0; i < degrees->Length; ++i)
        {
            if (degrees[i] < minDegree)
                return true;
        }

        return false;
    }

    generic <typename T>
    void RandomGraphGenerator::ShuffleList(List<T>^ list)
    {
        if (!list || list->Count <= 1)
            return;

        for (int i = list->Count - 1; i > 0; --i)
        {
            int j = m_random->Next(0, i + 1);
            T temp = list[i];
            list[i] = list[j];
            list[j] = temp;
        }
    }
}
