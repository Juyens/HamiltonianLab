#include "GraphAnalysisService.h"

#include <algorithm>
#include <limits>

namespace HamiltonianLab::Services
{
    namespace
    {
        System::String^ FormatCost(double value)
        {
            return value.ToString("F2", CultureInfo::InvariantCulture);
        }
    }

    GraphAnalysisService::GraphAnalysisService(GraphCanvas^ canvas)
        : m_canvas(canvas)
    {
    }

    GraphDocument^ GraphAnalysisService::GetDocument()
    {
        return (m_canvas != nullptr) ? m_canvas->Document : nullptr;
    }

    bool GraphAnalysisService::TryGetNodeOrdering(std::vector<int>& nodes, std::vector<int>& rest, Dictionary<int, System::String^>^% labelMap)
    {
        nodes.clear();
        rest.clear();
        labelMap = nullptr;

        auto document = GetDocument();
        if (!document || !document->Visual || !document->Visual->Nodes)
            return false;

        auto visualNodes = document->Visual->Nodes;
        if (visualNodes->Count <= 0)
            return false;

        nodes.reserve(visualNodes->Count);
        labelMap = gcnew Dictionary<int, System::String^>();
        for each (auto node in visualNodes)
        {
            if (!node)
                continue;
            nodes.push_back(node->LogicalId);
            labelMap[node->LogicalId] = node->Label != nullptr ? node->Label : node->LogicalId.ToString();
        }

        if (nodes.size() < 2)
            return false;

        std::sort(nodes.begin(), nodes.end());
        rest.assign(nodes.begin() + 1, nodes.end());

        return !rest.empty();
    }

    bool GraphAnalysisService::TryGetEdgeWeight(int u, int v, double% weight)
    {
        weight = 0.0;
        auto document = GetDocument();
        if (!document || !document->Core)
            return false;

        auto core = document->Core;
        if (!core->HasEdge(u, v))
            return false;

        weight = core->GetWeight(u, v);
        return true;
    }

    void GraphAnalysisService::ShowStatusMessage(System::String^ text)
    {
        if (!m_canvas)
            return;

        if (m_canvas->InvokeRequired)
        {
            array<System::Object^>^ args = gcnew array<System::Object^>(1);
            args[0] = text;
            m_canvas->BeginInvoke(gcnew SetStatusDelegate(this, &GraphAnalysisService::ShowStatusMessage), args);
            return;
        }

        if (System::String::IsNullOrEmpty(text))
            m_canvas->ClearStatusLabel();
        else
            m_canvas->SetStatusLabel(text);

        m_canvas->Refresh();
    }

    System::String^ GraphAnalysisService::BuildCycleLabel(const std::vector<int>& cycle, Dictionary<int, System::String^>^ labelMap)
    {
        if (!labelMap)
            return String::Empty;

        auto labels = gcnew List<System::String^>();
        for (int id : cycle)
        {
            System::String^ value = nullptr;
            if (labelMap->TryGetValue(id, value))
                labels->Add(value);
            else
                labels->Add(id.ToString());
        }

        return String::Join(L" -> ", labels);
    }

