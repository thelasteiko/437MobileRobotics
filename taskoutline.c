{\rtf1\ansi\ansicpg1252\deff0\deflang1033{\fonttbl{\f0\fmodern\fprq1\fcharset0 Courier New;}{\f1\fswiss\fcharset0 Arial;}}
{\colortbl ;\red0\green0\blue0;}
{\*\generator Msftedit 5.41.15.1515;}\viewkind4\uc1\pard\cf1\f0\fs20 /***********************************\par
 *    ROBOTICS ASSIGNMENT 2        *\par
 *    Group #5                     *\par
 *    Cody McMahon                 *\par
 *    ADD NAME HERE                *\par
 *    ADD NAME HERE                *\par
 ***********************************/\par
\par
//THIS IS ALL JUST PSEUDOCODE AT THIS POINT\par
\par
\par
\par
//number between 0 and 1 for the "Exponentially Weighted Moving Average.\par
//0 means current is 100% 1 means past results are 100%\par
#define ALPHA 0.5\par
\par
\par
//global variables because robotC is awful\par
\par
//Weighted infrared reading\par
static float infraVal = -1;//-1 to mark that they are non initialized\par
//the 2 light sensor readings\par
static int lightsense[2] = \{-1,-1\};\par
//"boolean" if wandering or following a lone\par
static int isWander = 1;\par
\par
/*\par
void switchMode()\{\par
  if(isWander)\{\par
    stopTask(wander);\par
    startTask(followLine);\par
    isWander = 0;\par
  \}\par
  else\{\par
    stopTask(followLine);\par
    startTask(wander,kDefaultTaskPriority);\par
    isWander = 1;\par
  \}\par
\}\par
\par
*/\par
//change this to whatever sensor we are using\par
task infrasense()\{\par
  float rawReading;\par
  infraVal = getIRDistance(S3);\par
  while(1)\{\par
    rawReading = getIRDistance(S3);\par
    infraVal = (rawReading * (1.0 - ALPHA)) + (infraVal * (ALPHA))\par
  \}\par
\}\par
\par
task lightsense()\{\par
  \par
  //this should use a different algorithm because for our purposes it just needs to know\par
  //if its on the lines\par
  \par
\}\par
/*\par
task lightsense2()\{\par
  \par
  //both sensors can be different tasks or not\par
  \par
\}\par
*/\par
\par
task wander()\{\par
\par
  //put wander code here\par
\par
\}\par
\par
task followLine()\{\par
\par
  //put followLine code here\par
\par
\}\par
\par
\par
task main()\{\par
  //init thingshere?\par
  startTask(wander,kDefaultTaskPriority);\par
  startTask(infrasense,kDefaultTaskPriority);\par
  startTask(lightsense,kDefaultTaskPriority);  \par
  startTask(lightsense2,kDefaultTaskPriority);\par
\par
  //busy waiting loop so program does not end\par
  while(1);\par
\}\par
\cf0\f1\par
}
 