#pragma once

namespace HamiltonianLab::Models::Visual
{
    using namespace System::Drawing;

    public ref class VisualStyle sealed
    {
    public:
        Color Fill = Color::LightBlue;
        Color Stroke = Color::Black;
        float StrokeWidth = 1.0f;
        Color LabelColor = Color::Black;
        System::String^ LabelFontName = L"Segoe UI";
        float LabelFontSize = 9.0f;
    };

    public ref class VisualNode sealed
    {
    public:
        property int LogicalId;
        property PointF Position;
        property float Radius;
        property System::String^ Label;
        property VisualStyle^ Style;
        property bool Selected;
        property bool Hovered;

        VisualNode(int logicalId, PointF pos, float radius);
        bool HitTest(PointF p, float extraTolerance);
        RectangleF GetBounds(float inflate);
    };
}
