#pragma once
#include <windows.h>
#include <mmsystem.h> 
#include "Invernadero.h"
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


namespace ProyectoInvernadero {
	//----------------------------------------------------------------------------------------------
	public ref class ControlApplicationInterface : public System::Windows::Forms::Form
	{
		boolean IsAliveRead = false, EndLoopClient = false;
		boolean IsAliveWrite = false, EndLoopWrite = false;
		boolean IsAliveRefresh = false, EndLoopRefresh = false;
		Thread^ ServerT, ^WriteT, ^RefreshT;

		double ReadValue = -1;
		private: System::Windows::Forms::Button^  button1;
		private: System::Windows::Forms::CheckBox^  ClientC;
	private: System::Windows::Forms::Label^  SensorTempValue;



		private: System::Windows::Forms::RichTextBox^  Consola;
		array<Invernadero^>^aInvernadero = gcnew array<Invernadero^>(10);


	private: System::Windows::Forms::Label^  SensorHumValue;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::CheckBox^  AlarmActivate;
	private: System::Windows::Forms::CheckBox^  Ventilador;
	private: System::Windows::Forms::CheckBox^  Calefactor;
	private: System::Windows::Forms::CheckBox^  Ilumination;
	private: System::Windows::Forms::NumericUpDown^  tempMax;
	private: System::Windows::Forms::NumericUpDown^  tempMin;
	private: System::Windows::Forms::NumericUpDown^  tempMaxRang;
	private: System::Windows::Forms::NumericUpDown^  tempMinRang;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ProgressBar^  progressBarLDR;
	private: System::Windows::Forms::ProgressBar^  progressBarPresenc;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TrackBar^  trackBar2;










			 array<TcpClient^>^ client = gcnew array<TcpClient^>(10);



		//----------------------------------------------------------------------------------------------

		void ControlApplicationInterface::Server()
		{
			if (!IsAliveRead) {
				ServerT = gcnew Thread(gcnew ThreadStart(this, &ControlApplicationInterface::ServerThread));
				ServerT->Start();
			}
		}

		//----------------------------------------------------------------------------------------------

		void ControlApplicationInterface::ServerThread() {
			
			while (true)
			{
				try
				{
					if (true)
					{
						if (true)
						{
							// Set the TcpListener on port 13000.
							Int32 port = 13000;
							IPAddress^ localAddr = IPAddress::Parse("192.168.2.204");
							// TcpListener* server = new TcpListener(port);
							TcpListener^ server = gcnew TcpListener(localAddr, port);

							// Start listening for client requests.
							server->Start();

							// Buffer for reading data
							array<Byte>^bytes = gcnew array<Byte>(20);

							// Enter the listening loop.
							//for (int i = 0; i < 10;i++)
							//{
							int i = 0;
							Consola->Text += ("Waiting for a connection... \n");
							// Perform a blocking call to accept requests.
							// You could also user server.AcceptSocket() here.
							client[i] = server->AcceptTcpClient();
							aInvernadero[i] = gcnew Invernadero(client[i]);
							aInvernadero[i]->Read();
							aInvernadero[i]->Write();
							Consola->Text += ("Connected!\n");
							//}
						}
					}
					
				}
				catch (Exception^ e)
				{

				}
			}
			
		}

		//----------------------------------------------------------------------------------------------

		void ControlApplicationInterface::RefreshValues()
		{
			if (!IsAliveRefresh) {
				RefreshT = gcnew Thread(gcnew ThreadStart(this, &ControlApplicationInterface::RefreshThread));
				RefreshT->Start();
			}
		}

		//----------------------------------------------------------------------------------------------

		void ControlApplicationInterface::RefreshThread() {
			IsAliveRefresh = true;
			FILETIME StartTime, EndTime;
			while (!EndLoopRefresh)
			{
				////////--Incio del tiempo de Calculo--//////
				GetSystemTimePreciseAsFileTime(&StartTime);
				ULARGE_INTEGER ui1;
				ui1.LowPart = StartTime.dwLowDateTime;
				ui1.HighPart = StartTime.dwHighDateTime;
				/////////////////////////////////////////////
				try
				{
					SensorTempValue->Text = aInvernadero[0]->getTempValue().ToString() + "º";
					SensorHumValue->Text = aInvernadero[0]->getHumValue().ToString()+"%";
					progressBarPresenc->Value = aInvernadero[0]->getPresencValue();
					progressBarLDR->Value = aInvernadero[0]->getLDRValue();
					if (client[0]->Available) ClientC->Checked = client[0]->Connected;

				}
				catch (Exception^ e)
				{

					//Consola2->Text = e->ToString();
				}
				
				////////--Fin del tiempo de Calculo--////////
				GetSystemTimePreciseAsFileTime(&EndTime);
				ULARGE_INTEGER ui2;
				ui2.LowPart = EndTime.dwLowDateTime;
				ui2.HighPart = EndTime.dwHighDateTime;
				///////////////////////////////////////////
				Sleep(100 - (int)((ui2.QuadPart - ui1.QuadPart) / 10000.0) % 1000 - 1);
			}
			IsAliveRefresh = false;
		}

		//----------------------------------------------------------------------------------------------

	public:
		ControlApplicationInterface(void)
		{
			InitializeComponent();
		}

	protected:
		~ControlApplicationInterface()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		System::ComponentModel::Container ^components;
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->ClientC = (gcnew System::Windows::Forms::CheckBox());
			this->SensorTempValue = (gcnew System::Windows::Forms::Label());
			this->Consola = (gcnew System::Windows::Forms::RichTextBox());
			this->SensorHumValue = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->AlarmActivate = (gcnew System::Windows::Forms::CheckBox());
			this->Ventilador = (gcnew System::Windows::Forms::CheckBox());
			this->Calefactor = (gcnew System::Windows::Forms::CheckBox());
			this->Ilumination = (gcnew System::Windows::Forms::CheckBox());
			this->tempMax = (gcnew System::Windows::Forms::NumericUpDown());
			this->tempMin = (gcnew System::Windows::Forms::NumericUpDown());
			this->tempMaxRang = (gcnew System::Windows::Forms::NumericUpDown());
			this->tempMinRang = (gcnew System::Windows::Forms::NumericUpDown());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->progressBarPresenc = (gcnew System::Windows::Forms::ProgressBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->progressBarLDR = (gcnew System::Windows::Forms::ProgressBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->button2 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMax))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMin))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMaxRang))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMinRang))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(15, 14);
			this->button1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(128, 102);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Iniciar Servidor";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ControlApplicationInterface::button1_Click);
			// 
			// ClientC
			// 
			this->ClientC->Appearance = System::Windows::Forms::Appearance::Button;
			this->ClientC->Enabled = false;
			this->ClientC->FlatAppearance->CheckedBackColor = System::Drawing::Color::Lime;
			this->ClientC->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ClientC->Location = System::Drawing::Point(148, 14);
			this->ClientC->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->ClientC->Name = L"ClientC";
			this->ClientC->Size = System::Drawing::Size(196, 102);
			this->ClientC->TabIndex = 1;
			this->ClientC->Text = L"Client Connected";
			this->ClientC->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->ClientC->UseVisualStyleBackColor = true;
			// 
			// SensorTempValue
			// 
			this->SensorTempValue->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->SensorTempValue->Location = System::Drawing::Point(16, 38);
			this->SensorTempValue->Name = L"SensorTempValue";
			this->SensorTempValue->Size = System::Drawing::Size(146, 52);
			this->SensorTempValue->TabIndex = 2;
			this->SensorTempValue->Text = L"000";
			this->SensorTempValue->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// Consola
			// 
			this->Consola->Location = System::Drawing::Point(15, 167);
			this->Consola->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Consola->Name = L"Consola";
			this->Consola->Size = System::Drawing::Size(328, 330);
			this->Consola->TabIndex = 5;
			this->Consola->Text = L"";
			// 
			// SensorHumValue
			// 
			this->SensorHumValue->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->SensorHumValue->Location = System::Drawing::Point(168, 38);
			this->SensorHumValue->Name = L"SensorHumValue";
			this->SensorHumValue->Size = System::Drawing::Size(146, 52);
			this->SensorHumValue->TabIndex = 8;
			this->SensorHumValue->Text = L"000";
			this->SensorHumValue->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(2, 200);
			this->trackBar1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->trackBar1->Maximum = 2;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(520, 56);
			this->trackBar1->TabIndex = 9;
			this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::trackBar1_ValueChanged);
			// 
			// AlarmActivate
			// 
			this->AlarmActivate->Appearance = System::Windows::Forms::Appearance::Button;
			this->AlarmActivate->Checked = true;
			this->AlarmActivate->CheckState = System::Windows::Forms::CheckState::Checked;
			this->AlarmActivate->Location = System::Drawing::Point(472, 15);
			this->AlarmActivate->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->AlarmActivate->Name = L"AlarmActivate";
			this->AlarmActivate->Size = System::Drawing::Size(147, 126);
			this->AlarmActivate->TabIndex = 10;
			this->AlarmActivate->Text = L"Conectar Alarma";
			this->AlarmActivate->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->AlarmActivate->UseVisualStyleBackColor = true;
			this->AlarmActivate->CheckedChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::AlarmActivate_CheckedChanged);
			// 
			// Ventilador
			// 
			this->Ventilador->Appearance = System::Windows::Forms::Appearance::Button;
			this->Ventilador->Location = System::Drawing::Point(3, 95);
			this->Ventilador->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Ventilador->Name = L"Ventilador";
			this->Ventilador->Size = System::Drawing::Size(520, 41);
			this->Ventilador->TabIndex = 11;
			this->Ventilador->Text = L"Ventilador";
			this->Ventilador->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->Ventilador->UseVisualStyleBackColor = true;
			this->Ventilador->CheckedChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::Ventilador_CheckedChanged);
			// 
			// Calefactor
			// 
			this->Calefactor->Appearance = System::Windows::Forms::Appearance::Button;
			this->Calefactor->Location = System::Drawing::Point(3, 2);
			this->Calefactor->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Calefactor->Name = L"Calefactor";
			this->Calefactor->Size = System::Drawing::Size(520, 42);
			this->Calefactor->TabIndex = 12;
			this->Calefactor->Text = L"Calefactor";
			this->Calefactor->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->Calefactor->UseVisualStyleBackColor = true;
			this->Calefactor->CheckedChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::Calefactor_CheckedChanged);
			// 
			// Ilumination
			// 
			this->Ilumination->Appearance = System::Windows::Forms::Appearance::Button;
			this->Ilumination->Location = System::Drawing::Point(3, 49);
			this->Ilumination->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Ilumination->Name = L"Ilumination";
			this->Ilumination->Size = System::Drawing::Size(520, 41);
			this->Ilumination->TabIndex = 13;
			this->Ilumination->Text = L"Iluminación";
			this->Ilumination->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->Ilumination->UseVisualStyleBackColor = true;
			this->Ilumination->CheckedChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::Ilumination_CheckedChanged);
			// 
			// tempMax
			// 
			this->tempMax->Location = System::Drawing::Point(16, 46);
			this->tempMax->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tempMax->Name = L"tempMax";
			this->tempMax->Size = System::Drawing::Size(147, 22);
			this->tempMax->TabIndex = 14;
			this->tempMax->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 28, 0, 0, 0 });
			this->tempMax->ValueChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::tempMax_ValueChanged);
			// 
			// tempMin
			// 
			this->tempMin->Location = System::Drawing::Point(16, 116);
			this->tempMin->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tempMin->Name = L"tempMin";
			this->tempMin->Size = System::Drawing::Size(147, 22);
			this->tempMin->TabIndex = 15;
			this->tempMin->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20, 0, 0, 0 });
			this->tempMin->ValueChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::tempMin_ValueChanged);
			// 
			// tempMaxRang
			// 
			this->tempMaxRang->Location = System::Drawing::Point(168, 46);
			this->tempMaxRang->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tempMaxRang->Name = L"tempMaxRang";
			this->tempMaxRang->Size = System::Drawing::Size(147, 22);
			this->tempMaxRang->TabIndex = 16;
			this->tempMaxRang->ValueChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::tempMaxRang_ValueChanged);
			// 
			// tempMinRang
			// 
			this->tempMinRang->Location = System::Drawing::Point(168, 116);
			this->tempMinRang->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tempMinRang->Name = L"tempMinRang";
			this->tempMinRang->Size = System::Drawing::Size(147, 22);
			this->tempMinRang->TabIndex = 17;
			this->tempMinRang->ValueChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::tempMinRang_ValueChanged);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->progressBarPresenc);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->progressBarLDR);
			this->panel1->Controls->Add(this->SensorHumValue);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->SensorTempValue);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Location = System::Drawing::Point(351, 15);
			this->panel1->Margin = System::Windows::Forms::Padding(4);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(635, 101);
			this->panel1->TabIndex = 18;
			// 
			// progressBarPresenc
			// 
			this->progressBarPresenc->Location = System::Drawing::Point(472, 38);
			this->progressBarPresenc->Margin = System::Windows::Forms::Padding(4);
			this->progressBarPresenc->Maximum = 1;
			this->progressBarPresenc->Name = L"progressBarPresenc";
			this->progressBarPresenc->Size = System::Drawing::Size(147, 53);
			this->progressBarPresenc->Step = 1;
			this->progressBarPresenc->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarPresenc->TabIndex = 24;
			// 
			// label1
			// 
			this->label1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label1->Location = System::Drawing::Point(16, 10);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(146, 28);
			this->label1->TabIndex = 19;
			this->label1->Text = L"Temperatura";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// progressBarLDR
			// 
			this->progressBarLDR->Location = System::Drawing::Point(320, 38);
			this->progressBarLDR->Margin = System::Windows::Forms::Padding(4);
			this->progressBarLDR->Maximum = 2;
			this->progressBarLDR->Name = L"progressBarLDR";
			this->progressBarLDR->Size = System::Drawing::Size(147, 53);
			this->progressBarLDR->Step = 1;
			this->progressBarLDR->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarLDR->TabIndex = 23;
			// 
			// label4
			// 
			this->label4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label4->Location = System::Drawing::Point(472, 10);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(146, 28);
			this->label4->TabIndex = 22;
			this->label4->Text = L"Intrusión";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label3
			// 
			this->label3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label3->Location = System::Drawing::Point(320, 10);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(146, 28);
			this->label3->TabIndex = 21;
			this->label3->Text = L"Nivel de Oscuridad";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label2
			// 
			this->label2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label2->Location = System::Drawing::Point(168, 10);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(146, 28);
			this->label2->TabIndex = 20;
			this->label2->Text = L"Humedad";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->label8);
			this->panel2->Controls->Add(this->label7);
			this->panel2->Controls->Add(this->label6);
			this->panel2->Controls->Add(this->label5);
			this->panel2->Controls->Add(this->tempMaxRang);
			this->panel2->Controls->Add(this->AlarmActivate);
			this->panel2->Controls->Add(this->tempMax);
			this->panel2->Controls->Add(this->tempMinRang);
			this->panel2->Controls->Add(this->tempMin);
			this->panel2->Location = System::Drawing::Point(351, 123);
			this->panel2->Margin = System::Windows::Forms::Padding(4);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(635, 166);
			this->panel2->TabIndex = 19;
			// 
			// label8
			// 
			this->label8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label8->Location = System::Drawing::Point(168, 85);
			this->label8->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(146, 28);
			this->label8->TabIndex = 30;
			this->label8->Text = L"Rang Emerg T.Min";
			this->label8->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label7
			// 
			this->label7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label7->Location = System::Drawing::Point(168, 15);
			this->label7->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(146, 28);
			this->label7->TabIndex = 29;
			this->label7->Text = L"Rang Emerg T.Max";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label6
			// 
			this->label6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label6->Location = System::Drawing::Point(16, 85);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(146, 28);
			this->label6->TabIndex = 26;
			this->label6->Text = L"Temperatura Min";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label5
			// 
			this->label5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label5->Location = System::Drawing::Point(16, 15);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(146, 28);
			this->label5->TabIndex = 25;
			this->label5->Text = L"Temperatura Max";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// checkBox1
			// 
			this->checkBox1->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox1->Location = System::Drawing::Point(351, 295);
			this->checkBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(103, 203);
			this->checkBox1->TabIndex = 20;
			this->checkBox1->Text = L"Control Manual";
			this->checkBox1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::checkBox1_CheckedChanged);
			// 
			// panel3
			// 
			this->panel3->Controls->Add(this->trackBar2);
			this->panel3->Controls->Add(this->trackBar1);
			this->panel3->Controls->Add(this->Calefactor);
			this->panel3->Controls->Add(this->Ventilador);
			this->panel3->Controls->Add(this->Ilumination);
			this->panel3->Enabled = false;
			this->panel3->Location = System::Drawing::Point(460, 297);
			this->panel3->Margin = System::Windows::Forms::Padding(4);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(525, 262);
			this->panel3->TabIndex = 21;
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(3, 140);
			this->trackBar2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->trackBar2->Maximum = 2;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(520, 56);
			this->trackBar2->TabIndex = 14;
			this->trackBar2->ValueChanged += gcnew System::EventHandler(this, &ControlApplicationInterface::trackBar2_ValueChanged);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(15, 123);
			this->button2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(329, 43);
			this->button2->TabIndex = 22;
			this->button2->Text = L"Limpiar Consola";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ControlApplicationInterface::button2_Click);
			// 
			// ControlApplicationInterface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1049, 653);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->Consola);
			this->Controls->Add(this->ClientC);
			this->Controls->Add(this->button1);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"ControlApplicationInterface";
			this->Text = L"Servidor Invernadero";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ControlApplicationInterface::ControlApplicationInterface_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMax))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMin))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMaxRang))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempMinRang))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
			this->ResumeLayout(false);

		}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		Server();
		RefreshValues();
		//Write();
	}
