void fx_lineFollower() {
  
  while(1) {
    fx_CarDirection(readLED()); //assuming max turning-degree = 35deg (one side)
  }
}

double readLED() {
  //for recording dT for I_term
  static unsigned long previousTime = 0;
  static unsigned long presentTime = 0;
  presentTime = millis();
  static double deltaTime = 0;
  deltaTime = double(presentTime - previousTime)/1000.0;
  if(previousTime==0) {
    deltaTime = 0;
  }
  
  static int LED[7] = {0};
  LED[2] = digitalRead(2);
  LED[3] = digitalRead(3);
  LED[4] = digitalRead(4);
  LED[5] = digitalRead(5);
  LED[6] = digitalRead(6);

  //how many LEDs detected black line?
  static double LED_counts = 0;
  LED_counts = LED[2] + LED[3] + LED[4] + LED[5] + LED[6];

  //ex: If LED[5] and LED[6] both read black line(output = 1, 2, respectively), then LEDxWeight(w/o PID) is 1.5
  static int weight[7] = {0, 0, -2, -1, 0, 1, 2};
  static double LEDxWeight = 0;
  LEDxWeight = LED[2] * weight[2] + LED[3] * weight[3] + LED[4] * weight[4] + LED[5] * weight[5] + LED[6] * weight[6];
  LEDxWeight = PID(LEDxWeight, deltaTime);
  previousTime = presentTime;
  return (LEDxWeight/LED_counts);
}

double PID(double error, double dT) {
  static double P = 17;
  static double I = 0.1;
  static double P_term = 0;
  P_term = P * error;
  static double errorSum = 0;
  errorSum = errorSum + (error*dT);
  static double I_term = 0;
  I_term = I * errorSum;
  return (P_term + I_term);
}
