int lineCount = 0;
int LED[7] = {0};
bool on_line = false;
double LED_counts = 0;
double LEDxWeight = 0;

void fx_lineFollower(int linesToCross, int followerSettings, int lineCountSettings) { //front=1, back=2
  lineCount = 0;
  while (lineCount <= linesToCross) {
    fx_CarDirection( readLED(followerSettings, lineCountSettings) ); //assuming max turning-degree = 35deg (one side)
  }
}

double readLED(int fs, int lcs) {
  //for recording dT for I_term
  static unsigned long previousTime = 0;
  static unsigned long presentTime = 0;
  presentTime = millis();
  static double deltaTime = 0;
  deltaTime = double(presentTime - previousTime) / 1000.0;
  if (previousTime == 0) {
    deltaTime = 0;
  }

  //using front or back IR as follower? front=1, back=2
  switch (fs) {
    case 1:
      readFrontIR();
      break;
    case 2:
      readBackIR();
      break;
  }

  //how many LEDs detected black line?
  LED_counts = LED[2] + LED[3] + LED[4] + LED[5] + LED[6];

  //ex: If LED[5] and LED[6] both read black line(output = 1, 2, respectively), then LEDxWeight(w/o PID) is 1.5
  static int weight[7] = {0, 0, -2, -1, 0, 1, 2};
  LEDxWeight = LED[2] * weight[2] + LED[3] * weight[3] + LED[4] * weight[4] + LED[5] * weight[5] + LED[6] * weight[6];
  LEDxWeight = LEDxWeight / LED_counts;
  LEDxWeight = PID(LEDxWeight, deltaTime, fs);
  previousTime = presentTime;

  //lines count
  switch (lcs) {
    case 1:
      readFrontIR();
      break;
    case 2:
      readBackIR();
      break;
  }
  if (on_line == false) {
    if (LED_counts >= 3) {
      lineCount++;
      on_line = true;
    }
  }
  else if (on_line == true) {
    if (LED_counts < 3) {
      on_line = false;
    }
  }
  
  return (LEDxWeight);
}

double PID(double error, double dT, int setting) {
  static double P = 0;
  static double I = 0;
  if (setting == 1) {
    P = 17;
    I = 0.1;
  }
  else if (setting == 2) {
    P = 9;
    I = 0.1;
  }
  static double P_term = 0;
  P_term = P * error;
  static double errorSum = 0;
  errorSum = errorSum + (error * dT);
  static double I_term = 0;
  I_term = I * errorSum;
  return (P_term + I_term);
}

void readFrontIR() {
  LED[2] = digitalRead(2);
  LED[3] = digitalRead(3);
  LED[4] = digitalRead(4);
  LED[5] = digitalRead(5);
  LED[6] = digitalRead(6);
}

void readBackIR() {
  LED[2] = analogRead(A2);
  if (LED[2] > 300) {
    LED[2] = 1;
  }
  else {
    LED[2] = 0;
  }
  
  LED[3] = analogRead(A3);
  if (LED[3] > 300) {
    LED[3] = 1;
  }
  else {
    LED[3] = 0;
  }
  
  LED[4] = analogRead(A4);
  if (LED[4] > 300) {
    LED[4] = 1;
  }
  else {
    LED[4] = 0;
  }
  
  LED[5] = analogRead(A5);
  if (LED[5] > 300) {
    LED[5] = 1;
  }
  else {
    LED[5] = 0;
  }
  
  LED[6] = digitalRead(13);
}
