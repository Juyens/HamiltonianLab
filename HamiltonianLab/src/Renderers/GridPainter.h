#pragma once

namespace HamiltonianLab::Renderers
{
    using namespace System;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;

    public enum class GridOrient { Vertical, Horizontal };

    public ref class GridPainter
    {
    public:
        property bool Show
        {
            bool get() { return m_show; }
            void set(bool v) { m_show = v; }
        }

        property int Step 
        {
            int get() { return m_step; }
            void set(int v) { m_step = Math::Max(4, v); }
        }

        property int MajorEvery 
        {
            int get() { return m_majorEvery; }
            void set(int v) { m_majorEvery = Math::Max(1, v); }
        }

        property Color MinorColor
        {
            Color get() { return m_minorColor; }
            void set(Color value)
            {
                if (m_minorColor.ToArgb() == value.ToArgb())
                    return;

                m_minorColor = value;
                RebuildPens();
            }
        }

        property Color MajorColor
        {
            Color get() { return m_majorColor; }
            void set(Color value)
            {
                if (m_majorColor.ToArgb() == value.ToArgb()) 
                    return;

                m_majorColor = value;
                RebuildPens();
            }
        }

    public:
        GridPainter();
        ~GridPainter();

        void Render(Graphics^ g, Size clientSize);
        PointF Snap(PointF p);

    private:
        void Initialize();
        void DrawGridLines(Graphics^ g, GridOrient orient, int primaryLen, int secondaryLen);
        GraphicsState^ PushCrispLines(Graphics^ g);
        void Pop(Graphics^ g, GraphicsState^ st);
        void RebuildPens();

    private:
        bool m_show;
        int m_step;
        int m_majorEvery;

        Pen^ m_pMinor;
        Pen^ m_pMajor;

        Color m_minorColor;
        Color m_majorColor;
    };
}
