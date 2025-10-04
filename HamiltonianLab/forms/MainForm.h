#pragma once

#include <Controllers/MenuCommandController.h>
#include <Renderers/GraphCanvas.h>
#include <Services/RandomGraphGenerator.h>
#include <Tools/RandomGraphTool.h>

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
			if (menu)
			{
				delete menu;
				menu = nullptr;
			}

			if (randTool)
			{
				delete randTool;
				randTool = nullptr;
			}

			if (randGen)
			{
				delete randGen;
				randGen = nullptr;
			}

			if (components)
			{
				delete components;
				components = nullptr;
			}
		}

	private:
		void Initialize();
		ToolMode ParseToolMode(System::String^ tag);
		MenuCommand ParseMenuCommand(System::String^ tag);

	private:
		Renderers::GraphCanvas^ graphCanvas;
		Controllers::MenuCommandController^ menu;
		Tools::RandomGraphTool^ randTool;
		Services::RandomGraphGenerator^ randGen;

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
		System::Windows::Forms::MenuStrip^ menuStrip;
		System::Windows::Forms::ToolStripMenuItem^ menuFile;
		System::Windows::Forms::ToolStripMenuItem^ itemNewGraph;
		System::Windows::Forms::ToolStripMenuItem^ itemExit;
		System::Windows::Forms::ToolStripMenuItem^ menuAlgorithms;
		System::Windows::Forms::ToolStripMenuItem^ itemFindCycles;
		System::Windows::Forms::ToolStripMenuItem^ itemSolveTsp;
		System::Windows::Forms::ToolStripMenuItem^ itemShowMatrix;
		System::Windows::Forms::ToolStripMenuItem^ menuHelp;
		System::Windows::Forms::ToolStripMenuItem^ itemAbout;
		System::Windows::Forms::ToolStripMenuItem^ itemInstructions;
		System::Windows::Forms::ToolStripButton^ toolStripRandom;
		System::Windows::Forms::ToolStripTextBox^ toolStripNodeCount;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator7;
		System::Windows::Forms::ToolStripLabel^ toolStripLabel;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator8;
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
			this->toolStripRandom = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripLabel = (gcnew System::Windows::Forms::ToolStripLabel());
			this->toolStripNodeCount = (gcnew System::Windows::Forms::ToolStripTextBox());
			this->toolStripSeparator8 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->menuFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemNewGraph = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuAlgorithms = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemFindCycles = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemSolveTsp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemShowMatrix = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemInstructions = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip->SuspendLayout();
			this->menuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// canvasPanel
			// 
			this->canvasPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->canvasPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->canvasPanel->Location = System::Drawing::Point(12, 68);
			this->canvasPanel->Name = L"canvasPanel";
			this->canvasPanel->Size = System::Drawing::Size(1256, 640);
			this->canvasPanel->TabIndex = 0;
			// 
			// toolStrip
			// 
			this->toolStrip->BackColor = System::Drawing::SystemColors::MenuBar;
			this->toolStrip->ImageScalingSize = System::Drawing::Size(20, 20);
			this->toolStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(16) {
				this->toolStripSeparator1,
					this->toolStripSelect, this->toolStripSeparator2, this->toolStripNode, this->toolStripSeparator3, this->toolStripEdge, this->toolStripSeparator4,
					this->toolStripWeight, this->toolStripSeparator5, this->toolStripDelete, this->toolStripSeparator6, this->toolStripRandom, this->toolStripSeparator7,
					this->toolStripLabel, this->toolStripNodeCount, this->toolStripSeparator8
			});
			this->toolStrip->Location = System::Drawing::Point(0, 28);
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
			// toolStripRandom
			// 
			this->toolStripRandom->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripRandom->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripRandom.Image")));
			this->toolStripRandom->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripRandom->Name = L"toolStripRandom";
			this->toolStripRandom->Size = System::Drawing::Size(29, 24);
			this->toolStripRandom->Tag = L"Random";
			this->toolStripRandom->Text = L"Generar grafo aleatorio";
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			this->toolStripSeparator7->Size = System::Drawing::Size(6, 27);
			// 
			// toolStripLabel
			// 
			this->toolStripLabel->Name = L"toolStripLabel";
			this->toolStripLabel->Size = System::Drawing::Size(60, 24);
			this->toolStripLabel->Text = L"Nodos :";
			// 
			// toolStripNodeCount
			// 
			this->toolStripNodeCount->BackColor = System::Drawing::SystemColors::Window;
			this->toolStripNodeCount->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->toolStripNodeCount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->toolStripNodeCount->MaxLength = 2;
			this->toolStripNodeCount->Name = L"toolStripNodeCount";
			this->toolStripNodeCount->Size = System::Drawing::Size(30, 27);
			// 
			// toolStripSeparator8
			// 
			this->toolStripSeparator8->Name = L"toolStripSeparator8";
			this->toolStripSeparator8->Size = System::Drawing::Size(6, 27);
			// 
			// menuStrip
			// 
			this->menuStrip->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->menuFile, this->menuAlgorithms,
					this->menuHelp
			});
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(1280, 28);
			this->menuStrip->TabIndex = 7;
			this->menuStrip->Text = L"menuStrip1";
			// 
			// menuFile
			// 
			this->menuFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->itemNewGraph,
					this->itemExit
			});
			this->menuFile->Name = L"menuFile";
			this->menuFile->Size = System::Drawing::Size(73, 24);
			this->menuFile->Text = L"Archivo";
			// 
			// itemNewGraph
			// 
			this->itemNewGraph->Name = L"itemNewGraph";
			this->itemNewGraph->Size = System::Drawing::Size(175, 26);
			this->itemNewGraph->Tag = L"NewGraph";
			this->itemNewGraph->Text = L"Nuevo grafo";
			this->itemNewGraph->Click += gcnew System::EventHandler(this, &MainForm::OnMenuItemClick);
			// 
			// itemExit
			// 
			this->itemExit->Name = L"itemExit";
			this->itemExit->Size = System::Drawing::Size(175, 26);
			this->itemExit->Tag = L"Exit";
			this->itemExit->Text = L"Salir";
			this->itemExit->Click += gcnew System::EventHandler(this, &MainForm::OnMenuItemClick);
			// 
			// menuAlgorithms
			// 
			this->menuAlgorithms->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->itemFindCycles,
					this->itemSolveTsp, this->itemShowMatrix
			});
			this->menuAlgorithms->Name = L"menuAlgorithms";
			this->menuAlgorithms->Size = System::Drawing::Size(97, 24);
			this->menuAlgorithms->Text = L"Algoritmos";
			// 
			// itemFindCycles
			// 
			this->itemFindCycles->Name = L"itemFindCycles";
			this->itemFindCycles->Size = System::Drawing::Size(294, 26);
			this->itemFindCycles->Tag = L"FindCycles";
			this->itemFindCycles->Text = L"Enumerar ciclos hamiltonianos";
			this->itemFindCycles->Click += gcnew System::EventHandler(this, &MainForm::OnMenuItemClick);
			// 
			// itemSolveTsp
			// 
			this->itemSolveTsp->Name = L"itemSolveTsp";
			this->itemSolveTsp->Size = System::Drawing::Size(294, 26);
			this->itemSolveTsp->Tag = L"SolveTsp";
			this->itemSolveTsp->Text = L"Resolver TSP";
			this->itemSolveTsp->Click += gcnew System::EventHandler(this, &MainForm::OnMenuItemClick);
			// 
			// itemShowMatrix
			// 
			this->itemShowMatrix->Name = L"itemShowMatrix";
			this->itemShowMatrix->Size = System::Drawing::Size(294, 26);
			this->itemShowMatrix->Tag = L"ShowMatrix";
			this->itemShowMatrix->Text = L"Mostrar matriz de adyacencia";
			this->itemShowMatrix->Click += gcnew System::EventHandler(this, &MainForm::OnMenuItemClick);
			// 
			// menuHelp
			// 
			this->menuHelp->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->itemAbout,
					this->itemInstructions
			});
			this->menuHelp->Name = L"menuHelp";
			this->menuHelp->Size = System::Drawing::Size(65, 24);
			this->menuHelp->Text = L"Ayuda";
			// 
			// itemAbout
			// 
			this->itemAbout->Name = L"itemAbout";
			this->itemAbout->Size = System::Drawing::Size(225, 26);
			this->itemAbout->Tag = L"About";
			this->itemAbout->Text = L"Acerca de…";
			this->itemAbout->Click += gcnew System::EventHandler(this, &MainForm::OnMenuItemClick);
			// 
			// itemInstructions
			// 
			this->itemInstructions->Name = L"itemInstructions";
			this->itemInstructions->Size = System::Drawing::Size(225, 26);
			this->itemInstructions->Tag = L"Instructions";
			this->itemInstructions->Text = L"Instrucciones de uso";
			this->itemInstructions->Click += gcnew System::EventHandler(this, &MainForm::OnMenuItemClick);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(1280, 720);
			this->Controls->Add(this->toolStrip);
			this->Controls->Add(this->menuStrip);
			this->Controls->Add(this->canvasPanel);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Hv0.1";
			this->toolStrip->ResumeLayout(false);
			this->toolStrip->PerformLayout();
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void OnToolButtonClick(System::Object^ sender, System::EventArgs^ e)
		{
			auto button = dynamic_cast<ToolStripButton^>(sender);

			if (!button)
				return;

			ToolMode mode = ParseToolMode(safe_cast<System::String^>(button->Tag));
			if (this->graphCanvas)
				this->graphCanvas->SetToolMode(mode);
		}

		System::Void OnMenuItemClick(System::Object^ sender, System::EventArgs^ e)
		{
			auto item = dynamic_cast<System::Windows::Forms::ToolStripMenuItem^>(sender);

			if (!item)
				return;

			MenuCommand command = ParseMenuCommand(safe_cast<System::String^>(item->Tag));

			switch (command)
			{
			case MenuCommand::NewGraph:
			{
				if (!this->graphCanvas)
					return;

				auto document = this->graphCanvas->Document;
				if (!document)
					return;

				document->Clear();
				this->graphCanvas->Invalidate();

				if (this->toolStripNode)
					this->toolStripNode->PerformClick();
				else
					this->graphCanvas->SetToolMode(ToolMode::AddNode);

				if (this->toolStripNodeCount)
				{
					this->toolStripNodeCount->SelectAll();
					if (this->toolStripNodeCount->Control)
						this->toolStripNodeCount->Control->Focus();
				}

				break;
			}
			case MenuCommand::Exit:
			{
				if (this->menu)
				{
					delete this->menu;
					this->menu = nullptr;
				}

				if (this->randTool)
				{
					delete this->randTool;
					this->randTool = nullptr;
				}

				if (this->randGen)
				{
					delete this->randGen;
					this->randGen = nullptr;
				}

				this->Close();
				break;
			}
			default:
				break;
			}
		}
	};
}
