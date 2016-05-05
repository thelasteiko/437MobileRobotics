#pragma config(Sensor, S4, sonar4, sonar4, sendorEV3_Ultrasonic)
#pragma config(Sensor, S1, llight, sensorEV3_color, modeEV3Color_Color)
#pragma config(Sensor, S2, rlight, sensorEV3_color, modeEV3Color_Color)
/***********************************
*    ROBOTICS ASSIGNMENT 2         *
*    Group #5                      *
*    Hannah Silva (the beautiful)  *
*    Melinda Robertson (the grand) *
***********************************/
#define SONIC_IN_RANGE 91
#define SONIC_NEAR 5
/*Motor and touch sensor indexes.*/
#define leftMotor 1 //LEFT_MOTOR
#define rightMotor 2

#define MAXSPEED 70  //speed of opposite motor to turn
#define MINSPEED 65   //speed of motor in direction of turn
#define REGSPEED 50   //the normal speed of both motors
#define SHARPSPEED 100  //for turning a sharp corner

#define MINTURN 40 //the minimum number of cycles that the robot should be turning
#define MAXTURN 45 //the maximum number of cycles that the robot should be turning

#define iLENGTH 7   //the length of the array
#define itime 0     //the number of cycles until the next turn
#define iturn 1     //the next turn to take; right:0, left:1
#define iturntime 2 //the number of cycles to be turning
#define inumturns 3 //keeps track of how many left and right turns there are
#define istate  4   //the last state of the sensors
#define istatetime 5 //amount of time since last state
#define withinInch 6 //whether or not the robot is within an inch of an object
//irobot[istate] == 0
int irobot[iLENGTH]; //holds information about the robot state
//sensor-near: 0; w-w:1; b-b:2; w-b:3; b-w:4
enum ROBOT_STATE {
  sonic, ww, bb, wb, bw
};

//number between 0 and 1 for the "Exponentially Weighted Moving Average.
//0 means current is 100% 1 means past results are 100%
#define ALPHA 0.7
#define ALPHA_SONAR 0.6
#define LENGTH 4
#define lastReading 0
#define average 1
#define canread 2
//info: last reading, avg
float sensor_sonic[LENGTH];
float sensor_rlight[LENGTH];
float sensor_llight[LENGTH];

float BOUND = 10;

int my_rand(int min, int max) {
	/*Returns a random number between the min and max.*/
	int num = (rand()%(max-min))+min;
	return abs(num);
}

float weightedAvg(float lr, float avg) {
	return (lr + ALPHA * (avg-lr));
}

float weightedAvg_Sonar(float lr, float avg) {
	return (lr + ALPHA_SONAR * (avg-lr));
}

void setMotor(int powerL, int powerR) {
	setMotorSpeed(leftMotor, powerL);
	setMotorSpeed(rightMotor, powerR);
}

void startup() {
	while (getButtonPress(buttonAny) != 1) {
		displayBigTextLine(4, "Right: %d", sensor_rlight[lastReading]);
		displayBigTextLine(8, "Left: %d", sensor_llight[lastReading]);
	}
	eraseDisplay();
	float white = (sensor_rlight[lastReading] + sensor_llight[lastReading])/2;
	displayBigTextLine(4, "Accepted: %d", white);
	sleep(500);
	while (getButtonPress(buttonAny) != 1) {
		displayBigTextLine(4, "Right: %d", sensor_rlight[lastReading]);
		displayBigTextLine(8, "Left: %d", sensor_llight[lastReading]);
	}
	eraseDisplay();
	float black = (sensor_rlight[lastReading] + sensor_llight[lastReading])/2;
	sleep(500);
	displayBigTextLine(4, "Accepted: %d", black);
	BOUND = (white + black)/2 - 17;
	sleep(500);
	displayBigTextLine(4, "Set Bound: %d", BOUND);
	sleep(500);
	while (getButtonPress(buttonAny) != 1) {
		displayCenteredBigTextLine(4, "Dist: %3d", sensor_sonic[lastReading]);
	}
	NEAR = sensor_sonic[lastReading];
	displayBigTextLine(4, "Accepted: %d", NEAR);
	sleep(500);
	eraseDisplay();
	int i = 100;
	int sums = 0, suml = 0, sumr = 0;
	while (i) {
		sums = sums + sensor_sonic[lastReading] * pow((1-ALPHA_SONAR),i);
		suml = suml + sensor_llight[lastReading] * pow((1-ALPHA),i);
		sumr = sumr + sensor_rlight[lastReading] * pow((1-ALPHA),i);
		i = i - 1;
		sleep(20);
	}
	sensor_sonic[average] = (sums + sensor_sonic[lastReading]) * ALPHA_SONAR;
	sensor_llight[average] = (suml + sensor_llight[lastReading]) * ALPHA;
	sensor_rlight[average] = (sumr + sensor_rlight[lastReading]) * ALPHA;
	displayBigTextLine(4, "Starting...%d,", sensor_sonic[average]);
	displayBigTextLine(8, "%d, %d", sensor_llight[average], sensor_rlight[average]);
	sleep(2000);
	eraseDisplay();
}

