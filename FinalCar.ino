#include <Wire.h>
#include <Servo.h> //PWM FREQUENCY=50Hz(Period=20ms)

Servo servo_ESC;
Servo servo_CarDirection;
/*
Servo servo_Yaw;
*/

// servo_CarDirection calibration
int CarDir_cali = -5;

void setup() {
  //MPU6050 Setup
//  Serial.begin(9600);
//  Wire.begin();
//  setupMPU();

  //ESC Setup(SkyWalker50A + XXD2700KV + 4S Li-Po battery)
  servo_ESC.attach(9,700,2400);  // default <Servo.h> settings are: min=544, max=2400
  servo_ESC.writeMicroseconds(700);  // Set the throttle position to 0

  //CarDirection Setup(MG996R)
  servo_CarDirection.attach(10);
  servo_CarDirection.write(90+CarDir_cali);

/*
  //Yaw Setup(MG996R)
  servo_Yaw.attach(11,1000,2000);
  servo_Yaw.write(90);
*/

  //IR(linear style)
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  
}

void loop() {
  
//  recordAccelRegisters();
//  recordGyroRegisters();
//  printData();
//  Serial.println(velocity());
//  delay(100);

  delay(5000);
  servo_ESC.writeMicroseconds(800);
  fx_lineFollower(2, 1, 1);
}
