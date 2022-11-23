#pragma once

#include "Try.h"
#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"

#define LEFT_BUTTON   0x0000
#define MIDDLE_BUTTON 0x0001
#define RIGHT_BUTTON  0x0002
#define WHEEL_UP     1
#define WHEEL_DOWN  -1

//#define ADD_FACE		1
//#define SELECT_POINT	2
//#define DEL_FACE		3


Tri_Mesh *mesh;

xform xf;
GLCamera camera;
float fov = 0.7f;
std::string filename = "./resource/3DModel/test.obj";	// load model

static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};
Mouse::button Mouse_State = Mouse::ROTATE;

namespace OpenMesh_EX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadModelToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openModelDialog;
	private: System::Windows::Forms::SaveFileDialog^  saveModelDialog;
	private: System::Windows::Forms::ToolStripMenuItem^  saveModelToolStripMenuItem;
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::RadioButton^ radioButton2;
	private: System::Windows::Forms::RadioButton^ radioButton1;
	private: System::Windows::Forms::RadioButton^ radioButton3;
	private: System::Windows::Forms::CheckBox^ OneRing_Face_checkBox1;
	private: System::Windows::Forms::CheckBox^ OneRing_Vertex_checkBox2;


	private: System::Windows::Forms::Button^ NewM_Button;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl2;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::RadioButton^ TextureMode;

	private: System::Windows::Forms::RadioButton^ PickMode;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::PictureBox^ pictureBox3;
	private: System::Windows::Forms::PictureBox^ pictureBox4;
	private: System::Windows::Forms::RadioButton^ radioButton4;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::RadioButton^ radioButton7;
	private: System::Windows::Forms::RadioButton^ radioButton6;
	private: System::Windows::Forms::RadioButton^ radioButton5;
	private: System::Windows::Forms::GroupBox^ OneRingGroupBox;
	private: System::Windows::Forms::HScrollBar^ RotateScrollBar1;
	private: System::Windows::Forms::GroupBox^ RotateBox;
	private: System::Windows::Forms::HScrollBar^ ScaleScrollBar1;
	private: System::Windows::Forms::Label^ Scael_label;
	private: System::Windows::Forms::Label^ Rotate_label;
	private: System::Windows::Forms::RadioButton^ radioButton9;
	private: System::Windows::Forms::RadioButton^ radioButton8;
	private: System::Windows::Forms::PictureBox^ pictureBox6;
	private: System::Windows::Forms::PictureBox^ pictureBox5;
	private: System::Windows::Forms::Button^ Save_button;
	private: System::Windows::Forms::RadioButton^ radioButton11;
	private: System::Windows::Forms::RadioButton^ radioButton10;
	private: System::Windows::Forms::PictureBox^ pictureBox8;
	private: System::Windows::Forms::PictureBox^ pictureBox7;




	protected:

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting5 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat5 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting6 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat6 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->OneRing_Face_checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->OneRing_Vertex_checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->NewM_Button = (gcnew System::Windows::Forms::Button());
			this->hkoglPanelControl2 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->TextureMode = (gcnew System::Windows::Forms::RadioButton());
			this->PickMode = (gcnew System::Windows::Forms::RadioButton());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton9 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton8 = (gcnew System::Windows::Forms::RadioButton());
			this->pictureBox6 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->radioButton7 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton6 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton5 = (gcnew System::Windows::Forms::RadioButton());
			this->OneRingGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->RotateScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->RotateBox = (gcnew System::Windows::Forms::GroupBox());
			this->Scael_label = (gcnew System::Windows::Forms::Label());
			this->Rotate_label = (gcnew System::Windows::Forms::Label());
			this->ScaleScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->Save_button = (gcnew System::Windows::Forms::Button());
			this->pictureBox7 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox8 = (gcnew System::Windows::Forms::PictureBox());
			this->radioButton10 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton11 = (gcnew System::Windows::Forms::RadioButton());
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->BeginInit();
			this->OneRingGroupBox->SuspendLayout();
			this->RotateBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1076, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->loadModelToolStripMenuItem,
					this->saveModelToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadModelToolStripMenuItem
			// 
			this->loadModelToolStripMenuItem->Name = L"loadModelToolStripMenuItem";
			this->loadModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->loadModelToolStripMenuItem->Text = L"Load Model";
			this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
			// 
			// saveModelToolStripMenuItem
			// 
			this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
			this->saveModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->saveModelToolStripMenuItem->Text = L"Save Model";
			this->saveModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveModelToolStripMenuItem_Click);
			// 
			// openModelDialog
			// 
			this->openModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openModelDialog_FileOk);
			// 
			// saveModelDialog
			// 
			this->saveModelDialog->DefaultExt = L"obj";
			this->saveModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveModelDialog_FileOk);
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting5->Far = 1000;
			hkcoglPanelCameraSetting5->Fov = 45;
			hkcoglPanelCameraSetting5->Near = -1000;
			hkcoglPanelCameraSetting5->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting5;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 24);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat5->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat5->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat5->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat5;
			this->hkoglPanelControl1->Size = System::Drawing::Size(582, 522);
			this->hkoglPanelControl1->TabIndex = 2;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::hkoglPanelControl1_KeyPress);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseUp);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButton3);
			this->groupBox1->Controls->Add(this->radioButton2);
			this->groupBox1->Controls->Add(this->radioButton1);
			this->groupBox1->Location = System::Drawing::Point(594, 125);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(114, 133);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Pick Mode";
			this->groupBox1->Visible = false;
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(6, 97);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(69, 16);
			this->radioButton3->TabIndex = 2;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"DEL Face";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->Click += gcnew System::EventHandler(this, &MyForm::radioButton3_Click);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(6, 58);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(70, 16);
			this->radioButton2->TabIndex = 1;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"Pick Point";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->Click += gcnew System::EventHandler(this, &MyForm::radioButton2_Click);
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(6, 21);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(67, 16);
			this->radioButton1->TabIndex = 0;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Pick Face";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->Click += gcnew System::EventHandler(this, &MyForm::radioButton1_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->LargeChange = 1;
			this->trackBar1->Location = System::Drawing::Point(0, 65);
			this->trackBar1->Minimum = 1;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(114, 45);
			this->trackBar1->TabIndex = 1;
			this->trackBar1->Value = 1;
			this->trackBar1->Visible = false;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar1_Scroll);
			// 
			// OneRing_Face_checkBox1
			// 
			this->OneRing_Face_checkBox1->AutoSize = true;
			this->OneRing_Face_checkBox1->Location = System::Drawing::Point(6, 21);
			this->OneRing_Face_checkBox1->Name = L"OneRing_Face_checkBox1";
			this->OneRing_Face_checkBox1->Size = System::Drawing::Size(45, 16);
			this->OneRing_Face_checkBox1->TabIndex = 4;
			this->OneRing_Face_checkBox1->Text = L"Face";
			this->OneRing_Face_checkBox1->UseVisualStyleBackColor = true;
			this->OneRing_Face_checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::OneRing_Face_checkBox1_CheckedChanged);
			// 
			// OneRing_Vertex_checkBox2
			// 
			this->OneRing_Vertex_checkBox2->AutoSize = true;
			this->OneRing_Vertex_checkBox2->Location = System::Drawing::Point(6, 43);
			this->OneRing_Vertex_checkBox2->Name = L"OneRing_Vertex_checkBox2";
			this->OneRing_Vertex_checkBox2->Size = System::Drawing::Size(55, 16);
			this->OneRing_Vertex_checkBox2->TabIndex = 5;
			this->OneRing_Vertex_checkBox2->Text = L"Vertex";
			this->OneRing_Vertex_checkBox2->UseVisualStyleBackColor = true;
			this->OneRing_Vertex_checkBox2->CheckedChanged += gcnew System::EventHandler(this, &MyForm::OneRing_Vertex_checkBox2_CheckedChanged);
			// 
			// NewM_Button
			// 
			this->NewM_Button->Location = System::Drawing::Point(623, 535);
			this->NewM_Button->Name = L"NewM_Button";
			this->NewM_Button->Size = System::Drawing::Size(75, 23);
			this->NewM_Button->TabIndex = 6;
			this->NewM_Button->Text = L"Para";
			this->NewM_Button->UseVisualStyleBackColor = true;
			this->NewM_Button->Click += gcnew System::EventHandler(this, &MyForm::NewM_Button_Click);
			// 
			// hkoglPanelControl2
			// 
			hkcoglPanelCameraSetting6->Far = 1000;
			hkcoglPanelCameraSetting6->Fov = 45;
			hkcoglPanelCameraSetting6->Near = -1000;
			hkcoglPanelCameraSetting6->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl2->Camera_Setting = hkcoglPanelCameraSetting6;
			this->hkoglPanelControl2->Location = System::Drawing::Point(495, 572);
			this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
			hkcoglPanelPixelFormat6->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat6->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat6->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl2->Pixel_Format = hkcoglPanelPixelFormat6;
			this->hkoglPanelControl2->Size = System::Drawing::Size(35, 35);
			this->hkoglPanelControl2->TabIndex = 7;
			this->hkoglPanelControl2->Visible = false;
			this->hkoglPanelControl2->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl2_Load);
			this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl2_Paint);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->TextureMode);
			this->groupBox2->Controls->Add(this->PickMode);
			this->groupBox2->Location = System::Drawing::Point(594, 25);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(114, 89);
			this->groupBox2->TabIndex = 8;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Mode";
			// 
			// TextureMode
			// 
			this->TextureMode->AutoSize = true;
			this->TextureMode->Location = System::Drawing::Point(6, 56);
			this->TextureMode->Name = L"TextureMode";
			this->TextureMode->Size = System::Drawing::Size(59, 16);
			this->TextureMode->TabIndex = 1;
			this->TextureMode->TabStop = true;
			this->TextureMode->Text = L"Texture";
			this->TextureMode->UseVisualStyleBackColor = true;
			this->TextureMode->Click += gcnew System::EventHandler(this, &MyForm::TextureMode_Click);
			// 
			// PickMode
			// 
			this->PickMode->AutoSize = true;
			this->PickMode->Location = System::Drawing::Point(6, 21);
			this->PickMode->Name = L"PickMode";
			this->PickMode->Size = System::Drawing::Size(73, 16);
			this->PickMode->TabIndex = 0;
			this->PickMode->TabStop = true;
			this->PickMode->Text = L"Pick Mode";
			this->PickMode->UseVisualStyleBackColor = true;
			this->PickMode->Click += gcnew System::EventHandler(this, &MyForm::PickMode_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(28, 15);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(100, 100);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 9;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(180, 15);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(100, 100);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 10;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(28, 157);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(100, 100);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox3->TabIndex = 11;
			this->pictureBox3->TabStop = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(180, 157);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(100, 100);
			this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox4->TabIndex = 12;
			this->pictureBox4->TabStop = false;
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Checked = true;
			this->radioButton4->Location = System::Drawing::Point(43, 121);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(46, 16);
			this->radioButton4->TabIndex = 13;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"Pic 1";
			this->radioButton4->UseVisualStyleBackColor = true;
			this->radioButton4->Click += gcnew System::EventHandler(this, &MyForm::radioButton4_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->radioButton11);
			this->groupBox3->Controls->Add(this->radioButton10);
			this->groupBox3->Controls->Add(this->pictureBox8);
			this->groupBox3->Controls->Add(this->pictureBox7);
			this->groupBox3->Controls->Add(this->radioButton9);
			this->groupBox3->Controls->Add(this->radioButton8);
			this->groupBox3->Controls->Add(this->pictureBox6);
			this->groupBox3->Controls->Add(this->pictureBox5);
			this->groupBox3->Controls->Add(this->radioButton7);
			this->groupBox3->Controls->Add(this->radioButton6);
			this->groupBox3->Controls->Add(this->pictureBox4);
			this->groupBox3->Controls->Add(this->radioButton5);
			this->groupBox3->Controls->Add(this->pictureBox3);
			this->groupBox3->Controls->Add(this->radioButton4);
			this->groupBox3->Controls->Add(this->pictureBox1);
			this->groupBox3->Controls->Add(this->pictureBox2);
			this->groupBox3->Location = System::Drawing::Point(755, 27);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(309, 580);
			this->groupBox3->TabIndex = 14;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Texture";
			// 
			// radioButton9
			// 
			this->radioButton9->AutoSize = true;
			this->radioButton9->Location = System::Drawing::Point(195, 403);
			this->radioButton9->Name = L"radioButton9";
			this->radioButton9->Size = System::Drawing::Size(46, 16);
			this->radioButton9->TabIndex = 20;
			this->radioButton9->Text = L"Pic 6";
			this->radioButton9->UseVisualStyleBackColor = true;
			this->radioButton9->Click += gcnew System::EventHandler(this, &MyForm::radioButton9_Click);
			// 
			// radioButton8
			// 
			this->radioButton8->AutoSize = true;
			this->radioButton8->Location = System::Drawing::Point(43, 403);
			this->radioButton8->Name = L"radioButton8";
			this->radioButton8->Size = System::Drawing::Size(46, 16);
			this->radioButton8->TabIndex = 19;
			this->radioButton8->Text = L"Pic 5";
			this->radioButton8->UseVisualStyleBackColor = true;
			this->radioButton8->Click += gcnew System::EventHandler(this, &MyForm::radioButton8_Click);
			// 
			// pictureBox6
			// 
			this->pictureBox6->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox6.Image")));
			this->pictureBox6->Location = System::Drawing::Point(180, 297);
			this->pictureBox6->Name = L"pictureBox6";
			this->pictureBox6->Size = System::Drawing::Size(100, 100);
			this->pictureBox6->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox6->TabIndex = 18;
			this->pictureBox6->TabStop = false;
			// 
			// pictureBox5
			// 
			this->pictureBox5->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox5.Image")));
			this->pictureBox5->Location = System::Drawing::Point(28, 297);
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->Size = System::Drawing::Size(100, 100);
			this->pictureBox5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox5->TabIndex = 17;
			this->pictureBox5->TabStop = false;
			// 
			// radioButton7
			// 
			this->radioButton7->AutoSize = true;
			this->radioButton7->Location = System::Drawing::Point(195, 263);
			this->radioButton7->Name = L"radioButton7";
			this->radioButton7->Size = System::Drawing::Size(46, 16);
			this->radioButton7->TabIndex = 16;
			this->radioButton7->Text = L"Pic 4";
			this->radioButton7->UseVisualStyleBackColor = true;
			this->radioButton7->Click += gcnew System::EventHandler(this, &MyForm::radioButton7_Click);
			// 
			// radioButton6
			// 
			this->radioButton6->AutoSize = true;
			this->radioButton6->Location = System::Drawing::Point(43, 263);
			this->radioButton6->Name = L"radioButton6";
			this->radioButton6->Size = System::Drawing::Size(46, 16);
			this->radioButton6->TabIndex = 15;
			this->radioButton6->Text = L"Pic 3";
			this->radioButton6->UseVisualStyleBackColor = true;
			this->radioButton6->Click += gcnew System::EventHandler(this, &MyForm::radioButton6_Click);
			// 
			// radioButton5
			// 
			this->radioButton5->AutoSize = true;
			this->radioButton5->Location = System::Drawing::Point(195, 121);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(46, 16);
			this->radioButton5->TabIndex = 14;
			this->radioButton5->Text = L"Pic 2";
			this->radioButton5->UseVisualStyleBackColor = true;
			this->radioButton5->Click += gcnew System::EventHandler(this, &MyForm::radioButton5_Click);
			// 
			// OneRingGroupBox
			// 
			this->OneRingGroupBox->Controls->Add(this->OneRing_Face_checkBox1);
			this->OneRingGroupBox->Controls->Add(this->OneRing_Vertex_checkBox2);
			this->OneRingGroupBox->Controls->Add(this->trackBar1);
			this->OneRingGroupBox->Location = System::Drawing::Point(594, 275);
			this->OneRingGroupBox->Name = L"OneRingGroupBox";
			this->OneRingGroupBox->Size = System::Drawing::Size(116, 125);
			this->OneRingGroupBox->TabIndex = 15;
			this->OneRingGroupBox->TabStop = false;
			this->OneRingGroupBox->Text = L"OneRing";
			this->OneRingGroupBox->Visible = false;
			// 
			// RotateScrollBar1
			// 
			this->RotateScrollBar1->Location = System::Drawing::Point(12, 31);
			this->RotateScrollBar1->Maximum = 360;
			this->RotateScrollBar1->Name = L"RotateScrollBar1";
			this->RotateScrollBar1->Size = System::Drawing::Size(116, 17);
			this->RotateScrollBar1->TabIndex = 16;
			this->RotateScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &MyForm::RotateScrollBar1_Scroll);
			// 
			// RotateBox
			// 
			this->RotateBox->Controls->Add(this->Scael_label);
			this->RotateBox->Controls->Add(this->Rotate_label);
			this->RotateBox->Controls->Add(this->ScaleScrollBar1);
			this->RotateBox->Controls->Add(this->RotateScrollBar1);
			this->RotateBox->Location = System::Drawing::Point(594, 406);
			this->RotateBox->Name = L"RotateBox";
			this->RotateBox->Size = System::Drawing::Size(141, 108);
			this->RotateBox->TabIndex = 17;
			this->RotateBox->TabStop = false;
			// 
			// Scael_label
			// 
			this->Scael_label->AutoSize = true;
			this->Scael_label->Font = (gcnew System::Drawing::Font(L"新細明體", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Scael_label->Location = System::Drawing::Point(10, 60);
			this->Scael_label->Name = L"Scael_label";
			this->Scael_label->Size = System::Drawing::Size(29, 12);
			this->Scael_label->TabIndex = 18;
			this->Scael_label->Text = L"Scale";
			// 
			// Rotate_label
			// 
			this->Rotate_label->AutoSize = true;
			this->Rotate_label->Font = (gcnew System::Drawing::Font(L"新細明體", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Rotate_label->Location = System::Drawing::Point(10, 17);
			this->Rotate_label->Name = L"Rotate_label";
			this->Rotate_label->Size = System::Drawing::Size(35, 12);
			this->Rotate_label->TabIndex = 19;
			this->Rotate_label->Text = L"Rotate";
			// 
			// ScaleScrollBar1
			// 
			this->ScaleScrollBar1->Location = System::Drawing::Point(12, 74);
			this->ScaleScrollBar1->Minimum = 1;
			this->ScaleScrollBar1->Name = L"ScaleScrollBar1";
			this->ScaleScrollBar1->Size = System::Drawing::Size(116, 17);
			this->ScaleScrollBar1->TabIndex = 17;
			this->ScaleScrollBar1->Value = 1;
			this->ScaleScrollBar1->ValueChanged += gcnew System::EventHandler(this, &MyForm::ScaleScrollBar1_ValueChanged);
			// 
			// Save_button
			// 
			this->Save_button->Location = System::Drawing::Point(623, 584);
			this->Save_button->Name = L"Save_button";
			this->Save_button->Size = System::Drawing::Size(75, 23);
			this->Save_button->TabIndex = 18;
			this->Save_button->Text = L"Save";
			this->Save_button->UseVisualStyleBackColor = true;
			this->Save_button->Click += gcnew System::EventHandler(this, &MyForm::Save_button_Click);
			// 
			// pictureBox7
			// 
			this->pictureBox7->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox7.Image")));
			this->pictureBox7->Location = System::Drawing::Point(28, 439);
			this->pictureBox7->Name = L"pictureBox7";
			this->pictureBox7->Size = System::Drawing::Size(100, 100);
			this->pictureBox7->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox7->TabIndex = 21;
			this->pictureBox7->TabStop = false;
			// 
			// pictureBox8
			// 
			this->pictureBox8->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox8.Image")));
			this->pictureBox8->Location = System::Drawing::Point(180, 439);
			this->pictureBox8->Name = L"pictureBox8";
			this->pictureBox8->Size = System::Drawing::Size(100, 100);
			this->pictureBox8->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox8->TabIndex = 22;
			this->pictureBox8->TabStop = false;
			// 
			// radioButton10
			// 
			this->radioButton10->AutoSize = true;
			this->radioButton10->Location = System::Drawing::Point(43, 545);
			this->radioButton10->Name = L"radioButton10";
			this->radioButton10->Size = System::Drawing::Size(46, 16);
			this->radioButton10->TabIndex = 23;
			this->radioButton10->Text = L"Pic 7";
			this->radioButton10->UseVisualStyleBackColor = true;
			this->radioButton10->Click += gcnew System::EventHandler(this, &MyForm::radioButton10_Click);
			// 
			// radioButton11
			// 
			this->radioButton11->AutoSize = true;
			this->radioButton11->Location = System::Drawing::Point(195, 545);
			this->radioButton11->Name = L"radioButton11";
			this->radioButton11->Size = System::Drawing::Size(46, 16);
			this->radioButton11->TabIndex = 24;
			this->radioButton11->Text = L"Pic 8";
			this->radioButton11->UseVisualStyleBackColor = true;
			this->radioButton11->Click += gcnew System::EventHandler(this, &MyForm::radioButton11_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1076, 638);
			this->Controls->Add(this->Save_button);
			this->Controls->Add(this->RotateBox);
			this->Controls->Add(this->OneRingGroupBox);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->hkoglPanelControl2);
			this->Controls->Add(this->NewM_Button);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->groupBox3);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"PickMEsh";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->EndInit();
			this->OneRingGroupBox->ResumeLayout(false);
			this->OneRingGroupBox->PerformLayout();
			this->RotateBox->ResumeLayout(false);
			this->RotateBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e)
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW is not initialized\n";
		return;
	}
	InitOpenGL();
	InitData(filename);

}
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
	RenderMeshWindow();
}

