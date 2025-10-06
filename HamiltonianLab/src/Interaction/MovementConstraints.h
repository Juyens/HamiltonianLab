#pragma once

#include <Interaction/IViewportService.h>

namespace HamiltonianLab::Interaction
{
    using namespace System::Collections::Generic;
    using namespace System::Drawing;

    public ref class MovementConstraints
    {
    public:
        MovementConstraints(IViewportService^ viewportService);

        PointF ConstrainPoint(PointF point);
        PointF ConstrainNode(PointF point, float radius);
        RectangleF ConstrainRectangle(RectangleF rect);
        PointF ConstrainDelta(System::Collections::Generic::IEnumerable<PointF>^ origins, System::Collections::Generic::IEnumerable<float>^ radii, PointF desiredDelta);

    private:
        IViewportService^ m_viewportService;
    };
}