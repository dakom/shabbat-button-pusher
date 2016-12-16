#include <Servo.h>


/*
   DEFINITIONS
*/

#define PIN_MOTOR 3
#define STATE_NEUTRAL 0
#define STATE_START 1
#define STATE_FORWARD 2
#define STATE_REVERSE 3
#define POSITION_ORIGIN 20
#define POSITION_END 170

typedef struct {
  Servo servo;
  uint8_t state;
} MotorInfo;

MotorInfo motors;


/*
   TRIGGERS
*/

/*
   MOTORS
*/
void updateMotorState() {

  switch (motors.state) {
    //the hardcoded pos assignments should always be there inherently, it's just a sanity enforcement
    case STATE_FORWARD:
      motors.servo.write(POSITION_END);
      motors.state = STATE_REVERSE;
      break;

    case STATE_REVERSE:
      motors.servo.write(POSITION_ORIGIN);
      motors.state = STATE_NEUTRAL;
      //motors.state = STATE_START;
      break;
    case STATE_START:
        motors.servo.write(POSITION_ORIGIN);
        motors.state = STATE_FORWARD;
        break;
  default:
      //always return early if motor wasn't already running here...
      return;
  }

  //give the servo time to settle into position
  delay(2000);
}

/*
   MAIN - SETUP
*/
void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
 
  motors.state = STATE_NEUTRAL;
  motors.servo.attach(PIN_MOTOR); //controlling several in parallel off the same pin

  motors.state = STATE_START;
   
}

/*
   MAIN - LOOP
*/

void loop() {


  updateMotorState();
  //TODO: Show/update menu...

}
