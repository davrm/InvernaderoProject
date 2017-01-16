#include "ControlApplicationInterface.h"	

using namespace System;
using namespace System::Windows::Forms;


void Main(array<String^>^ args)
{

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ProyectoInvernadero::ControlApplicationInterface Interface;
	Application::Run(%Interface);

}