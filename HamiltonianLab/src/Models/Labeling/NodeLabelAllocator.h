#pragma once

namespace HamiltonianLab::Models::Labeling
{
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Text;

    public ref class NodeLabelAllocator
    {
    public:
        NodeLabelAllocator();

        String^ Next();
        void Reset();
        void Recompute(IEnumerable<String^>^ existing);

    private:
        String^ GenerateLabel(int index);
        int TryParseLabelToIndex(String^ label);

    private:
        int m_counter;
    };
}