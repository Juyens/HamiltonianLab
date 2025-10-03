#pragma once

#include <Interaction/IInvalidationSink.h>
#include <Interaction/MovementConstraints.h>
#include <Models/GraphDocument.h>

namespace HamiltonianLab::Interaction
{
    using namespace System::Drawing;

    public enum class MarqueeSelectionMode
    {
        Replace,
        Add,
        Toggle
    };

    public ref class MarqueeSelectionController
    {
    public:
        MarqueeSelectionController(Models::GraphDocument^ document,
            MovementConstraints^ constraints,
            IInvalidationSink^ invalidationSink);

        property bool IsActive
        {
            bool get();
        }

        property RectangleF CurrentRectangle
        {
            RectangleF get();
        }

        void Begin(PointF anchor, MarqueeSelectionMode mode);
        void Update(PointF current);
        void Complete();
        void Cancel();

    private:
        RectangleF NormalizeRectangle(RectangleF rect);
        System::Drawing::Rectangle InflateForDisplay(RectangleF rect);
        void InvalidateOverlay(RectangleF previousRect, RectangleF nextRect);

    private:
        Models::GraphDocument^ m_document;
        MovementConstraints^ m_constraints;
        IInvalidationSink^ m_invalidationSink;
        bool m_active;
        PointF m_anchor;
        RectangleF m_rect;
        MarqueeSelectionMode m_mode;
    };
}