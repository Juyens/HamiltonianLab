#pragma once

#include <Tools/BaseTool.h>
#include <Models/GraphDocument.h>

namespace HamiltonianLab
{
    public ref class SelectTool : public BaseTool
    {
    public:
        SelectTool(GraphDocument^ doc) : BaseTool(doc) {}

        virtual void OnEnter() override { m_dragging = false; }
        virtual void OnLeave() override { m_dragging = false; }

        virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override
        {
            if (e->Button != System::Windows::Forms::MouseButtons::Left) return;

            m_anchor = System::Drawing::PointF((float)e->X, (float)e->Y);
            auto selection = Doc->Selection;
            auto hit = Doc->GetNodeAt(m_anchor);
            HamiltonianLab::Models::Visual::VisualEdge^ hitEdge = nullptr;
            if (!hit)
                hitEdge = Doc->GetEdgeAt(m_anchor);

            const bool shift = (Control::ModifierKeys & System::Windows::Forms::Keys::Shift) == System::Windows::Forms::Keys::Shift;
            const bool ctrl = (Control::ModifierKeys & System::Windows::Forms::Keys::Control) == System::Windows::Forms::Keys::Control;

            if (hit)
            {
                if (!shift && !ctrl)
                {
                    selection->Clear();
                    selection->Add(hit);
                }
                else if (ctrl)
                {
                    selection->Toggle(hit);
                }
                else if (shift)
                {
                    selection->Add(hit);
                }
                m_dragging = false;
            }
            else if (hitEdge)
            {
                if (!shift && !ctrl)
                {
                    selection->Clear();
                    selection->Add(hitEdge);
                }
                else if (ctrl)
                {
                    selection->Toggle(hitEdge);
                }
                else if (shift)
                {
                    selection->Add(hitEdge);
                }
                m_dragging = false;
            }
            else
            {
                m_dragging = true;
                m_rect = System::Drawing::RectangleF(m_anchor.X, m_anchor.Y, 0, 0);
                if (!shift && !ctrl)
                {
                    selection->Clear();
                }
            }
        }

        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override
        {
            if (!m_dragging) return;
            System::Drawing::PointF p((float)e->X, (float)e->Y);
            float x = System::Math::Min(m_anchor.X, p.X);
            float y = System::Math::Min(m_anchor.Y, p.Y);
            float w = System::Math::Abs(p.X - m_anchor.X);
            float h = System::Math::Abs(p.Y - m_anchor.Y);
            m_rect = System::Drawing::RectangleF(x, y, w, h);
        }

        virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs^ e) override
        {
            if (!m_dragging) return;
            m_dragging = false;

            const bool shift = (Control::ModifierKeys & System::Windows::Forms::Keys::Shift) == System::Windows::Forms::Keys::Shift;
            const bool ctrl = (Control::ModifierKeys & System::Windows::Forms::Keys::Control) == System::Windows::Forms::Keys::Control;

            auto hits = Doc->Visual->QueryNodesIn(m_rect);
            auto selection = Doc->Selection;
            for each (auto n in hits)
            {
                if (ctrl)
                    selection->Toggle(n);
                else
                    selection->Add(n);
            }
        }

        virtual void OnRenderOverlay(System::Drawing::Graphics^ g) override
        {
            if (!m_dragging) return;

            System::Drawing::Rectangle rect(
                System::Math::Min(m_rect.X, m_rect.X + m_rect.Width),
                System::Math::Min(m_rect.Y, m_rect.Y + m_rect.Height),
                System::Math::Abs(m_rect.Width),
                System::Math::Abs(m_rect.Height)
            );

            auto hi = System::Drawing::SystemColors::Highlight;
            auto fill = System::Drawing::Color::FromArgb(64, hi);
            auto border1 = System::Drawing::Color::FromArgb(200, hi);
            auto border2 = System::Drawing::Color::FromArgb(160, System::Drawing::Color::White);

            auto bg = gcnew System::Drawing::SolidBrush(fill);
            g->FillRectangle(bg, rect);

            auto p1 = gcnew System::Drawing::Pen(border1, 1.0f);
            p1->Alignment = System::Drawing::Drawing2D::PenAlignment::Inset;
            g->DrawRectangle(p1, rect);

            System::Drawing::Rectangle inner(rect.X + 1, rect.Y + 1,
                System::Math::Max(0, rect.Width - 2),
                System::Math::Max(0, rect.Height - 2));

            if (inner.Width > 0 && inner.Height > 0)
            {
                auto p2 = gcnew System::Drawing::Pen(border2, 1.0f);
                p2->Alignment = System::Drawing::Drawing2D::PenAlignment::Inset;
                g->DrawRectangle(p2, inner);
                delete p2;
            }

            delete p1;
            delete bg;
        }

        virtual void OnKeyDown(KeyEventArgs^ e) override
        {
            if (!Doc || !Doc->Selection) return;

            if (e->KeyCode == System::Windows::Forms::Keys::Back)
            {
                Doc->Selection->DeleteSelection();
                RequestRepaint();
                e->Handled = true;
                return;
            }

            if (e->KeyCode == System::Windows::Forms::Keys::Delete)
            {
                Doc->Selection->DeleteSelection();
                RequestRepaint();
                e->Handled = true;
                return;
            }

            if (e->KeyCode == System::Windows::Forms::Keys::Escape)
            {
                Doc->Selection->Clear();
                RequestRepaint();
                e->Handled = true;
                return;
            }
        }

        virtual System::Windows::Forms::Cursor^ GetCursor() override
        {
            return System::Windows::Forms::Cursors::Hand;
        }

    private:
        bool m_dragging{ false };
        System::Drawing::PointF m_anchor;
        System::Drawing::RectangleF m_rect;
    };
}