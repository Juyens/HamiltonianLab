#pragma once

#include <Models/GraphDocument.h>

namespace HamiltonianLab
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Globalization;
	using namespace HamiltonianLab::Models;
	using namespace HamiltonianLab::Models::Visual;

	public ref class AdjacencyMatrixForm : public System::Windows::Forms::Form
	{
	public:
		AdjacencyMatrixForm(GraphDocument^ document)
		{
			InitializeComponent();
			LoadMatrix(document);
		}

	protected:
		~AdjacencyMatrixForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void LoadMatrix(GraphDocument^ document);
		void AddEmptyPlaceholder();
		static int CompareNodes(VisualNode^ left, VisualNode^ right);
		static System::String^ BuildDisplayLabel(VisualNode^ node);

	private:
		System::ComponentModel::Container^ components;
		System::Windows::Forms::DataGridView^ matrixGrid;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->matrixGrid = (gcnew System::Windows::Forms::DataGridView());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->matrixGrid))->BeginInit();
			this->SuspendLayout();
			// 
			// matrixGrid
			// 
			this->matrixGrid->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->matrixGrid->Dock = System::Windows::Forms::DockStyle::Fill;
			this->matrixGrid->Location = System::Drawing::Point(0, 0);
			this->matrixGrid->Name = L"matrixGrid";
			this->matrixGrid->RowHeadersWidth = 51;
			this->matrixGrid->RowTemplate->Height = 24;
			this->matrixGrid->Size = System::Drawing::Size(720, 480);
			this->matrixGrid->TabIndex = 0;
			// 
			// AdjacencyMatrixForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->ClientSize = System::Drawing::Size(720, 480);
			this->Controls->Add(this->matrixGrid);
			this->MinimumSize = System::Drawing::Size(480, 320);
			this->Name = L"AdjacencyMatrixForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Matriz de adyacencia";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->matrixGrid))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