#pragma region Parameteriztion
private: System::Void hkoglPanelControl2_Load(System::Object^ sender, System::EventArgs^ e)
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW is not initialized\n";
		return;
	}
	InitOpenGL();
	InitData(filename);
	InitCamera2();
}
private: System::Void hkoglPanelControl2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	RenderTexCoordWindow();
}
#pragma endregion


#pragma region Mouse Action (Camera Controller)
private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left )
	{
		Mouse_Press(LEFT_BUTTON, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		Mouse_Press(MIDDLE_BUTTON, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		Mouse_Press(RIGHT_BUTTON, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
}
private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left)
	{
		Mouse_Moving(e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		Mouse_Moving(e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		Mouse_Moving(e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}

}
private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Delta < 0)
	{
		camera1.wheelEvent(WHEEL_DOWN);
		hkoglPanelControl1->Invalidate();
	}
	else
	{
		camera1.wheelEvent(WHEEL_UP);
		hkoglPanelControl1->Invalidate();
	}
}

private: System::Void hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left)
	{
		Mouse_Release(LEFT_BUTTON, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		Mouse_Release(MIDDLE_BUTTON, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
	if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		Mouse_Release(RIGHT_BUTTON, e->X, e->Y);
		hkoglPanelControl1->Invalidate();
	}
}
private: System::Void hkoglPanelControl1_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
	if (e->KeyChar == (char)System::Windows::Forms::Keys::W)
	{
		keypress_function(Camera_Movement::FORWARD);
	}
	if (e->KeyChar == (char)System::Windows::Forms::Keys::A)
	{
		keypress_function(Camera_Movement::LEFT);
	}
	if (e->KeyChar == (char)System::Windows::Forms::Keys::S)
	{
		keypress_function(Camera_Movement::BACKWARD);
	}
	if (e->KeyChar == (char)System::Windows::Forms::Keys::D)
	{
		keypress_function(Camera_Movement::RIGHT);
	}
	hkoglPanelControl1->Invalidate();
}


#pragma endregion

#pragma region Load/Save
private: System::Void loadModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	openModelDialog->Filter = "Model(*.obj)|*obj";
	openModelDialog->Multiselect = false;
	openModelDialog->ShowDialog();
}
private: System::Void openModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	// std::string filename;
	MarshalString(openModelDialog->FileName, filename);

	InitData(filename);

	/*if (mesh != NULL)
		delete mesh;

	mesh = new Tri_Mesh;

	if (ReadFile(filename, mesh))
		std::cout << filename << std::endl;*/

	hkoglPanelControl1->Invalidate();
}
private: System::Void saveModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	saveModelDialog->Filter = "Model(*.obj)|*obj";
	saveModelDialog->ShowDialog();
}
private: System::Void saveModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	std::string filename;
	MarshalString(saveModelDialog->FileName, filename);

	// TODO mesh -> MeshObject.mode
	if (SaveFile(filename, mesh))
		std::cout << filename << std::endl;
}
#pragma endregion

