#include <Wire.h>
#include <Servo.h> //PWM FREQUENCY=50Hz(Period=20ms)

Servo servo_ESC;
Servo servo_CarDirection;
Servo servo_Yaw;

void setup() {
  //MPU6050 Setup
  Serial.begin(9600);
  Wire.begin();
  setupMPU();

  //ESC Setup(SkyWalker50A + XXD2700KV +4S)
  servo_ESC.attach(9,700,2400);  // min=544, max=2400                             //看要不要改成ESC.attach(9,1000,2000);  然後之後用ESC.write(0~180) 或 int speed,pulse_width;  pulse_width=map(speed,0,100,1000,2000); ESC.writeMicroseconds(pulse_width);
  servo_ESC.writeMicroseconds(700);  // Set the throttle position to 0         //需要視情況更改

  //CarDirection Setup(MG996R)
  servo_CarDirection.attach(10);
  servo_CarDirection.write(90);

  //Yaw Setup(MG996R)
  servo_Yaw.attach(11,1000,2000);         //尚未驗證
  servo_Yaw.write(90);                    //尚未驗證

  //IR(linear style)
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  
}

void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  printData();
  Serial.println(velocity());
  delay(100);
  
}
