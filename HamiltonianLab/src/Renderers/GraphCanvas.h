#pragma once

#include <Common/ToolMode.h>
#include <Interaction/IInvalidationSink.h>
#include <Interaction/IViewportService.h>
#include <Models/GraphDocument.h>
#include <Renderers/GridPainter.h>
#include <Renderers/SceneRenderer.h>
#include <Tools/ToolController.h>

namespace HamiltonianLab::Renderers
{
    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    using namespace HamiltonianLab::Models;

    public ref class GraphCanvas : public Control,
        public HamiltonianLab::Interaction::IInvalidationSink,
        public HamiltonianLab::Interaction::IViewportService
    {
    public:
        GraphCanvas();
        ~GraphCanvas();

    public:
        void SetToolMode(ToolMode mode);
        property HamiltonianLab::Models::GraphDocument^ Document
        {
            HamiltonianLab::Models::GraphDocument^ get() { return m_doc; }
        }

        property HamiltonianLab::Models::Selection^ Selection
        {
            HamiltonianLab::Models::Selection^ get() { return m_doc ? m_doc->Selection : nullptr; }
        }

        virtual void RequestInvalidate() = HamiltonianLab::Interaction::IInvalidationSink::RequestInvalidate;
        virtual void RequestInvalidate(System::Drawing::Rectangle rect) = HamiltonianLab::Interaction::IInvalidationSink::RequestInvalidate;

        virtual property System::Drawing::RectangleF ViewportBounds
        {
            System::Drawing::RectangleF get() = HamiltonianLab::Interaction::IViewportService::ViewportBounds::get;
        }

    protected:
        virtual void OnPaint(PaintEventArgs^ e) override;
        virtual void OnPaintBackground(PaintEventArgs^ e) override;
        virtual void OnResize(EventArgs^ e) override;
        virtual void OnMouseDown(MouseEventArgs^ e) override;
        virtual void OnMouseMove(MouseEventArgs^ e) override;
        virtual void OnMouseUp(MouseEventArgs^ e) override;
        virtual void OnMouseLeave(EventArgs^ e) override;
        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;
        virtual void OnMouseWheel(MouseEventArgs^ e) override;

    private:
        void InitStyles();
        void InvalidateRegion(System::Drawing::Rectangle rect);
        void UpdateCursor();

    private:
        GraphDocument^ m_doc;
        GridPainter^ m_grid;
        SceneRenderer^ m_renderer;
        HamiltonianLab::ToolController^ m_toolController;
    };
}