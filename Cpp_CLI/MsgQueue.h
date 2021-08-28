#pragma once

namespace SampleSerial
{
    using namespace System;
    using namespace System::Collections;
    using namespace System::Threading;

    ref class MsgQueue
    {
    private:
        /// 自身のインスタンス
        static MsgQueue^ instance;

        /// キューオブジェクト
        Queue^ queue;

        /// イベントオブジェクト
        ManualResetEvent^ event;

    private:
        MsgQueue(int capacity);

    public:
        static MsgQueue^ GetInstance(int capacity);

        void SetMsg(String^ msg);
        String^ GetMsg();
    };
}