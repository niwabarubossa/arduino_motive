#include "MsgQueue.h"

namespace SampleSerial
{

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="capcity">キューイング数</param>
    MsgQueue::MsgQueue(int capacity)
    {
        queue = gcnew Queue(capacity);
        queue->Clear();
        event = gcnew ManualResetEvent(false);
    }

    /// <summary>
    /// インスタンスを返却します。
    /// </summary>
    /// <param name="capacity">キューイング数</param>
    MsgQueue^ MsgQueue::GetInstance(int capacity)
    {
        if (instance == nullptr)
        {
            instance = gcnew MsgQueue(capacity);
        }
        return instance;
    }

    /// <summary>
    /// メッセージをセットします。
    /// <summary>
    /// <param name="msg">メッセージ</param>
    void MsgQueue::SetMsg(String^ msg)
    {
        // キューオブジェクトをロックします。
        Monitor::Enter(queue);

        // メッセージをキューイングします。
        queue->Enqueue(msg);

        // イベントを発行します。
        event->Set();

        // キューオブジェクトのロックを解除します。
        Monitor::Exit(queue);
    }

    /// <summary>
    /// メッセージを取得します。
    /// </summary>
    /// <returns>
    /// メッセージ
    /// </returns>
    String^ MsgQueue::GetMsg()
    {
        String^ msg;

        // イベント待ち
        // SetMsgメソッドが呼び出されると、イベント待ちが解除されます。
        event->WaitOne();

        // キューオブジェクトをロックします。
        Monitor::Enter(queue);

        // キューイングされている場合
        if (queue->Count > 0)
        {
            // メッセージを取得します。
            msg = (String^) queue->Dequeue();

            // キューイングされたメッセージがなくなった場合
            if (queue->Count == 0)
            {
                // 非シグナル状態にします。
                event->Reset();
            }
        }

        // キューイングされたメッセージがない場合
        else
        {
            // 非シグナル状態にします。
            event->Reset();
        }

        // キューオブジェクトのロックを解除します。
        Monitor::Exit(queue);

        return msg;
    }
}