int turn_right_wander() {
	/*Sets the motors to turn right.*/
	int speed = my_rand(MINSPEED, MAXSPEED);
	setMotorSpeed(leftMotor, speed);
	setMotorSpeed(rightMotor, speed-my_rand(10, 13));
	//reset time until stop turning
	irobot[iturntime] = my_rand(MINTURN, MAXTURN);
	irobot[inumturns] = irobot[inumturns] + 1;
	return 0;
}

int turn_right() {
  setMotor(REGSPEED,0)
}

int turn_left_wander() {
	/*Sets the motors to turn left.*/
	int speed = my_rand(MINSPEED, MAXSPEED);
	setMotorSpeed(rightMotor, speed);
	setMotorSpeed(leftMotor, speed-my_rand(10, 13));
	//reset time until stop turning
	irobot[iturntime] = my_rand(MINTURN, MAXTURN);
	irobot[inumturns] = irobot[inumturns] - 1;
	return 0;
}

int turn_left() {
  setMotor(0, REGSPEED);
}
int straight() {
  setMotor(REGSPEED, REGSPEED);
}
void wander(){

	//check if a touch sensor is pressed
	//prev_turn = check_touch(run, prev_turn);
	if (irobot[iturntime] > 0) { //currently turning
		irobot[iturntime] = irobot[iturntime]-1;
		} else if (irobot[iturntime] == 0){ //stop turning
		irobot[iturntime] = -1;
	}

	if (irobot[iturntime] < 0) { //should not be turning
		if (irobot[itime]) { //wait until zero to turn
			irobot[itime] = irobot[itime]-1;
			} else { //time to turn
			if (irobot[iturn]) {
				turn_left_wander();
				} else {
				turn_right_wander();
			}
		}
	}

	//See if we have been turning in one direction for too long.
	if (irobot[inumturns] >= 1) { //turn back left
		irobot[iturn] = 1;
		} else if (irobot[inumturns] <= -1) { //turn back right
		irobot[iturn] = 0;
		} else { //go randomly
		irobot[iturn] = my_rand(0, 2);
	}

}
task infrasense(){
	while (true) {
		sensor_sonic[canread] = 0;
		sensor_sonic[lastReading] = SensorValue[sonar4];
		sensor_sonic[average] = weightedAvg_Sonar(sensor_sonic[lastReading], sensor_sonic[average]);
		if (sensor_sonic[average] <= 91) {
			irobot[istate] = 0;
			setLEDColor(ledGreen);
		} else
			setLEDColor(ledRed);
		if (sensor_sonic[average] <= 5)
			irobot[withinInch] = 1;
		else
			irobot[withinInch] = 0;
		sensor_sonic[canread] = 1;
		//displayBigTextLine(4, "Sonic: %d", sensor_sonic[average]);
		sleep(20);
	}
}

task right_lightsense(){
	while (true) {
		sensor_rlight[canread] = 0;
		sensor_rlight[lastReading] = SensorValue[rlight];
		sensor_rlight[average] = weightedAvg(sensor_rlight[lastReading], sensor_rlight[average]);
		sensor_rlight[canread] = 1;
		sleep(20);
	}
}

task left_lightsense(){
	while (true) {
		sensor_llight[canread] = 0;
		sensor_llight[lastReading] = SensorValue[llight];
		sensor_llight[average] = weightedAvg(sensor_llight[lastReading], sensor_llight[average]);
		sensor_llight[canread] = 1;
		sleep(20);
	}
}