#pragma region Radio Button Control
private: System::Void radioButton1_Click(System::Object^ sender, System::EventArgs^ e) 
{
	pickMode = PickMode::ADD_FACE;
	hkoglPanelControl1->Invalidate();
}
private: System::Void radioButton2_Click(System::Object^ sender, System::EventArgs^ e)
{
	pickMode = PickMode::SELECT_POINT;
	hkoglPanelControl1->Invalidate();
}
private: System::Void radioButton3_Click(System::Object^ sender, System::EventArgs^ e) 
{	
	pickMode = PickMode::DEL_FACE;
	hkoglPanelControl1->Invalidate();
}
#pragma endregion

#pragma region One Ring
private: System::Void OneRing_Face_checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	// oneRing face
	if (OneRing_Face_checkBox1->Checked)
	{
		OneRingCheckFace = true;

		if (OneRing_Vertex_checkBox2->Checked)
			OneRing_Vertex_checkBox2->Checked = false;
	}
	else
	{
		OneRingCheckFace = false;
	}

	if (OneRing_Vertex_checkBox2->Checked || OneRing_Face_checkBox1->Checked)
	{
		trackBar1->Visible = true;
	}
	else trackBar1->Visible = false;

	hkoglPanelControl1->Invalidate();
}
private: System::Void OneRing_Vertex_checkBox2_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	// oneRing vertex
	if (OneRing_Vertex_checkBox2->Checked)
	{
		OneRingCheckVertex = true;

		if (OneRing_Face_checkBox1->Checked)
			OneRing_Face_checkBox1->Checked = false;
	}
	else
	{
		OneRingCheckVertex = false;
	}

	if (OneRing_Vertex_checkBox2->Checked || OneRing_Face_checkBox1->Checked)
	{
		trackBar1->Visible = true;
	}
	else trackBar1->Visible = false;

	hkoglPanelControl1->Invalidate();
}

