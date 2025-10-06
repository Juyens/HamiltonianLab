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
    using namespace System::Diagnostics;
    using namespace System::Globalization;
    using namespace System::Threading::Tasks;

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
        void ShowStatusMessage(System::String^ text);
        void HighlightCycle(const std::vector<int>& cycle);
        void ShowMessageBox(System::String^ text, System::String^ caption, MessageBoxButtons buttons, MessageBoxIcon icon);
        void DisplayCycleSummary(int count, double minCost, double maxCost, System::Collections::Generic::List<System::String^>^ cycles);
        System::String^ BuildCycleLabel(const std::vector<int>& cycle, System::Collections::Generic::Dictionary<int, System::String^>^ labelMap);

        void RunEnumerateHamiltonianCyclesTask();
        void RunSolveTspTask();
        void CompleteHamiltonianEnumeration(int count, double minCost, double maxCost, System::Collections::Generic::List<System::String^>^ cycles);
        void CompleteTspSolution(System::Collections::Generic::List<int>^ cycleNodes, double totalCost, System::TimeSpan duration, System::String^ pathLabel);
        void EnumerateHamiltonianCyclesRecursive(const std::vector<int>& nodes, const std::vector<int>& rest, System::Collections::Generic::Dictionary<int, int>^ indexLookup, std::vector<bool>& used, std::vector<int>& current, int startNode, double currentCost, double& bestBound, int& cycleCount, double& minCost, double& maxCost, System::Collections::Generic::List<System::String^>^ listedCycles, System::Collections::Generic::Dictionary<int, System::String^>^ labelMap, const std::vector<double>& weights, int dimension, int& statusCounter, int statusThreshold, long long& exploredCount);
        int GetNodeIndex(System::Collections::Generic::Dictionary<int, int>^ indexLookup, int nodeId);

        delegate void ShowMessageBoxDelegate(System::String^ text, System::String^ caption,
            System::Windows::Forms::MessageBoxButtons buttons, System::Windows::Forms::MessageBoxIcon icon);
        delegate void SetStatusDelegate(System::String^ text);
        delegate void CompleteHamiltonianEnumerationDelegate(int count, double minCost, double maxCost,
            System::Collections::Generic::List<System::String^>^ cycles);
        delegate void CompleteTspSolutionDelegate(System::Collections::Generic::List<int>^ cycleNodes,
            double totalCost, System::TimeSpan duration, System::String^ pathLabel);

    private:
        GraphCanvas^ m_canvas;
    };
}