#include "SceneRenderer.h"

namespace HamiltonianLab::Renderers
{
    using namespace System::Drawing::Drawing2D;

    SceneRenderer::SceneRenderer(GridPainter^ gridPainter)
        : m_gridPainter(gridPainter)
    {
    }

    void SceneRenderer::Render(Graphics^ g, System::Drawing::Size clientSize, GraphDocument^ document, HamiltonianLab::ITool^ activeTool)
    {
        if (!g)
            return;

        g->SmoothingMode = SmoothingMode::AntiAlias;

        if (m_gridPainter)
        {
            m_gridPainter->Render(g, clientSize);
        }

        if (document && document->Visual)
        {
            DrawEdges(g, document->Visual);
            DrawNodes(g, document->Visual);
        }

        if (document && document->Selection)
        {
            document->Selection->Render(g);
        }

        if (activeTool)
        {
            activeTool->OnRenderOverlay(g);
        }
    }

    void SceneRenderer::DrawEdges(Graphics^ g, VisualGraph^ visual)
    {
        if (!visual)
            return;

        for each (auto edge in visual->Edges)
        {
            auto from = visual->FindNodeByLogicalId(edge->FromLogicalId);
            auto to = visual->FindNodeByLogicalId(edge->ToLogicalId);
            if (!from || !to)
                continue;

            Color stroke = edge->Stroke;
            float width = edge->StrokeWidth;
            if (edge->Selected)
            {
                stroke = Color::DodgerBlue;
                width += 2.0f;
            }
            else if (edge->Hovered)
            {
                stroke = Color::SteelBlue;
                width += 1.0f;
            }

            auto pen = gcnew Pen(stroke, width <= 0 ? 1.0f : width);
            pen->StartCap = LineCap::Round;
            pen->EndCap = LineCap::Round;
            g->DrawLine(pen, from->Position, to->Position);
            delete pen;

            DrawEdgeLabel(g, edge, from, to);
        }
    }

    void SceneRenderer::DrawEdgeLabel(Graphics^ g, VisualEdge^ edge, VisualNode^ from, VisualNode^ to)
    {
        if (!edge)
            return;

        if (!edge->Label || edge->Label->Length == 0)
            return;

        PointF labelPosition = edge->LabelPos;
        if (labelPosition.IsEmpty)
        {
            labelPosition = PointF(
                (from->Position.X + to->Position.X) * 0.5f,
                (from->Position.Y + to->Position.Y) * 0.5f);
        }

        auto font = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Regular, GraphicsUnit::Point);
        auto format = gcnew StringFormat();
        format->Alignment = StringAlignment::Center;
        format->LineAlignment = StringAlignment::Center;

        RectangleF bounds(labelPosition.X - 20.0f, labelPosition.Y - 10.0f, 40.0f, 20.0f);

        auto bg = gcnew SolidBrush(Color::FromArgb(200, Color::White));
        auto border = gcnew Pen(Color::FromArgb(160, Color::Black), 1.0f);
        auto textBrush = gcnew SolidBrush(Color::Black);

        g->FillRectangle(bg, bounds);
        g->DrawRectangle(border, Rectangle::Round(bounds));
        g->DrawString(edge->Label, font, textBrush, bounds, format);

        delete textBrush;
        delete border;
        delete bg;
        delete format;
        delete font;
    }

    void SceneRenderer::DrawNodes(Graphics^ g, VisualGraph^ visual)
    {
        if (!visual)
            return;

        for each (auto node in visual->Nodes)
        {
            auto style = node->Style;
            Color fill = style->Fill;
            Color stroke = style->Stroke;
            float strokeWidth = style->StrokeWidth;

            if (node->Selected)
            {
                stroke = Color::DodgerBlue;
                strokeWidth += 2.0f;
            }
            else if (node->Hovered)
            {
                stroke = Color::SteelBlue;
                strokeWidth += 1.0f;
            }

            auto rect = node->GetBounds(0.0f);
            auto brush = gcnew SolidBrush(fill);
            auto pen = gcnew Pen(stroke, strokeWidth <= 0 ? 1.0f : strokeWidth);
            g->FillEllipse(brush, rect);
            g->DrawEllipse(pen, rect);

            if (node->Label != nullptr && node->Label->Length > 0)
            {
                auto font = gcnew System::Drawing::Font(style->LabelFontName, style->LabelFontSize);
                auto format = gcnew StringFormat();
                format->Alignment = StringAlignment::Center;
                format->LineAlignment = StringAlignment::Center;
                auto textBrush = gcnew SolidBrush(style->LabelColor);
                g->DrawString(node->Label, font, textBrush, rect, format);
                delete textBrush;
                delete format;
                delete font;
            }

            delete pen;
            delete brush;
        }
    }
}