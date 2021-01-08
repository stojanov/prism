#include "Test2.h"
#include "Core/Window.h"

void Test2()
{
	Prism::Core::Window myWindow;

	myWindow.Init();
	myWindow.Create(1000, 1000, "Test Window");

	myWindow.Loop();
}