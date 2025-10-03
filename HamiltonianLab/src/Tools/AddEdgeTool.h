#pragma once

#include <Tools/BaseTool.h>
#include <Models/GraphDocument.h>

namespace HamiltonianLab
{
    public ref class AddEdgeTool : public BaseTool
    {
    public:
        AddEdgeTool(GraphDocument^ doc) : BaseTool(doc) {}

        virtual void OnEnter() override { m_from = nullptr; }
        virtual void OnLeave() override { m_from = nullptr; }

        virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override
        {
            if (e->Button == System::Windows::Forms::MouseButtons::Right)
            {
                m_from = nullptr;
                return;
            }

            if (e->Button != System::Windows::Forms::MouseButtons::Left)
                return;


            System::Drawing::PointF p((float)e->X, (float)e->Y);
            auto hit = Doc->GetNodeAt(p);

            if (!hit)
            {
                m_from = nullptr;
                return;
            }

            if (m_from == nullptr)
            {
                m_from = hit;
            }
            else if (hit != m_from)
            {
                Doc->AddEdgeByLogicalIds(m_from->LogicalId, hit->LogicalId, 1.0);
                m_from = hit;
            }
        }

        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override
        {
            m_lastPos = System::Drawing::PointF((float)e->X, (float)e->Y);
        }

        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override
        {
            if (e->KeyCode == System::Windows::Forms::Keys::Escape) m_from = nullptr;
        }

        virtual void OnRenderOverlay(System::Drawing::Graphics^ g) override
        {
            if (m_from != nullptr)
            {
                g->DrawLine(System::Drawing::Pens::LightGray, m_from->Position, m_lastPos);
            }
        }

        virtual System::Windows::Forms::Cursor^ GetCursor() override
        {
            return System::Windows::Forms::Cursors::SizeNESW;
        }

    private:
        HamiltonianLab::Models::Visual::VisualNode^ m_from = nullptr;
        System::Drawing::PointF m_lastPos;
    };
}