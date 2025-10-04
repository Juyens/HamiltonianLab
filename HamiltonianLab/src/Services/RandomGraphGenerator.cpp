#include "RandomGraphGenerator.h"

namespace HamiltonianLab::Services
{
    RandomGraphGenerator::RandomGraphGenerator(GraphCanvas^ canvas)
        : m_canvas(canvas)
        , m_random(gcnew Random())
    {
    }

    void RandomGraphGenerator::Generate(GraphDocument^ doc, int nodeCount)
    {
        if (!doc || nodeCount <= 0)
            return;

        ClearDocument(doc);

        System::Drawing::Rectangle client = m_canvas ? m_canvas->ClientRectangle : System::Drawing::Rectangle(0, 0, 1280, 720);
        const int margin = 60;

        if (client.Width < margin * 2 || client.Height < margin * 2)
        {
            client = System::Drawing::Rectangle(client.X, client.Y, Math::Max(client.Width, margin * 2), Math::Max(client.Height, margin * 2));
        }

        float defaultRadius = doc->Visual ? doc->Visual->DefaultNodeRadius : 24.0f;

        for (int i = 0; i < nodeCount; ++i)
        {
            PointF position = CreateRandomPoint(client, margin);
            doc->AddNode(position, defaultRadius, nullptr);
        }

        auto visual = doc->Visual;
        if (!visual || !visual->Nodes)
            return;

        auto nodes = visual->Nodes;
        const double edgeProbability = 0.30;

        for (int i = 0; i < nodes->Count; ++i)
        {
            for (int j = i + 1; j < nodes->Count; ++j)
            {
                if (m_random->NextDouble() > edgeProbability)
                    continue;

                int uLogicalId = nodes[i]->LogicalId;
                int vLogicalId = nodes[j]->LogicalId;

                if (doc->Core->HasEdge(uLogicalId, vLogicalId))
                    continue;

                int weight = m_random->Next(1, 21);
                doc->AddEdgeByLogicalIds(uLogicalId, vLogicalId, weight);
            }
        }
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

    PointF RandomGraphGenerator::CreateRandomPoint(System::Drawing::Rectangle clientArea, int margin)
    {
        double rx = m_random->NextDouble();
        double ry = m_random->NextDouble();

        float minX = static_cast<float>(clientArea.Left + margin);
        float maxX = static_cast<float>(clientArea.Right - margin);
        float minY = static_cast<float>(clientArea.Top + margin);
        float maxY = static_cast<float>(clientArea.Bottom - margin);

        if (maxX <= minX)
            maxX = minX + 1.0f;
        if (maxY <= minY)
            maxY = minY + 1.0f;

        float x = minX + static_cast<float>(rx * (maxX - minX));
        float y = minY + static_cast<float>(ry * (maxY - minY));

        return PointF(x, y);
    }
}
