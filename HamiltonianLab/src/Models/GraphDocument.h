#pragma once

#include <Models/Visual/VisualGraph.h>
#include <Models/Selection/Selection.h>
#include <Interop/ManagedGraph.h>

namespace HamiltonianLab::Models
{
    using namespace HamiltonianLab::Models::Visual;
    using namespace HamiltonianLab::Models::Logical;
    using namespace HamiltonianLab::Interop;

    public ref class GraphDocument
    {
    public:
        GraphDocument();

        property ManagedGraph^ Core
        {
            ManagedGraph^ get()
            {
                return m_core;
            }
        }

        property VisualGraph^ Visual
        {
            VisualGraph^ get()
            {
                return m_visual;
            }
        }

        property Selection^ Selection
        {
            HamiltonianLab::Models::Selection^ get()
            {
                return m_selection;
            }
        }

        property bool IsModified
        {
            bool get()
            {
                return m_modified;
            }
        }

        VisualNode^ AddNode(PointF position, float radius, System::String^ label);
        bool RemoveNodeByLogicalId(int logicalId);

        VisualEdge^ AddEdgeByLogicalIds(int uLogicalId, int vLogicalId, double w);
        bool RemoveEdgeByLogicalId(int edgeLogicalId);
        void SetWeightByLogicalIds(int uLogicalId, int vLogicalId, double w);
        double GetWeightByLogicalIds(int uLogicalId, int vLogicalId);

        void MarkModified();
        void ClearModified();

        int NodeCount();
        int EdgeCount();
        bool IsComplete();
        bool ValidateSimple();

        VisualNode^ GetNodeAt(PointF p);
        VisualEdge^ GetEdgeAt(PointF p);
        VisualNode^ FindNodeByLogicalId(int logicalId);

    private:
        ManagedGraph^ m_core;
        VisualGraph^ m_visual;
        HamiltonianLab::Models::Selection^ m_selection;
        bool m_modified;
    };
}