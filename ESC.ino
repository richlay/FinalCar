void fx_ESC(int v) {
    if(v>=700 && v<=2400) {
        servo_ESC.writeMicroseconds(v);  // Set the throttle position
      }
}
