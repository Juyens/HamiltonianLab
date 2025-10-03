#include "SelectTool.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace HamiltonianLab::Interaction;
using namespace HamiltonianLab::Models;
using namespace HamiltonianLab::Models::Visual;

namespace HamiltonianLab
{
    SelectTool::SelectTool(GraphDocument^ doc,
        IInvalidationSink^ invalidationSink,
        IViewportService^ viewportService)
        : BaseTool(doc)
        , m_constraints(gcnew MovementConstraints(viewportService))
        , m_marqueeController(gcnew MarqueeSelectionController(doc, m_constraints, invalidationSink))
        , m_dragController(gcnew SelectionDragController(doc, m_constraints, invalidationSink))
        , m_activeInteraction(ActiveInteraction::None)
    {
    }

    void SelectTool::OnEnter()
    {
        m_activeInteraction = ActiveInteraction::None;
    }

    void SelectTool::OnLeave()
    {
        if (m_marqueeController && m_marqueeController->IsActive)
            m_marqueeController->Cancel();
        if (m_dragController && m_dragController->IsDragging)
            m_dragController->Cancel();
        m_activeInteraction = ActiveInteraction::None;
    }

    void SelectTool::OnMouseDown(MouseEventArgs^ e)
    {
        if (!Doc || e->Button != MouseButtons::Left)
            return;

        bool shift = (Control::ModifierKeys & Keys::Shift) == Keys::Shift;
        bool ctrl = (Control::ModifierKeys & Keys::Control) == Keys::Control;

        PointF point(static_cast<float>(e->X), static_cast<float>(e->Y));

        auto node = Doc->GetNodeAt(point);
        VisualEdge^ edge = nullptr;
        if (!node)
            edge = Doc->GetEdgeAt(point);

        if (node)
        {
            HandleNodeClick(node, shift, ctrl, point);
        }
        else if (edge)
        {
            HandleEdgeClick(edge, shift, ctrl);
        }
        else
        {
            StartMarquee(point, shift, ctrl);
        }
    }

    void SelectTool::OnMouseMove(MouseEventArgs^ e)
    {
        if (!Doc)
            return;

        PointF point(static_cast<float>(e->X), static_cast<float>(e->Y));

        switch (m_activeInteraction)
        {
        case ActiveInteraction::Marquee:
            if (m_marqueeController)
                m_marqueeController->Update(point);
            break;
        case ActiveInteraction::Drag:
            if (m_dragController)
                m_dragController->Update(point);
            break;
        default:
            break;
        }
    }

    void SelectTool::OnMouseUp(MouseEventArgs^ e)
    {
        if (e->Button != MouseButtons::Left)
            return;

        switch (m_activeInteraction)
        {
        case ActiveInteraction::Marquee:
            if (m_marqueeController)
                m_marqueeController->Complete();
            break;
        case ActiveInteraction::Drag:
            if (m_dragController)
                m_dragController->EndDrag();
            break;
        default:
            break;
        }

        m_activeInteraction = ActiveInteraction::None;
    }

    void SelectTool::OnKeyDown(KeyEventArgs^ e)
    {
        if (!Doc || !Doc->Selection)
            return;

        if (e->KeyCode == Keys::Back || e->KeyCode == Keys::Delete)
        {
            Doc->Selection->DeleteSelection();
            RequestRepaint();
            e->Handled = true;
            return;
        }

        if (e->KeyCode == Keys::Escape)
        {
            Doc->Selection->Clear();
            RequestRepaint();
            e->Handled = true;
            return;
        }
    }

    void SelectTool::OnRenderOverlay(Graphics^ g)
    {
        if (!m_marqueeController || !m_marqueeController->IsActive)
            return;

        auto rectF = m_marqueeController->CurrentRectangle;
        if (rectF.Width <= 0.0f && rectF.Height <= 0.0f)
            return;

        System::Drawing::Rectangle rect = Rectangle::Ceiling(rectF);

        auto hi = SystemColors::Highlight;
        auto fill = Color::FromArgb(64, hi);
        auto border1 = Color::FromArgb(200, hi);
        auto border2 = Color::FromArgb(160, Color::White);

        auto bg = gcnew SolidBrush(fill);
        g->FillRectangle(bg, rect);

        auto p1 = gcnew Pen(border1, 1.0f);
        p1->Alignment = Drawing2D::PenAlignment::Inset;
        g->DrawRectangle(p1, rect);

        System::Drawing::Rectangle inner(rect.X + 1, rect.Y + 1,
            Math::Max(0, rect.Width - 2),
            Math::Max(0, rect.Height - 2));

        if (inner.Width > 0 && inner.Height > 0)
        {
            auto p2 = gcnew Pen(border2, 1.0f);
            p2->Alignment = Drawing2D::PenAlignment::Inset;
            g->DrawRectangle(p2, inner);
            delete p2;
        }

        delete p1;
        delete bg;
    }

    Cursor^ SelectTool::GetCursor()
    {
        return Cursors::Hand;
    }

    void SelectTool::HandleNodeClick(VisualNode^ node, bool shift, bool ctrl, PointF point)
    {
        auto selection = Doc->Selection;
        if (!selection)
            return;

        if (!shift && !ctrl)
        {
            selection->Clear();
            selection->Add(node);
        }
        else if (ctrl)
        {
            selection->Toggle(node);
        }
        else if (shift)
        {
            selection->Add(node);
        }

        if (!shift && !ctrl)
        {
            StartDrag(point);
        }
        else
        {
            m_activeInteraction = ActiveInteraction::None;
        }
    }

    void SelectTool::HandleEdgeClick(VisualEdge^ edge, bool shift, bool ctrl)
    {
        auto selection = Doc->Selection;
        if (!selection)
            return;

        if (!shift && !ctrl)
        {
            selection->Clear();
            selection->Add(edge);
        }
        else if (ctrl)
        {
            selection->Toggle(edge);
        }
        else if (shift)
        {
            selection->Add(edge);
        }

        m_activeInteraction = ActiveInteraction::None;
    }

    void SelectTool::StartMarquee(PointF anchor, bool shift, bool ctrl)
    {
        if (!m_marqueeController)
            return;

        auto mode = GetMarqueeMode(shift, ctrl);
        m_marqueeController->Begin(anchor, mode);
        m_activeInteraction = ActiveInteraction::Marquee;
    }

    void SelectTool::StartDrag(PointF anchor)
    {
        if (!m_dragController)
            return;

        if (m_dragController->BeginDrag(anchor))
        {
            m_activeInteraction = ActiveInteraction::Drag;
        }
        else
        {
            m_activeInteraction = ActiveInteraction::None;
        }
    }

    MarqueeSelectionMode SelectTool::GetMarqueeMode(bool shift, bool ctrl)
    {
        if (ctrl)
            return MarqueeSelectionMode::Toggle;
        if (shift)
            return MarqueeSelectionMode::Add;
        return MarqueeSelectionMode::Replace;
    }
}