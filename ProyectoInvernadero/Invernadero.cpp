#include "Invernadero.h"



Invernadero::Invernadero(TcpClient^ client)
{
	clientInvernadero = client;
	stream = clientInvernadero->GetStream();
}

//----------------------------------------------------------------------------------------------

void Invernadero::Read()
{
	if (!IsAliveRead) {
		ReadT = gcnew Thread(gcnew ThreadStart(this, &Invernadero::ReadThread));
		ReadT->Start();
	}
}

//----------------------------------------------------------------------------------------------

void Invernadero::ReadThread() {
	try
	{

		while (true)
		{

			
			while (clientInvernadero->Connected)
			{
				stream->Read(bytes, 0, 6);
				TempValue = double(BitConverter::ToInt16(bytes, 0));
				HumValue = BitConverter::ToInt16(bytes, 2);
				LDRValue = bytes[4];
				PresencValue = bytes[5];			
			}
			clientInvernadero->Close();
		}
	}
	catch (Exception^ e)
	{

	}
}

//----------------------------------------------------------------------------------------------



void Invernadero::Write()
{
	if (!IsAliveRead) {
		WriteT = gcnew Thread(gcnew ThreadStart(this, &Invernadero::WriteThread));
		WriteT->Start();
	}
}

//----------------------------------------------------------------------------------------------

void Invernadero::WriteThread() {
	IsAliveWrite = true;
	FILETIME StartTime, EndTime;
	Byte sum = 0x00;
	byte zum = 0x00;
	byte calef = 0x00;
	byte vent = 0x00;
	byte ilum = 0x00;
	byte riego = 0x00;
	while (!EndLoopWrite)
	{
		
		////////--Incio del tiempo de Calculo--//////
		GetSystemTimePreciseAsFileTime(&StartTime);
		ULARGE_INTEGER ui1;
		ui1.LowPart = StartTime.dwLowDateTime;
		ui1.HighPart = StartTime.dwHighDateTime;
		/////////////////////////////////////////////
		try
		{
			/////////Tratamiento de Datos///////////////
			if (!ControlManual)
			{
				
				
				///-//////////////////////////////////
				///Controlador de Humedad -- Riego Automatico -- servo -- ventilador
				if (HumedadMax < HumValue)
				{
					riego = 0x00;
					servo = 0x01;
					vent = 0x01;
				}
				else if (HumedadMin > HumValue)
				{
					riego = 0x01;
					servo = 0x00;
					vent = 0x00;
				}
				else
				{
					riego = 0x00;
					servo = 0x00;
					vent = 0x00;

				}
				///Controlador de temperatura -- Ventilador -- Calefactor -- Servo //
				if (TemperaturaMax < TempValue)
				{
					if (Math::Abs(TemperaturaMax - TempValue)>EmetgenciaTempAlta)
					{
						servo = 0x02;
						vent = 0x01;
						calef = 0x00;
					}
					else
					{
						servo = 0x01;
						vent = 0x00;
						calef = 0x00;
					}
				}
				else if (TemperaturaMin > TempValue)
				{
					if (Math::Abs(TemperaturaMin - TempValue)> EmergenciaTempBaja)
					{
						servo = 0x00;
						vent = 0x01;
						calef = 0x01;
					}
					else
					{
						servo = 0x00;
						vent = 0x01;
						calef = 0x01;
					}
				}
				else
				{
					servo = 0x00;
					vent = 0x00;
					calef = 0x00;
				}
				///-//////////////////////////////////
				///Controlador de iluminacion -- Iluminacion
				if (LDRValue == 0x01)
				{
					ilum = 0x01;
				}
				else ilum = 0x00;
				///-//////////////////////////////////
				///Deteccion de Intrusos -- Zumbador//
				if (PresencValue == 0x01 && AlarmActivate) {
					if (zum == 0x01) {
						zum = 0x00;
						ilum = 0x00;
					}
					else {
						zum = 0x01;
						ilum = 0x01;
					}
				}
				else zum = 0x00;
				///-//////////////////////////////////
			}
			else
			{
				if (CalefBool) calef = 0x01;
				else calef = 0x00;
				if (IluminationBool) ilum = 0x01;
				else ilum = 0x00;
				if (VentBool) vent = 0x01;
				else vent = 0x00;
				servo = servo1;
				riego = riego1;
			}
			



			
		
			///////////////////////////////////////////
			//ConsolaApp->Text += "Enviando... \n";
			array<Byte>^msg = gcnew array<Byte>(7);
			msg[0] = 0xFF;
			msg[1] = servo; // Servo
			msg[2] = calef; // Calefactor
			msg[3] = ilum; // iluminacion
			msg[4] = vent; // ventilador
			msg[5] = zum;//zum;  // Zumbador
			msg[6] = riego; // Riego Auto
			stream->Write(msg, 0, msg->Length);
			//ConsolaApp->Text += "Enviado \n";
			sum += 0x01;
		}
		catch (Exception^ e)
		{
			Console::WriteLine(e->ToString());
		}
		////////--Fin del tiempo de Calculo--////////
		GetSystemTimePreciseAsFileTime(&EndTime);
		ULARGE_INTEGER ui2;
		ui2.LowPart = EndTime.dwLowDateTime;
		ui2.HighPart = EndTime.dwHighDateTime;
		///////////////////////////////////////////
		Sleep(300 - (int)((ui2.QuadPart - ui1.QuadPart) / 10000.0) % 1000 - 1);
	}
	IsAliveWrite = false;
}

Byte Invernadero::getByteRead()
{
	return data;
}

Byte Invernadero::getPresencValue()
{
	return PresencValue;
}

Byte Invernadero::getLDRValue()
{
	return LDRValue;
}

double Invernadero::getTempValue()
{
	return TempValue;
}

double Invernadero::getHumValue()
{
	return HumValue;
}

//----------------------------------------------------------------------------------------------