private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	OneRingTime = trackBar1->Value;
	cout << OneRingTime << endl;
}
#pragma endregion

private: System::Void NewM_Button_Click(System::Object^ sender, System::EventArgs^ e) 
{
	Para();
	hkoglPanelControl2->Invalidate();
}

#pragma region Pick / Texcture Mode
private: System::Void PickMode_Click(System::Object^ sender, System::EventArgs^ e) 
{
	groupBox1->Visible = true;
	OneRingGroupBox->Visible = true;
	pickMode = PickMode::None;
	radioButton1->Checked = false;
	radioButton2->Checked = false;
	radioButton3->Checked = false;

	drawTexture = false;
	hkoglPanelControl1->Invalidate();
}
private: System::Void TextureMode_Click(System::Object^ sender, System::EventArgs^ e)
{
	groupBox1->Visible = false;
	OneRingGroupBox->Visible = false;
	pickMode = PickMode::None;

	drawTexture = true;
	
	model.ClearAllSelectedFace();
	
	hkoglPanelControl1->Invalidate();
}
#pragma endregion

#pragma Pick Texture
private: System::Void radioButton4_Click(System::Object^ sender, System::EventArgs^ e)
{
	PickTextureNum = 0;
	hkoglPanelControl1->Invalidate();
}

private: System::Void radioButton5_Click(System::Object^ sender, System::EventArgs^ e) 
{
	PickTextureNum = 1;
	hkoglPanelControl1->Invalidate();
}

