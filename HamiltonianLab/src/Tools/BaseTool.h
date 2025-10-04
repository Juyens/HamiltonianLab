#pragma once

#include <Tools/ITool.h>
#include <Models/GraphDocument.h>

namespace HamiltonianLab
{
    using namespace HamiltonianLab::Models;

    public ref class BaseTool abstract : public ITool
    {
    public:
        BaseTool(GraphDocument^ doc) : m_doc(doc) {}

    protected:
        property GraphDocument^ Doc
        {
            GraphDocument^ get() { return m_doc; }
        }

    public:
        virtual void OnEnter() {}
        virtual void OnLeave() {}

        virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^) {}
        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^) {}
        virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs^) {}
        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^) {}

        virtual void OnRenderOverlay(System::Drawing::Graphics^) {}

        virtual System::Windows::Forms::Cursor^ GetCursor()
        { 
            return System::Windows::Forms::Cursors::Default;
        }

        virtual bool ConsumeRepaintRequest()
        {
            bool need = m_needsRepaint;
            m_needsRepaint = false;
            return need;
        }

    protected:
        void RequestRepaint() { m_needsRepaint = true; }

    private:
        GraphDocument^ m_doc;
        bool m_needsRepaint = false;
    };
}
