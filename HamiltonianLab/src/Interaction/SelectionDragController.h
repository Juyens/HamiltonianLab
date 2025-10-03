#pragma once

#include <Interaction/IInvalidationSink.h>
#include <Interaction/MovementConstraints.h>
#include <Models/GraphDocument.h>

namespace HamiltonianLab::Interaction
{
    using namespace System::Collections::Generic;
    using namespace System::Drawing;

    public ref class SelectionDragController
    {
    public:
        SelectionDragController(Models::GraphDocument^ document,
            MovementConstraints^ constraints,
            IInvalidationSink^ invalidationSink);

        property bool IsDragging
        {
            bool get();
        }

        bool BeginDrag(PointF anchor);
        void Update(PointF current);
        void EndDrag();
        void Cancel();

    private:
        void ApplyDelta(PointF delta);
        System::Drawing::Rectangle ComputeInvalidationBounds();

    private:
        Models::GraphDocument^ m_document;
        MovementConstraints^ m_constraints;
        IInvalidationSink^ m_invalidationSink;

        bool m_dragging;
        PointF m_anchor;
        PointF m_currentDelta;

        List<Models::Visual::VisualNode^>^ m_nodes;
        List<PointF>^ m_originalPositions;
        List<float>^ m_radii;
    };
}