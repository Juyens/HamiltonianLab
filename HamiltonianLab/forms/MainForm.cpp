#include "MainForm.h"

namespace HamiltonianLab
{
	void MainForm::Initialize()
	{
		graphCanvas = gcnew Renderers::GraphCanvas();
		this->graphCanvas->Parent = this->canvasPanel;
		this->graphCanvas->Dock = System::Windows::Forms::DockStyle::Fill;

		auto document = this->graphCanvas ? this->graphCanvas->Document : nullptr;
	}

	ToolMode MainForm::ParseToolMode(System::String^ tag)
	{
		if (System::String::IsNullOrEmpty(tag)) 
			return ToolMode::None;

		if (tag->Equals(L"Select"))    
			return ToolMode::Select;
		if (tag->Equals(L"AddNode"))   
			return ToolMode::AddNode;
		if (tag->Equals(L"AddEdge"))   
			return ToolMode::AddEdge;
		if (tag->Equals(L"SetWeight")) 
			return ToolMode::SetWeight;
		if (tag->Equals(L"Delete"))    
			return ToolMode::Delete;

		return ToolMode::Select;
	}

	MenuCommand MainForm::ParseMenuCommand(System::String^ tag)
	{
		if (System::String::IsNullOrEmpty(tag)) 
			return MenuCommand::None;

		if (tag->Equals(L"NewGraph"))
			return MenuCommand::NewGraph;
		if (tag->Equals(L"NewGraphRandom"))
			return MenuCommand::NewGraphRandom;
		if (tag->Equals(L"Exit"))            
			return MenuCommand::Exit;
		if (tag->Equals(L"FindCycles"))      
			return MenuCommand::FindCycles;
		if (tag->Equals(L"SolveTsp"))       
			return MenuCommand::SolveTsp;
		if (tag->Equals(L"ShowMatrix"))      
			return MenuCommand::ShowMatrix;
		if (tag->Equals(L"About"))           
			return MenuCommand::About;
		if (tag->Equals(L"Instructions"))    
			return MenuCommand::Instructions;

		return MenuCommand::About;
	}
}