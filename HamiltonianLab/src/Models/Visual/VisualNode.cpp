#include "VisualNode.h"

namespace HamiltonianLab::Models::Visual
{
    VisualNode::VisualNode(int logicalId, PointF pos, float radius)
    {
        LogicalId = logicalId;
        Position = pos;
        Radius = radius;
        Style = gcnew VisualStyle();
        Label = nullptr;
        Selected = false;
        Hovered = false;
    }

    bool VisualNode::HitTest(PointF p, float extraTolerance)
    {
        const float r = Radius + extraTolerance;
        const float dx = p.X - Position.X;
        const float dy = p.Y - Position.Y;
        return (dx * dx + dy * dy) <= (r * r);
    }

    RectangleF VisualNode::GetBounds(float inflate)
    {
        float r = Radius + inflate;
        return RectangleF(Position.X - r, Position.Y - r, 2 * r, 2 * r);
    }
}
