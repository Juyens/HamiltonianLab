#pragma once

#include <Models/Visual/VisualNode.h>

namespace HamiltonianLab::Models::Visual
{
    using namespace System;
    using namespace System::Drawing;

    public enum class EdgeVisualKind
    {
        Straight
    };

    public ref class VisualEdge sealed
    {
    public:
        property int LogicalId;
        property int FromLogicalId;
        property int ToLogicalId;

        property EdgeVisualKind Kind;
        property float StrokeWidth;
        property Color Stroke;
        property double Weight;
        property System::String^ Label;
        property PointF LabelPos;
        property bool IsDirected;
        property bool Selected;
        property bool Hovered;

        VisualEdge(int logicalId, int u, int v);
        static bool HitTestSegment(PointF p, PointF a, PointF b, float tolerance);
    };
}