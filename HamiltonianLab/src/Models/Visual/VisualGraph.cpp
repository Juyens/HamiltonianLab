#include "VisualGraph.h"

namespace HamiltonianLab::Models::Visual
{
    VisualGraph::VisualGraph()
    {
        Nodes = gcnew List<VisualNode^>();
        Edges = gcnew List<VisualEdge^>();
        DefaultNodeRadius = 20.0f;
        HitTolerance = 8.0f;
        m_labelCounter = 0;
    }

    VisualNode^ VisualGraph::AddNode(int logicalId, PointF position, float radius)
    {
        auto n = gcnew VisualNode(
            logicalId,
            position,
            radius <= 0 ? DefaultNodeRadius : radius
        );

        n->Label = GenerateNextAvailableLabel();

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

    VisualEdge^ VisualGraph::AddEdge(int logicalEdgeId, int fromLogicalId, int toLogicalId, float w)
    {
        auto e = gcnew VisualEdge(logicalEdgeId, fromLogicalId, toLogicalId);
        e->Label = w.ToString();
;       Edges->Add(e);
        return e;
    }

    bool VisualGraph::RemoveEdgeByLogicalId(int logicalEdgeId)
    {
        for (int i = 0; i < Edges->Count; ++i)
        {
            if (Edges[i]->LogicalId == logicalEdgeId)
            {
                Edges->RemoveAt(i);
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
        int maxIndex = -1;
        for each (auto n in Nodes)
        {
            int idx = TryParseLabelToIndex(n->Label);
            if (idx > maxIndex) maxIndex = idx;
        }

        m_labelCounter = (maxIndex < 0) ? 0 : (maxIndex + 1);
    }

    void VisualGraph::Clear()
    {
        Nodes->Clear();
        Edges->Clear();
        m_labelCounter = 0;
    }

    String^ VisualGraph::GenerateLabel(int index)
    {
        StringBuilder^ sb = gcnew StringBuilder();
        while (index >= 0)
        {
            sb->Insert(0, wchar_t('A' + (index % 26)));
            index = (index / 26) - 1;
        }
        return sb->ToString();
    }

    int VisualGraph::TryParseLabelToIndex(String^ label)
    {
        if (String::IsNullOrEmpty(label)) return -1;
        int value = 0;
        for (int i = 0; i < label->Length; ++i)
        {
            wchar_t c = label[i];
            if (c < 'A' || c > 'Z') return -1;
            value = value * 26 + (c - 'A' + 1);
        }
        return value - 1;
    }

    String^ VisualGraph::GenerateNextAvailableLabel()
    {
        while (true)
        {
            String^ candidate = GenerateLabel(m_labelCounter);
            bool exists = false;
            for each (auto n in Nodes)
            {
                if (n->Label != nullptr && n->Label->Equals(candidate))
                {
                    exists = true; break;
                }
            }
            m_labelCounter++;

            if (!exists) 
                return candidate;
        }
    }
}
