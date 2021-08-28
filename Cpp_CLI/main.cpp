#include "MainForm.h"

using namespace SampleSerial;
[STAThreadAttribute] int main()
{
    // メインフォームを作成し、表示します。
    Form ^ form = gcnew MainForm();
    form->ShowDialog();
    return 0;
}