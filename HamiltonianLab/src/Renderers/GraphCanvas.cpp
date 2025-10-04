#include "GraphCanvas.h"

namespace HamiltonianLab::Renderers
{
    GraphCanvas::GraphCanvas()
        : m_doc(gcnew GraphDocument())
        , m_grid(gcnew GridPainter())
        , m_renderer(gcnew SceneRenderer(m_grid))
        , m_toolController(gcnew HamiltonianLab::ToolController(m_doc, this))
        , m_statusLabel(nullptr)
        , m_hasStatusLabel(false)
        , m_structureChangedHandler(nullptr)
    {
        InitStyles();

        this->BackColor = Color::White;
        this->DoubleBuffered = true;
        this->TabStop = true;

        this->SetStyle(ControlStyles::Selectable, true);

        m_toolController->SetInvalidateCallback(gcnew System::Action<System::Drawing::Rectangle>(this, &GraphCanvas::InvalidateRegion));
        UpdateCursor();

        if (m_doc)
        {
            m_structureChangedHandler = gcnew System::EventHandler(this, &GraphCanvas::OnDocumentStructureChanged);
            m_doc->StructureChanged += m_structureChangedHandler;
        }
    }

    GraphCanvas::~GraphCanvas()
    {
        if (m_doc && m_structureChangedHandler)
        {
            m_doc->StructureChanged -= m_structureChangedHandler;
            m_structureChangedHandler = nullptr;
        }

        if (m_toolController)
        {
            delete m_toolController;
            m_toolController = nullptr;
        }

        if (m_renderer)
        {
            delete m_renderer;
            m_renderer = nullptr;
        }

        if (m_grid)
        {
            delete m_grid;
            m_grid = nullptr;
        }

        if (m_doc)
        {
            delete m_doc;
            m_doc = nullptr;
        }
    }

    void GraphCanvas::SetToolMode(ToolMode mode)
    {
        if (!m_toolController)
            return;

        bool requiresRepaint = m_toolController->SetToolMode(mode);

        UpdateCursor();

        if (requiresRepaint)
            Invalidate();
    }

    void GraphCanvas::RequestInvalidate()
    {
        this->Invalidate();
    }

    void GraphCanvas::RequestInvalidate(System::Drawing::Rectangle rect)
    {
        InvalidateRegion(rect);
    }

    System::Drawing::RectangleF GraphCanvas::ViewportBounds::get()
    {
        auto client = this->ClientRectangle;
        return System::Drawing::RectangleF(
            static_cast<float>(client.Left),
            static_cast<float>(client.Top),
            static_cast<float>(client.Width),
            static_cast<float>(client.Height));
    }

    void GraphCanvas::OnPaint(PaintEventArgs^ e)
    {
        Control::OnPaint(e);

        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        if (!m_renderer)
            return;

        m_renderer->Render(e->Graphics, this->ClientSize, m_doc, m_toolController ? m_toolController->ActiveTool : nullptr);
        DrawStatusLabel(e->Graphics);
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

        if (!m_toolController)
            return;

        this->Focus();

        if (m_toolController->HandleMouseDown(e))
            Invalidate();
    }

    void GraphCanvas::OnMouseMove(MouseEventArgs^ e)
    {
        Control::OnMouseMove(e);

        if (!m_toolController)
            return;

        if (m_toolController->HandleMouseMove(e))
            Invalidate();
    }

    void GraphCanvas::OnMouseUp(MouseEventArgs^ e)
    {
        Control::OnMouseUp(e);

        if (!m_toolController)
            return;

        if (m_toolController->HandleMouseUp(e))
            Invalidate();
    }

    void GraphCanvas::OnMouseLeave(EventArgs^ e)
    {
        Control::OnMouseLeave(e);

        if (!m_toolController)
            return;

        if (m_toolController->HandleMouseLeave())
            Invalidate();
    }

    void GraphCanvas::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        if (m_toolController && m_toolController->HandleKeyDown(e))
            Invalidate();

        if (e->Handled)
            return;

