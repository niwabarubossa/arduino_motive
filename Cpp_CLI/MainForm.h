#pragma once

#include "SerialUtil.h"
#include "MsgQueue.h"

namespace SampleSerial
{
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// MainForm の概要
    /// </summary>
public ref class MainForm : public System::Windows::Forms::Form
    {
    public:
        MainForm(void)
        {
            InitializeComponent();
            //
            //TODO: ここにコンストラクター コードを追加します
            //
        }

    protected:
        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::TextBox^ TxtBoxSendData;

    protected:
    private:
        System::Windows::Forms::Button^ BtnSend;

    private:
        System::Windows::Forms::TextBox^ TxtBoxRcvData;

    protected:
    private:
        System::Windows::Forms::Button^ BtnClear;

    private:
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        void InitializeComponent(void)
        {
            this->TxtBoxSendData = (gcnew System::Windows::Forms::TextBox());
            this->BtnSend = (gcnew System::Windows::Forms::Button());
            this->TxtBoxRcvData = (gcnew System::Windows::Forms::TextBox());
            this->BtnClear = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            //
            // TxtBoxSendData
            //
            this->TxtBoxSendData->Location = System::Drawing::Point(8, 8);
            this->TxtBoxSendData->Name = L"TxtBoxSendData";
            this->TxtBoxSendData->Size = System::Drawing::Size(272, 19);
            this->TxtBoxSendData->TabIndex = 0;
            //
            // BtnSend
            //
            this->BtnSend->Location = System::Drawing::Point(288, 8);
            this->BtnSend->Name = L"BtnSend";
            this->BtnSend->Size = System::Drawing::Size(75, 23);
            this->BtnSend->TabIndex = 1;
            this->BtnSend->Text = L"送信";
            this->BtnSend->UseVisualStyleBackColor = true;
            this->BtnSend->Click += gcnew System::EventHandler(this, &MainForm::BtnSend_Click);
            //
            // TxtBoxRcvData
            //
            this->TxtBoxRcvData->Location = System::Drawing::Point(8, 40);
            this->TxtBoxRcvData->Multiline = true;
            this->TxtBoxRcvData->Name = L"TxtBoxRcvData";
            this->TxtBoxRcvData->Size = System::Drawing::Size(352, 240);
            this->TxtBoxRcvData->TabIndex = 2;
            //
            // BtnClear
            //
            this->BtnClear->Location = System::Drawing::Point(288, 288);
            this->BtnClear->Name = L"BtnClear";
            this->BtnClear->Size = System::Drawing::Size(75, 23);
            this->BtnClear->TabIndex = 3;
            this->BtnClear->Text = L"クリア";
            this->BtnClear->UseVisualStyleBackColor = true;
            this->BtnClear->Click += gcnew System::EventHandler(this, &MainForm::BtnClear_Click);
            //
            // MainForm
            //
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(369, 314);
            this->Controls->Add(this->BtnClear);
            this->Controls->Add(this->TxtBoxRcvData);
            this->Controls->Add(this->BtnSend);
            this->Controls->Add(this->TxtBoxSendData);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->Name = L"MainForm";
            this->Text = L"シリアル通信サンプル";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
            this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();
        }
#pragma endregion
    private:
        delegate void SetMsgDelegate(String^ msg);

        SerialUtil^ serialUtil;
        MsgQueue^ msgQueue;
        Thread^ receiveThread;
        bool stopReceiveThread;

    private:
        System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e);
        System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
        System::Void BtnSend_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void BtnClear_Click(System::Object^ sender, System::EventArgs^ e);

    private:
        void StartReceiveThread();
        void ReceiveThread();
        void SetMsg(String^ msg);
        void StopReceiveThread();
    };
}