#include "MenuCommandController.h"

namespace HamiltonianLab::Controllers
{
    MenuCommandController::MenuCommandController(
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
        System::Action^ showMatrixAction)
        : m_canvas(canvas)
        , m_document(document)
        , m_itemNewGraph(itemNewGraph)
        , m_itemNewGraphRandom(itemNewGraphRandom)
        , m_itemExit(itemExit)
        , m_itemFindCycles(itemFindCycles)
        , m_itemSolveTsp(itemSolveTsp)
        , m_itemShowMatrix(itemShowMatrix)
        , m_randomTool(randomTool)
        , m_randomGenerator(randomGenerator)
        , m_newGraphAction(newGraphAction)
        , m_exitAction(exitAction)
        , m_enumerateCyclesAction(enumerateCyclesAction)
        , m_solveTspAction(solveTspAction)
        , m_showMatrixAction(showMatrixAction)
        , m_wired(false)
    {
    }

    void MenuCommandController::WireUp()
    {
        if (m_wired)
            return;

        AttachHandler(m_itemNewGraph, gcnew EventHandler(this, &MenuCommandController::OnNewGraph));
        AttachHandler(m_itemNewGraphRandom, gcnew EventHandler(this, &MenuCommandController::OnNewRandomGraph));
        AttachHandler(m_itemExit, gcnew EventHandler(this, &MenuCommandController::OnExit));
        AttachHandler(m_itemFindCycles, gcnew EventHandler(this, &MenuCommandController::OnEnumerateCycles));
        AttachHandler(m_itemSolveTsp, gcnew EventHandler(this, &MenuCommandController::OnSolveTsp));
        AttachHandler(m_itemShowMatrix, gcnew EventHandler(this, &MenuCommandController::OnShowMatrix));

        m_wired = true;
    }

    void MenuCommandController::AttachHandler(ToolStripMenuItem^ item, EventHandler^ handler)
    {
        if (!item || !handler)
            return;

        item->Click += handler;
    }

    void MenuCommandController::OnNewGraph(Object^ sender, EventArgs^ e)
    {
        if (m_newGraphAction != nullptr)
            m_newGraphAction->Invoke();
    }

    void MenuCommandController::OnNewRandomGraph(Object^ sender, EventArgs^ e)
    {
        if (!m_randomTool || !m_randomGenerator || !m_document)
            return;

        int nodeCount = 0;
        if (!m_randomTool->TryGetNodeCount(nodeCount))
            return;

        m_randomGenerator->Generate(m_document, nodeCount);

        if (m_canvas)
            m_canvas->Invalidate();
    }

    void MenuCommandController::OnExit(Object^ sender, EventArgs^ e)
    {
        if (m_exitAction != nullptr)
            m_exitAction->Invoke();
    }

    void MenuCommandController::OnEnumerateCycles(Object^ sender, EventArgs^ e)
    {
        if (m_enumerateCyclesAction != nullptr)
            m_enumerateCyclesAction->Invoke();
    }

    void MenuCommandController::OnSolveTsp(Object^ sender, EventArgs^ e)
    {
        if (m_solveTspAction != nullptr)
            m_solveTspAction->Invoke();
    }

    void MenuCommandController::OnShowMatrix(Object^ sender, EventArgs^ e)
    {
        if (m_showMatrixAction != nullptr)
            m_showMatrixAction->Invoke();
    }
}