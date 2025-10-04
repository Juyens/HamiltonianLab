#pragma once

#include <Common/MenuCommand.h>
#include <Renderers/GraphCanvas.h>
#include <Services/RandomGraphGenerator.h>
#include <Tools/RandomGraphTool.h>

namespace HamiltonianLab::Controllers
{
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace HamiltonianLab;
    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Renderers;
    using namespace HamiltonianLab::Services;
    using namespace HamiltonianLab::Tools;

    public ref class MenuCommandController
    {
    public:
        MenuCommandController(
            GraphCanvas^ canvas,
            GraphDocument^ document,
            ToolStripMenuItem^ itemNewGraph,
            ToolStripMenuItem^ itemNewGraphRandom,
            ToolStripMenuItem^ itemExit,
            ToolStripMenuItem^ itemFindCycles,
            ToolStripMenuItem^ itemSolveTsp,
            ToolStripMenuItem^ itemShowMatrix,
            RandomGraphTool^ randomTool,
            RandomGraphGenerator^ randomGenerator,
            System::Action^ newGraphAction,
            System::Action^ exitAction,
            System::Action^ enumerateCyclesAction,
            System::Action^ solveTspAction,
            System::Action^ showMatrixAction);

        void WireUp();

    private:
        void OnNewGraph(System::Object^ sender, System::EventArgs^ e);
        void OnNewRandomGraph(System::Object^ sender, System::EventArgs^ e);
        void OnExit(System::Object^ sender, System::EventArgs^ e);
        void OnEnumerateCycles(System::Object^ sender, System::EventArgs^ e);
        void OnSolveTsp(System::Object^ sender, System::EventArgs^ e);
        void OnShowMatrix(System::Object^ sender, System::EventArgs^ e);

        void AttachHandler(ToolStripMenuItem^ item, System::EventHandler^ handler);

    private:
        GraphCanvas^ m_canvas;
        GraphDocument^ m_document;
        ToolStripMenuItem^ m_itemNewGraph;
        ToolStripMenuItem^ m_itemNewGraphRandom;
        ToolStripMenuItem^ m_itemExit;
        ToolStripMenuItem^ m_itemFindCycles;
        ToolStripMenuItem^ m_itemSolveTsp;
        ToolStripMenuItem^ m_itemShowMatrix;
        RandomGraphTool^ m_randomTool;
        RandomGraphGenerator^ m_randomGenerator;
        System::Action^ m_newGraphAction;
        System::Action^ m_exitAction;
        System::Action^ m_enumerateCyclesAction;
        System::Action^ m_solveTspAction;
        System::Action^ m_showMatrixAction;
        bool m_wired;
    };
}