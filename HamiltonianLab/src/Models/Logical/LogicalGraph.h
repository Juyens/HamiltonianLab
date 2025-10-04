#pragma once

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

namespace HamiltonianLab::Models::Logical
{
    struct LogicalNode
    {
        int id;
    };

    struct LogicalEdge
    {
        int id;
        int u;
        int v;
        double w;
    };

    struct Neighbor
    {
        int to;
        int edgeId;
    };

    class LogicalGraph
    {
    public:
        LogicalGraph();

        int  AddNode();
        bool HasNode(int nodeId) const;
        bool RemoveNode(int nodeId);
        size_t NodeCount() const;

        int  AddEdge(int a, int b, double w = 1.0);
        bool HasEdge(int a, int b) const;
        bool RemoveEdge(int a, int b);
        bool RemoveEdgeById(int edgeId);
        size_t EdgeCount() const;

        double GetWeight(int a, int b) const;
        void   SetWeight(int a, int b, double w);

        const std::vector<Neighbor>& Neighbors(int nodeId) const;

        void Clear();

        bool IsComplete() const;
        bool ValidateSimple() const;


    private:
        static std::pair<int, int> Canonical(int a, int b);
        static std::uint64_t PackKey(int u, int v);
        static void  EraseNeighbor(std::vector<Neighbor>& vec, int to);
        static bool ContainsNeighbor(const std::vector<Neighbor>& vec, int to);
        int EdgeId(int a, int b) const;

    private:
        int m_nextNodeId{ 0 };
        int m_nextEdgeId{ 0 };

        std::unordered_map<int, LogicalNode> m_nodes;
        std::unordered_map<int, LogicalEdge> m_edges;
        std::unordered_map<std::uint64_t, int> m_edgeKeyToId;
        std::unordered_map<int, std::vector<Neighbor>> m_adj;
    };
}