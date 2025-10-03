#include "GraphCanvas.h"

namespace HamiltonianLab::Renderers
{
    GraphCanvas::GraphCanvas()
        : m_doc(gcnew GraphDocument())
        , m_grid(gcnew GridPainter())
        , m_renderer(gcnew SceneRenderer(m_grid))
        , m_toolController(gcnew HamiltonianLab::ToolController(m_doc, this))
    {
        InitStyles();

        this->BackColor = Color::White;
        this->DoubleBuffered = true;
        this->TabStop = true;

        this->SetStyle(ControlStyles::Selectable, true);

        m_toolController->SetInvalidateCallback(gcnew System::Action<System::Drawing::Rectangle>(this, &GraphCanvas::InvalidateRegion));
        UpdateCursor();
    }

    GraphCanvas::~GraphCanvas()
    {
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

    void GraphCanvas::OnPaint(PaintEventArgs^ e)
    {
        Control::OnPaint(e);

        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        if (!m_renderer)
            return;

        m_renderer->Render(e->Graphics, this->ClientSize, m_doc, m_toolController ? m_toolController->ActiveTool : nullptr);
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
}
