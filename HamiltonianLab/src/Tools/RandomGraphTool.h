#pragma once

#include <Renderers/GraphCanvas.h>
#include <Services/RandomGraphGenerator.h>

namespace HamiltonianLab::Tools
{
    using namespace System;
    using namespace System::Globalization;
    using namespace System::Windows::Forms;
    using namespace HamiltonianLab::Renderers;
    using namespace HamiltonianLab::Services;

    public ref class RandomGraphTool
    {
    public:
        RandomGraphTool(
            ToolStripButton^ generateButton,
            ToolStripTextBox^ nodeCountTextBox,
            GraphCanvas^ canvas,
            RandomGraphGenerator^ generator);

        void WireUp();
        bool TryGetNodeCount(int% nodeCount);

    private:
        void OnGenerateClicked(System::Object^ sender, System::EventArgs^ e);
        bool ValidateNodeCount(int value);
        void ShowValidationError();

    private:
        ToolStripButton^ m_generateButton;
        ToolStripTextBox^ m_nodeCountTextBox;
        GraphCanvas^ m_canvas;
        RandomGraphGenerator^ m_generator;
        bool m_wired;
    };
}