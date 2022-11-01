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
std::string filename = "./resource/3DModel/Simpson.obj";	// load model

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
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
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
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(926, 24);
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
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 24);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(509, 454);
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
			this->groupBox1->Location = System::Drawing::Point(515, 45);
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
			this->trackBar1->Location = System::Drawing::Point(515, 307);
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
			this->OneRing_Face_checkBox1->Location = System::Drawing::Point(515, 263);
			this->OneRing_Face_checkBox1->Name = L"OneRing_Face_checkBox1";
			this->OneRing_Face_checkBox1->Size = System::Drawing::Size(100, 16);
			this->OneRing_Face_checkBox1->TabIndex = 4;
			this->OneRing_Face_checkBox1->Text = L"One Ring - Face";
			this->OneRing_Face_checkBox1->UseVisualStyleBackColor = true;
			this->OneRing_Face_checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::OneRing_Face_checkBox1_CheckedChanged);
			// 
			// OneRing_Vertex_checkBox2
			// 
			this->OneRing_Vertex_checkBox2->AutoSize = true;
			this->OneRing_Vertex_checkBox2->Location = System::Drawing::Point(515, 285);
			this->OneRing_Vertex_checkBox2->Name = L"OneRing_Vertex_checkBox2";
			this->OneRing_Vertex_checkBox2->Size = System::Drawing::Size(110, 16);
			this->OneRing_Vertex_checkBox2->TabIndex = 5;
			this->OneRing_Vertex_checkBox2->Text = L"One Ring - Vertex";
			this->OneRing_Vertex_checkBox2->UseVisualStyleBackColor = true;
			this->OneRing_Vertex_checkBox2->CheckedChanged += gcnew System::EventHandler(this, &MyForm::OneRing_Vertex_checkBox2_CheckedChanged);
			// 
			// NewM_Button
			// 
			this->NewM_Button->Location = System::Drawing::Point(521, 201);
			this->NewM_Button->Name = L"NewM_Button";
			this->NewM_Button->Size = System::Drawing::Size(75, 23);
			this->NewM_Button->TabIndex = 6;
			this->NewM_Button->Text = L"New Mesh";
			this->NewM_Button->UseVisualStyleBackColor = true;
			this->NewM_Button->Click += gcnew System::EventHandler(this, &MyForm::NewM_Button_Click);
			// 
			// hkoglPanelControl2
			// 
			hkcoglPanelCameraSetting2->Far = 1000;
			hkcoglPanelCameraSetting2->Fov = 45;
			hkcoglPanelCameraSetting2->Near = -1000;
			hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl2->Camera_Setting = hkcoglPanelCameraSetting2;
			this->hkoglPanelControl2->Location = System::Drawing::Point(676, 110);
			this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
			hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl2->Pixel_Format = hkcoglPanelPixelFormat2;
			this->hkoglPanelControl2->Size = System::Drawing::Size(220, 220);
			this->hkoglPanelControl2->TabIndex = 7;
			this->hkoglPanelControl2->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl2_Load);
			this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl2_Paint);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->TextureMode);
			this->groupBox2->Controls->Add(this->PickMode);
			this->groupBox2->Location = System::Drawing::Point(515, 367);
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
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(926, 479);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->hkoglPanelControl2);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->NewM_Button);
			this->Controls->Add(this->OneRing_Vertex_checkBox2);
			this->Controls->Add(this->OneRing_Face_checkBox1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->menuStrip1);
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

	// TODO mesh -> MeshObject.moede
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
	model.Parameterization();
	cout << "------Parameterization finished------" << endl;
	hkoglPanelControl2->Invalidate();
}

#pragma region Pick / Texcture Mode
private: System::Void PickMode_Click(System::Object^ sender, System::EventArgs^ e) 
{
	groupBox1->Visible = true;
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
	pickMode = PickMode::None;

	drawTexture = true;
	hkoglPanelControl1->Invalidate();
}
#pragma endregion

};
}