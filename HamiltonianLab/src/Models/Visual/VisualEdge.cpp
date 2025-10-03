#include "VisualEdge.h"

namespace HamiltonianLab::Models::Visual
{
    VisualEdge::VisualEdge(int logicalId, int u, int v)
    {
        LogicalId = logicalId;
        FromLogicalId = u;
        ToLogicalId = v;
        Kind = EdgeVisualKind::Straight;
        StrokeWidth = 1.0f;
        Stroke = Color::Black;
        Weight = 0.0;
        Label = nullptr;
        LabelPos = PointF(0, 0);
        LabelFontSize = 9.0f;
        IsDirected = false;
        Selected = false;
        Hovered = false;
    }

    bool VisualEdge::HitTestSegment(PointF p, PointF a, PointF b, float tolerance)
    {
        const float vx = b.X - a.X, vy = b.Y - a.Y;
        const float wx = p.X - a.X, wy = p.Y - a.Y;
        const float vv = vx * vx + vy * vy;
        float t = vv > 0 ? (wx * vx + wy * vy) / vv : 0.0f;
        if (t < 0.0f) t = 0.0f; else if (t > 1.0f) t = 1.0f;

        const float qx = a.X + t * vx, qy = a.Y + t * vy;
        const float dx = p.X - qx, dy = p.Y - qy;
        return (dx * dx + dy * dy) <= (tolerance * tolerance);
    }
}