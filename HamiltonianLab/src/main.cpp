#include <MainForm.h>

using namespace HamiltonianLab;

[STAThread]
int main()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MainForm());

    return 0;
}