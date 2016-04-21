#pragma config(Sensor, S2, llight, sensorEV3_color, modeEV3Color_Color)
#pragma config(Sensor, S3, rlight, sensorEV3_color, modeEV3Color_Color)
#pragma config(Motor,  motorB, leftwheel,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC, rightwheel,    tmotorEV3_Large, PIDControl, encoder)


void setMotor(int powerL, int powerR) {
	setMotorSpeed(leftwheel, powerL);
	setMotorSpeed(rightwheel, powerR);
}

task main()
{
	int lSensor;
	int rSensor;
	while (true) {
		lSensor = SensorValue(llight);
		rSensor = SensorValue(rlight);

		if (lSensor > 2 && rSensor > 2) {
			//robot forward
			setMotor(50, 50);
		} else if (lSensor <= 2 && rSensor > 2) {
			//turn left(left motor stop, right motor forward)
			setMotor(0, 50);
		} else if (lSensor > 2 && rSensor <= 2) {
			//turn right(left motor forward, right motor stop)
			setMotor(50, 0);
		} else {
			//robot forward
			setMotor(50, 50);
		}

		sleep(50);
	}


}
