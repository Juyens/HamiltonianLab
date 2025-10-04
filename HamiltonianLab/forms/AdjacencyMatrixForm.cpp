#include "AdjacencyMatrixForm.h"

namespace HamiltonianLab
{
	void AdjacencyMatrixForm::LoadMatrix(GraphDocument^ document)
	{
        this->matrixGrid->Columns->Clear();
        this->matrixGrid->Rows->Clear();

        if (document == nullptr)
            return;

        auto visual = document->Visual;
        if (visual == nullptr || visual->Nodes == nullptr || visual->Nodes->Count == 0)
        {
            AddEmptyPlaceholder();
            return;
        }

        auto nodes = gcnew List<VisualNode^>();
        for each (VisualNode ^ node in visual->Nodes)
        {
            if (node != nullptr)
                nodes->Add(node);
        }

        if (nodes->Count == 0)
        {
            AddEmptyPlaceholder();
            return;
        }

        nodes->Sort(gcnew Comparison<VisualNode^>(AdjacencyMatrixForm::CompareNodes));

        auto culture = CultureInfo::CurrentCulture;
        auto core = document->Core;

        for (int c = 0; c < nodes->Count; ++c)
        {
            auto header = BuildDisplayLabel(nodes[c]);
            auto column = gcnew DataGridViewTextBoxColumn();
            column->Name = header;
            column->HeaderText = header;
            column->ReadOnly = true;
            column->SortMode = DataGridViewColumnSortMode::NotSortable;
            this->matrixGrid->Columns->Add(column);
        }

        for (int r = 0; r < nodes->Count; ++r)
        {
            auto row = gcnew DataGridViewRow();
            row->CreateCells(this->matrixGrid);
            row->HeaderCell->Value = BuildDisplayLabel(nodes[r]);

            for (int c = 0; c < nodes->Count; ++c)
            {
                System::String^ cellValue = nullptr;

                if (r == c)
                {
                    cellValue = L"-";
                }
                else if (core != nullptr && core->HasEdge(nodes[r]->LogicalId, nodes[c]->LogicalId))
                {
                    double weight = core->GetWeight(nodes[r]->LogicalId, nodes[c]->LogicalId);
                    cellValue = weight.ToString(L"0.###", culture);
                }
                else
                {
                    cellValue = L"0";
                }

                row->Cells[c]->Value = cellValue;
            }

            this->matrixGrid->Rows->Add(row);
        }

        this->matrixGrid->AutoResizeColumns(DataGridViewAutoSizeColumnsMode::AllCells);
        this->matrixGrid->AutoResizeRowHeadersWidth(DataGridViewRowHeadersWidthSizeMode::AutoSizeToDisplayedHeaders);
	}

	void AdjacencyMatrixForm::AddEmptyPlaceholder()
	{
        this->matrixGrid->Columns->Clear();
        this->matrixGrid->Rows->Clear();

        auto column = gcnew DataGridViewTextBoxColumn();
        column->Name = L"Vacío";
        column->HeaderText = L"Sin nodos";
        column->ReadOnly = true;
        column->SortMode = DataGridViewColumnSortMode::NotSortable;
        this->matrixGrid->Columns->Add(column);

        this->matrixGrid->Rows->Add(1);
        this->matrixGrid->Rows[0]->HeaderCell->Value = L"-";
        this->matrixGrid->Rows[0]->Cells[0]->Value = L"-";
	}

	int AdjacencyMatrixForm::CompareNodes(VisualNode^ left, VisualNode^ right)
	{
        if (left == nullptr || right == nullptr)
            return 0;

        bool leftHasLabel = !System::String::IsNullOrEmpty(left->Label);
        bool rightHasLabel = !System::String::IsNullOrEmpty(right->Label);

        if (leftHasLabel && rightHasLabel)
        {
            int cmp = System::String::Compare(left->Label, right->Label, StringComparison::CurrentCultureIgnoreCase);
            if (cmp != 0)
                return cmp;
        }
        else if (leftHasLabel != rightHasLabel)
        {
            return leftHasLabel ? -1 : 1;
        }

        if (left->LogicalId < right->LogicalId)
            return -1;
        if (left->LogicalId > right->LogicalId)
            return 1;
        return 0;
	}

	System::String^ AdjacencyMatrixForm::BuildDisplayLabel(VisualNode^ node)
	{
        if (node == nullptr)
            return L"";

        if (!System::String::IsNullOrEmpty(node->Label))
            return System::String::Format(CultureInfo::CurrentCulture, L"{0} ({1})", node->Label, node->LogicalId);

        return System::String::Format(CultureInfo::CurrentCulture, L"Nodo {0}", node->LogicalId);
	}
}
