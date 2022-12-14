//Check thresh values before submission

/*
----States----
[Disabled - 0] [Idle - 1] [Running - 2] [Error - 3]
*/

void setup()
{
    
}

void loop(){
  //update all values before switch statement

  switch(state){
    case 0: //Disabled
      break;
    case 1: //Idle
      if(waterLevel <= waterThresh){
        state = 3;
      }else if(temp > tempThresh){
        state = 2;
      }
      break;

    case 2: //Running
      //start running fan
      if(waterLevel < waterThresh){
        state = 3;
      }else if(temp <= tempThresh){
        state = 1;
      }
      break;

    case 3: //Error
      break;

    default:
      state = 1;
      break;
  }
}