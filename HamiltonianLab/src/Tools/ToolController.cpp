#include "ToolController.h"

namespace HamiltonianLab
{
    ToolController::ToolController(GraphDocument^ document, Control^ hostControl)
        : m_document(document)
        , m_tools(gcnew Dictionary<ToolMode, HamiltonianLab::ITool^>())
        , m_activeTool(nullptr)
        , m_mode(ToolMode::None)
        , m_hoveredNode(nullptr)
        , m_hoveredEdge(nullptr)
        , m_invalidate(nullptr)
    {
        m_tools->Add(ToolMode::Select, gcnew SelectTool(m_document));
        m_tools->Add(ToolMode::AddNode, gcnew AddNodeTool(m_document));
        m_tools->Add(ToolMode::AddEdge, gcnew AddEdgeTool(m_document));
        m_tools->Add(ToolMode::Delete, gcnew DeleteTool(m_document));
        m_tools->Add(ToolMode::SetWeight, gcnew SetWeightTool(m_document, this, hostControl));

        SetToolMode(ToolMode::Select);
    }

    ToolController::~ToolController()
    {
        if (m_activeTool)
        {
            m_activeTool->OnLeave();
            m_activeTool = nullptr;
        }

        if (m_tools != nullptr)
        {
            for each (ITool ^ tool in m_tools->Values)
            {
                delete tool;
            }
            m_tools->Clear();
            m_tools = nullptr;
        }

        m_document = nullptr;
    }

    void ToolController::SetInvalidateCallback(Action<System::Drawing::Rectangle>^ callback)
    {
        m_invalidate = callback;
    }

    bool ToolController::SetToolMode(ToolMode mode)
    {
        if (m_mode == mode)
            return false;

        if (mode == ToolMode::Delete)
        {
            if (m_document && m_document->Selection)
            {
                m_document->Selection->DeleteSelection();
                return true;
            }
            return false;
        }

        if (!m_tools || !m_tools->ContainsKey(mode))
            return false;

        if (m_activeTool)
            m_activeTool->OnLeave();

        m_mode = mode;
        m_activeTool = m_tools[mode];

        if (m_activeTool)
            m_activeTool->OnEnter();

        return true;
    }

    bool ToolController::HandleMouseDown(MouseEventArgs^ e)
    {
        if (!m_activeTool)
            return false;

        m_activeTool->OnMouseDown(e);
        ConsumeActiveToolRepaint();
        return true;
    }

    bool ToolController::HandleMouseMove(MouseEventArgs^ e)
    {
        if (!m_document)
            return false;

        PointF p((float)e->X, (float)e->Y);
        auto node = m_document->GetNodeAt(p);
        VisualEdge^ edge = nullptr;
        if (!node)
            edge = m_document->GetEdgeAt(p);

        UpdateHovered(node, edge);

        if (!m_activeTool)
            return false;

        m_activeTool->OnMouseMove(e);
        ConsumeActiveToolRepaint();
        return true;
    }

    bool ToolController::HandleMouseUp(MouseEventArgs^ e)
    {
        if (!m_activeTool)
            return false;

        m_activeTool->OnMouseUp(e);
        ConsumeActiveToolRepaint();
        return true;
    }

    bool ToolController::HandleMouseLeave()
    {
        UpdateHovered(nullptr, nullptr);
        return false;
    }

    bool ToolController::HandleKeyDown(KeyEventArgs^ e)
    {
        if (!m_activeTool)
            return false;

        m_activeTool->OnKeyDown(e);
        return ConsumeActiveToolRepaint();
    }

    void ToolController::RequestInvalidate(HamiltonianLab::Models::Visual::VisualEdge^ edge)
    {
        InvalidateVisual(edge);
    }

    bool ToolController::ConsumeActiveToolRepaint()
    {
        if (!m_activeTool)
            return false;

        return m_activeTool->ConsumeRepaintRequest();
    }

    void ToolController::UpdateHovered(VisualNode^ node, VisualEdge^ edge)
    {
        if (node != nullptr)
            edge = nullptr;

        if (m_hoveredNode == node && m_hoveredEdge == edge)
            return;

        auto previousNode = m_hoveredNode;
        auto previousEdge = m_hoveredEdge;

        m_hoveredNode = node;
        m_hoveredEdge = edge;

        if (previousNode && previousNode != node)
        {
            previousNode->Hovered = false;
            InvalidateVisual(previousNode);
        }

        if (previousEdge && previousEdge != edge)
        {
            previousEdge->Hovered = false;
            InvalidateVisual(previousEdge);
        }

        if (m_hoveredNode)
        {
            m_hoveredNode->Hovered = true;
            InvalidateVisual(m_hoveredNode);
        }
        else if (m_hoveredEdge)
        {
            m_hoveredEdge->Hovered = true;
            InvalidateVisual(m_hoveredEdge);
        }
    }

    void ToolController::InvalidateVisual(VisualNode^ node)
    {
        if (!node)
            return;

        auto rect = GetNodeInvalidRect(node);
        if (rect.IsEmpty)
            return;

        if (m_invalidate)
            m_invalidate(rect);
    }

    void ToolController::InvalidateVisual(VisualEdge^ edge)
    {
        if (!edge)
            return;

        auto rect = GetEdgeInvalidRect(edge);
        if (rect.IsEmpty)
            return;

        if (m_invalidate)
            m_invalidate(rect);
    }

    System::Drawing::Rectangle ToolController::GetNodeInvalidRect(VisualNode^ node)
    {
        auto bounds = node->GetBounds(4.0f);
        bounds.Inflate(4.0f, 4.0f);
        return Rectangle::Ceiling(bounds);
    }

    System::Drawing::Rectangle ToolController::GetEdgeInvalidRect(VisualEdge^ edge)
    {
        if (!m_document || !m_document->Visual)
            return System::Drawing::Rectangle();

        auto visual = m_document->Visual;
        auto from = visual->FindNodeByLogicalId(edge->FromLogicalId);
        auto to = visual->FindNodeByLogicalId(edge->ToLogicalId);
        if (!from || !to)
            return System::Drawing::Rectangle();

        float left = Math::Min(from->Position.X, to->Position.X);
        float top = Math::Min(from->Position.Y, to->Position.Y);
        float width = Math::Abs(from->Position.X - to->Position.X);
        float height = Math::Abs(from->Position.Y - to->Position.Y);
        RectangleF rect(left, top, width, height);
        rect.Inflate(visual->HitTolerance + 4.0f, visual->HitTolerance + 4.0f);
        return Rectangle::Ceiling(rect);
    }
}