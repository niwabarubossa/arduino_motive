#include "MainForm.h"

namespace SampleSerial
{

    /// <summary>
    /// フォームが表示された時に呼び出されます。
    /// </summary>
    System::Void MainForm::MainForm_Load(System::Object ^ sender, System::EventArgs ^ e)
    {
        // シリアルユーティリティオブジェクトを作成します。
        // シリアル通信で必要なパラメータは、ここでは固定値とします。
        serialUtil = SerialUtil::GetInstance("COM2", 9600, Parity::None, 8, StopBits::One, Handshake::None, 1000);
        // メッセージキューオブジェクトを作成します。
        msgQueue = MsgQueue::GetInstance(10);

        // 受信スレッド停止フラグをクリアします。
        stopReceiveThread = false;

        // 受信スレッドを開始します。
        StartReceiveThread();
    }

    /// <summary>
    /// フォームが閉じられる時に呼び出されます。
    /// </summary>
    System::Void MainForm::MainForm_FormClosing(System::Object ^ sender, System::Windows::Forms::FormClosingEventArgs ^ e)
    {
        // 受信スレッドを停止します。
        StopReceiveThread();

        // シリアル通信の終了処理を行います。
        serialUtil->Close();
    }

    /// <summary>
    /// 送信ボタンが押下された時に呼び出されます。
    /// </summary>
    System::Void MainForm::BtnSend_Click(System::Object ^ sender, System::EventArgs ^ e)
    {
        // 送信用テキストボックスに入力された文字列をシリアルで送信します。
        String ^ sendData = TxtBoxSendData->Text;
        if (String::IsNullOrEmpty(sendData))
        {
            return;
        }

        String ^ errMsg;
        if (!serialUtil->SendData(sendData, errMsg))
        {
            MessageBox::Show(errMsg, "エラー", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    /// <summary>
    /// クリアボタンが押下された時に呼び出されます。
    /// </summary>
    System::Void MainForm::BtnClear_Click(System::Object ^ sender, System::EventArgs ^ e)
    {
        // 受信メッセージを表示するテキストボックスをクリアします。
        TxtBoxRcvData->Text = "";
    }

    /// <summary>
    /// 受信スレッドを開始します。
    /// </summary>
    void MainForm::StartReceiveThread()
    {
        ThreadStart ^ start = gcnew ThreadStart(this, &MainForm::ReceiveThread);
        receiveThread = gcnew Thread(start);
        receiveThread->Start();
    }

    /// <summary>
    /// 受信スレッド処理
    /// </summary>
    void MainForm::ReceiveThread()
    {
        // stopReceiveThreadでループを抜けるため、無限ループします。
        while (true)
        {
            // メッセージ受信待ち
            String ^ msg = msgQueue->GetMsg();

            // 受信スレッド停止フラグが設定された場合
            if (stopReceiveThread)
            {
                // ループを抜けます、受信スレッドを終了します。
                break;
            }

            // 受信した文字列をテキストボックスに追加表示します。
            SetMsg(msg);
        }
    }

    /// <summary>
    /// 受信したメッセージをテキストボックスに追加します。
    /// 受信スレッドから呼び出されるため、Invokeメソッドによりデリゲート経由で処理します。
    /// </summary>
    void MainForm::SetMsg(String ^ msg)
    {
        if (this->InvokeRequired)
        {
            SetMsgDelegate ^ method = gcnew SetMsgDelegate(this, &MainForm::SetMsg);
            array<Object ^> ^ args = {msg};
            this->Invoke(method, args);
        }
        else
        {
            TxtBoxRcvData->Text += msg;
        }
    }

    /// <summary>
    /// 受信スレッドを停止します。
    /// </summary>
    void MainForm::StopReceiveThread()
    {
        // 受信スレッド停止フラグを設定します。
        stopReceiveThread = true;

        // 受信スレッドのメッセージ待ちを解除するため、空文字列をメッセージキューに設定します。
        msgQueue->SetMsg("");

        // 受信スレッドが終了するのを待ちます。
        receiveThread->Join();
    }
}