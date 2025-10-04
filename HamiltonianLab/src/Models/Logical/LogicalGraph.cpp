#include "LogicalGraph.h"

namespace HamiltonianLab::Models::Logical
{
    LogicalGraph::LogicalGraph() = default;

    int LogicalGraph::AddNode()
    {
        int id = ++m_nextNodeId;
        m_nodes.emplace(id, LogicalNode{ id });
        m_adj.try_emplace(id, std::vector<Neighbor>{});
        return id;
    }

    bool LogicalGraph::HasNode(int nodeId) const
    {
        return m_nodes.find(nodeId) != m_nodes.end();
    }

    bool LogicalGraph::RemoveNode(int nodeId)
    {
        auto it = m_nodes.find(nodeId);
        if (it == m_nodes.end()) return false;

        auto adjIt = m_adj.find(nodeId);
        if (adjIt != m_adj.end())
        {
            auto neighbors = adjIt->second;
            for (const auto& nb : neighbors)
                RemoveEdge(nodeId, nb.to);
        }

        m_adj.erase(nodeId);
        m_nodes.erase(it);
        return true;
    }

    size_t LogicalGraph::NodeCount() const
    {
        return m_nodes.size();
    }

    void LogicalGraph::Clear()
    {
        m_nodes.clear();
        m_edges.clear();
        m_edgeKeyToId.clear();
        m_adj.clear();
        m_nextNodeId = 0;
        m_nextEdgeId = 0;
    }

    int LogicalGraph::AddEdge(int a, int b, double w)
    {
        if (a == b) throw std::invalid_argument("Self-loops no permitidos en grafo simple.");
        if (!HasNode(a) || !HasNode(b)) throw std::invalid_argument("AddEdge: nodo inexistente.");

        auto [u, v] = Canonical(a, b);
        auto key = PackKey(u, v);

        auto it = m_edgeKeyToId.find(key);
        if (it != m_edgeKeyToId.end())
        {
            int eid = it->second;
            m_edges[eid].w = w;
            return eid;
        }

        int id = ++m_nextEdgeId;
        m_edges.emplace(id, LogicalEdge{ id, u, v, w });
        m_edgeKeyToId.emplace(key, id);

        m_adj[u].push_back(Neighbor{ v, id });
        m_adj[v].push_back(Neighbor{ u, id });
        return id;
    }

    bool LogicalGraph::HasEdge(int a, int b) const
    {
        if (a == b) return false;
        auto [u, v] = Canonical(a, b);
        return m_edgeKeyToId.find(PackKey(u, v)) != m_edgeKeyToId.end();
    }

    bool LogicalGraph::RemoveEdge(int a, int b)
    {
        if (a == b) return false;
        auto [u, v] = Canonical(a, b);
        auto key = PackKey(u, v);

        auto it = m_edgeKeyToId.find(key);
        if (it == m_edgeKeyToId.end()) return false;

        int eid = it->second;
        m_edgeKeyToId.erase(it);
        m_edges.erase(eid);

        auto& Au = m_adj[u];
        auto& Av = m_adj[v];
        EraseNeighbor(Au, v);
        EraseNeighbor(Av, u);
        return true;
    }

    bool LogicalGraph::RemoveEdgeById(int edgeId)
    {
        auto eit = m_edges.find(edgeId);
        if (eit == m_edges.end()) return false;

        int u = eit->second.u, v = eit->second.v;
        m_edges.erase(eit);
        m_edgeKeyToId.erase(PackKey(u, v));
        EraseNeighbor(m_adj[u], v);
        EraseNeighbor(m_adj[v], u);
        return true;
    }

    size_t LogicalGraph::EdgeCount() const
    {
        return m_edges.size();
    }

    double LogicalGraph::GetWeight(int a, int b) const
    {
        int eid = EdgeId(a, b);
        if (eid <= 0) throw std::out_of_range("GetWeight: arista no existe.");
        return m_edges.at(eid).w;
    }

    void LogicalGraph::SetWeight(int a, int b, double w)
    {
        int eid = EdgeId(a, b);
        if (eid <= 0) throw std::out_of_range("SetWeight: arista no existe.");
        m_edges[eid].w = w;
    }

    const std::vector<Neighbor>& LogicalGraph::Neighbors(int nodeId) const
    {
        auto it = m_adj.find(nodeId);
        if (it == m_adj.end()) throw std::out_of_range("Neighbors: nodo no existe.");
        return it->second;
    }

    bool LogicalGraph::IsComplete() const
    {
        const size_t n = NodeCount();
        const size_t expected = (n * (n - 1)) / 2;
        return EdgeCount() == expected;
    }

    bool LogicalGraph::ValidateSimple() const
    {
        for (const auto& kv : m_edges)
        {
            const auto& e = kv.second;
            if (!(e.u < e.v)) return false;
            auto key = PackKey(e.u, e.v);
            auto it = m_edgeKeyToId.find(key);
            if (it == m_edgeKeyToId.end() || it->second != e.id) return false;
        }

        for (const auto& kv : m_adj)
        {
            int u = kv.first;
            const auto& listU = kv.second;
            for (const auto& nb : listU)
            {
                if (!HasNode(nb.to)) return false;

                auto [a, b] = Canonical(u, nb.to);
                auto itE = m_edgeKeyToId.find(PackKey(a, b));
                if (itE == m_edgeKeyToId.end()) return false;
                if (m_edges.at(itE->second).id != nb.edgeId) return false;

                auto itV = m_adj.find(nb.to);
                if (itV == m_adj.end()) return false;
                if (!ContainsNeighbor(itV->second, u)) return false;
            }
        }

        return true;
    }

    std::pair<int, int> LogicalGraph::Canonical(int a, int b)
    {
        return (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
    }

    std::uint64_t LogicalGraph::PackKey(int u, int v)
    {
        return (static_cast<std::uint64_t>(static_cast<std::uint32_t>(u)) << 32)
            | static_cast<std::uint32_t>(v);
    }

    void LogicalGraph::EraseNeighbor(std::vector<Neighbor>& vec, int to)
    {
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [to] (const Neighbor& n) { return n.to == to; }),
            vec.end());
    }

    bool LogicalGraph::ContainsNeighbor(const std::vector<Neighbor>& vec, int to)
    {
        return std::any_of(vec.begin(), vec.end(),
            [to] (const Neighbor& n) { return n.to == to; });
    }

    int LogicalGraph::EdgeId(int a, int b) const
    {
        if (a == b) return 0;
        auto [u, v] = Canonical(a, b);
        auto it = m_edgeKeyToId.find(PackKey(u, v));
        return (it == m_edgeKeyToId.end()) ? 0 : it->second;
    }
}