#include <Adafruit_MotorShield.h>
#include <lenlib.h> 
#include <Servo.h> 
#include <Wire.h>
#include "IO.h" 
#include "A_TaskLineFollow.h"
#include "B_TaskLabyrinth.h"
#include "C_TaskBalls.h"
#include "D_TaskSlope.h"

#define C_THIS_VERSION "Main 2015-10-06"

// = 0 om ljust, =1 om svart
#define C_DARK_1 1
#define C_LIGHT_0 0

#define C_MOTOR_LEFT_1 1
#define C_MOTOR_RIGHT_2 2

#define C_ActionStill 0
#define C_ActionTurnLeft 1
#define C_ActionTurnRight 2
#define C_ActionGoForward 3
#define C_ActionSearch 4

#define C_SPEED_HIGH   90
#define C_SPEED_MIDDLE 70
#define C_SPEED_LOW    50
#define C_DELAY 30

// Sets the Task which the robot is solving.
enum robotTaskEnum {
  rtLinefollow,
  rtLabyrinth,
  rtBalls,
  rtSlope
};




mySensors Sensors; // create Sensors object
myMotors Motors;  // create Motors object

// Declaring Variables.
static struct ioStruct stat_IO;


    
// ============================================================================================

void setup() 
{ 
  // Initiate IO-struct to start values
  stat_IO.iosMessageChArr[0] = 0;
  stat_IO.iosDelayMS = 0;
  
  // Initiate robot state and actions.
  // robotState.task = rtLinefollow;
  // robotState.state = rsStarting;
  // robotAction = aeActionStill;
  
  Motors.beginMotors();   // start motors
  Sensors.beginSensors(); // start sensors

  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println(C_THIS_VERSION);

  // w8 b4 starting.
  delay(4000);
} // setup

// ============================================================================================


