#pragma once

namespace HamiltonianLab
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

    public interface class ITool
    {
        void OnEnter();
        void OnLeave();

        void OnMouseDown(MouseEventArgs^ e);
        void OnMouseMove(MouseEventArgs^ e);
        void OnMouseUp(MouseEventArgs^ e);
        void OnKeyDown(KeyEventArgs^ e);

        void OnRenderOverlay(Graphics^ g);

        bool ConsumeRepaintRequest();

        Cursor^ GetCursor();
    };
}