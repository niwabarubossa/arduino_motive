#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>  
#include <sys/ioctl.h> 
#include <fcntl.h>     
#include <termios.h>   
#include <unistd.h>    
#include <thread>
#include <iostream> //timer
#include <chrono> //timer

#define SERIAL_PORT "/dev/cu.usbmodem14401"

int main(int argc, char *argv[])
{
  unsigned char msg[] = "serial port open...\n";
  char buf[255]; // バッファ
  int fd;             // ファイルディスクリプタ
  struct termios tio; // シリアル通信設定
  int baudRate = B38400;
  int i,len,ret,size;

  // fd = open(SERIAL_PORT, O_RDWR | O_NDELAY | O_NOCTTY); // デバイスをオープンする
  fd = open(SERIAL_PORT, O_RDWR);
  if (fd < 0)
  {
    printf("open error\n");
    return -1;
  }

  tio.c_cflag += CREAD;  // 受信有効
  tio.c_cflag += CLOCAL; // ローカルライン（モデム制御なし）
  tio.c_cflag += CS8;    // データビット:8bit
  tio.c_cflag += 0;      // ストップビット:1bit
  tio.c_cflag += 0;      // パリティ:None

  cfsetispeed(&tio, baudRate);
  cfsetospeed(&tio, baudRate);

  cfmakeraw(&tio); // RAWモード

  tcsetattr(fd, TCSANOW, &tio); // デバイスに設定を行う

  ioctl(fd, TCSANOW, &tio); // ポートの設定を有効にする

  // 送受信処理ループ
  while (1)
  {
    len = read(fd, buf, sizeof(buf));
    if (0 < len)
    {
      for (i = 0; i < len; i++)
      {
        // printf("%02X", buf[i]);
        printf("%c", buf[i]);
      }
      printf("no data\n");
    }
    // エコーバック
    // write(fd, buf, len);
  }

  close(fd); // デバイスのクローズ
  return 0;
}