private: System::Void ControlApplicationInterface_Load(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void ControlApplicationInterface_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	exit(0);
}
private: System::Void trackBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	aInvernadero[0]->servo1 = (Byte)trackBar1->Value;
}
private: System::Void AlarmActivate_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (AlarmActivate->Checked)aInvernadero[0]->AlarmActivate = true;
	else aInvernadero[0]->AlarmActivate = false;
}
private: System::Void Ventilador_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (Ventilador->Checked)aInvernadero[0]->VentBool = true;
	else aInvernadero[0]->VentBool = false;

}
private: System::Void Calefactor_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (Calefactor->Checked)aInvernadero[0]->CalefBool = true;
	else aInvernadero[0]->CalefBool = false;
}
private: System::Void Ilumination_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (Ilumination->Checked)aInvernadero[0]->IluminationBool = true;
	else aInvernadero[0]->IluminationBool = false;
}

private: System::Void tempMax_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	aInvernadero[0]->TemperaturaMax = (int)tempMax->Value;
}
private: System::Void tempMin_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	aInvernadero[0]->TemperaturaMin = (int)tempMin->Value;
}
private: System::Void tempMaxRang_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	aInvernadero[0]->EmetgenciaTempAlta= (int)tempMaxRang->Value;
}
private: System::Void tempMinRang_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	aInvernadero[0]->EmergenciaTempBaja = (int)tempMinRang->Value;
}
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	if (checkBox1->Checked) 
	{
		aInvernadero[0]->ControlManual = true;
		panel3->Enabled = true;
	}
	else 
	{
		aInvernadero[0]->ControlManual = false;
		panel3->Enabled = false;
	}


}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	Consola->Text = "";
}

private: System::Void trackBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	aInvernadero[0]->riego1 = (Byte)trackBar2->Value;
}
};
}
