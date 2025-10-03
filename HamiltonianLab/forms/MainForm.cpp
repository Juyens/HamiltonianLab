#include "MainForm.h"

namespace HamiltonianLab
{
	void MainForm::Initialize()
	{
		graphCanvas = gcnew Renderers::GraphCanvas();
		this->graphCanvas->Parent = this->canvasPanel;
		this->graphCanvas->Dock = System::Windows::Forms::DockStyle::Fill;
	}

	ToolMode MainForm::ParseToolMode(System::String^ tag)
	{
		if (tag == L"Select")     
			return HamiltonianLab::ToolMode::Select;
		if (tag == L"Delete")    
			return HamiltonianLab::ToolMode::Delete;
		if (tag == L"AddNode")    
			return HamiltonianLab::ToolMode::AddNode;
		if (tag == L"AddEdge")    
			return HamiltonianLab::ToolMode::AddEdge;
		if (tag == L"SetWeight")  
			return HamiltonianLab::ToolMode::SetWeight;

		return HamiltonianLab::ToolMode::Select;
	}
}