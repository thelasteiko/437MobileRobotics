/***********************************
 *    ROBOTICS ASSIGNMENT 2        *
 *    Group #5                     *
 *    Cody McMahon                 *
 *    ADD NAME HERE                *
 *    ADD NAME HERE                *
 ***********************************/

//THIS IS ALL JUST PSEUDOCODE AT THIS POINT



//number between 0 and 1 for the "Exponentially Weighted Moving Average.
//0 means current is 100% 1 means past results are 100%
#define ALPHA 0.5


//global variables because robotC is awful

//Weighted infrared reading
static float infraVal = -1;//-1 to mark that they are non initialized
//the 2 light sensor readings
static int lightsense[2] = {-1,-1};
//"boolean" if wandering or following a lone
static int isWander = 1;

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
  float rawReading;
  infraVal = getIRDistance(S3);
  while(1){
    rawReading = getIRDistance(S3);
    infraVal = (rawReading * (1.0 - ALPHA)) + (infraVal * (ALPHA))
  }
}

task lightsense(){
  
  //this should use a different algorithm because for our purposes it just needs to know
  //if its on the lines
  
}
/*
task lightsense2(){
  
  //both sensors can be different tasks or not
  
}
*/

task wander(){

  //put wander code here

}

task followLine(){

  //put followLine code here

}


task main(){
  //init thingshere?
  startTask(wander,kDefaultTaskPriority);
  startTask(infrasense,kDefaultTaskPriority);
  startTask(lightsense,kDefaultTaskPriority);  
  startTask(lightsense2,kDefaultTaskPriority);

  //busy waiting loop so program does not end
  while(1);
}
