#include "Selection.h"

#include <Models/GraphDocument.h>

namespace HamiltonianLab::Models
{
    Selection::Selection(GraphDocument^ document)
    {
        m_document = document;
        m_selectedNodes = gcnew HashSet<int>();
        m_selectedEdges = gcnew HashSet<int>();
    }

    GraphDocument^ Selection::Document::get()
    {
        return m_document;
    }

    void Selection::Clear()
    {
        if (!m_document) return;

        for each (int nodeId in m_selectedNodes)
        {
            auto node = m_document->FindNodeByLogicalId(nodeId);
            if (node) MarkNode(node, false);
        }

        for each (int edgeId in m_selectedEdges)
        {
            auto edge = m_document->Visual->FindEdgeByLogicalId(edgeId);
            if (edge) MarkEdge(edge, false);
        }

        m_selectedNodes->Clear();
        m_selectedEdges->Clear();
    }

    void Selection::Add(VisualNode^ node)
    {
        if (!node) return;
        if (m_selectedNodes->Add(node->LogicalId))
        {
            MarkNode(node, true);
        }
    }

    void Selection::Add(VisualEdge^ edge)
    {
        if (!edge) return;
        if (m_selectedEdges->Add(edge->LogicalId))
        {
            MarkEdge(edge, true);
        }
    }

    void Selection::Toggle(VisualNode^ node)
    {
        if (!node) return;
        if (m_selectedNodes->Contains(node->LogicalId))
        {
            Remove(node);
        }
        else
        {
            Add(node);
        }
    }

    void Selection::Toggle(VisualEdge^ edge)
    {
        if (!edge) return;
        if (m_selectedEdges->Contains(edge->LogicalId))
        {
            Remove(edge);
        }
        else
        {
            Add(edge);
        }
    }

    void Selection::Remove(VisualNode^ node)
    {
        if (!node) return;
        if (m_selectedNodes->Remove(node->LogicalId))
        {
            MarkNode(node, false);
        }
    }

    void Selection::Remove(VisualEdge^ edge)
    {
        if (!edge) return;
        if (m_selectedEdges->Remove(edge->LogicalId))
        {
            MarkEdge(edge, false);
        }
    }

    void Selection::RemoveNodeByLogicalId(int logicalId)
    {
        if (!m_selectedNodes->Contains(logicalId))
            return;

        auto node = m_document ? m_document->FindNodeByLogicalId(logicalId) : nullptr;
        if (node) MarkNode(node, false);
        m_selectedNodes->Remove(logicalId);
    }

    void Selection::RemoveEdgeByLogicalId(int logicalId)
    {
        if (!m_selectedEdges->Contains(logicalId))
            return;

        auto edge = m_document && m_document->Visual ? m_document->Visual->FindEdgeByLogicalId(logicalId) : nullptr;
        if (edge) MarkEdge(edge, false);
        m_selectedEdges->Remove(logicalId);
    }

    bool Selection::IsSelected(VisualNode^ node)
    {
        if (!node) return false;
        return m_selectedNodes->Contains(node->LogicalId);
    }

    bool Selection::IsSelected(VisualEdge^ edge)
    {
        if (!edge) return false;
        return m_selectedEdges->Contains(edge->LogicalId);
    }

    void Selection::DeleteSelection()
    {
        if (!m_document) return;

        auto edges = gcnew System::Collections::Generic::List<int>(m_selectedEdges);
        auto nodes = gcnew System::Collections::Generic::List<int>(m_selectedNodes);

        for each (int edgeId in edges)
        {
            m_document->RemoveEdgeByLogicalId(edgeId);
        }

        for each (int nodeId in nodes)
        {
            m_document->RemoveNodeByLogicalId(nodeId);
        }
    }

    void Selection::Render(Graphics^ g)
    {
        if (!g || !m_document) return;

        auto highlight = gcnew Pen(Color::CornflowerBlue, 1.0f);
        highlight->DashStyle = Drawing2D::DashStyle::Dash;

        auto edgeHighlight = gcnew Pen(Color::CornflowerBlue, 2.0f);
        edgeHighlight->DashStyle = Drawing2D::DashStyle::Dash;
        edgeHighlight->StartCap = Drawing2D::LineCap::Round;
        edgeHighlight->EndCap = Drawing2D::LineCap::Round;
        edgeHighlight->LineJoin = Drawing2D::LineJoin::Round;

        if (m_document->Visual)
        {
            auto visual = m_document->Visual;
            for each (int edgeId in m_selectedEdges)
            {
                auto edge = visual->FindEdgeByLogicalId(edgeId);
                if (!edge) continue;
                auto from = visual->FindNodeByLogicalId(edge->FromLogicalId);
                auto to = visual->FindNodeByLogicalId(edge->ToLogicalId);
                if (!from || !to) continue;

                System::Drawing::PointF p1 = from->Position;
                System::Drawing::PointF p2 = to->Position;
                TrimAtNodeEdges(p1, p2, from->Radius, to->Radius, 4.0f);

                g->DrawLine(edgeHighlight, p1, p2);
            }
        }

        for each (int nodeId in m_selectedNodes)
        {
            auto node = m_document->FindNodeByLogicalId(nodeId);
            if (!node) continue;
            auto bounds = node->GetBounds(4.0f);
            g->DrawEllipse(highlight, bounds);
        }

        delete edgeHighlight;
        delete highlight;
    }

    void Selection::MarkNode(VisualNode^ node, bool selected)
    {
        node->Selected = selected;
    }

    void Selection::MarkEdge(VisualEdge^ edge, bool selected)
    {
        edge->Selected = selected;
    }

    void Selection::TrimAtNodeEdges(System::Drawing::PointF% a, System::Drawing::PointF% b, float ra, float rb, float pad)
    {
        auto dx = b.X - a.X;
        auto dy = b.Y - a.Y;
        auto len = std::sqrt(dx * dx + dy * dy);
        if (len <= 1e-3f) return;

        auto ux = dx / len;
        auto uy = dy / len;

        a = System::Drawing::PointF(a.X + ux * (ra + pad), a.Y + uy * (ra + pad));
        b = System::Drawing::PointF(b.X - ux * (rb + pad), b.Y - uy * (rb + pad));
    }
}