private: System::Void radioButton6_Click(System::Object^ sender, System::EventArgs^ e)
{
	PickTextureNum = 2;
	hkoglPanelControl1->Invalidate();
}

private: System::Void radioButton7_Click(System::Object^ sender, System::EventArgs^ e) 
{
	PickTextureNum = 3;
	hkoglPanelControl1->Invalidate();
}
private: System::Void radioButton8_Click(System::Object^ sender, System::EventArgs^ e) 
{
	PickTextureNum = 4;
	hkoglPanelControl1->Invalidate();
}

private: System::Void radioButton9_Click(System::Object^ sender, System::EventArgs^ e) 
{
	PickTextureNum = 5;
	hkoglPanelControl1->Invalidate();
}
private: System::Void radioButton10_Click(System::Object^ sender, System::EventArgs^ e) 
{
	PickTextureNum = 6;
	hkoglPanelControl1->Invalidate();
}
private: System::Void radioButton11_Click(System::Object^ sender, System::EventArgs^ e) 
{
	PickTextureNum = 7;
	hkoglPanelControl1->Invalidate();
}
#pragma endregion


private: System::Void RotateScrollBar1_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) 
{
	// Para(RotateScrollBar1->Value);
	ObjTemp.back().Parameterization(RotateScrollBar1->Value);
	prevUVRotateAngle = RotateScrollBar1->Value;
	hkoglPanelControl1->Invalidate();
	hkoglPanelControl2->Invalidate();
}

private: System::Void ScaleScrollBar1_ValueChanged(System::Object^ sender, System::EventArgs^ e) 
{
	UpdateScale_from_Panel(ScaleScrollBar1->Value);
	hkoglPanelControl1->Invalidate();
}
private: System::Void Save_button_Click(System::Object^ sender, System::EventArgs^ e) 
{
	Save_File();
	 Save_material();
}

};
}