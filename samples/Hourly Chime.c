
/*--------------------------------------------------------------------------------------------------------*\
|*                                                                                                        *|
|*                                        - StringDelete Demo -                                           *|
|*                                            ROBOTC on NXT                                               *|
|*                                                                                                        *|
|*  This program demos the "StringFromChars()" function.  The function is passed a string and an array    *|
|*  of bytes or chars (characters, ex. 'A').                                                              *|
|*                                                                                                        *|
|*                                        ROBOT CONFIGURATION                                             *|
|*    NOTES:                                                                                              *|
|*    1)  Only the NXT brick is required, no motors, no sensors.                                          *|
|*                                                                                                        *|
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
\*---------------------------------------------------------------------------------------------------4246-*/

task main()
{

  int time;
  int hour;    // Create int 'hour'.
  int minutesInHour=60; //Can be changed to a small number to watch easily or kept at 60 for accurate time keeping

  while(true)  // Infinite Loop:
  {
    time = nClockMinutes;
    if((nClockMinutes % minutesInHour) == 0)  // If the current time divided by 60 has a remainder of 0 (on the hour):
    {
      hour = nClockMinutes / minutesInHour;   // Set variable 'hour' to the current time.

      displayCenteredTextLine(2, "The Hour is:");  /* Display the current */
      displayCenteredBigTextLine(3, "%d", hour);   /* hour, on the hour.  */

      for(int i=0; i<hour; i++)  // Loop 'hour' number of times (1am gets 1 loop, 8pm gets 20 loops).
      {
        playSound(soundBlip);    // Play the sound, 'soundBeepBeep'.
        wait1Msec(250);              // Wait a quarter of a second before looping again.
      }
    }
  }
}
