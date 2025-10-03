#pragma once

#include <Models/Logical/LogicalGraph.h>
#include <msclr/marshal_cppstd.h>

namespace HamiltonianLab::Interop
{
	using namespace HamiltonianLab::Models::Logical;

    public value struct NeighborInfo
    {
        int To;
        int EdgeId;
    };

    public ref class ManagedGraph
    {
    public:
        ManagedGraph();
        ~ManagedGraph();
        !ManagedGraph();

        int AddNode();
        bool HasNode(int nodeId);
        bool RemoveNode(int nodeId);
        int  NodeCount();

        int  AddEdge(int a, int b, double w);
        bool HasEdge(int a, int b);
        bool RemoveEdge(int a, int b);
        bool RemoveEdgeById(int edgeId);
        int  EdgeCount();

        double GetWeight(int a, int b);
        void   SetWeight(int a, int b, double w);

        cli::array<NeighborInfo>^ Neighbors(int nodeId);

        bool IsComplete();
        bool ValidateSimple();

    private:
        void EnsureNotDisposed();

    private:
        LogicalGraph* m_native;
        bool m_disposed;
    };
}