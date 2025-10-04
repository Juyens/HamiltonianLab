#include "RandomGraphTool.h"

namespace HamiltonianLab::Tools
{
    RandomGraphTool::RandomGraphTool(
        ToolStripButton^ generateButton,
        ToolStripTextBox^ nodeCountTextBox,
        GraphCanvas^ canvas,
        RandomGraphGenerator^ generator)
        : m_generateButton(generateButton)
        , m_nodeCountTextBox(nodeCountTextBox)
        , m_canvas(canvas)
        , m_generator(generator)
        , m_wired(false)
    {
    }

    void RandomGraphTool::WireUp()
    {
        if (m_wired)
            return;

        if (m_generateButton != nullptr)
        {
            m_generateButton->Click += gcnew EventHandler(this, &RandomGraphTool::OnGenerateClicked);
        }

        m_wired = true;
    }

    bool RandomGraphTool::TryGetNodeCount(int% nodeCount)
    {
        nodeCount = 0;

        if (!m_nodeCountTextBox)
            return false;

        System::String^ text = m_nodeCountTextBox->Text;
        text = text ? text->Trim() : String::Empty;

        int parsed = 0;
        if (!Int32::TryParse(text, NumberStyles::Integer, CultureInfo::InvariantCulture, parsed))
        {
            ShowValidationError();
            return false;
        }

        if (!ValidateNodeCount(parsed))
        {
            ShowValidationError();
            return false;
        }

        nodeCount = parsed;
        return true;
    }

    void RandomGraphTool::OnGenerateClicked(Object^ sender, EventArgs^ e)
    {
        int nodeCount = 0;
        if (!TryGetNodeCount(nodeCount))
            return;

        if (!m_canvas || !m_generator)
            return;

        auto doc = m_canvas->Document;
        if (!doc)
            return;

        m_generator->Generate(doc, nodeCount);
        m_canvas->Invalidate();
    }

    bool RandomGraphTool::ValidateNodeCount(int value)
    {
        return value >= 8 && value <= 16;
    }

    void RandomGraphTool::ShowValidationError()
    {
        MessageBox::Show(
            L"La cantidad de nodos debe estar entre 8 y 16.",
            L"Error",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error);
    }
}