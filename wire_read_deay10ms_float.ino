#include <Wire.h>

#include <MadgwickAHRS.h>　//マジウィックフィルタ
Madgwick MadgwickFilter;

// MPU-6050のアドレス、レジスタ設定値
#define MPU6050_WHO_AM_I     0x75  // Read Only
#define MPU6050_PWR_MGMT_1   0x6B  // Read and Write
#define MPU_ADDRESS  0x68

unsigned long time = 0;

// デバイス初期化時に実行される
void setup() {
  Wire.begin();
  // PCとの通信を開始
  Serial.begin(38400); //C++側の都合上，ボーレートの上限がこれであるため．
  // 初回の読み出し
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MPU6050_WHO_AM_I);  //MPU6050_PWR_MGMT_1
  Wire.write(0x00);
  Wire.endTransmission();
  // 動作モードの読み出し
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MPU6050_PWR_MGMT_1);  //MPU6050_PWR_MGMT_1レジスタの設定
  Wire.write(0x00);
  Wire.endTransmission(); 
  
  MadgwickFilter.begin(500); //500Hz
}


void loop() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);
  while (Wire.available() < 14);
  float axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw, Temperature;
  time = micros();
  axRaw = Wire.read() << 8 | Wire.read();
  ayRaw = Wire.read() << 8 | Wire.read();
  azRaw = Wire.read() << 8 | Wire.read();
  Temperature = Wire.read() << 8 | Wire.read();
  gxRaw = Wire.read() << 8 | Wire.read();
  gyRaw = Wire.read() << 8 | Wire.read();
  gzRaw = Wire.read() << 8 | Wire.read();

  MadgwickFilter.updateIMU(gxRaw,gyRaw,gzRaw,axRaw,ayRaw,azRaw);
  
  // 加速度値を分解能で割って加速度(G)に変換する
  int16_t acc_x = axRaw / 16384.0;  //FS_SEL_0 16,384 LSB / g
  int16_t acc_y = ayRaw / 16384.0;
  int16_t acc_z = azRaw / 16384.0;

  // 角速度値を分解能で割って角速度(degrees per sec)に変換する
//  float gyro_x = gxRaw / 131.0;//FS_SEL_0 131 LSB / (°/s)
//  float gyro_y = gyRaw / 131.0;
//  float gyro_z = gzRaw / 131.0;

  int16_t pitch = MadgwickFilter.getPitch();
  int16_t roll = MadgwickFilter.getRoll();
  int16_t yaw = MadgwickFilter.getYaw();
  if ( acc_x != 0 || acc_y != 0 || acc_z != 0 ){
    Serial.print(acc_x);  Serial.print(",");
    Serial.print(acc_y);  Serial.print(",");
    Serial.print(acc_z);  Serial.print(",");
    Serial.print(pitch); Serial.print(",");
    Serial.print(roll); Serial.print(",");
    Serial.print(yaw); Serial.println("");
    Serial.println(micros() - time);
  }
  delay(10);
}