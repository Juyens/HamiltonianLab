#pragma once

#include <Models/GraphDocument.h>
#include <Renderers/GraphCanvas.h>

namespace HamiltonianLab::Services
{
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Drawing;
    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Renderers;

    public ref class RandomGraphGenerator
    {
    public:
        RandomGraphGenerator(GraphCanvas^ canvas);

        void Generate(GraphDocument^ doc, int nodeCount);
        void SetRandomSeed(int seed);
        void ClearRandomSeed();

    private:
        void ClearDocument(GraphDocument^ doc);
        int ClampNodeCount(int nodeCount);
        System::Drawing::Rectangle EnsureValidClientArea(System::Drawing::Rectangle clientArea, int margin);
        void PopulateNodes(GraphDocument^ doc, int nodeCount, System::Drawing::Rectangle clientArea, int margin, float defaultRadius);
        void BuildEdges(GraphDocument^ doc, int minDegree, int maxDegree, int minWeight, int maxWeight);
        PointF CreateNodePosition(System::Drawing::Rectangle clientArea, float circleRadius, int index, int totalCount, float jitterRadius);
        bool TryAddEdge(GraphDocument^ doc, List<int>^ logicalIds, cli::array<bool, 2>^ adjacency, cli::array<int>^ degrees, int uIndex, int vIndex, int maxDegree, int minWeight, int maxWeight);
        bool HasNodeBelowMinimumDegree(cli::array<int>^ degrees, int minDegree);

        generic <typename T>
        void ShuffleList(List<T>^ list);

    private:
        GraphCanvas^ m_canvas;
        System::Random^ m_random;
    };
}
