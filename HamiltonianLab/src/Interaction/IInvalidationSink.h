#pragma once

namespace HamiltonianLab::Interaction
{
    public interface class IInvalidationSink
    {
    public:
        virtual void RequestInvalidate() = 0;
        virtual void RequestInvalidate(System::Drawing::Rectangle rect) = 0;
    };
}