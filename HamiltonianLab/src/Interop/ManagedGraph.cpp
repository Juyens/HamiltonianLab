#include "ManagedGraph.h"

namespace HamiltonianLab::Interop
{
    ManagedGraph::ManagedGraph()
        : m_native(new LogicalGraph()), m_disposed(false)
    {
    }

    ManagedGraph::~ManagedGraph()
    {
        if (!m_disposed)
        {
            this->!ManagedGraph();
            m_disposed = true;
        }
    }

    ManagedGraph::!ManagedGraph()
    {
        if (m_native)
        {
            delete m_native;
            m_native = nullptr;
        }
    }

    void ManagedGraph::EnsureNotDisposed()
    {
        if (m_disposed || m_native == nullptr)
            throw gcnew System::ObjectDisposedException("ManagedGraph");
    }

    int ManagedGraph::AddNode()
    {
        EnsureNotDisposed();
        return m_native->AddNode();
    }

    bool ManagedGraph::HasNode(int nodeId)
    {
        EnsureNotDisposed();
        return m_native->HasNode(nodeId);
    }

    bool ManagedGraph::RemoveNode(int nodeId)
    {
        EnsureNotDisposed();
        return m_native->RemoveNode(nodeId);
    }

    int ManagedGraph::NodeCount()
    {
        EnsureNotDisposed();
        return static_cast<int>(m_native->NodeCount());
    }

    int ManagedGraph::AddEdge(int a, int b, double w)
    {
        EnsureNotDisposed();
        return m_native->AddEdge(a, b, w);
    }

    bool ManagedGraph::HasEdge(int a, int b)
    {
        EnsureNotDisposed();
        return m_native->HasEdge(a, b);
    }

    bool ManagedGraph::RemoveEdge(int a, int b)
    {
        EnsureNotDisposed();
        return m_native->RemoveEdge(a, b);
    }

    bool ManagedGraph::RemoveEdgeById(int edgeId)
    {
        EnsureNotDisposed();
        return m_native->RemoveEdgeById(edgeId);
    }

    int ManagedGraph::EdgeCount()
    {
        EnsureNotDisposed();
        return static_cast<int>(m_native->EdgeCount());
    }

    double ManagedGraph::GetWeight(int a, int b)
    {
        EnsureNotDisposed();
        return m_native->GetWeight(a, b);
    }

    void ManagedGraph::SetWeight(int a, int b, double w)
    {
        EnsureNotDisposed();
        m_native->SetWeight(a, b, w);
    }

    void ManagedGraph::Clear()
    {
        EnsureNotDisposed();
        m_native->Clear();
    }

    cli::array<NeighborInfo>^ ManagedGraph::Neighbors(int nodeId)
    {
        EnsureNotDisposed();

        const auto& vec = m_native->Neighbors(nodeId);
        auto out = gcnew cli::array<NeighborInfo>(static_cast<int>(vec.size()));
        for (int i = 0; i < static_cast<int>(vec.size()); ++i)
        {
            NeighborInfo ni;
            ni.To = vec[i].to;
            ni.EdgeId = vec[i].edgeId;
            out[i] = ni;
        }
        return out;
    }

    bool ManagedGraph::IsComplete()
    {
        EnsureNotDisposed();
        return m_native->IsComplete();
    }

    bool ManagedGraph::ValidateSimple()
    {
        EnsureNotDisposed();
        return m_native->ValidateSimple();
    }
}