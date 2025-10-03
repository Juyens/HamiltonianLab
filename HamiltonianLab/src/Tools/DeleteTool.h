#pragma once

#include <Tools/BaseTool.h>
#include <Models/GraphDocument.h>

namespace HamiltonianLab
{
    public ref class DeleteTool : public BaseTool
    {
    public:
        DeleteTool(GraphDocument^ doc) : BaseTool(doc) {}

        virtual System::Windows::Forms::Cursor^ GetCursor() override
        {
            return System::Windows::Forms::Cursors::No;
        }
    };
}
