#pragma once

#include <Models/GraphDocument.h>
#include <Renderers/GraphCanvas.h>
#include <vector>

namespace HamiltonianLab::Services
{
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Windows::Forms;
    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Renderers;

    public ref class GraphAnalysisService
    {
    public:
        GraphAnalysisService(GraphCanvas^ canvas);

        void EnumerateHamiltonianCycles();
        void SolveTspBruteForce();

    private:
        GraphDocument^ GetDocument();
        bool TryGetNodeOrdering(std::vector<int>& nodes, std::vector<int>& rest, System::Collections::Generic::Dictionary<int, System::String^>^% labelMap);
        bool TryGetEdgeWeight(int u, int v, double% weight);
        void HighlightCycle(const std::vector<int>& cycle);
        void ShowStatusMessage(System::String^ text);
        void DisplayCycleSummary(int count, double minCost, double maxCost, System::Collections::Generic::List<System::String^>^ cycles);
        System::String^ BuildCycleLabel(const std::vector<int>& cycle, System::Collections::Generic::Dictionary<int, System::String^>^ labelMap);

    private:
        GraphCanvas^ m_canvas;
    };
}