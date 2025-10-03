#pragma once

#include <Tools/BaseTool.h>

namespace HamiltonianLab
{
    public ref class AddNodeTool : public BaseTool
    {
    public:
        AddNodeTool(GraphDocument^ doc) : BaseTool(doc) {}

        virtual void OnEnter() override {}
        virtual void OnLeave() override {}

        virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override
        {
            if (e->Button != System::Windows::Forms::MouseButtons::Left) 
                return;

            System::Drawing::PointF p((float)e->X, (float)e->Y);

            if (Doc->GetNodeAt(p) != nullptr) 
                return;

            Doc->AddNode(p, 0.0f, nullptr);
        }

        virtual System::Windows::Forms::Cursor^ GetCursor() override
        {
            return System::Windows::Forms::Cursors::Cross;
        }
    };
}