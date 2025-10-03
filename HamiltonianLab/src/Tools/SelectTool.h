#pragma once

#include <Interaction/MarqueeSelectionController.h>
#include <Interaction/SelectionDragController.h>
#include <Interaction/MovementConstraints.h>
#include <Tools/BaseTool.h>

namespace HamiltonianLab
{
    public ref class SelectTool : public BaseTool
    {
    public:
        SelectTool(Models::GraphDocument^ doc,
            Interaction::IInvalidationSink^ invalidationSink,
            Interaction::IViewportService^ viewportService);

        virtual void OnEnter() override;
        virtual void OnLeave() override;

        virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;
        virtual void OnRenderOverlay(System::Drawing::Graphics^ g) override;

        virtual System::Windows::Forms::Cursor^ GetCursor() override;

    private:
        enum class ActiveInteraction
        {
            None,
            Marquee,
            Drag
        };

        void HandleNodeClick(Models::Visual::VisualNode^ node, bool shift, bool ctrl, System::Drawing::PointF point);
        void HandleEdgeClick(Models::Visual::VisualEdge^ edge, bool shift, bool ctrl);
        void StartMarquee(System::Drawing::PointF anchor, bool shift, bool ctrl);
        void StartDrag(System::Drawing::PointF anchor);
        Interaction::MarqueeSelectionMode GetMarqueeMode(bool shift, bool ctrl);

    private:
        Interaction::MovementConstraints^ m_constraints;
        Interaction::MarqueeSelectionController^ m_marqueeController;
        Interaction::SelectionDragController^ m_dragController;
        ActiveInteraction m_activeInteraction;
    };
}