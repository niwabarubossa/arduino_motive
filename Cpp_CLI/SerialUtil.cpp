#include "SerialUtil.h"

#define ENCODING "Shift_JIS" // 文字コードはShift_JISの前提です。
#define RCV_BUFF_LEN (10)    // 受信バッファ長です。1文字ずつ受信するためそんなに大きくしていません。

namespace SampleSerial
{

    /// <summary>
    /// コンストラクタ
    /// </summary>
    SerialUtil::SerialUtil(String^ portName, int baudRate, Parity parity, int dataBits, StopBits stopBits, Handshake handshake, int timeout)
    {
        // シリアル通信の初期化を行います。
        InitSerial(portName, baudRate, parity, dataBits, stopBits, handshake, timeout);

        msgQueue = MsgQueue::GetInstance(100);
        receiveThread = nullptr;
        stopReceiveThread = false;

        // 受信スレッドを開始します。
        StartReceiveThread();
    }

    /// <summray>
    /// インスタンスを生成します。
    /// </summary>
    SerialUtil^ SerialUtil::GetInstance(String^ portName, int baudRate, Parity parity, int dataBits, StopBits stopBits, Handshake handshake, int timeout)
    {
        if (instance == nullptr)
        {
            instance = gcnew SerialUtil(portName, baudRate, parity, dataBits, stopBits, handshake, timeout);
        }
        return instance;
    }

    /// <summary>
    /// データを送信します。
    /// </summray>
    bool SerialUtil::SendData(String^ data, String^ % errMsg)
    {
        bool result = true;
        try
        {
            array<Byte>^ buffer = Encoding::GetEncoding(ENCODING)->GetBytes(data);
            int lenght = buffer->Length;
            serial->Write(buffer, 0, lenght);
        }
        catch (Exception^ e)
        {
            errMsg = e->Message;
            result = false;
        }
        return result;
    }

    /// <summary>
    /// 終了時の処理を行います。
    /// </summary>
    void SerialUtil::Close()
    {
        // 受信スレッドを停止します。
        // Join()する前に受信スレッドがクリアされることがあるため
        // nullチェックを行ってからJoin()します。
        stopReceiveThread = true;
        if (receiveThread != nullptr)
        {
            receiveThread->Join();
        }

        // シリアルをクローズします。
        serial->Close();
    }

    /// <summary>
    /// シリアル通信の初期化処理を行います。
    /// </summary>
    void SerialUtil::InitSerial(String^ portName, int baudRate, Parity parity, int dataBits, StopBits stopBits, Handshake handshake, int timeout)
    {
        serial = gcnew SerialPort(portName, baudRate, parity, dataBits, stopBits);
        serial->Handshake = handshake;
        serial->ReadTimeout = timeout;
        serial->Open();
    }

    /// <summray>
    /// 受信スレッドを開始します。
    /// </summray>
    void SerialUtil::StartReceiveThread()
    {
        ThreadStart^ start = gcnew ThreadStart(this, &SerialUtil::ReceiveThread);
        receiveThread = gcnew Thread(start);
        receiveThread->Start();
    }

    /// <summary>
    /// 受信スレッド処理
    /// </summary>
    void SerialUtil::ReceiveThread()
    {
        // タイムアウト例外が発生してループするため、
        // whileの条件式で受信スレッド停止フラグをチェックします。
        array<Byte>^ buffer = gcnew array<Byte>(RCV_BUFF_LEN);
        while (!stopReceiveThread)
        {
            try
            {
                int bufLen = buffer->Length;
                for (int i = 0; i < bufLen; i++)
                {
                    buffer[i] = 0;
                }

                // シリアルからデータを受信した場合
                int len = serial->Read(buffer, 0, 1);
                if (len > 0)
                {
                    // 受信したデータを文字列に変更し、メッセージキューに設定します。
                    String^ data = Encoding::GetEncoding(ENCODING)->GetString(buffer, 0, 1);
                    msgQueue->SetMsg(data);
                }
            }
            catch (Exception^)
            {
                // 必要に応じてエラー処理を行ってください。
            }
        }
    }
}