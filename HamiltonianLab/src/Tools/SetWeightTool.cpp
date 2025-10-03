#include "SetWeightTool.h"

#include <Tools/ToolController.h>

namespace HamiltonianLab
{
    namespace
    {
        constexpr float kLabelHalfWidth = 20.0f;
        constexpr float kLabelHalfHeight = 10.0f;
        constexpr float kLabelNormalOffset = 16.0f;
    }

    SetWeightTool::SetWeightTool(GraphDocument^ doc, ToolController^ controller, Control^ hostControl)
        : BaseTool(doc)
        , m_controller(controller)
        , m_host(hostControl)
        , m_editor(nullptr)
        , m_activeEdge(nullptr)
        , m_originalLabel(nullptr)
        , m_originalWeight(0.0)
        , m_keyPressHandler(nullptr)
        , m_keyDownHandler(nullptr)
        , m_leaveHandler(nullptr)
    {
    }

    void SetWeightTool::OnEnter()
    {
        CancelEdit();
    }

    void SetWeightTool::OnLeave()
    {
        CancelEdit();
    }

    void SetWeightTool::OnMouseDown(MouseEventArgs^ e)
    {
        if (!Doc)
            return;

        if (e->Button == MouseButtons::Right)
        {
            CancelEdit();
            return;
        }

        if (e->Button != MouseButtons::Left)
            return;

        PointF point((float)e->X, (float)e->Y);

        auto edge = HitTestEdgeLabel(point);
        if (!edge)
        {
            CancelEdit();
            return;
        }

        if (m_activeEdge && m_activeEdge != edge)
        {
            CancelEdit();
        }

        BeginEdit(edge);
    }

    void SetWeightTool::OnMouseMove(MouseEventArgs^)
    {
    }

    void SetWeightTool::OnMouseUp(MouseEventArgs^)
    {
    }

    void SetWeightTool::OnKeyDown(KeyEventArgs^ e)
    {
        if (!e)
            return;

        if (e->KeyCode == Keys::Escape)
        {
            CancelEdit();
            e->Handled = true;
        }
    }

    Cursor^ SetWeightTool::GetCursor()
    {
        return Cursors::IBeam;
    }

    void SetWeightTool::BeginEdit(VisualEdge^ edge)
    {
        if (!edge || !Doc || !Doc->Visual || !m_host)
            return;

        if (m_editor != nullptr)
        {
            CancelEdit();
        }

        RectangleF bounds = GetEdgeLabelBounds(edge);
        if (bounds.IsEmpty)
            return;

        m_activeEdge = edge;
        m_originalLabel = edge->Label;
        m_originalWeight = edge->Weight;

        TextBox^ editor = gcnew TextBox();
        editor->BorderStyle = BorderStyle::None;
        editor->Font = m_host->Font;
        editor->TextAlign = HorizontalAlignment::Center;
        editor->MaxLength = 10; // Max digits for Int32

        String^ seed = edge->Label;
        int parsedSeed = 0;
        if (String::IsNullOrEmpty(seed) || !Int32::TryParse(seed, NumberStyles::None, CultureInfo::InvariantCulture, parsedSeed) || parsedSeed < 0)
        {
            int fallback = System::Convert::ToInt32(System::Math::Max(0.0, edge->Weight));
            seed = fallback.ToString(CultureInfo::InvariantCulture);
        }

        editor->Text = seed;
        editor->Size = System::Drawing::Size((int)System::Math::Ceiling(bounds.Width), (int)System::Math::Ceiling(bounds.Height));
        editor->Location = System::Drawing::Point((int)System::Math::Round(bounds.X), (int)System::Math::Round(bounds.Y));

        m_keyPressHandler = gcnew KeyPressEventHandler(this, &SetWeightTool::OnEditorKeyPress);
        m_keyDownHandler = gcnew KeyEventHandler(this, &SetWeightTool::OnEditorKeyDown);
        m_leaveHandler = gcnew EventHandler(this, &SetWeightTool::OnEditorLeave);

        editor->KeyPress += m_keyPressHandler;
        editor->KeyDown += m_keyDownHandler;
        editor->Leave += m_leaveHandler;

        m_editor = editor;

        m_host->Controls->Add(editor);
        editor->BringToFront();
        editor->SelectAll();
        editor->Focus();

        InvalidateEdge(edge);
    }

    void SetWeightTool::CloseEditor(bool revertChanges)
    {
        VisualEdge^ edge = m_activeEdge;
        if (edge && revertChanges)
        {
            edge->Label = m_originalLabel;
            edge->Weight = m_originalWeight;
        }

        RemoveEditor();
        m_activeEdge = nullptr;
        m_originalLabel = nullptr;
        m_originalWeight = 0.0;

        if (edge)
            InvalidateEdge(edge);
    }

