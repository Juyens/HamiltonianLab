#include "SelectionDragController.h"

namespace HamiltonianLab::Interaction
{
    using namespace System;
    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Models::Visual;

    SelectionDragController::SelectionDragController(GraphDocument^ document,
        MovementConstraints^ constraints,
        IInvalidationSink^ invalidationSink)
        : m_document(document)
        , m_constraints(constraints)
        , m_invalidationSink(invalidationSink)
        , m_dragging(false)
        , m_anchor(PointF())
        , m_currentDelta(PointF())
        , m_nodes(gcnew List<VisualNode^>())
        , m_originalPositions(gcnew List<PointF>())
        , m_radii(gcnew List<float>())
    {
    }

    bool SelectionDragController::IsDragging::get()
    {
        return m_dragging;
    }

    bool SelectionDragController::BeginDrag(PointF anchor)
    {
        if (!m_document || !m_document->Selection || !m_document->Visual)
            return false;

        m_nodes->Clear();
        m_originalPositions->Clear();
        m_radii->Clear();

        auto selection = m_document->Selection;
        auto visual = m_document->Visual;

        for each (VisualNode ^ node in visual->Nodes)
        {
            if (selection->IsSelected(node))
            {
                m_nodes->Add(node);
                m_originalPositions->Add(node->Position);
                m_radii->Add(node->Radius);
            }
        }

        if (m_nodes->Count == 0)
            return false;

        m_dragging = true;
        m_anchor = anchor;
        m_currentDelta = PointF(0.0f, 0.0f);
        return true;
    }

    void SelectionDragController::Update(PointF current)
    {
        if (!m_dragging)
            return;

        PointF desiredDelta(current.X - m_anchor.X, current.Y - m_anchor.Y);
        if (m_constraints != nullptr)
        {
            desiredDelta = m_constraints->ConstrainDelta(m_originalPositions, m_radii, desiredDelta);
        }

        ApplyDelta(desiredDelta);
    }

    void SelectionDragController::EndDrag()
    {
        if (!m_dragging)
            return;

        m_dragging = false;
        m_nodes->Clear();
        m_originalPositions->Clear();
        m_radii->Clear();
        m_currentDelta = PointF();
    }

    void SelectionDragController::Cancel()
    {
        if (!m_dragging)
            return;

        ApplyDelta(PointF(0.0f, 0.0f));
        m_dragging = false;
        m_nodes->Clear();
        m_originalPositions->Clear();
        m_radii->Clear();
        m_currentDelta = PointF();
    }

    void SelectionDragController::ApplyDelta(PointF delta)
    {
        if (!m_dragging)
            return;

        if (Math::Abs(delta.X - m_currentDelta.X) < 1e-3f && Math::Abs(delta.Y - m_currentDelta.Y) < 1e-3f)
            return;

        auto before = ComputeInvalidationBounds();

        for (int i = 0; i < m_nodes->Count; ++i)
        {
            auto node = m_nodes[i];
            auto origin = m_originalPositions[i];
            node->Position = PointF(origin.X + delta.X, origin.Y + delta.Y);
        }

        m_currentDelta = delta;

        auto after = ComputeInvalidationBounds();

        if (m_invalidationSink != nullptr)
        {
            if (!before.IsEmpty)
                m_invalidationSink->RequestInvalidate(before);
            if (!after.IsEmpty)
                m_invalidationSink->RequestInvalidate(after);
        }
    }

    System::Drawing::Rectangle SelectionDragController::ComputeInvalidationBounds()
    {
        if (m_nodes->Count == 0)
            return System::Drawing::Rectangle();

        bool first = true;
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;

        for each (VisualNode ^ node in m_nodes)
        {
            auto bounds = node->GetBounds(4.0f);
            bounds.Inflate(4.0f, 4.0f);

            if (first)
            {
                left = bounds.Left;
                top = bounds.Top;
                right = bounds.Right;
                bottom = bounds.Bottom;
                first = false;
            }
            else
            {
                left = Math::Min(left, bounds.Left);
                top = Math::Min(top, bounds.Top);
                right = Math::Max(right, bounds.Right);
                bottom = Math::Max(bottom, bounds.Bottom);
            }
        }

        if (first)
            return System::Drawing::Rectangle();

        RectangleF rect(left, top, right - left, bottom - top);
        rect.Inflate(2.0f, 2.0f);
        return Rectangle::Ceiling(rect);
    }
}