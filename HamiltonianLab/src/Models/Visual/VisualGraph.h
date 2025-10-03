#pragma once

#include <Models/Visual/VisualNode.h>
#include <Models/Visual/VisualEdge.h>

namespace HamiltonianLab::Models::Visual
{
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Drawing;
    using namespace System::Text;

    public ref class VisualGraph
    {
    public:
        property List<VisualNode^>^ Nodes;
        property List<VisualEdge^>^ Edges;

        property float DefaultNodeRadius;
        property float HitTolerance;

    public:
        VisualGraph();
        VisualNode^ AddNode(int logicalId, PointF position, float radius);
        bool RemoveNodeByLogicalId(int logicalId);
        VisualNode^ FindNodeByLogicalId(int logicalId);
        VisualNode^ GetNodeAt(PointF p);

        VisualEdge^ AddEdge(int logicalEdgeId, int fromLogicalId, int toLogicalId, float w);
        bool RemoveEdgeByLogicalId(int logicalEdgeId);
        VisualEdge^ FindEdgeByLogicalId(int logicalEdgeId);
        VisualEdge^ GetEdgeAt(PointF p);

        System::Collections::Generic::IEnumerable<VisualNode^>^ QueryNodesIn(RectangleF rect);

        void RecomputeLabelCounterFromExisting();
        void Clear();

    private:
        String^ GenerateLabel(int index);
        int TryParseLabelToIndex(String^ label);
        String^ GenerateNextAvailableLabel();

    private:
        int m_labelCounter;
    };
}
