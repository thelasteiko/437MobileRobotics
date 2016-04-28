#pragma config(Sensor, S4, sonar4, sonar4, sendorEV3_Ultrasonic)
#pragma config(Sensor, S1, llight, sensorEV3_color, modeEV3Color_Color)
#pragma config(Sensor, S2, rlight, sensorEV3_color, modeEV3Color_Color)
/***********************************
*    ROBOTICS ASSIGNMENT 2        *
*    Group #5                     *
*    Hannah Silva (the beautiful) *
*    Melinda Robertson (the grand)*
***********************************/

//http://help.robotc.net/WebHelpMindstorms/index.htm#Resources/topics/LEGO_EV3/ROBOTC/Buttons/waitForButtonPress.htm

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
//sensor-near: 0; w-w:1; b-b:2; w-b:3; b-w:4
#define istate  4   //the last state of the sensors
#define istatetime 5 //amount of time since last state
#define withinInch 6 //whether or not the robot is within an inch of an object
//irobot[istate] == 0
int irobot[iLENGTH]; //holds information about the robot state
int count; //how many turns it took
int check_turn; //0 = right; 1 = left


//number between 0 and 1 for the "Exponentially Weighted Moving Average.
//0 means current is 100% 1 means past results are 100%
#define ALPHA 0.7
#define LENGTH 4
#define lastReading 0
#define average 1
#define canread 2
//info: last reading, avg
float sensor_sonic[LENGTH];
float sensor_rlight[LENGTH];
float sensor_llight[LENGTH];

float BOUND = 10;
float NEAR = 30;

int my_rand(int min, int max) {
	/*Returns a random number between the min and max.*/
	int num = (rand()%(max-min))+min;
	return abs(num);
}

float weightedAvg(float lr, float avg) {
	return (lr + ALPHA * (avg-lr));
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
		sums = sums + sensor_sonic[lastReading] * pow((1-ALPHA),i);
		suml = suml + sensor_llight[lastReading] * pow((1-ALPHA),i);
		sumr = sumr + sensor_rlight[lastReading] * pow((1-ALPHA),i);
		i = i - 1;
		sleep(20);
  }
  sensor_sonic[average] = (sums + sensor_sonic[lastReading]) * ALPHA;
  sensor_llight[average] = (suml + sensor_llight[lastReading]) * ALPHA;
  sensor_rlight[average] = (sumr + sensor_rlight[lastReading]) * ALPHA;
  displayBigTextLine(4, "Starting...%d,", sensor_sonic[average]);
  displayBigTextLine(8, "%d, %d", sensor_llight[average], sensor_rlight[average]);
  sleep(2000);
}

int reset_motor() {
	/*Resets the speed of the motors to go forward
	and the values for determining when to turn.*/
	setMotorSpeed(leftMotor, REGSPEED);
	setMotorSpeed(rightMotor, REGSPEED);
	//reset time until turn
	irobot[itime] = 1;
	irobot[iturntime] = -1;
	irobot[inumturns] = 0;
	return 0;
}

int turn_right() {
	/*Sets the motors to turn right.*/
	int speed = my_rand(MINSPEED, MAXSPEED);
	setMotorSpeed(leftMotor, speed);
	setMotorSpeed(rightMotor, speed-my_rand(10, 13));
	//reset time until stop turning
	irobot[iturntime] = my_rand(MINTURN, MAXTURN);
	irobot[inumturns] = irobot[inumturns] + 1;
	return 0;
}

int turn_left() {
	/*Sets the motors to turn left.*/
	int speed = my_rand(MINSPEED, MAXSPEED);
	setMotorSpeed(rightMotor, speed);
	setMotorSpeed(leftMotor, speed-my_rand(10, 13));
	//reset time until stop turning
	irobot[iturntime] = my_rand(MINTURN, MAXTURN);
	irobot[inumturns] = irobot[inumturns] - 1;
	return 0;
}

//change this to whatever sensor we are using
task infrasense(){
  while (true) {
    sensor_sonic[canread] = 0;
    sensor_sonic[lastReading] = SensorValue[sonar4];
    sensor_sonic[average] = weightedAvg(sensor_sonic[lastReading], sensor_sonic[average]);
    if (sensor_sonic[average] <= 91)
    	irobot[istate] = 0;
   	if (sensor_sonic[average] <= 5)
   		irobot[withinInch] = 1;
   	else
   		irobot[withinInch] = 0;
    sensor_sonic[canread] = 1;
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

void approachObject() {
	int powers = RegSpeed + (sensor_sonic[average]/2);
	int starter = powers;
	//sleep(1000);
	while (irobot[istate] == 0) { //if sees object
		//powers = sensor_sonic[average] * (-1/3) + starter;
		powers = sensor_sonic[average] * 0.25 + starter;
		displayBigTextLine(8, "Power: %d", powers);
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
			displayBigTextLine(4, "Sonic: %d", sensor_sonic[average]);
			irobot[istate] = 1;
		}
	}
	

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
				turn_left();
			} else {
				turn_right();
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



void followLine(){
    //sensor-near: 0; w-w:1; b-b:2; w-b:3; b-w:4
    
  if (sensor_llight[average] <= BOUND && sensor_rlight[average] > BOUND) { //left sees black
    if(irobot[istate] != 4) {
      irobot[istatetime] = 0;
      irobot[istate] = 4;
      setMotor(0, REGSPEED);
    }
	} else if (sensor_llight[average] > BOUND && sensor_rlight[average] <= BOUND) { //right sees black
    if(irobot[istate] != 3) {
      irobot[istatetime] = 0;
      irobot[istate] = 3;
      setMotor(REGSPEED, 0);
    }
	} else {	//both see black
			//robot forward
      if(irobot[istate] != 2) {
        if (irobot[istatetime] <= 1) {
          setMotor(REGSPEED, 0);
          sleep(50);
        }
      }
      irobot[istatetime] = 0;
      irobot[istate] = 2;
      setMotor(REGSPEED, REGSPEED);
	}

}

task movement() {
	int run = 1; //keeps track of how many loops
	while (run) {
		//displayBigTextLine(4, "L Average: %d", sensor_llight[average]);
		//displayBigTextLine(8, "R Average: %d", sensor_rlight[average]);
		//displayBigTextLine(10, "Bound: %d", BOUND);
	if (irobot[istate] == 0) approachObject();
	else wander();
  //else if (sensor_llight[average] > BOUND && sensor_rlight[average] > BOUND) { //both see white
  //    if(irobot[istate] == 2) {
  //      if (irobot[istatetime] <= 5) {
  //        setMotor(0,REGSPEED);
  //        sleep(100);
  //      }
  //      irobot[istate] = 1;
  //      irobot[istatetime] = 0;
  //    }
  //		wander();
		//} else
  //    followLine();
    
    //irobot[istatetime] = irobot[istatetime] + 1;
		sleep(50);
		run = run + 1;
	}
}


task main(){
	//reset_motor();
	int prev_turn = 0;
	count = 0;
	check_turn = 2; //left = 1, right = 0
	startTask(infrasense,kDefaultTaskPriority);
	//startTask(right_lightsense,kDefaultTaskPriority);
	//startTask(left_lightsense,kDefaultTaskPriority);
	startup();
	startTask(movement,kDefaultTaskPriority);

	//busy waiting loop so program does not end
	while(1);
}
