#include "NodeLabelAllocator.h"

namespace HamiltonianLab::Models::Labeling
{
    NodeLabelAllocator::NodeLabelAllocator()
        : m_counter(0)
    {
    }

    String^ NodeLabelAllocator::Next()
    {
        String^ label = GenerateLabel(m_counter);
        m_counter++;
        return label;
    }

    void NodeLabelAllocator::Reset()
    {
        m_counter = 0;
    }

    void NodeLabelAllocator::Recompute(IEnumerable<String^>^ existing)
    {
        if (existing == nullptr)
        {
            m_counter = 0;
            return;
        }

        int maxIndex = -1;
        for each (String ^ label in existing)
        {
            int idx = TryParseLabelToIndex(label);
            if (idx > maxIndex)
            {
                maxIndex = idx;
            }
        }

        m_counter = (maxIndex < 0) ? 0 : (maxIndex + 1);
    }

    String^ NodeLabelAllocator::GenerateLabel(int index)
    {
        StringBuilder^ sb = gcnew StringBuilder();
        while (index >= 0)
        {
            sb->Insert(0, wchar_t('A' + (index % 26)));
            index = (index / 26) - 1;
        }
        return sb->ToString();
    }

    int NodeLabelAllocator::TryParseLabelToIndex(String^ label)
    {
        if (String::IsNullOrEmpty(label)) return -1;
        int value = 0;
        for (int i = 0; i < label->Length; ++i)
        {
            wchar_t c = label[i];
            if (c < 'A' || c > 'Z') return -1;
            value = value * 26 + (c - 'A' + 1);
        }
        return value - 1;
    }
}