#pragma once
#include <windows.h>
#include <mmsystem.h> 
#pragma comment(lib, "winmm.lib")
using namespace std;
using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Threading;
using namespace System::Globalization;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text;


ref class Invernadero
{
public:
	Invernadero(TcpClient ^ client);
	void Read();
	void ReadThread();
	void Write();
	void WriteThread();
	Byte getByteRead();
	Byte getPresencValue();
	Byte getLDRValue();
	double getTempValue();
	double getHumValue();
	Byte servo = 0;
	Byte servo1 = 0;
	Byte riego1 = 0;
	bool AlarmActivate=true;
	bool IluminationBool = false;
	bool CalefBool = false;
	bool VentBool = false;
	bool ControlManual = false;
	int TemperaturaMin = 20;
	int TemperaturaMax = 28;
	int HumedadMin = 20;
	int HumedadMax = 50;
	int EmergenciaTempBaja = 0;
	int EmetgenciaTempAlta = 0;
	System::Windows::Forms::RichTextBox^  ConsolaApp;
private:
	boolean IsAliveRead = false, EndLoopClient = false;
	boolean IsAliveWrite = false, EndLoopWrite = false;
	boolean IsAliveRefresh = false, EndLoopRefresh = false;
	Thread^ ReadT, ^WriteT, ^RefreshT;
	
	TcpClient^ clientInvernadero;
	array<Byte>^bytes = gcnew array<Byte>(256);
	NetworkStream^ stream;
	Byte data;
	double TempValue=0;
	double HumValue=0;
	Byte LDRValue = 0x00;
	Byte PresencValue = 0x00;



};

