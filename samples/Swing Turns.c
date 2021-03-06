#pragma config(StandardModel, "EV3_REMBOT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*------------------------------------------------------------------------------------------------
  This program will have your robot make a right swing turn for one (1) second and then make a
  left swing turn afterwards. At the end of the program, all motors will shut down automatically
  and turn off.

                            ROBOT CONFIGURATION: LEGO EV3 REM Bot
    MOTORS & SENSORS:
    [I/O Port]          [Name]              [Type]                			[Location]
    MotorC        			leftMotor           LEGO EV3 Motor		      Left side motor
    MotorB       				rightMotor          LEGO EV3 Motor		      Right side motor (reversed)
------------------------------------------------------------------------------------------------*/

task main()
{
	setMotorSpeed(leftMotor, 0);			//Set the leftMotor (motor1) to Off
	setMotorSpeed(rightMotor, 100);  	//Set the rightMotor (motor6) to full power forward (100)
	sleep(1000);											//Wait for 1 second before continuing on in the program.

	setMotorSpeed(leftMotor, 100);		//Set the leftMotor (motor1) to full power forward (100)
	setMotorSpeed(rightMotor, 0);  		//Set the rightMotor (motor6) to full power reverse (-100)
	sleep(1000);											//Wait for 1 second before continuing on in the program.
}
