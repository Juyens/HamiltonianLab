#include "GraphAnalysisService.h"

#include <algorithm>
#include <limits>

namespace HamiltonianLab::Services
{
    namespace
    {
        bool HasSufficientNodes(int nodeCount)
        {
            return nodeCount >= 3;
        }

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

    void GraphAnalysisService::ShowStatusMessage(System::String^ text)
    {
        if (!m_canvas)
            return;

        if (String::IsNullOrEmpty(text))
            m_canvas->ClearStatusLabel();
        else
            m_canvas->SetStatusLabel(text);

        m_canvas->Refresh();
        Application::DoEvents();
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

    void GraphAnalysisService::DisplayCycleSummary(int count, double minCost, double maxCost, List<System::String^>^ cycles)
    {
        auto sb = gcnew StringBuilder();
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

        std::vector<int> nodes;
        std::vector<int> rest;
        Dictionary<int, System::String^>^ labelMap = nullptr;
        if (!TryGetNodeOrdering(nodes, rest, labelMap))
        {
            ShowStatusMessage("Sin datos para enumerar ciclos");
            MessageBox::Show("Debe existir al menos un ciclo con 8 nodos o más para enumerar.", "Enumerar ciclos", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        if (!HasSufficientNodes(static_cast<int>(nodes.size())))
        {
            ShowStatusMessage("Sin ciclos hamiltonianos");
            MessageBox::Show("Se requieren al menos tres nodos para formar un ciclo hamiltoniano.", "Enumerar ciclos", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        ShowStatusMessage("Enumerando ciclos hamiltonianos...");

        int start = nodes.front();
        std::sort(rest.begin(), rest.end());

        int cycleCount = 0;
        double minCost = std::numeric_limits<double>::infinity();
        double maxCost = 0.0;
        auto listedCycles = gcnew List<System::String^>();

        do
        {
            if (rest.size() > 1 && rest.front() > rest.back())
                continue;

            std::vector<int> cycle;
            cycle.reserve(rest.size() + 2);
            cycle.push_back(start);

            bool valid = true;
            double cost = 0.0;
            int previous = start;
            for (int v : rest)
            {
                double weight = 0.0;
                if (!TryGetEdgeWeight(previous, v, weight))
                {
                    valid = false;
                    break;
                }

                cost += weight;
                cycle.push_back(v);
                previous = v;
            }

            if (!valid)
                continue;

            double backWeight = 0.0;
            if (!TryGetEdgeWeight(previous, start, backWeight))
                continue;

            cost += backWeight;
            cycle.push_back(start);

            ++cycleCount;
            minCost = (std::min)(minCost, cost);
            maxCost = (std::max)(maxCost, cost);

            if (listedCycles->Count < 10)
            {
                auto text = BuildCycleLabel(cycle, labelMap);
                text = String::Format("{0} (costo: {1})", text, FormatCost(cost));
                listedCycles->Add(text);
            }
        }
        while (std::next_permutation(rest.begin(), rest.end()));

        if (cycleCount == 0)
        {
            ShowStatusMessage("Sin ciclos hamiltonianos");
            MessageBox::Show("No se encontraron ciclos hamiltonianos en el grafo.", "Enumerar ciclos", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }

        System::String^ label = String::Format("Ciclos hamiltonianos encontrados: {0}", cycleCount);
        ShowStatusMessage(label);
        DisplayCycleSummary(cycleCount, minCost, maxCost, listedCycles);
    }

    void GraphAnalysisService::HighlightCycle(const std::vector<int>& cycle)
    {
        auto document = GetDocument();
        if (!document || !document->Visual)
            return;

        document->ClearEdgeHighlights();

        auto visual = document->Visual;
        for (size_t i = 0; i + 1 < cycle.size(); ++i)
        {
            int u = cycle[i];
            int v = cycle[i + 1];
            auto edge = visual->FindEdgeBetween(u, v);
            if (!edge)
                continue;

            edge->Highlighted = true;
            edge->HighlightStroke = Color::Yellow;
        }

        if (m_canvas)
            m_canvas->Invalidate();
    }

    void GraphAnalysisService::SolveTspBruteForce()
    {
        auto document = GetDocument();
        if (!document)
            return;

        document->ClearEdgeHighlights();

        std::vector<int> nodes;
        std::vector<int> rest;
        Dictionary<int, System::String^>^ labelMap = nullptr;
        if (!TryGetNodeOrdering(nodes, rest, labelMap))
        {
            MessageBox::Show("No hay suficientes nodos para resolver el TSP.", "Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            if (m_canvas)
                m_canvas->Invalidate();
            return;
        }

        const std::size_t n = nodes.size();
        if (n < 3)
        {
            MessageBox::Show("Se requieren al menos tres nodos para encontrar un ciclo hamiltoniano.", "Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            if (m_canvas)
                m_canvas->Invalidate();
            return;
        }

        const std::size_t bruteForceLimit = 11;
        if (n > bruteForceLimit)
        {
            MessageBox::Show("El algoritmo de fuerza bruta solo se permite hasta 11 nodos por motivos de rendimiento.", "Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        int start = nodes.front();
        std::sort(rest.begin(), rest.end());

        double bestCost = std::numeric_limits<double>::infinity();
        std::vector<int> bestCycle;

        do
        {
            if (rest.size() > 1 && rest.front() > rest.back())
                continue;

            bool valid = true;
            double cost = 0.0;
            int previous = start;
            std::vector<int> cycle;
            cycle.reserve(rest.size() + 2);
            cycle.push_back(start);

            for (int v : rest)
            {
                double weight = 0.0;
                if (!TryGetEdgeWeight(previous, v, weight))
                {
                    valid = false;
                    break;
                }

                cost += weight;
                cycle.push_back(v);
                previous = v;
            }

            if (!valid)
                continue;

            double backWeight = 0.0;
            if (!TryGetEdgeWeight(previous, start, backWeight))
                continue;

            cost += backWeight;
            cycle.push_back(start);

            if (cost < bestCost)
            {
                bestCost = cost;
                bestCycle = cycle;
            }
        }
        while (std::next_permutation(rest.begin(), rest.end()));

        if (bestCycle.empty())
        {
            MessageBox::Show("No se encontró un ciclo hamiltoniano que cubra todos los nodos.", "Resolver TSP", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }

        HighlightCycle(bestCycle);

        auto summary = BuildCycleLabel(bestCycle, labelMap);
        summary = String::Format("Camino óptimo: {0}\nCosto total: {1}", summary, FormatCost(bestCost));
        MessageBox::Show(summary, "Resultado TSP", MessageBoxButtons::OK, MessageBoxIcon::None);
    }
}