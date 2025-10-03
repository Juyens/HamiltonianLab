#pragma once

#include <Renderers/GraphCanvas.h>

namespace HamiltonianLab
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			Initialize();
		}

	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void Initialize();
		ToolMode ParseToolMode(System::String^ tag);

	private:
		Renderers::GraphCanvas^ graphCanvas;

	private:
		System::Windows::Forms::Panel^ canvasPanel;
		System::Windows::Forms::ToolTip^ toolTip;
		System::Windows::Forms::ToolStrip^ toolStrip;
		System::Windows::Forms::ToolStripButton^ toolStripSelect;
		System::Windows::Forms::ToolStripButton^ toolStripNode;
		System::Windows::Forms::ToolStripButton^ toolStripEdge;
		System::Windows::Forms::ToolStripButton^ toolStripWeight;
		System::Windows::Forms::ToolStripButton^ toolStripDelete;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator2;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator3;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator4;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator5;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator6;
		System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->canvasPanel = (gcnew System::Windows::Forms::Panel());
			this->toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->toolStrip = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripSelect = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripNode = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripEdge = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripWeight = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripDelete = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// canvasPanel
			// 
			this->canvasPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->canvasPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->canvasPanel->Location = System::Drawing::Point(12, 38);
			this->canvasPanel->Name = L"canvasPanel";
			this->canvasPanel->Size = System::Drawing::Size(1256, 670);
			this->canvasPanel->TabIndex = 0;
			// 
			// toolStrip
			// 
			this->toolStrip->BackColor = System::Drawing::SystemColors::MenuBar;
			this->toolStrip->ImageScalingSize = System::Drawing::Size(20, 20);
			this->toolStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {
				this->toolStripSeparator1,
					this->toolStripSelect, this->toolStripSeparator2, this->toolStripNode, this->toolStripSeparator3, this->toolStripEdge, this->toolStripSeparator4,
					this->toolStripWeight, this->toolStripSeparator5, this->toolStripDelete, this->toolStripSeparator6
			});
			this->toolStrip->Location = System::Drawing::Point(0, 0);
			this->toolStrip->Name = L"toolStrip";
			this->toolStrip->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->toolStrip->Size = System::Drawing::Size(1280, 27);
			this->toolStrip->TabIndex = 6;
			this->toolStrip->Text = L"toolStrip1";
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 27);
			// 
			// toolStripSelect
			// 
			this->toolStripSelect->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripSelect->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripSelect.Image")));
			this->toolStripSelect->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripSelect->Name = L"toolStripSelect";
			this->toolStripSelect->Size = System::Drawing::Size(29, 24);
			this->toolStripSelect->Tag = L"Select";
			this->toolStripSelect->Text = L"Seleccionar";
			this->toolStripSelect->Click += gcnew System::EventHandler(this, &MainForm::OnToolButtonClick);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(6, 27);
			// 
			// toolStripNode
			// 
			this->toolStripNode->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripNode->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripNode.Image")));
			this->toolStripNode->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripNode->Name = L"toolStripNode";
			this->toolStripNode->Size = System::Drawing::Size(29, 24);
			this->toolStripNode->Tag = L"AddNode";
			this->toolStripNode->Text = L"Agregar Nodo";
			this->toolStripNode->Click += gcnew System::EventHandler(this, &MainForm::OnToolButtonClick);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(6, 27);
			// 
			// toolStripEdge
			// 
			this->toolStripEdge->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripEdge->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripEdge.Image")));
			this->toolStripEdge->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripEdge->Name = L"toolStripEdge";
			this->toolStripEdge->Size = System::Drawing::Size(29, 24);
			this->toolStripEdge->Tag = L"AddEdge";
			this->toolStripEdge->Text = L"Agregar Arista";
			this->toolStripEdge->Click += gcnew System::EventHandler(this, &MainForm::OnToolButtonClick);
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(6, 27);
			// 
			// toolStripWeight
			// 
			this->toolStripWeight->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripWeight->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripWeight.Image")));
			this->toolStripWeight->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripWeight->Name = L"toolStripWeight";
			this->toolStripWeight->Size = System::Drawing::Size(29, 24);
			this->toolStripWeight->Tag = L"SetWeight";
			this->toolStripWeight->Text = L"Asignar Peso";
			this->toolStripWeight->Click += gcnew System::EventHandler(this, &MainForm::OnToolButtonClick);
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			this->toolStripSeparator5->Size = System::Drawing::Size(6, 27);
			// 
			// toolStripDelete
			// 
			this->toolStripDelete->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripDelete->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripDelete.Image")));
			this->toolStripDelete->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripDelete->Name = L"toolStripDelete";
			this->toolStripDelete->Size = System::Drawing::Size(29, 24);
			this->toolStripDelete->Tag = L"Delete";
			this->toolStripDelete->Text = L"Eliminar";
			this->toolStripDelete->Click += gcnew System::EventHandler(this, &MainForm::OnToolButtonClick);
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			this->toolStripSeparator6->Size = System::Drawing::Size(6, 27);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(1280, 720);
			this->Controls->Add(this->toolStrip);
			this->Controls->Add(this->canvasPanel);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Hv0.1";
			this->toolStrip->ResumeLayout(false);
			this->toolStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void OnToolButtonClick(System::Object^ sender, System::EventArgs^ e)
		{
			ToolStripButton^ b = dynamic_cast<ToolStripButton^>(sender);
			if (!b || b->Tag == nullptr)
				return;

			ToolMode mode = ParseToolMode(safe_cast<System::String^>(b->Tag));
			if (this->graphCanvas)
				this->graphCanvas->SetToolMode(mode);
		}
    };
}
