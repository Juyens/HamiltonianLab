#pragma once

#include <Common/ToolMode.h>
#include <Models/GraphDocument.h>
#include <Renderers/GridPainter.h>
#include <Tools/ITool.h>
#include <Tools/SelectTool.h>
#include <Tools/AddNodeTool.h>
#include <Tools/AddEdgeTool.h>
#include <Tools/DeleteTool.h>

namespace HamiltonianLab::Renderers
{
    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    using namespace HamiltonianLab::Models;

    public ref class GraphCanvas : public Control
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

    protected:
        virtual void OnPaint(PaintEventArgs^ e) override;
        virtual void OnPaintBackground(PaintEventArgs^ e) override;
        virtual void OnResize(EventArgs^ e) override;
        virtual void OnMouseDown(MouseEventArgs^ e) override;
        virtual void OnMouseMove(MouseEventArgs^ e) override;
        virtual void OnMouseUp(MouseEventArgs^ e) override;
        virtual void OnMouseLeave(EventArgs^ e) override;
        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;

    private:
        void InitStyles();
        void DrawScene(Graphics^ g);
        void UpdateHovered(Models::Visual::VisualNode^ node, Models::Visual::VisualEdge^ edge);
        void InvalidateVisual(Models::Visual::VisualNode^ node);
        void InvalidateVisual(Models::Visual::VisualEdge^ edge);
        System::Drawing::Rectangle GetNodeInvalidRect(Models::Visual::VisualNode^ node);
        System::Drawing::Rectangle GetEdgeInvalidRect(Models::Visual::VisualEdge^ edge);
        void SetActiveTool(ToolMode mode);

    private:
        ToolMode m_mode{ ToolMode::Select };
        GraphDocument^ m_doc;
        GridPainter^ m_grid;
        System::Collections::Generic::Dictionary<ToolMode, HamiltonianLab::ITool^>^ m_tools;
        HamiltonianLab::ITool^ m_activeTool;
        Models::Visual::VisualNode^ m_hoveredNode;
        Models::Visual::VisualEdge^ m_hoveredEdge;
    };
}