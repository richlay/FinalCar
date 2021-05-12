long accelX_raw, accelY_raw, accelZ_raw;
float accelX_g, accelY_g, accelZ_g;

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9, register map)  //嘗試改clock會對特定要取的資訊會較準確?e.g.欲取gyro則用gyro的clock
  Wire.endTransmission();
    
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 

  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1A); //Accessing the register 1A - Configuration (Sec. 4.3) 
  Wire.write(0b00000110); //Setting the  Digital Low Pass Filter (DLPF) to Accel bandwidth260 delay0 256 0.98
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX_raw = long(Wire.read()<<8);
  accelX_raw |= Wire.read();         //Store first two bytes into accelX_raw
  accelY_raw = long(Wire.read()<<8);
  accelY_raw |= Wire.read();         //Store first two bytes into accelY_raw
  accelZ_raw = long(Wire.read()<<8);
  accelZ_raw |= Wire.read();        //Store first two bytes into accelZ_raw
  processAccelData();
}

void processAccelData(){
  accelX_g = accelX_raw / 16384.0;
  accelY_g = accelY_raw / 16384.0; 
  accelZ_g = accelZ_raw / 16384.0;
}

long gyroX_raw, gyroY_raw, gyroZ_raw;
float gyroX_deg, gyroY_deg, gyroZ_deg;

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX_raw = Wire.read()<<8;
  gyroX_raw |= Wire.read();         //Store first two bytes into gyroX_raw
  gyroY_raw = Wire.read()<<8;
  gyroY_raw |= Wire.read();         //Store first two bytes into gyroY_raw
  gyroZ_raw = Wire.read()<<8;
  gyroZ_raw |= Wire.read();         //Store first two bytes into gyroZ_raw
  processGyroData();
}

void processGyroData() {
  gyroX_deg = gyroX_raw / 131.0;
  gyroY_deg = gyroY_raw / 131.0; 
  gyroZ_deg = gyroZ_raw / 131.0;
}

void printData() {
//  Serial.print("Gyro (deg)");
//  Serial.print(" X=");
//  Serial.print(gyroX_deg);
//  Serial.print(" Y=");
//  Serial.print(gyroY_deg);
//  Serial.print(" Z=");
//  Serial.print(gyroZ_deg);
//  Serial.print(" Accel (g)");
//  Serial.print(" X=");
//  Serial.print(accelX_g);
//  Serial.print(" Y=");
//  Serial.print(accelY_g);
//  Serial.print(" Z=");
//  Serial.println(accelZ_g);
  

  Serial.print(gyroX_deg);
  Serial.print(" ");
  Serial.print(gyroY_deg);
  Serial.print(" ");
  Serial.print(gyroZ_deg);

  Serial.print(" ");
  Serial.print(2*(accelX_g-0.03));
  Serial.print(" ");
  Serial.print(2*(accelY_g-0.78));
  Serial.print(" ");
  Serial.print(2*(accelZ_g-0.2));
  Serial.print(" ");
//  Serial.println;
}

double acceleration() {
  recordAccelRegisters();
  static double squareSum = 0;
  squareSum = (2*(accelX_g)*2*(accelX_g)) + (2*(accelY_g-0.78)*2*(accelY_g-0.78));
  return sqrt(squareSum)*981;
}

double velocity() {
  static unsigned long presentTime = 1;
  static double presentAcceleration = 1;
  static double presentVelocity = 1;
  presentTime = millis();
  static unsigned long previousTime = presentTime;
  presentAcceleration = acceleration();
  static double previousAcceleration = presentAcceleration;
  static double deltaTime = 0;
  deltaTime = (presentTime-previousTime)/1000.0;
  static double previousVelocity = 0;
  presentVelocity = (presentAcceleration+previousAcceleration)*deltaTime/2.0 + previousVelocity; //上底加下底乘高除2
  
  previousAcceleration = presentAcceleration; 
  previousTime = presentTime;
  
  previousVelocity = presentVelocity;
  return presentVelocity;
}

double distance() {
  static unsigned long presentTime = 1;
  static double presentAcceleration = 1;
  static double presentDistance = 1;
  presentTime = millis();
  static unsigned long previousTime = presentTime;
  presentAcceleration = acceleration();
  static double previousAcceleration = presentAcceleration;
  static unsigned long deltaTime = 0;
  deltaTime = (presentTime-previousTime)/1000.0;
  static double previousDistance = 0;
  presentDistance = ((presentAcceleration+previousAcceleration)/2.0)*(deltaTime*deltaTime)/2.0 + previousDistance; //兩點平均值乘t平方除2
  
  previousAcceleration = presentAcceleration; 
  previousTime = presentTime;
  
  previousDistance = presentDistance;
  return presentDistance;
}

double angularVelocity() {
  recordAccelRegisters();
  return gyroZ_deg;
}

double angularDistance() {
  static unsigned long presentTime = 1;
  static double presentAngularVelocity = 1;
  static double presentAngularDistance = 1;
  presentTime = millis();
  static unsigned long previousTime = presentTime;
  presentAngularVelocity = angularVelocity();
  static double previousAngularVelocity = presentAngularVelocity;
  static double previousAngularDistance = 0;
  presentAngularDistance = (presentAngularVelocity+previousAngularVelocity)*(presentTime-previousTime)/2.0 + previousAngularDistance; //上底加下底乘高除2
  
  previousAngularVelocity = presentAngularVelocity; 
  previousTime = presentTime;
  
  previousAngularDistance = presentAngularDistance;
  return presentAngularVelocity;
}
