#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(9600);
  MiniR4.PWR.setBattCell(2);   //設定電池顆數，2顆鋰電池
  MiniR4.I2C1.MXLaser.begin(); //初始化雷射測距模組,使用I2C第1組埠
  MiniR4.LED.begin(7);  //初始化LED
  MiniR4.M1.setSpeed(0); //設定M1(Left)馬達速度
  MiniR4.M2.setSpeed(0); //設定M2(Right)馬達速度
  MiniR4.I2C2.MXColor.begin(); //初始化顏色感測模組,使用I2C第2組埠
  
  
}

void loop() 
{
  // int colorID = MiniR4.I2C2.MXColor.getColorID(); //取得顏色ID
  // int a=MiniR4.I2C2.MXColor.getColor(ColorType::R); //取得紅色數值
  // MiniR4.I2C2.MXColor.setLight(true, true, 64); //開啟感測器燈光，使用PWM模式，亮度128
  // Serial.println("Color ID: ");
  // Serial.println(colorID);
  // delay(1000);
  // Serial.print("R: ");
  // Serial.println(a);
  // delay(1000);
  MiniR4.PS2.read_gamepad(false, 0); //讀取PS2手把狀態
  
  int pwr_raw = MiniR4.PS2.Analog(PSS_LY); //讀取左搖桿Y軸原始值
  int pwr = (pwr_raw > 120 && pwr_raw < 135) ? 0 : map(pwr_raw, 0, 255, 100, -100); //讀取左搖桿Y軸，死區120-135
  // int pwr=map(pwr_raw,0,255,100,-100); //讀取左搖桿Y軸，無死區
  int str_raw = MiniR4.PS2.Analog(PSS_RX); //讀取右搖桿X軸原始值
  // int str = (str_raw > 120 && str_raw < 135) ? 0 : map(str_raw, 0, 255, -100, 100); //讀取右搖桿X軸，死區120-135
  int str=map(str_raw,0,255,-100,100); //讀取右搖桿X軸，無死區
  //===終端機顯示搖桿的撥動值===
  Serial.print("\rPWR: "); // \r代表回到行首
  Serial.print(pwr);
  Serial.print("  STR: ");
  Serial.print(str);
  // ===========================
  MiniR4.M1.setSpeed(pwr + str); //設定M1馬達速度
  MiniR4.M2.setSpeed(-pwr + str); //設定M2馬達速度
  // delay(100);

  // ======
  int r_square=MiniR4.PS2.Button(PSB_SQUARE); //讀取R1按鈕狀態
  int r_circle=MiniR4.PS2.Button(PSB_CIRCLE); //讀取R2按鈕狀態
  int r_blue=MiniR4.PS2.Button(PSB_BLUE); //讀取L1按鈕狀態
  int r_green=MiniR4.PS2.Button(PSB_GREEN); //讀取L2按鈕狀態
  int r1_pressed=MiniR4.PS2.Button(PSB_R1); //讀取R1按鈕
  int r2_pressed=MiniR4.PS2.Button(PSB_R2); //讀取R2按鈕
  Serial.print("  R_square:");
  Serial.print(r_square);
  Serial.print(" R_circle:");
  Serial.print(r_circle);
  Serial.print(" R_blue:");
  Serial.print(r_blue);
  Serial.print(" R_green:");  
  Serial.print(r_green);
  Serial.print(" R1_pressed:");
  Serial.print(r1_pressed);   
  Serial.print(" R2_pressed:");
  Serial.print(r2_pressed);
  Serial.print("    "); //多印幾個空白，避免殘影
  // ====





}