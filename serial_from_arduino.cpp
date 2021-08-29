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
#include <string.h> //strcpy
#include <cmath>

#define SERIAL_PORT "/dev/cu.usbmodem14201"
// #define SERIAL_PORT "/dev/cu.usbmodem14401"

int main(int argc, char *argv[])
{
  unsigned char msg[] = "serial port open...\n";
  char buf[255]; // バッファ
  int fd;             // ファイルディスクリプタ
  struct termios tio; // シリアル通信設定
  int baudRate = B38400; //ボーレート（bpsとは厳密には異なるが，シリアル通信では1回の復調で1bit）
  int i,len,ret,size;

  clock_t start = 0, end = 0;

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
  char line_buf[256] = "";
  while (1)
  {
    len = read(fd, buf, sizeof(buf));
    if (0 < len)
    {
      // printf("start\n");
      start = clock();
      for (i = 0; i < len; i++)
      {
        char c(buf[i]);
        if( c == '\n' ){
          printf("%s\n",line_buf);
          int num = atoi(line_buf);



          strcpy(line_buf, "");    // 行バッファクリア
        }else{            // 行バッファに1文字追加
          int len = strlen(line_buf);
          line_buf[len] = c;
          line_buf[len+1] = '\0';
        }
      }
      end = clock();
      // printf ("%0.8f sec\n",((float) end - start)/CLOCKS_PER_SEC);
    }
    end = clock();
    // write(fd, buf, len);　// エコーバック
  }

  close(fd); // デバイスのクローズ
  return 0;
}

//モーキャプのクウォータニオンをオイラー角に変換する
double QuaternionToRoll(double q0,double q1,double q2,double q3){
  double q0q0 = q0 * q0;
  double q0q1 = q0 * q1;
  double q0q2 = q0 * q2;
  double q0q3 = q0 * q3;
  double q1q1 = q1 * q1;
  double q1q2 = q1 * q2;
  double q1q3 = q1 * q3;
  double q2q2 = q2 * q2;
  double q2q3 = q2 * q3;
  double q3q3 = q3 * q3;
  double roll = atan2(2.0 * (q2q3 + q0q1), q0*q0 - q1q1 - q2q2 + q3q3);
  double pitch = asin(2.0 * (q0q2 - q1q3));
  double yaw = atan2(2.0 * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3);
  return roll;
}