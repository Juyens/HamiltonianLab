#pragma once

#include <Common/ToolMode.h>
#include <Models/GraphDocument.h>
#include <Tools/AddEdgeTool.h>
#include <Tools/AddNodeTool.h>
#include <Tools/DeleteTool.h>
#include <Tools/SelectTool.h>
#include <Tools/SetWeightTool.h>

namespace HamiltonianLab
{
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Models::Visual;

    public ref class ToolController
    {
    public:
        ToolController(GraphDocument^ document, System::Windows::Forms::Control^ hostControl);
        ~ToolController();

        void SetInvalidateCallback(Action<System::Drawing::Rectangle>^ callback);

        bool SetToolMode(ToolMode mode);

        bool HandleMouseDown(MouseEventArgs^ e);
        bool HandleMouseMove(MouseEventArgs^ e);
        bool HandleMouseUp(MouseEventArgs^ e);
        bool HandleMouseLeave();
        bool HandleKeyDown(KeyEventArgs^ e);

        property HamiltonianLab::ITool^ ActiveTool
        {
            HamiltonianLab::ITool^ get() { return m_activeTool; }
        }

        property GraphDocument^ Document
        {
            GraphDocument^ get() { return m_document; }
        }

        void RequestInvalidate(HamiltonianLab::Models::Visual::VisualEdge^ edge);

    private:
        bool ConsumeActiveToolRepaint();
        void UpdateHovered(VisualNode^ node, VisualEdge^ edge);
        void InvalidateVisual(VisualNode^ node);
        void InvalidateVisual(VisualEdge^ edge);
        System::Drawing::Rectangle GetNodeInvalidRect(VisualNode^ node);
        System::Drawing::Rectangle GetEdgeInvalidRect(VisualEdge^ edge);

    private:
        GraphDocument^ m_document;
        Dictionary<ToolMode, HamiltonianLab::ITool^>^ m_tools;
        HamiltonianLab::ITool^ m_activeTool;
        ToolMode m_mode;
        VisualNode^ m_hoveredNode;
        VisualEdge^ m_hoveredEdge;
        Action<System::Drawing::Rectangle>^ m_invalidate;
    };
}