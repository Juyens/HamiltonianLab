#pragma once

namespace HamiltonianLab::Interaction
{
    using namespace System::Drawing;

    public interface class IViewportService
    {
    public:
        virtual property RectangleF ViewportBounds
        {
            RectangleF get() = 0;
        }
    };
}