void setMotor(int powerL, int powerR) {
	setMotorSpeed(leftMotor, powerL);
	setMotorSpeed(rightMotor, powerR);
}

enum ROBOT_STATE get_state() {
 if(sensor_sonic[average] <= SONIC_IN_RANGE) {
   return sonic;
  if (sensor_sonic[average] <= SONIC_NEAR) {
    irobot[withinInch] = 1;
  } else {
    irobot[withinInch] = 0;
  }
 } else if (sensor_llight[average] <= BOUND &&
    sensor_rlight[average] > BOUND) { //left sees black
      return bw;
 } else if (sensor_llight[average] > BOUND &&
    sensor_rlight[average] <= BOUND) { //right sees black
      return wb;
 } else if (sensor_llight[average] <= BOUND &&
    sensor_rlight[average] <= BOUND) { //both black
      return bb;
 } else { //both white
   return ww;
 }
}

int findLine(int state) {
  int i = 0;
  if(state == ww) {
    setMotor(-REGSPEED,REGSPEED);
    i = nSysTime;
    while(nSysTime - i < 500) {
      if(get_state() == bw || get_state() == bb)
        break;
		}
    if (get_state() == ww) {
      setMotor(REGSPEED,-REGSPEED);
			i = nSysTime;
			while(nSysTime - i < 900) {
				if (get_state() == wb || get_state() == bb)
					break;
			}
    }
	}
  else if (state == bb) {
    setMotor(REGSPEED,0);
    i = nSysTime;
    while(nSysTime - i < 1000) {
      if(get_state() == bw || get_state() == ww)
        break;
		}
  }
}

int motor_control() {
  /*
  Set the motor control depending on the state.
  */
  int state = 0;
  if (irobot[istatetime] < 5) {
    state = irobot[istate];
  } else state = get_state();
  int laststate = irobot[istatetime];
  switch (laststate) {
    case sonic:
    break;
    case ww:
    if(state == bb) findLine(state);
    else if (state == bw) turn_left();
    else if (state == wb) turn_right();
    else wander();
    break;
    case bb:
    if (state == ww) findLine(state);
    else if (state == bw) turn_left();
    else if (state == wb) turn_right();
    else straight();
    break;
    case bw:
    if (state == bb) turn_right();
    else if (state == ww) turn_left();
    else straight();
    break;
    case wb:
    if (state == bb) turn_left();
    else if (state == ww) turn_right();
    else straight();
    break;
  }
  return state;
}

void approachObject() {
	//playSound(soundBeepBeep);
	int powers = RegSpeed + (sensor_sonic[average]/2);
	//int starter = powers;
	//sleep(1000);
	while (irobot[istate] == 0) { //if sees object
		//powers = sensor_sonic[average] * (-1/3) + starter;
		powers = sensor_sonic[average] * 1.5;
		//displayBigTextLine(8, "Power: %d", powers);
		setMotor(powers, powers);
		if (irobot[withinInch]) {
			setMotor(0,0);
			sleep(3000);
			setMotor(-50, -50);
			sleep(1000);
			setMotor(100, -100);
			sleep(200);
		}
		sleep(100);
		if (sensor_sonic[average] > 91) {
			//displayBigTextLine(4, "Sonic: %d", sensor_sonic[average]);
			irobot[istate] = 1;
		}
	}
}

task main() {
  int run = 1;
  int state;
  //TODO setup and start tasks
  startTask(infrasense,kDefaultTaskPriority);
	startTask(right_lightsense,kDefaultTaskPriority);
	startTask(left_lightsense,kDefaultTaskPriority);
  startup();
  while(run) {
    displayBigTextLine(4, "L Average: %d", sensor_llight[average]);
		displayBigTextLine(8, "R Average: %d", sensor_rlight[average]);
		displayBigTextLine(11, "Bound: %d", BOUND);
    state = motor_control();
    if (state == sonic)
      approachObject();//TODO do sonic stuff
    if (state != irobot[istate]) {
      irobot[istate] = state;
      irobot[istatetime] = 0;
    }
    irobot[istatetime] = irobot[istatetime] + 1;
    run = run + 1;
    sleep(50);
  }
}
