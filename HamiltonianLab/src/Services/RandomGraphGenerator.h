#pragma once

#include <Models/GraphDocument.h>
#include <Renderers/GraphCanvas.h>

namespace HamiltonianLab::Services
{
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Drawing;
    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Renderers;

    public ref class RandomGraphGenerator
    {
    public:
        RandomGraphGenerator(GraphCanvas^ canvas);

        void Generate(GraphDocument^ doc, int nodeCount);

    private:
        void ClearDocument(GraphDocument^ doc);
        PointF CreateRandomPoint(System::Drawing::Rectangle clientArea, int margin);

    private:
        GraphCanvas^ m_canvas;
        System::Random^ m_random;
    };
}
