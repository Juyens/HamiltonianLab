#include "GridPainter.h"

namespace HamiltonianLab::Renderers
{
    GridPainter::GridPainter()
        : m_pMinor(nullptr)
        , m_pMajor(nullptr)
        , m_show(true)
        , m_step(24)
        , m_majorEvery(5)
        , m_minorColor(Color::FromArgb(40, 0, 0, 0))
        , m_majorColor(Color::FromArgb(80, 0, 0, 0))
    {
        Initialize();
    }

    GridPainter::~GridPainter()
    {
        delete m_pMinor; m_pMinor = nullptr;
        delete m_pMajor; m_pMajor = nullptr;
    }

    void GridPainter::Render(Graphics^ g, Size clientSize)
    {
        if (!m_show || clientSize.Width <= 0 || clientSize.Height <= 0)
            return;

        GraphicsState^ st = PushCrispLines(g);
        try
        {
            DrawGridLines(g, GridOrient::Vertical, clientSize.Width, clientSize.Height);
            DrawGridLines(g, GridOrient::Horizontal, clientSize.Height, clientSize.Width);
        }
        finally
        {
            Pop(g, st);
        }
    }

    PointF GridPainter::Snap(PointF p)
    {
        float s = static_cast<float>(Math::Max(4, m_step));
        float x = static_cast<float>(Math::Round(p.X / s)) * s;
        float y = static_cast<float>(Math::Round(p.Y / s)) * s;
        return PointF(x, y);
    }

    void GridPainter::Initialize()
    {
        RebuildPens();
    }

    void GridPainter::DrawGridLines(Graphics^ g, GridOrient orient, int primaryLen, int secondaryLen)
    {
        int step = Math::Max(4, m_step);
        int majorEach = Math::Max(1, m_majorEvery);
        int idx = 0;
        float secondary = static_cast<float>(secondaryLen);

        for (int pos = 0; pos <= primaryLen; pos += step, ++idx)
        {
            bool major = ((idx % majorEach) == 0);
            Pen^ p = major ? m_pMajor : m_pMinor;

            if (orient == GridOrient::Vertical)
            {
                float x = static_cast<float>(pos) + 0.5f;
                g->DrawLine(p, x, 0.0f, x, secondary);
            }
            else
            {
                float y = static_cast<float>(pos) + 0.5f;
                g->DrawLine(p, 0.0f, y, secondary, y);
            }
        }
    }

    GraphicsState^ GridPainter::PushCrispLines(Graphics^ g)
    {
        GraphicsState^ st = g->Save();
        g->SmoothingMode = SmoothingMode::None;
        g->PixelOffsetMode = PixelOffsetMode::Half;
        return st;
    }

    void GridPainter::Pop(Graphics^ g, GraphicsState^ st)
    {
        if (st != nullptr)
            g->Restore(st);
    }

    void GridPainter::RebuildPens()
    {
        if (m_pMinor) { delete m_pMinor; m_pMinor = nullptr; }
        if (m_pMajor) { delete m_pMajor; m_pMajor = nullptr; }

        m_pMinor = gcnew Pen(m_minorColor, 1.0f);
        m_pMinor->Alignment = PenAlignment::Center;

        m_pMajor = gcnew Pen(m_majorColor, 1.0f);
        m_pMajor->Alignment = PenAlignment::Center;
    }
}
