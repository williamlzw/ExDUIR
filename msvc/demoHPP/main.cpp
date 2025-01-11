#include "MainWindow.hpp"

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPWSTR wzCmd, _In_ INT nCmdShow)
{
	MainWindow::GetInstance().CreateMainWindow();
	return 0;
}