#pragma once

#include <Models/GraphDocument.h>
#include <Renderers/GridPainter.h>
#include <Tools/ITool.h>

namespace HamiltonianLab::Renderers
{
    using namespace System::Drawing;

    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Models::Visual;

    public ref class SceneRenderer
    {
    public:
        SceneRenderer(GridPainter^ gridPainter);

        void Render(Graphics^ g, System::Drawing::Size clientSize, GraphDocument^ document, HamiltonianLab::ITool^ activeTool);

    private:
        void DrawEdges(Graphics^ g, VisualGraph^ visual, HamiltonianLab::ITool^ activeTool);
        void DrawEdgeLabel(Graphics^ g, VisualEdge^ edge, VisualNode^ from, VisualNode^ to, HamiltonianLab::ITool^ activeTool);
        void DrawNodes(Graphics^ g, VisualGraph^ visual);

    private:
        GridPainter^ m_gridPainter;
    };
}