#pragma once

#include <Models/GraphDocument.fwd.h>
#include <Models/Visual/VisualNode.h>
#include <Models/Visual/VisualEdge.h>

namespace HamiltonianLab::Models
{
    using namespace System::Collections::Generic;
    using namespace System::Drawing;
    using namespace HamiltonianLab::Models::Visual;

    public ref class Selection
    {
    public:
        Selection(GraphDocument^ document);

        property GraphDocument^ Document
        {
            GraphDocument^ get();
        }

        void Clear();
        void Add(VisualNode^ node);
        void Add(VisualEdge^ edge);
        void Toggle(VisualNode^ node);
        void Toggle(VisualEdge^ edge);
        void Remove(VisualNode^ node);
        void Remove(VisualEdge^ edge);
        void RemoveNodeByLogicalId(int logicalId);
        void RemoveEdgeByLogicalId(int logicalId);
        bool IsSelected(VisualNode^ node);
        bool IsSelected(VisualEdge^ edge);
        void DeleteSelection();

        void Render(Graphics^ g);

    private:
        void MarkNode(VisualNode^ node, bool selected);
        void MarkEdge(VisualEdge^ edge, bool selected);
        void TrimAtNodeEdges(System::Drawing::PointF% a,
            System::Drawing::PointF% b,
            float ra, float rb, float pad);

    private:
        GraphDocument^ m_document;
        HashSet<int>^ m_selectedNodes;
        HashSet<int>^ m_selectedEdges;
    };
}