void loop() 
{
  // Read all sensors
  stat_IO.iosReflFrontLeft  = Sensors.readReflect0(); // Read digital value of reflect sensor 0
  stat_IO.iosReflFrontCenter  = Sensors.readReflect1(); // Read digital value of reflect sensor 1
  stat_IO.iosReflFrontRight  = Sensors.readReflect2(); // Read digital value of reflect sensor 2

  /*
  
  // Determin which task the robot is preforming
  switch(robotState.task)
  {
    case rtLinefollow:
    {
      taskLineFollow(&stat_IO);
      break;
    }
    case rtLabyrinth:
    taskLabyrinth();
    break;
    
    case rtBalls:
    taskBalls();
    break;
    
    case rtSlope:
    taskSlope();
    break;
    
    default:
    break;
  }

  switch(robotState.state)
  {
    case rsStarting:
    if (!((stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_LIGHT_0)) && !((stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_DARK_1)))
    {
      robotState.state = rsLineFollow;
    }
    else
    if ((stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_LIGHT_0))
    {
      robotState.state = rsLineSearch;
    }
    else
    {
    //  robotState.state = nextState;
    }
    break;


  // Track which state the robot is in.
  switch(robotState.state)
  {
    case rsStarting:
    if (!((stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_LIGHT_0)) && !((stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_DARK_1)))
    {
      robotState.state = rsLineFollow;
    }
    else
    if ((stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_LIGHT_0))
    {
      robotState.state = rsLineSearch;
    }
    else
    {
    //  robotState.state = nextState;
    }
    break;

    case rsLineFollow:
    if ( (stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_LIGHT_0) )
    {
      // black is found only on Left - turn left!
      stat_IO.iosLeftEngine.direction = deBackward;
      stat_IO.iosLeftEngine.speed = C_SPEED_LOW;
      stat_IO.iosRightEngine.direction = deForward;
      stat_IO.iosRightEngine.speed = C_SPEED_MIDDLE;
      
      
    }
      
    if( (stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_DARK_1) )
    {
      // black is found only on Right - turn right!
      stat_IO.iosRightEngine.direction = deBackward;
      stat_IO.iosRightEngine.speed = C_SPEED_LOW;
      stat_IO.iosLeftEngine.direction = deForward;
      stat_IO.iosLeftEngine.speed = C_SPEED_MIDDLE;
    }


    if ( 
        ( (stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_LIGHT_0) )
        ||
        ( (stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_LIGHT_0) )
        ||
        ( (stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_DARK_1) )
        ||
        ( (stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_DARK_1) )
      )
    {
      stat_IO.iosLeftEngine.direction = deForward;
      stat_IO.iosLeftEngine.speed = C_SPEED_MIDDLE;
      stat_IO.iosRightEngine.direction = deForward;
      stat_IO.iosRightEngine.speed = C_SPEED_MIDDLE;
    }    
    break;

    case rsLineSearch:
    
    break;

    case rsFinnished:
    break;

    defalut:
    // To do: Error handling
    break;
  } // Switch

  // Activate Output
  if(stat_IO.iosLeftEngine.direction == deBackward)
  {
    Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, stat_IO.iosLeftEngine.speed);
  }
  if(stat_IO.iosLeftEngine.direction == deForward)
  {
    Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, stat_IO.iosLeftEngine.speed);
  }
  if(stat_IO.iosRightEngine.direction == deBackward)
  {
    Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, stat_IO.iosRightEngine.speed);
  }
  if(stat_IO.iosRightEngine.direction == deForward)
  {
    Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, stat_IO.iosRightEngine.speed);
  }

  // Evaluate sensor data
  ActionMode = C_ActionStill;
  if ( 
       ( (stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_LIGHT_0) )
       ||
       ( (stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_DARK_1) )
     )
  {
    // No black is found - search for it!
    // OR
    // Black is found on Left + Rigth but not on Middle - search fot it!
    ActionMode = C_ActionSearch;
  }

  if ( (stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_LIGHT_0) )
  {
    // black is found only on Left - turn left!
    ActionMode = C_ActionTurnLeft;
    searchCounter = 0;
  }

  if( (stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_LIGHT_0) && (stat_IO.iosReflFrontRight == C_DARK_1) )
  {
    // black is found only on Right - turn right!
    ActionMode = C_ActionTurnRight;
    searchCounter = 0;
  }


  if ( 
      ( (stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_LIGHT_0) )
       ||
      ( (stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_LIGHT_0) )
        ||
      ( (stat_IO.iosReflFrontLeft == C_LIGHT_0) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_DARK_1) )
       ||
      ( (stat_IO.iosReflFrontLeft == C_DARK_1) &&  (stat_IO.iosReflFrontCenter == C_DARK_1) && (stat_IO.iosReflFrontRight == C_DARK_1) )
     )
  {
    // Black is found... - go forward!
    ActionMode = C_ActionGoForward;
    searchCounter = 0;
  }


  if(ActionMode == C_ActionStill)
  {
      Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, 50);
      Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, 50);
      for (int i = 0; i<6; i++)
      {
        delay(1000);   
      }
  }

  
  // Activate outputs...
  LastAction = ActionMode;
  switch (ActionMode)
  {
    case C_ActionStill:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, 0);
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, 0);   
    break;
    
    case C_ActionTurnLeft:
      Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, C_SPEED_MIDDLE); 
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_HIGH);
    break;

    case C_ActionTurnRight:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_HIGH);
      Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_MIDDLE);    
    break;

    case C_ActionGoForward:
      Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_HIGH); // Run motor 1 & 2 forward
      Motors.runMotor(C_MOTOR_RIGHT_2, FORWARD, C_SPEED_HIGH);
    break;

    case C_ActionSearch:
      searchCounter++;
      if (searchCounter < 3)
      {
        break;
      }
  
      if(LastAction == C_ActionTurnLeft)
      {
        for (int i = 0; i<=5; i++)
        {
          // Turn right for 50 ms
          Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
          Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);
          delay(10)
        }
      }
      else
      {
        if(LastAction == C_ActionTurnRight)
        {
          // Turn left for 50 ms
          Motors.runMotor(C_MOTOR_LEFT_1, FORWARD, C_SPEED_MIDDLE);
          Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);
          delay(40)
        }
        else
        {
          if(LastAction == C_ActionGoForward)
          {
            for (int i = 0; i<=5; i++)
            {
              // Go backwards for 50 ms
              Motors.runMotor(C_MOTOR_LEFT_1, BACKWARD, C_SPEED_MIDDLE);
              Motors.runMotor(C_MOTOR_RIGHT_2, BACKWARD, C_SPEED_LOW);
              delay(10)
            }
          }
        }
        else
        {
          searchCounter = searchCounter + 1;
          factor = factor*0.95;
          int SearchSpeed = C_SPEED_MIDDLE;
          SearchSpeed = SearchSpeed*factor;

          // Begin Spiral search
          for (int i = 0; i<=5; i++)
            {
              Motors.runMotor(C_MOTOR_LEFT_1, FOREWARD, C_SPEED_MIDDLE);
              Motors.runMotor(C_MOTOR_RIGHT_2, FOREWARD, C_SPEED_LOW);
              delay(10)
            }
        }
    break;  
  } // switch 
       
  } // switch 
*/ 

  if (stat_IO.iosMessageChArr[0] != 0)
  {
    Serial.println(stat_IO.iosMessageChArr);
    stat_IO.iosMessageChArr[0] = 0;
  } // if

  if (stat_IO.iosDelayMS != 0)
  {
    delay(stat_IO.iosDelayMS);
    stat_IO.iosDelayMS = 0;
  } // if

} // loop
// ============================================================================================

