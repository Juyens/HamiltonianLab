#include "GraphDocument.h"

namespace HamiltonianLab::Models
{
    GraphDocument::GraphDocument()
    {
        m_core = gcnew ManagedGraph();
        m_visual = gcnew VisualGraph();
        m_selection = gcnew HamiltonianLab::Models::Selection(this);
        m_modified = false;
        m_structureChanged = nullptr;
    }

    void GraphDocument::StructureChanged::add(EventHandler^ h)
    {
        m_structureChanged = static_cast<EventHandler^>(Delegate::Combine(m_structureChanged, h));
    }

    void GraphDocument::StructureChanged::remove(EventHandler^ h)
    {
        m_structureChanged = static_cast<EventHandler^>(Delegate::Remove(m_structureChanged, h));
    }

    VisualNode^ GraphDocument::AddNode(PointF position, float radius, System::String^ label)
    {
        int logicalId = m_core->AddNode();
        auto n = m_visual->AddNode(logicalId, position, radius);
        if (label != nullptr)
        {
            n->Label = label;
            m_visual->RecomputeLabelCounterFromExisting();
        }
        MarkModified();
        RaiseStructureChanged();
        return n;
    }

    bool GraphDocument::RemoveNodeByLogicalId(int logicalId)
    {
        bool ok = m_core->RemoveNode(logicalId);
        if (!ok) return false;

        auto edgesToClear = gcnew System::Collections::Generic::List<int>();
        for each (auto edge in m_visual->Edges)
        {
            if (edge->FromLogicalId == logicalId || edge->ToLogicalId == logicalId)
                edgesToClear->Add(edge->LogicalId);
        }

        for each (int edgeId in edgesToClear)
            m_selection->RemoveEdgeByLogicalId(edgeId);

        m_selection->RemoveNodeByLogicalId(logicalId);
        bool removed = m_visual->RemoveNodeByLogicalId(logicalId);
        if (removed)
        {
            MarkModified();
            RaiseStructureChanged();
        }
        return removed;
    }

    VisualEdge^ GraphDocument::AddEdgeByLogicalIds(int uLogicalId, int vLogicalId, double w)
    {
        int edgeId = m_core->AddEdge(uLogicalId, vLogicalId, w);
        MarkModified();
        auto edge = m_visual->AddEdge(edgeId, uLogicalId, vLogicalId, w);
        RaiseStructureChanged();
        return edge;
    }

    bool GraphDocument::RemoveEdgeByLogicalId(int edgeLogicalId)
    {
        bool ok = m_core->RemoveEdgeById(edgeLogicalId);
        if (!ok) return false;

        m_selection->RemoveEdgeByLogicalId(edgeLogicalId);
        MarkModified();
        bool removed = m_visual->RemoveEdgeByLogicalId(edgeLogicalId);
        if (removed)
            RaiseStructureChanged();
        return removed;
    }

    void GraphDocument::SetWeightByLogicalIds(int uLogicalId, int vLogicalId, double w)
    {
        m_core->SetWeight(uLogicalId, vLogicalId, w);
        MarkModified();
    }

    double GraphDocument::GetWeightByLogicalIds(int uLogicalId, int vLogicalId)
    {
        return m_core->GetWeight(uLogicalId, vLogicalId);
    }

    void GraphDocument::Clear()
    {
        if (m_selection)
            m_selection->Clear();

        if (m_visual)
            m_visual->Clear();

        if (m_core)
            m_core->Clear();

        RaiseStructureChanged();
    }

    void GraphDocument::ClearEdgeHighlights()
    {
        if (m_visual)
            m_visual->ClearEdgeHighlights();
    }

    void GraphDocument::MarkModified()
    {
        m_modified = true;
    }

    void GraphDocument::ClearModified()
    {
        m_modified = false;
    }

    int GraphDocument::NodeCount()
    {
        return m_core->NodeCount();
    }

    int GraphDocument::EdgeCount()
    {
        return m_core->EdgeCount();
    }

    bool GraphDocument::IsComplete()
    {
        return m_core->IsComplete();
    }

    bool GraphDocument::ValidateSimple()
    {
        return m_core->ValidateSimple();
    }

    VisualNode^ GraphDocument::GetNodeAt(PointF p)
    {
        return m_visual->GetNodeAt(p);
    }

    VisualEdge^ GraphDocument::GetEdgeAt(PointF p)
    {
        return m_visual->GetEdgeAt(p);
    }

    VisualNode^ GraphDocument::FindNodeByLogicalId(int logicalId)
    {
        return m_visual->FindNodeByLogicalId(logicalId);
    }

    void GraphDocument::RaiseStructureChanged()
    {
        EventHandler^ handler = m_structureChanged;

        if (handler != nullptr)
            handler(this, EventArgs::Empty);
    }
}