        Control::OnKeyDown(e);
    }

    void GraphCanvas::OnMouseWheel(MouseEventArgs^ e)
    {
        if (!e)
            return;

        if (!m_doc || !m_doc->Visual)
        {
            Control::OnMouseWheel(e);
            return;
        }

        int delta = e->Delta;
        if (delta == 0)
        {
            auto handledArgs = dynamic_cast<System::Windows::Forms::HandledMouseEventArgs^>(e);
            if (handledArgs)
                handledArgs->Handled = true;
            return;
        }

        int direction = delta > 0 ? 1 : -1;
        bool ctrl = (Control::ModifierKeys & Keys::Control) == Keys::Control;
        float multiplier = ctrl ? 2.0f : 1.0f;

        const float radiusMin = 8.0f;
        const float radiusMax = 60.0f;
        const float strokeMin = 1.0f;
        const float strokeMax = 8.0f;
        const float fontMin = 8.0f;
        const float fontMax = 20.0f;

        float stepR = 2.0f * multiplier * static_cast<float>(direction);
        float stepE = 1.0f * multiplier * static_cast<float>(direction);
        float stepF = 1.0f * multiplier * static_cast<float>(direction);

        auto visual = m_doc->Visual;
        bool changed = false;
        double radiusAccumulator = 0.0;
        int radiusCount = 0;

        if (visual->Nodes)
        {
            for each (auto node in visual->Nodes)
            {
                if (!node)
                    continue;

                float newRadius = node->Radius + stepR;
                newRadius = System::Math::Max(radiusMin, System::Math::Min(radiusMax, newRadius));
                if (System::Math::Abs(newRadius - node->Radius) > 1e-3f)
                {
                    node->Radius = newRadius;
                    changed = true;
                }

                radiusAccumulator += node->Radius;
                ++radiusCount;
            }
        }

        float newDefaultRadius = visual->DefaultNodeRadius + stepR;
        newDefaultRadius = System::Math::Max(radiusMin, System::Math::Min(radiusMax, newDefaultRadius));
        if (System::Math::Abs(newDefaultRadius - visual->DefaultNodeRadius) > 1e-3f)
        {
            visual->DefaultNodeRadius = newDefaultRadius;
            changed = true;
        }

        if (visual->Edges)
        {
            for each (auto edge in visual->Edges)
            {
                if (!edge)
                    continue;

                float newStroke = edge->StrokeWidth + stepE;
                newStroke = System::Math::Max(strokeMin, System::Math::Min(strokeMax, newStroke));
                if (System::Math::Abs(newStroke - edge->StrokeWidth) > 1e-3f)
                {
                    edge->StrokeWidth = newStroke;
                    changed = true;
                }

                float currentFont = edge->LabelFontSize;
                float newFont = currentFont + stepF;
                newFont = System::Math::Max(fontMin, System::Math::Min(fontMax, newFont));
                if (System::Math::Abs(newFont - currentFont) > 1e-3f)
                {
                    edge->LabelFontSize = newFont;
                    changed = true;
                }
            }
        }

        float referenceRadius = radiusCount > 0
            ? static_cast<float>(radiusAccumulator / radiusCount)
            : visual->DefaultNodeRadius;
        float newHitTolerance = System::Math::Max(4.0f, referenceRadius * 0.35f);
        if (System::Math::Abs(newHitTolerance - visual->HitTolerance) > 1e-3f)
        {
            visual->HitTolerance = newHitTolerance;
            changed = true;
        }

        if (changed)
        {
            m_doc->MarkModified();
            this->Invalidate();
        }

        auto handledArgs = dynamic_cast<System::Windows::Forms::HandledMouseEventArgs^>(e);
        if (handledArgs)
            handledArgs->Handled = true;
    }

    void GraphCanvas::InitStyles()
    {
        this->SetStyle(ControlStyles::UserPaint, true);
        this->SetStyle(ControlStyles::AllPaintingInWmPaint, true);
        this->SetStyle(ControlStyles::Opaque, false);
        this->SetStyle(ControlStyles::OptimizedDoubleBuffer, true);
        this->UpdateStyles();
    }

    void GraphCanvas::InvalidateRegion(System::Drawing::Rectangle rect)
    {
        if (rect.IsEmpty)
        {
            this->Invalidate();
            return;
        }

        this->Invalidate(rect);
    }

    void GraphCanvas::UpdateCursor()
    {
        if (!m_toolController)
        {
            this->Cursor = Cursors::Default;
            return;
        }

        auto tool = m_toolController->ActiveTool;
        this->Cursor = tool ? tool->GetCursor() : Cursors::Default;
    }

    void GraphCanvas::SetStatusLabel(System::String^ text)
    {
        if (System::String::IsNullOrEmpty(text))
        {
            ClearStatusLabel();
            return;
        }

        bool wasDifferent = !HasStatusLabel || !m_statusLabel->Equals(text);
        m_statusLabel = text;
        m_hasStatusLabel = true;

        if (wasDifferent)
            this->Invalidate();
    }

    void GraphCanvas::ClearStatusLabel()
    {
        if (!HasStatusLabel)
        {
            m_statusLabel = nullptr;
            m_hasStatusLabel = false;
            return;
        }

        m_statusLabel = nullptr;
        m_hasStatusLabel = false;
        this->Invalidate();
    }

    void GraphCanvas::DrawStatusLabel(System::Drawing::Graphics^ g)
    {
        if (!g || !HasStatusLabel)
            return;

        auto text = m_statusLabel;
        auto font = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Bold, GraphicsUnit::Point);
        auto format = gcnew StringFormat();
        format->Alignment = StringAlignment::Near;
        format->LineAlignment = StringAlignment::Near;
        format->FormatFlags = StringFormatFlags::NoWrap;

        SizeF measured = g->MeasureString(text, font);
        const float padding = 6.0f;
        RectangleF bounds(8.0f, 8.0f, measured.Width + padding * 2.0f, measured.Height + padding * 2.0f);

        auto background = gcnew SolidBrush(Color::FromArgb(220, Color::WhiteSmoke));
        auto border = gcnew Pen(Color::FromArgb(255, 218, 165, 32), 1.5f);
        auto textBrush = gcnew SolidBrush(Color::FromArgb(255, 40, 40, 40));

        g->FillRectangle(background, bounds);
        g->DrawRectangle(border, Rectangle::Round(bounds));

        RectangleF textBounds(bounds.X + padding, bounds.Y + padding, bounds.Width - padding * 2.0f, bounds.Height - padding * 2.0f);
        g->DrawString(text, font, textBrush, textBounds, format);

        delete textBrush;
        delete border;
        delete background;
        delete format;
        delete font;
    }

    void GraphCanvas::OnDocumentStructureChanged(System::Object^ sender, System::EventArgs^ e)
    {
        if (m_doc)
            m_doc->ClearEdgeHighlights();

        m_statusLabel = nullptr;
        m_hasStatusLabel = false;
        this->Invalidate();
    }
}
