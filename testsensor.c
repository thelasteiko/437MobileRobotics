#pragma config(Sensor, S4, sonar4, sonar4, sendorEV3_Ultrasonic)
#pragma config(Sensor, S1, llight, sensorEV3_color, modeEV3Color_Color)
#pragma config(Sensor, S2, rlight, sensorEV3_color, modeEV3Color_Color)
#pragma config(Motor, motorB, motorLeft, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Morot, motorC, motorRight, tmotorEV3_Large, PIDControl, encoder)

#define ALPHA 0.3 //Divide by this number
#define LENGTH 3
#define lastReading 0
#define avgerage 1
#define canread
//info: last reading, avg
float sensor_sonic[LENGTH]
float sensor_rlight[LENGTH]
float sensor_llight[LENGTH]

float BOUND 50;


task sonicmonitor () {
  while (true) {
    sensor_sonic[canread] = 0;
    sensor_sonic[lastReading] = SensorValue[sonar4];
    sensor_sonic[canread] = 1;
    sleep(20);
  }
}

/*
Color sensor modes
0 - modeEV3Color_Reflected
1 - modeEV3Color_Ambient
2 - modeEV3Color_Color
3 - modeEV3Color_Reflected_Raw
4 - modeEV3Color_RGB_Raw
*/
task llightmonitor () {
  while (true) {
    sensor_llight[canread] = 0;
    sensor_llight[lastReading] = SensorValue[llight];
    sensor_llight[canread] = 1;
    sleep(20);
  }
}

task rlightmonitor() {
  while (true) {
    sensor_rlight[canread] = 0;
    sensor_rlight[lastReading] = SensorValue[rlight];
    sensor_rlight[canread] = 1;
    sleep(20);
  }
}

int weigtedAvg (int lastReading, int avg) {
  //TODO someway to account for outliers
  return (lastReading + ALPHA(avg-lastReading));
}

int startup() {
  while (getButtonPress(buttonAny) != 1) {
    displayBigTextLine(4, "Right: %d", sensor_rlight[lastReading]);
    displayBigTextLine(5, "Left: %d", sensor_llight[lastReading]);
  }
  float white = (sensor_rlight[lastReading] + sensor_llight[lastReading])/2;
  displayBigTextLine(4, "Accepted: %d", white);
  
  while (getButtonPress(buttonAny) != 1) {
    displayBigTextLine(4, "Right: %d", sensor_rlight[lastReading]);
    displayBigTextLine(5, "Left: %d", sensor_llight[lastReading]);
  }
  float black = (sensor_rlight[lastReading] + sensor_llight[lastReading])/2;
  displayBigTextLine(4, "Accepted: %d", black);
  BOUND = (white + black)/2;
  displayBigTextLine(4, "Set Bound: %d", BOUND);
  
  while (getButtonPress(buttonAny) != 1) {
    displayCenteredBigTextLine(4, "Dist: %3d cm", sensor_sonic[lastReading]);
  }
  displayBigTextLine(4, "Starting...");
}

task printout () {
  //Run through each sensor and display information.
  while (true) {
    if (sensor_sonic[canread])
      displayCenteredBigTextLine(4, "Dist: %3d cm", sensor_sonic[lastReading]);
    if(sensor_llight[canread])
      displayBigTextLine(4, "Reflected: %d", sensor_llight[lastReading]);
    sleep(100);
  }
}