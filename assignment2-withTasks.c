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

#define iLENGTH 4   //the length of the array
#define itime 0     //the number of cycles until the next turn
#define iturn 1     //the next turn to take; right:0, left:1
#define iturntime 2 //the number of cycles to be turning
#define inumturns 3 //keeps track of how many left and right turns there are
//TODO incorporate sensor information?
int irobot[iLENGTH]; //holds information about the robot state
int count; //how many turns it took
int check_turn; //0 = right; 1 = left


//number between 0 and 1 for the "Exponentially Weighted Moving Average.
//0 means current is 100% 1 means past results are 100%
#define ALPHA 0.3 //divide b/c we are working with integers
#define LENGTH 4
#define lastReading 0
#define average 1
#define canread 2
#define timesince 3
//info: last reading, avg
float sensor_sonic[LENGTH];
float sensor_rlight[LENGTH];
float sensor_llight[LENGTH];

float BOUND = 10;
float NEAR = 30;
//global variables because robotC is awful

//Weighted infrared reading
static float infraVal = -1;//-1 to mark that they are non initialized
//the 2 light sensor readings
static int lightsense[2] = {-1,-1};
//"boolean" if wandering or following a lone
static int isWander = 1;

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
  //BOUND = (white + black)/2;
  sleep(500);
  displayBigTextLine(4, "Set Bound: %d", BOUND);
  
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

//int backup(int duration) {
//	/*Sets the motors to reverse direction.*/
//	setMotorSpeed(leftMotor, -REGSPEED);	//Set the leftMotor (motor1) to half power (50)
//	setMotorSpeed(rightMotor, -REGSPEED);  //Set the rightMotor (motor6) to half power (50)
//	sleep(duration);
//	return 0;
//}

//int reverse() {
//	/*Turns the unit all the way around.*/
//	setMotorSpeed(leftMotor, -SHARPSPEED);		//Set the leftMotor (motor1) to full power reverse (-100)
//	setMotorSpeed(rightMotor, SHARPSPEED);  	//Set the rightMotor (motor6) to full power forward (100)
//	sleep(500);
//	return 0;
//}

//int turn_right_sharp() {
//	/*Turns a sharp right.*/
//	setMotorSpeed(leftMotor, -SHARPSPEED);		//Set the leftMotor (motor1) to full power reverse (-100)
//	setMotorSpeed(rightMotor, SHARPSPEED);  	//Set the rightMotor (motor6) to full power forward (100)
//	irobot[iturn] = 1; //keep going right
//	sleep(my_rand(300,400));
//	return 0;
//}

//int turn_left_sharp() {
//	/*Turns a sharp left.*/
//	setMotorSpeed(leftMotor, SHARPSPEED);		//Set the leftMotor (motor1) to full power reverse (-100)
//	setMotorSpeed(rightMotor, -SHARPSPEED);  	//Set the rightMotor (motor6) to full power forward (100)
//	irobot[iturn] = 0; //keep going left
//	sleep(my_rand(300,400));
//	return 0;
//}

/*
void switchMode(){
if(isWander){
stopTask(wander);
startTask(followLine);
isWander = 0;
}
else{
stopTask(followLine);
startTask(wander,kDefaultTaskPriority);
isWander = 1;
}
}

*/
//change this to whatever sensor we are using
task infrasense(){
  //float rawReading;
  //infraVal = getIRDistance(S3);
  //while(1){
  //  rawReading = getIRDistance(S3);
  //  infraVal = (rawReading * (1.0 - ALPHA)) + (infraVal * (ALPHA))
  //  infraVal = (rawReading + ALPHA(infraVal - rawReading))
  //}
  while (true) {
    sensor_sonic[canread] = 0;
    sensor_sonic[lastReading] = SensorValue[sonar4];
    sensor_sonic[average] = weightedAvg(sensor_sonic[lastReading], sensor_sonic[average]);
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

void setMotor(int powerL, int powerR) {
	setMotorSpeed(leftMotor, powerL);
	setMotorSpeed(rightMotor, powerR);
}

void followLine(){

  	if (sensor_llight[average] > BOUND && sensor_rlight[average] > BOUND) { //both see white
			//robot forward
  		//TODO: get rid of this condition once integrated in, we want to wander here
  		wander();
			//setMotor(REGSPEED, REGSPEED);
		} else if (sensor_llight[average] <= BOUND && sensor_rlight[average] > BOUND) { //left sees black
			//turn left(left motor stop, right motor forward)
			setMotor(0, REGSPEED);
		} else if (sensor_llight[average] > BOUND && sensor_rlight[average] <= BOUND) { //right sees black
			//turn right(left motor forward, right motor stop)
			setMotor(REGSPEED, 0);
		} else {	//both see black
			//robot forward
			setMotor(REGSPEED, REGSPEED);
		}

}

task movement() {
	int run = 1; //keeps track of how many loops
	while (run) {
		followLine();
		//wander();
    //if (sensor_sonic[canread])
      //displayCenteredBigTextLine(4, "Dist: %3d cm", sensor_sonic[lastReading]);
    //if(sensor_llight[canread])
    //  displayBigTextLine(4, "Reflected: %3d", sensor_llight[lastReading]);

		
    //TODO: decide when to run the followLine code.. is it when either left or right sees black?
      //switch(FLAG) {
      	//case FOLL
      	//case WANDER:
      	//wander();
      	//break;
      //}
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
	startTask(right_lightsense,kDefaultTaskPriority);
	startTask(left_lightsense,kDefaultTaskPriority);
	startup();
	startTask(movement,kDefaultTaskPriority);

	//busy waiting loop so program does not end
	while(1);
}
