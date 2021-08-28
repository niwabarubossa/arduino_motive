#pragma once

#include "MsgQueue.h"

namespace SampleSerial
{
    using namespace System;
    using namespace System::Threading;
    using namespace System::IO::Ports;
    using namespace System::Text;

    ref class SerialUtil
    {
    private:
        /// 自クラスのインスタンス
        static SerialUtil^ instance;

        /// シリアル処理を行うオブジェクト
        SerialPort^ serial;
        /// メッセージキュー
        MsgQueue^ msgQueue;
        /// 受信スレッド
        Thread^ receiveThread;
        /// 受信スレッド停止フラグ
        bool stopReceiveThread;

    private:
        SerialUtil(String^ portName, int baudRate, Parity parity, int dataBits, StopBits stopBits, Handshake handshake, int timeout);

    public:
        static SerialUtil^ GetInstance(String^ portName, int baudRate, Parity parity, int dataBits, StopBits stopBits, Handshake handshake, int timeout);

        bool SendData(String^ data, String^ % errMsg);
        void Close();

    private:
        void InitSerial(String^ portName, int baudRate, Parity parity, int dataBits, StopBits stopBits, Handshake handshake, int timeout);
        void StartReceiveThread();
        void ReceiveThread();
    };
}