    void SetWeightTool::CommitEdit()
    {
        if (!m_editor || !m_activeEdge)
        {
            CloseEditor(true);
            return;
        }

        String^ text = m_editor->Text;
        if (String::IsNullOrWhiteSpace(text))
        {
            CloseEditor(true);
            return;
        }

        text = text->Trim();

        int parsedValue = 0;
        if (!Int32::TryParse(text, NumberStyles::None, CultureInfo::InvariantCulture, parsedValue) || parsedValue < 0)
        {
            CloseEditor(true);
            return;
        }

        auto edge = m_activeEdge;

        edge->Weight = parsedValue;
        edge->Label = parsedValue.ToString(CultureInfo::InvariantCulture);

        Doc->SetWeightByLogicalIds(edge->FromLogicalId, edge->ToLogicalId, parsedValue);
        Doc->MarkModified();

        CloseEditor(false);
    }

    void SetWeightTool::CancelEdit()
    {
        CloseEditor(true);
    }

    void SetWeightTool::RemoveEditor()
    {
        if (!m_editor)
            return;

        TextBox^ editor = m_editor;
        m_editor = nullptr;

        if (m_keyPressHandler)
        {
            editor->KeyPress -= m_keyPressHandler;
            m_keyPressHandler = nullptr;
        }

        if (m_keyDownHandler)
        {
            editor->KeyDown -= m_keyDownHandler;
            m_keyDownHandler = nullptr;
        }

        if (m_leaveHandler)
        {
            editor->Leave -= m_leaveHandler;
            m_leaveHandler = nullptr;
        }

        if (editor->Parent)
            editor->Parent->Controls->Remove(editor);

        delete editor;
    }

    void SetWeightTool::OnEditorKeyPress(Object^, KeyPressEventArgs^ e)
    {
        if (!e)
            return;

        if (Char::IsControl(e->KeyChar))
            return;

        if (!Char::IsDigit(e->KeyChar))
            e->Handled = true;
    }

    void SetWeightTool::OnEditorKeyDown(Object^, KeyEventArgs^ e)
    {
        if (!e)
            return;

        if (e->KeyCode == Keys::Enter)
        {
            e->Handled = true;
            e->SuppressKeyPress = true;
            CommitEdit();
            return;
        }

        if (e->KeyCode == Keys::Escape)
        {
            e->Handled = true;
            e->SuppressKeyPress = true;
            CancelEdit();
            return;
        }
    }

    void SetWeightTool::OnEditorLeave(Object^, EventArgs^)
    {
        CancelEdit();
    }

    VisualEdge^ SetWeightTool::HitTestEdgeLabel(PointF p)
    {
        if (!Doc || !Doc->Visual)
            return nullptr;

        auto visual = Doc->Visual;
        auto edge = Doc->GetEdgeAt(p);
        if (edge)
        {
            auto bounds = GetEdgeLabelBounds(edge);
            if (!bounds.IsEmpty && bounds.Contains(p))
                return edge;
        }

        for each (auto candidate in visual->Edges)
        {
            if (!candidate)
                continue;

            auto bounds = GetEdgeLabelBounds(candidate);
            if (!bounds.IsEmpty && bounds.Contains(p))
                return candidate;
        }

        return nullptr;
    }

    RectangleF SetWeightTool::GetEdgeLabelBounds(VisualEdge^ edge)
    {
        if (!edge || !Doc || !Doc->Visual)
            return RectangleF();

        PointF center = ComputeLabelPosition(edge);
        if (center.IsEmpty)
            return RectangleF();

        return RectangleF(center.X - kLabelHalfWidth, center.Y - kLabelHalfHeight,
            kLabelHalfWidth * 2.0f, kLabelHalfHeight * 2.0f);
    }

    PointF SetWeightTool::ComputeLabelPosition(VisualEdge^ edge)
    {
        if (!edge || !Doc || !Doc->Visual)
            return PointF();

        auto visual = Doc->Visual;
        auto from = visual->FindNodeByLogicalId(edge->FromLogicalId);
        auto to = visual->FindNodeByLogicalId(edge->ToLogicalId);
        if (!from || !to)
            return PointF();

        if (!edge->LabelPos.IsEmpty)
            return edge->LabelPos;

        PointF midpoint((from->Position.X + to->Position.X) * 0.5f, (from->Position.Y + to->Position.Y) * 0.5f);
        float dx = to->Position.X - from->Position.X;
        float dy = to->Position.Y - from->Position.Y;
        float length = (float)System::Math::Sqrt(dx * dx + dy * dy);
        if (length <= 1e-3f)
            return midpoint;

        float nx = -dy / length;
        float ny = dx / length;
        return PointF(midpoint.X + nx * kLabelNormalOffset, midpoint.Y + ny * kLabelNormalOffset);
    }

    void SetWeightTool::InvalidateEdge(VisualEdge^ edge)
    {
        if (!edge)
            return;

        if (m_controller)
        {
            m_controller->RequestInvalidate(edge);
        }
        else if (m_host)
        {
            RectangleF bounds = GetEdgeLabelBounds(edge);
            if (!bounds.IsEmpty)
                m_host->Invalidate(Rectangle::Ceiling(bounds));
        }
    }
}