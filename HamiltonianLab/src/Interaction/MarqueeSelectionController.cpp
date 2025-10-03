#include "MarqueeSelectionController.h"

namespace HamiltonianLab::Interaction
{
    using namespace System;
    using namespace HamiltonianLab::Models;
    using namespace HamiltonianLab::Models::Visual;

    MarqueeSelectionController::MarqueeSelectionController(GraphDocument^ document,
        MovementConstraints^ constraints,
        IInvalidationSink^ invalidationSink)
        : m_document(document)
        , m_constraints(constraints)
        , m_invalidationSink(invalidationSink)
        , m_active(false)
        , m_anchor(PointF())
        , m_rect(RectangleF())
        , m_mode(MarqueeSelectionMode::Replace)
    {
    }

    bool MarqueeSelectionController::IsActive::get()
    {
        return m_active;
    }

    RectangleF MarqueeSelectionController::CurrentRectangle::get()
    {
        if (!m_active)
            return RectangleF();
        return NormalizeRectangle(m_rect);
    }

    void MarqueeSelectionController::Begin(PointF anchor, MarqueeSelectionMode mode)
    {
        if (m_constraints != nullptr)
            anchor = m_constraints->ConstrainPoint(anchor);

        m_active = true;
        m_anchor = anchor;
        m_rect = RectangleF(anchor.X, anchor.Y, 0.0f, 0.0f);
        m_mode = mode;

        if (mode == MarqueeSelectionMode::Replace && m_document && m_document->Selection)
        {
            m_document->Selection->Clear();
        }

        InvalidateOverlay(RectangleF(), NormalizeRectangle(m_rect));
    }

    void MarqueeSelectionController::Update(PointF current)
    {
        if (!m_active)
            return;

        if (m_constraints != nullptr)
            current = m_constraints->ConstrainPoint(current);

        auto previous = NormalizeRectangle(m_rect);
        m_rect = RectangleF(m_anchor.X, m_anchor.Y, current.X - m_anchor.X, current.Y - m_anchor.Y);
        auto normalized = NormalizeRectangle(m_rect);

        InvalidateOverlay(previous, normalized);
    }

    void MarqueeSelectionController::Complete()
    {
        if (!m_active)
            return;

        auto normalized = NormalizeRectangle(m_rect);

        if (m_document && m_document->Visual && m_document->Selection)
        {
            auto visual = m_document->Visual;
            auto selection = m_document->Selection;

            auto hits = visual->QueryNodesIn(normalized);
            if (hits != nullptr)
            {
                for each (VisualNode ^ node in hits)
                {
                    switch (m_mode)
                    {
                    case MarqueeSelectionMode::Toggle:
                        selection->Toggle(node);
                        break;
                    case MarqueeSelectionMode::Add:
                    case MarqueeSelectionMode::Replace:
                    default:
                        selection->Add(node);
                        break;
                    }
                }
            }
        }

        InvalidateOverlay(normalized, RectangleF());

        m_active = false;
        m_rect = RectangleF();
    }

    void MarqueeSelectionController::Cancel()
    {
        if (!m_active)
            return;

        auto normalized = NormalizeRectangle(m_rect);
        InvalidateOverlay(normalized, RectangleF());

        m_active = false;
        m_rect = RectangleF();
    }

    RectangleF MarqueeSelectionController::NormalizeRectangle(RectangleF rect)
    {
        if (rect.Width == 0.0f && rect.Height == 0.0f)
            return RectangleF(rect.X, rect.Y, 0.0f, 0.0f);

        auto left = Math::Min(rect.Left, rect.Right);
        auto top = Math::Min(rect.Top, rect.Bottom);
        auto right = Math::Max(rect.Left, rect.Right);
        auto bottom = Math::Max(rect.Top, rect.Bottom);

        return RectangleF(left, top, right - left, bottom - top);
    }

    System::Drawing::Rectangle MarqueeSelectionController::InflateForDisplay(RectangleF rect)
    {
        if (rect.Width == 0.0f && rect.Height == 0.0f)
        {
            rect = RectangleF(rect.X - 1.0f, rect.Y - 1.0f, 2.0f, 2.0f);
        }

        rect.Inflate(2.0f, 2.0f);
        return Rectangle::Ceiling(rect);
    }

    void MarqueeSelectionController::InvalidateOverlay(RectangleF previousRect, RectangleF nextRect)
    {
        if (m_invalidationSink == nullptr)
            return;

        if (previousRect.Width > 0.0f || previousRect.Height > 0.0f)
        {
            m_invalidationSink->RequestInvalidate(InflateForDisplay(previousRect));
        }

        if (nextRect.Width > 0.0f || nextRect.Height > 0.0f)
        {
            m_invalidationSink->RequestInvalidate(InflateForDisplay(nextRect));
        }
    }
}