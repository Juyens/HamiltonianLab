#include "VisualGraph.h"

namespace HamiltonianLab::Models::Visual
{
    VisualGraph::VisualGraph()
    {
        Nodes = gcnew List<VisualNode^>();
        Edges = gcnew List<VisualEdge^>();
        DefaultNodeRadius = 20.0f;
        HitTolerance = 8.0f;
        m_labeler = gcnew NodeLabelAllocator();
    }

    VisualNode^ VisualGraph::AddNode(int logicalId, PointF position, float radius)
    {
        auto n = gcnew VisualNode(
            logicalId,
            position,
            radius <= 0 ? DefaultNodeRadius : radius
        );

        n->Label = m_labeler->Next();

        Nodes->Add(n);
        return n;
    }

    bool VisualGraph::RemoveNodeByLogicalId(int logicalId)
    {
        for (int i = Edges->Count - 1; i >= 0; --i)
        {
            if (Edges[i]->FromLogicalId == logicalId || Edges[i]->ToLogicalId == logicalId)
                Edges->RemoveAt(i);
        }

        for (int i = 0; i < Nodes->Count; ++i)
        {
            if (Nodes[i]->LogicalId == logicalId)
            {
                Nodes->RemoveAt(i);
                if (Nodes->Count == 0 && Edges->Count == 0)
                    m_labeler->Reset();
                return true;
            }
        }
        return false;
    }

    VisualNode^ VisualGraph::FindNodeByLogicalId(int logicalId)
    {
        for each (auto n in Nodes)
            if (n->LogicalId == logicalId) return n;
        return nullptr;
    }

    VisualNode^ VisualGraph::GetNodeAt(PointF p)
    {
        for (int i = Nodes->Count - 1; i >= 0; --i)
        {
            if (Nodes[i]->HitTest(p, HitTolerance))
                return Nodes[i];
        }
        return nullptr;
    }

    VisualEdge^ VisualGraph::AddEdge(int logicalEdgeId, int fromLogicalId, int toLogicalId, double w)
    {
        auto e = gcnew VisualEdge(logicalEdgeId, fromLogicalId, toLogicalId);
        e->Weight = w;
        e->Label = w.ToString(System::Globalization::CultureInfo::InvariantCulture);
        Edges->Add(e);
        return e;
    }

    bool VisualGraph::RemoveEdgeByLogicalId(int logicalEdgeId)
    {
        for (int i = 0; i < Edges->Count; ++i)
        {
            if (Edges[i]->LogicalId == logicalEdgeId)
            {
                Edges->RemoveAt(i);
                if (Nodes->Count == 0 && Edges->Count == 0)
                    m_labeler->Reset();
                return true;
            }
        }
        return false;
    }

    VisualEdge^ VisualGraph::FindEdgeByLogicalId(int logicalEdgeId)
    {
        for each (auto e in Edges)
            if (e->LogicalId == logicalEdgeId) return e;
        return nullptr;
    }

    VisualEdge^ VisualGraph::FindEdgeBetween(int uLogicalId, int vLogicalId)
    {
        for each (auto e in Edges)
        {
            if (!e)
                continue;

            bool direct = (e->FromLogicalId == uLogicalId && e->ToLogicalId == vLogicalId);
            bool reverse = (e->FromLogicalId == vLogicalId && e->ToLogicalId == uLogicalId);
            if (direct || reverse)
                return e;
        }

        return nullptr;
    }

    VisualEdge^ VisualGraph::GetEdgeAt(PointF p)
    {
        for (int i = Edges->Count - 1; i >= 0; --i)
        {
            auto e = Edges[i];
            auto a = FindNodeByLogicalId(e->FromLogicalId);
            auto b = FindNodeByLogicalId(e->ToLogicalId);
            if (!a || !b) continue;

            if (VisualEdge::HitTestSegment(p, a->Position, b->Position, HitTolerance))
                return e;
        }
        return nullptr;
    }

    IEnumerable<VisualNode^>^ VisualGraph::QueryNodesIn(RectangleF rect)
    {
        auto result = gcnew List<VisualNode^>();
        for each (auto n in Nodes)
            if (rect.IntersectsWith(n->GetBounds(0.0f))) result->Add(n);
        return result;
    }

    void VisualGraph::RecomputeLabelCounterFromExisting()
    {
        auto labels = gcnew List<String^>();
        for each (auto n in Nodes)
        {
            labels->Add(n->Label);
            m_labeler->Recompute(labels);
        }
    }

    void VisualGraph::Clear()
    {
        Nodes->Clear();
        Edges->Clear();
        m_labeler->Reset();
    }

    void VisualGraph::ClearEdgeHighlights()
    {
        for each (auto edge in Edges)
        {
            if (!edge)
                continue;

            edge->Highlighted = false;
        }
    }
}
