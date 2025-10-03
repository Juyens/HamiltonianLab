#pragma once

#include <Tools/BaseTool.h>

namespace HamiltonianLab
{
    using namespace System;
    using namespace System::Drawing;
    using namespace System::Globalization;
    using namespace System::Windows::Forms;

    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Models::Visual;

    ref class ToolController;

    public ref class SetWeightTool : public BaseTool
    {
    public:
        SetWeightTool(GraphDocument^ doc, ToolController^ controller, Control^ hostControl);

        virtual void OnEnter() override;
        virtual void OnLeave() override;
        virtual void OnMouseDown(MouseEventArgs^ e) override;
        virtual void OnMouseMove(MouseEventArgs^ e) override;
        virtual void OnMouseUp(MouseEventArgs^ e) override;
        virtual void OnKeyDown(KeyEventArgs^ e) override;
        virtual Cursor^ GetCursor() override;

        property VisualEdge^ ActiveEdge
        {
            VisualEdge^ get() { return m_activeEdge; }
        }

        property bool IsEditing
        {
            bool get() { return m_editor != nullptr; }
        }

    private:
        void BeginEdit(VisualEdge^ edge);
        void CloseEditor(bool revertChanges);
        void CommitEdit();
        void CancelEdit();
        void RemoveEditor();
        void OnEditorKeyPress(Object^ sender, KeyPressEventArgs^ e);
        void OnEditorKeyDown(Object^ sender, KeyEventArgs^ e);
        void OnEditorLeave(Object^ sender, EventArgs^ e);
        VisualEdge^ HitTestEdgeLabel(PointF p);
        RectangleF GetEdgeLabelBounds(VisualEdge^ edge);
        PointF ComputeLabelPosition(VisualEdge^ edge);
        void InvalidateEdge(VisualEdge^ edge);

    private:
        ToolController^ m_controller;
        Control^ m_host;
        TextBox^ m_editor;
        VisualEdge^ m_activeEdge;
        String^ m_originalLabel;
        double m_originalWeight;
        KeyPressEventHandler^ m_keyPressHandler;
        KeyEventHandler^ m_keyDownHandler;
        EventHandler^ m_leaveHandler;
    };
}