    void GraphAnalysisService::RunEnumerateHamiltonianCyclesTask()
    {
        auto document = GetDocument();
        if (!document)
            return;

        std::vector<int> nodes;
        std::vector<int> rest;
        Dictionary<int, System::String^>^ labelMap = nullptr;
        if (!TryGetNodeOrdering(nodes, rest, labelMap))
        {
            ShowStatusMessage(L"Sin datos para enumerar ciclos");
            ShowMessageBox(L"No hay suficientes datos para enumerar ciclos hamiltonianos.", L"Enumerar ciclos", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        auto core = document->Core;
        if (!core)
        {
            ShowStatusMessage(L"Sin datos para enumerar ciclos");
            return;
        }

        const int dimension = static_cast<int>(nodes.size());
        Dictionary<int, int>^ indexLookup = gcnew Dictionary<int, int>();
        for (int i = 0; i < dimension; ++i)
            indexLookup[nodes[i]] = i;

        const double inf = std::numeric_limits<double>::infinity();
        std::vector<double> weights(static_cast<std::size_t>(dimension) * dimension, inf);
        for (int i = 0; i < dimension; ++i)
            weights[static_cast<std::size_t>(i) * dimension + i] = 0.0;

        for (int i = 0; i < dimension; ++i)
        {
            int u = nodes[i];
            for (int j = 0; j < dimension; ++j)
            {
                if (i == j)
                    continue;

                int v = nodes[j];
                if (core->HasEdge(u, v))
                    weights[static_cast<std::size_t>(i) * dimension + j] = core->GetWeight(u, v);
            }
        }

        std::vector<int> current;
        current.reserve(rest.size());
        std::vector<bool> used(rest.size(), false);
        double bestBound = std::numeric_limits<double>::infinity();
        int cycleCount = 0;
        double minCost = std::numeric_limits<double>::infinity();
        double maxCost = 0.0;
        auto listedCycles = gcnew List<System::String^>();
        int statusCounter = 0;
        long long exploredCount = 0;
        const int statusThreshold = 2048;

        EnumerateHamiltonianCyclesRecursive(nodes, rest, indexLookup, used, current, nodes.front(), 0.0, bestBound, cycleCount, minCost, maxCost, listedCycles, labelMap, weights, dimension, statusCounter, statusThreshold, exploredCount);

        if (cycleCount == 0)
        {
            ShowStatusMessage(L"Sin ciclos hamiltonianos");
            ShowMessageBox(L"No se encontraron ciclos hamiltonianos en el grafo.", L"Enumerar ciclos", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }

        ShowStatusMessage(String::Format(L"Ciclos hamiltonianos encontrados: {0}", cycleCount));

        if (m_canvas)
        {
            array<System::Object^>^ args = gcnew array<System::Object^>(4);
            args[0] = cycleCount;
            args[1] = minCost;
            args[2] = maxCost;
            args[3] = listedCycles;
            m_canvas->BeginInvoke(gcnew CompleteHamiltonianEnumerationDelegate(this, &GraphAnalysisService::CompleteHamiltonianEnumeration), args);
        }
        else
        {
            CompleteHamiltonianEnumeration(cycleCount, minCost, maxCost, listedCycles);
        }
    }

    void GraphAnalysisService::RunSolveTspTask()
    {
        auto document = GetDocument();
        if (!document)
            return;

        std::vector<int> nodes;
        std::vector<int> rest;
        Dictionary<int, System::String^>^ labelMap = nullptr;
        if (!TryGetNodeOrdering(nodes, rest, labelMap))
        {
            ShowStatusMessage(L"Sin datos para resolver el TSP");
            ShowMessageBox(L"No hay suficientes nodos para resolver el TSP.", L"Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        auto core = document->Core;
        if (!core)
        {
            ShowStatusMessage(L"Sin datos para resolver el TSP");
            return;
        }

        const int dimension = static_cast<int>(nodes.size());
        const double inf = std::numeric_limits<double>::infinity();

        std::vector<double> weights(static_cast<std::size_t>(dimension) * dimension, inf);
        for (int i = 0; i < dimension; ++i)
            weights[static_cast<std::size_t>(i) * dimension + i] = 0.0;

        for (int i = 0; i < dimension; ++i)
        {
            int u = nodes[i];
            for (int j = 0; j < dimension; ++j)
            {
                if (i == j)
                    continue;
                int v = nodes[j];
                if (core->HasEdge(u, v))
                    weights[static_cast<std::size_t>(i) * dimension + j] = core->GetWeight(u, v);
            }
        }

        Stopwatch^ stopwatch = Stopwatch::StartNew();

        int subsetCount = 1 << (dimension - 1);
        std::vector<double> dp(static_cast<std::size_t>(subsetCount) * dimension, inf);
        std::vector<int> parent(static_cast<std::size_t>(subsetCount) * dimension, -1);

        for (int j = 1; j < dimension; ++j)
        {
            int subset = 1 << (j - 1);
            double w = weights[0 * dimension + j];
            if (w < inf)
            {
                dp[static_cast<std::size_t>(subset) * dimension + j] = w;
                parent[static_cast<std::size_t>(subset) * dimension + j] = 0;
            }
        }

        for (int subset = 1; subset < subsetCount; ++subset)
        {
            for (int j = 1; j < dimension; ++j)
            {
                if ((subset & (1 << (j - 1))) == 0)
                    continue;

                int subsetWithoutJ = subset & ~(1 << (j - 1));
                if (subsetWithoutJ == 0)
                    continue;

                double best = dp[static_cast<std::size_t>(subset) * dimension + j];
                int bestPrev = parent[static_cast<std::size_t>(subset) * dimension + j];

                for (int k = 1; k < dimension; ++k)
                {
                    if ((subsetWithoutJ & (1 << (k - 1))) == 0)
                        continue;

                    double prevCost = dp[static_cast<std::size_t>(subsetWithoutJ) * dimension + k];
                    if (prevCost == inf)
                        continue;

                    double edgeCost = weights[static_cast<std::size_t>(k) * dimension + j];
                    if (edgeCost == inf)
                        continue;

                    double candidate = prevCost + edgeCost;
                    if (candidate < best)
                    {
                        best = candidate;
                        bestPrev = k;
                    }
                }
                dp[static_cast<std::size_t>(subset) * dimension + j] = best;
                parent[static_cast<std::size_t>(subset) * dimension + j] = bestPrev;
            }
        }

        int fullSubset = subsetCount - 1;
        double bestTour = inf;
        int bestLast = -1;

        for (int j = 1; j < dimension; ++j)
        {
            double pathCost = dp[static_cast<std::size_t>(fullSubset) * dimension + j];
            if (pathCost == inf)
                continue;
            double backCost = weights[static_cast<std::size_t>(j) * dimension + 0];
            if (backCost == inf)
                continue;
            double candidate = pathCost + backCost;
            if (candidate < bestTour)
            {
                bestTour = candidate;
                bestLast = j;
            }
        }

        stopwatch->Stop();
        System::TimeSpan elapsed = stopwatch->Elapsed;

        if (bestLast == -1 || bestTour == inf)
        {
            ShowStatusMessage(L"No hay solución TSP");
            ShowMessageBox(L"No hay solución.", L"Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        std::vector<int> stack;
        stack.reserve(dimension - 1);
        int subset = fullSubset;
        int current = bestLast;
        bool reconstructionFailed = false;

        while (subset != 0)
        {
            if (current <= 0)
            {
                reconstructionFailed = true;
                break;
            }

            stack.push_back(nodes[current]);
            int prev = parent[static_cast<std::size_t>(subset) * dimension + current];
            subset &= ~(1 << (current - 1));
            current = prev;

            if (subset != 0 && current < 0)
            {
                reconstructionFailed = true;
                break;
            }
        }

        if (reconstructionFailed || current != 0)
        {
            ShowStatusMessage(L"No hay solución TSP");
            ShowMessageBox(L"No hay solución.", L"Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        std::vector<int> cycle;
        cycle.reserve(stack.size() + 2);
        cycle.push_back(nodes.front());
        for (auto it = stack.rbegin(); it != stack.rend(); ++it)
            cycle.push_back(*it);
        cycle.push_back(nodes.front());

        auto managedCycle = gcnew List<int>(static_cast<int>(cycle.size()));
        for (int node : cycle)
            managedCycle->Add(node);

        auto pathLabel = BuildCycleLabel(cycle, labelMap);

        if (m_canvas)
        {
            array<System::Object^>^ args = gcnew array<System::Object^>(4);
            args[0] = managedCycle;
            args[1] = bestTour;
            args[2] = elapsed;
            args[3] = pathLabel;
            m_canvas->BeginInvoke(gcnew CompleteTspSolutionDelegate(this, &GraphAnalysisService::CompleteTspSolution), args);
        }
        else
        {
            CompleteTspSolution(managedCycle, bestTour, elapsed, pathLabel);
        }
    }

    void GraphAnalysisService::CompleteHamiltonianEnumeration(int count, double minCost, double maxCost, System::Collections::Generic::List<System::String^>^ cycles)
    {
        ShowStatusMessage(String::Format(L"Ciclos hamiltonianos encontrados: {0}", count));
        DisplayCycleSummary(count, minCost, maxCost, cycles);
    }

    void GraphAnalysisService::CompleteTspSolution(System::Collections::Generic::List<int>^ cycleNodes, double totalCost, System::TimeSpan duration, System::String^ pathLabel)
    {
        std::vector<int> nativeCycle;
        if (cycleNodes)
        {
            nativeCycle.reserve(cycleNodes->Count);
            for each (int nodeId in cycleNodes)
                nativeCycle.push_back(nodeId);
        }

        if (!nativeCycle.empty())
            HighlightCycle(nativeCycle);

        System::String^ summary = String::Format(L"Camino optimo: {0}\nCosto total: {1}", pathLabel, FormatCost(totalCost));
        summary = summary + String::Format(L"\nTiempo: {0:F2} ms", duration.TotalMilliseconds);

        ShowStatusMessage(String::Format(L"Costo TSP optimo: {0}", FormatCost(totalCost)));
        MessageBox::Show(summary, L"Resultado TSP", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void GraphAnalysisService::EnumerateHamiltonianCyclesRecursive(const std::vector<int>& nodes, const std::vector<int>& rest, System::Collections::Generic::Dictionary<int, int>^ indexLookup, std::vector<bool>& used, std::vector<int>& current, int startNode, double currentCost, double& bestBound, int& cycleCount, double& minCost, double& maxCost, System::Collections::Generic::List<System::String^>^ listedCycles, System::Collections::Generic::Dictionary<int, System::String^>^ labelMap, const std::vector<double>& weights, int dimension, int& statusCounter, int statusThreshold, long long& exploredCount)
    {
        const double inf = std::numeric_limits<double>::infinity();

        if (current.size() == rest.size())
        {
            if (current.size() > 1 && current.front() > current.back())
                return;

            int lastNode = current.empty() ? startNode : current.back();
            int lastIndex = GetNodeIndex(indexLookup, lastNode);
            int startIndex = GetNodeIndex(indexLookup, startNode);
            if (lastIndex < 0 || startIndex < 0)
                return;

            double backWeight = weights[static_cast<std::size_t>(lastIndex) * dimension + startIndex];
            if (backWeight == inf)
                return;

            double totalCost = currentCost + backWeight;
            bestBound = (std::min)(bestBound, totalCost);

            std::vector<int> cycle;

            cycle.reserve(current.size() + 2);
            cycle.push_back(startNode);
            for (int node : current)
                cycle.push_back(node);
            cycle.push_back(startNode);

            ++cycleCount;
            minCost = (std::min)(minCost, totalCost);
            maxCost = (std::max)(maxCost, totalCost);

            if (listedCycles && listedCycles->Count < 10)
            {
                auto text = BuildCycleLabel(cycle, labelMap);
                text = String::Format(L"{0} (costo: {1})", text, FormatCost(totalCost));
                listedCycles->Add(text);
            }

            return;
        }

        int previousNode = current.empty() ? startNode : current.back();
        int previousIndex = GetNodeIndex(indexLookup, previousNode);
        if (previousIndex < 0)
            return;

        for (std::size_t i = 0; i < rest.size(); ++i)
        {
            if (used[i])
                continue;

            int nextNode = rest[i];
            int nextIndex = GetNodeIndex(indexLookup, nextNode);
            if (nextIndex < 0)
                continue;

            double edgeWeight = weights[static_cast<std::size_t>(previousIndex) * dimension + nextIndex];
            if (edgeWeight == inf)
                continue;

            double tentative = currentCost + edgeWeight;
            if (tentative >= bestBound)
                continue;

            used[i] = true;
            current.push_back(nextNode);
            ++exploredCount;

            if (statusThreshold > 0)
            {
                ++statusCounter;
                if (statusCounter >= statusThreshold)
                {
                    statusCounter = 0;
                    ShowStatusMessage(String::Format(L"Explorando ciclos... rutas evaluadas: {0}", exploredCount));
                }
            }

            EnumerateHamiltonianCyclesRecursive(nodes, rest, indexLookup, used, current, startNode, tentative, bestBound, cycleCount, minCost, maxCost, listedCycles, labelMap, weights, dimension, statusCounter, statusThreshold, exploredCount);

            current.pop_back();
            used[i] = false;
        }
    }

    int GraphAnalysisService::GetNodeIndex(System::Collections::Generic::Dictionary<int, int>^ indexLookup, int nodeId)
    {
        if (!indexLookup)
            return -1;

        int value = -1;
        if (indexLookup->TryGetValue(nodeId, value))
            return value;

        return -1;
    }

    void GraphAnalysisService::DisplayCycleSummary(int count, double minCost, double maxCost, List<System::String^>^ cycles)
    {
        auto sb = gcnew System::Text::StringBuilder();
        sb->AppendFormat("Total de ciclos hamiltonianos: {0}\n", count);
        if (count > 0)
        {
            sb->AppendFormat("Costo mínimo: {0}\n", FormatCost(minCost));
            sb->AppendFormat("Costo máximo: {0}\n", FormatCost(maxCost));
        }

        if (cycles && cycles->Count > 0)
        {
            sb->AppendLine();
            sb->AppendLine("Ciclos enumerados:");
            for each (auto cycle in cycles)
                sb->AppendLine(cycle);
            if (count > cycles->Count)
                sb->AppendFormat("... y {0} ciclos adicionales.\n", count - cycles->Count);
        }

        MessageBox::Show(sb->ToString(), "Ciclos hamiltonianos", MessageBoxButtons::OK, MessageBoxIcon::None);
    }

    void GraphAnalysisService::EnumerateHamiltonianCycles()
    {
        auto document = GetDocument();
        if (!document)
            return;

        document->ClearEdgeHighlights();

        int nodeCount = document->NodeCount();
        if (nodeCount < 8)
        {
            ShowMessageBox(L"Se requieren al menos 8 nodos para analizar.", L"Enumerar ciclos", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        if (nodeCount > 16)
        {
            ShowMessageBox(L"El análisis solo soporta hasta 16 nodos.", L"Enumerar ciclos", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        ShowStatusMessage(L"Enumerando ciclos hamiltonianos...");
        Task::Run(gcnew System::Action(this, &GraphAnalysisService::RunEnumerateHamiltonianCyclesTask));
    }

    void GraphAnalysisService::HighlightCycle(const std::vector<int>& cycle)
    {
        auto document = GetDocument();
        if (!document || !document->Visual)
            return;

        document->ClearEdgeHighlights();

        auto visual = document->Visual;
        for (std::size_t i = 0; i + 1 < cycle.size(); ++i)
        {
            int u = cycle[i];
            int v = cycle[i + 1];
            auto edge = visual->FindEdgeBetween(u, v);
            if (!edge)
                continue;

            edge->Highlighted = true;
            edge->HighlightStroke = System::Drawing::Color::Yellow;
        }

        if (m_canvas)
            m_canvas->Invalidate();
    }

    void GraphAnalysisService::ShowMessageBox(System::String^ text, System::String^ caption, MessageBoxButtons buttons, MessageBoxIcon icon)
    {
        if (!m_canvas)
        {
            MessageBox::Show(text, caption, buttons, icon);
            return;
        }

        if (m_canvas->InvokeRequired)
        {
            array<System::Object^>^ args = gcnew array<System::Object^>(4);
            args[0] = text;
            args[1] = caption;
            args[2] = buttons;
            args[3] = icon;
            m_canvas->BeginInvoke(gcnew ShowMessageBoxDelegate(this, &GraphAnalysisService::ShowMessageBox), args);
            return;
        }

        MessageBox::Show(text, caption, buttons, icon);
    }

    void GraphAnalysisService::SolveTspBruteForce()
    {
        auto document = GetDocument();
        if (!document)
            return;

        document->ClearEdgeHighlights();

        int nodeCount = document->NodeCount();
        if (nodeCount < 8)
        {
            ShowMessageBox(L"Se requieren al menos 8 nodos para analizar.", L"Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            if (m_canvas)
                m_canvas->Invalidate();
            return;
        }

        if (nodeCount > 16)
        {
            ShowMessageBox(L"El análisis solo soporta hasta 16 nodos.", L"Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        ShowStatusMessage(L"Resolviendo TSP...");
        Task::Run(gcnew System::Action(this, &GraphAnalysisService::RunSolveTspTask));
    }
}