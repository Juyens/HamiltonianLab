#include "GraphCanvas.h"

namespace HamiltonianLab::Renderers
{
    GraphCanvas::GraphCanvas()
        : m_grid(gcnew GridPainter())
    {
        m_mode = ToolMode::Delete;

        m_doc = gcnew GraphDocument();
        m_tools = gcnew Dictionary<ToolMode, ITool^>();
        m_tools->Add(ToolMode::Select, gcnew SelectTool(m_doc));
        m_tools->Add(ToolMode::AddNode, gcnew AddNodeTool(m_doc));
        m_tools->Add(ToolMode::AddEdge, gcnew AddEdgeTool(m_doc));
        m_tools->Add(ToolMode::Delete, gcnew DeleteTool(m_doc));
        m_tools->Add(ToolMode::SetWeight, gcnew SelectTool(m_doc));

        m_hoveredNode = nullptr;
        m_hoveredEdge = nullptr;

        InitStyles();
        this->BackColor = Color::White;
        this->DoubleBuffered = true;
        this->TabStop = true;

        this->SetStyle(ControlStyles::Selectable, true);

        SetActiveTool(ToolMode::Select);
    }

    GraphCanvas::~GraphCanvas()
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

        delete m_grid; m_grid = nullptr;
        delete m_doc; m_doc = nullptr;
    }

    void GraphCanvas::SetToolMode(ToolMode mode)
    {
        if (m_mode == mode) 
            return;

        SetActiveTool(mode);
    }

    void GraphCanvas::OnPaint(PaintEventArgs^ e)
    {
        Control::OnPaint(e);

        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        m_grid->Render(g, this->ClientSize);

        DrawScene(g);

        if (m_doc && m_doc->Selection)
            m_doc->Selection->Render(g);

        if (m_activeTool)
            m_activeTool->OnRenderOverlay(g);
    }

    void GraphCanvas::OnPaintBackground(PaintEventArgs^ e)
    {
        e->Graphics->Clear(this->BackColor);
    }

    void GraphCanvas::OnResize(EventArgs^ e)
    {
        Control::OnResize(e);
        Invalidate();
    }

    void GraphCanvas::OnMouseDown(MouseEventArgs^ e)
    {
        Control::OnMouseDown(e);

        if (!m_activeTool) return;

        this->Focus();
        m_activeTool->OnMouseDown(e);
        Invalidate();
    }

    void GraphCanvas::OnMouseMove(MouseEventArgs^ e)
    {
        Control::OnMouseMove(e);

        if (!m_doc) return;

        PointF p((float)e->X, (float)e->Y);
        auto node = m_doc->GetNodeAt(p);
        VisualEdge^ edge = nullptr;
        if (!node)
            edge = m_doc->GetEdgeAt(p);

        UpdateHovered(node, edge);

        if (m_activeTool)
        {
            m_activeTool->OnMouseMove(e);
            Invalidate();
        }
    }

    void GraphCanvas::OnMouseUp(MouseEventArgs^ e)
    {
        Control::OnMouseUp(e);

        if (!m_activeTool) return;

        m_activeTool->OnMouseUp(e);
        Invalidate();
    }

    void GraphCanvas::OnMouseLeave(EventArgs^ e)
    {
        Control::OnMouseLeave(e);
        UpdateHovered(nullptr, nullptr);
    }

    void GraphCanvas::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        if (m_activeTool)
        {
            m_activeTool->OnKeyDown(e);
            if (m_activeTool->ConsumeRepaintRequest()) this->Invalidate();
            if (e->Handled) return;
        }

        Control::OnKeyDown(e);
    }

    void GraphCanvas::InitStyles()
    {
        this->SetStyle(ControlStyles::UserPaint, true);
        this->SetStyle(ControlStyles::AllPaintingInWmPaint, true);
        this->SetStyle(ControlStyles::Opaque, false);
        this->SetStyle(ControlStyles::OptimizedDoubleBuffer, true);
        this->UpdateStyles();
    }

    void GraphCanvas::DrawScene(Graphics^ g)
    {
        if (!m_doc || !m_doc->Visual)
            return;

        auto visual = m_doc->Visual;

        for each (auto edge in visual->Edges)
        {
            auto from = visual->FindNodeByLogicalId(edge->FromLogicalId);
            auto to = visual->FindNodeByLogicalId(edge->ToLogicalId);
            if (!from || !to) continue;

            Color stroke = edge->Stroke;
            float width = edge->StrokeWidth;
            if (edge->Selected)
            {
                stroke = Color::DodgerBlue;
                width += 2.0f;
            }
            else if (edge->Hovered)
            {
                stroke = Color::SteelBlue;
                width += 1.0f;
            }

            auto pen = gcnew Pen(stroke, width <= 0 ? 1.0f : width);
            pen->StartCap = LineCap::Round;
            pen->EndCap = LineCap::Round;
            g->DrawLine(pen, from->Position, to->Position);
            delete pen;
        }

        for each (auto node in visual->Nodes)
        {
            auto style = node->Style;
            Color fill = style->Fill;
            Color stroke = style->Stroke;
            float strokeWidth = style->StrokeWidth;

            if (node->Selected)
            {
                stroke = Color::DodgerBlue;
                strokeWidth += 2.0f;
            }
            else if (node->Hovered)
            {
                stroke = Color::SteelBlue;
                strokeWidth += 1.0f;
            }

            auto rect = node->GetBounds(0.0f);
            auto brush = gcnew SolidBrush(fill);
            auto pen = gcnew Pen(stroke, strokeWidth <= 0 ? 1.0f : strokeWidth);
            g->FillEllipse(brush, rect);
            g->DrawEllipse(pen, rect);

            if (node->Label != nullptr && node->Label->Length > 0)
            {
                auto font = gcnew System::Drawing::Font(style->LabelFontName, style->LabelFontSize);
                auto format = gcnew StringFormat();
                format->Alignment = StringAlignment::Center;
                format->LineAlignment = StringAlignment::Center;
                auto textBrush = gcnew SolidBrush(style->LabelColor);
                g->DrawString(node->Label, font, textBrush, rect, format);
                delete textBrush;
                delete format;
                delete font;
            }

            delete pen;
            delete brush;
        }
    }

    void GraphCanvas::UpdateHovered(VisualNode^ node, VisualEdge^ edge)
    {
        if (node != nullptr)
            edge = nullptr;

        if (m_hoveredNode == node && m_hoveredEdge == edge)
            return;

        auto prevNode = m_hoveredNode;
        auto prevEdge = m_hoveredEdge;

        m_hoveredNode = node;
        m_hoveredEdge = edge;

        if (prevNode && prevNode != node)
        {
            prevNode->Hovered = false;
            InvalidateVisual(prevNode);
        }

        if (prevEdge && prevEdge != edge)
        {
            prevEdge->Hovered = false;
            InvalidateVisual(prevEdge);
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

    void GraphCanvas::InvalidateVisual(VisualNode^ node)
    {
        if (!node) return;
        auto rect = GetNodeInvalidRect(node);
        if (!rect.IsEmpty)
            Invalidate(rect);
    }

    void GraphCanvas::InvalidateVisual(VisualEdge^ edge)
    {
        if (!edge) return;
        auto rect = GetEdgeInvalidRect(edge);
        if (!rect.IsEmpty)
            Invalidate(rect);
    }

    System::Drawing::Rectangle GraphCanvas::GetNodeInvalidRect(VisualNode^ node)
    {
        auto bounds = node->GetBounds(4.0f);
        bounds.Inflate(4.0f, 4.0f);
        return Rectangle::Ceiling(bounds);
    }

    System::Drawing::Rectangle GraphCanvas::GetEdgeInvalidRect(VisualEdge^ edge)
    {
        if (!m_doc || !m_doc->Visual)
            return System::Drawing::Rectangle();

        auto visual = m_doc->Visual;
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

    void GraphCanvas::SetActiveTool(ToolMode mode)
    {
        if (m_mode == mode) return;

        if (mode == ToolMode::Delete)
        {
            if (m_doc && m_doc->Selection)
            {
                m_doc->Selection->DeleteSelection();
                this->Invalidate();
            }
            return;
        }

        if (!m_tools || !m_tools->ContainsKey(mode))
            return;

        if (m_activeTool)
            m_activeTool->OnLeave();

        m_mode = mode;
        m_activeTool = m_tools[mode];

        if (m_activeTool)
        {
            m_activeTool->OnEnter();
            this->Cursor = m_activeTool->GetCursor();
        }
        else
        {
            this->Cursor = Cursors::Default;
        }
    }
}
