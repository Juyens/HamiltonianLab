#include "MovementConstraints.h"

namespace HamiltonianLab::Interaction
{
    using namespace System;

    MovementConstraints::MovementConstraints(IViewportService^ viewportService)
        : m_viewportService(viewportService)
    {
    }

    PointF MovementConstraints::ConstrainPoint(PointF point)
    {
        if (m_viewportService == nullptr)
            return point;

        auto bounds = m_viewportService->ViewportBounds;
        auto left = bounds.X;
        auto top = bounds.Y;
        auto right = bounds.X + bounds.Width;
        auto bottom = bounds.Y + bounds.Height;

        float x = static_cast<float>(Math::Max(left, Math::Min(right, point.X)));
        float y = static_cast<float>(Math::Max(top, Math::Min(bottom, point.Y)));
        return PointF(x, y);
    }

    PointF MovementConstraints::ConstrainNode(PointF point, float radius)
    {
        if (m_viewportService == nullptr)
            return point;

        auto bounds = m_viewportService->ViewportBounds;
        auto left = bounds.X + radius;
        auto top = bounds.Y + radius;
        auto right = bounds.X + bounds.Width - radius;
        auto bottom = bounds.Y + bounds.Height - radius;

        float x = static_cast<float>(Math::Max(left, Math::Min(right, point.X)));
        float y = static_cast<float>(Math::Max(top, Math::Min(bottom, point.Y)));
        return PointF(x, y);
    }

    RectangleF MovementConstraints::ConstrainRectangle(RectangleF rect)
    {
        if (m_viewportService == nullptr)
            return rect;

        PointF topLeft(Math::Min(rect.Left, rect.Right), Math::Min(rect.Top, rect.Bottom));
        PointF bottomRight(Math::Max(rect.Left, rect.Right), Math::Max(rect.Top, rect.Bottom));

        topLeft = ConstrainPoint(topLeft);
        bottomRight = ConstrainPoint(bottomRight);

        return RectangleF(topLeft.X, topLeft.Y,
            static_cast<float>(bottomRight.X - topLeft.X),
            static_cast<float>(bottomRight.Y - topLeft.Y));
    }

    PointF MovementConstraints::ConstrainDelta(IEnumerable<PointF>^ origins, IEnumerable<float>^ radii, PointF desiredDelta)
    {
        if (m_viewportService == nullptr || origins == nullptr || radii == nullptr)
            return desiredDelta;

        auto enumeratorOrigins = origins->GetEnumerator();
        auto enumeratorRadii = radii->GetEnumerator();

        bool hasAny = false;
        float minDx = -System::Single::PositiveInfinity;
        float maxDx = System::Single::PositiveInfinity;
        float minDy = -System::Single::PositiveInfinity;
        float maxDy = System::Single::PositiveInfinity;

        auto bounds = m_viewportService->ViewportBounds;
        auto left = bounds.X;
        auto top = bounds.Y;
        auto right = bounds.X + bounds.Width;
        auto bottom = bounds.Y + bounds.Height;

        while (enumeratorOrigins->MoveNext() && enumeratorRadii->MoveNext())
        {
            hasAny = true;
            PointF origin = enumeratorOrigins->Current;
            float radius = enumeratorRadii->Current;

            float localMinDx = (left + radius) - origin.X;
            float localMaxDx = (right - radius) - origin.X;
            float localMinDy = (top + radius) - origin.Y;
            float localMaxDy = (bottom - radius) - origin.Y;

            minDx = Math::Max(minDx, localMinDx);
            maxDx = Math::Min(maxDx, localMaxDx);
            minDy = Math::Max(minDy, localMinDy);
            maxDy = Math::Min(maxDy, localMaxDy);
        }

        if (!hasAny)
            return desiredDelta;

        float clampedDx = static_cast<float>(Math::Max(minDx, Math::Min(maxDx, desiredDelta.X)));
        float clampedDy = static_cast<float>(Math::Max(minDy, Math::Min(maxDy, desiredDelta.Y)));
        return PointF(clampedDx, clampedDy);
    }
}