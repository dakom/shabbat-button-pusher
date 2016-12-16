#include <Servo.h>


/*
   DEFINITIONS
*/

#define PIN_MOTOR 10
#define PIN_LED 13
#define STATE_NEUTRAL 0
#define STATE_FORWARD 1
#define STATE_REVERSE 2
#define POSITION_ORIGIN 0
#define POSITION_END 180

typedef struct {
  Servo servo;
  uint8_t pos;
  uint8_t state;
} MotorInfo;

typedef struct {
  int ledValue;
} DisplayInfo;

/*
   GLOBALS
*/

DisplayInfo display;
MotorInfo motors;

/*
   DISPLAY
*/

void updateDisplay() {
  int value = (motors.state == STATE_NEUTRAL) ? LOW : HIGH;

  if (display.ledValue != value) {
    digitalWrite(PIN_LED, value);
    display.ledValue = value;
  }
}


/*
   TRIGGERS
*/

bool manualPush() {
  //return (digitalRead(PIN_BUTTON) == HIGH) ? true : false;
  return true;
}

bool pollTimer() {
  //replace with timer code
}

/*
   MOTORS
*/
void updateMotorState() {

  switch (motors.state) {
    //the hardcoded pos assignments should always be there inherently, it's just a sanity enforcement
    case STATE_FORWARD:
      motors.servo.write(motors.pos++);
      if (motors.pos >= POSITION_END) {
        motors.pos = POSITION_END;
        motors.state = STATE_REVERSE;

      }
      break;

    case STATE_REVERSE:
      motors.servo.write(motors.pos--);
      if (motors.pos <= POSITION_ORIGIN) {
        motors.pos = POSITION_ORIGIN;
        motors.state = STATE_NEUTRAL;
      }
      break;
    default:
      if (manualPush() || pollTimer()) {
        motors.state = STATE_FORWARD;
        motors.pos = POSITION_ORIGIN;
        updateMotorState(); //recursive ftw
      }
      //always return early if motor wasn't already running here...
      return;
  }

  //evidently this is needed to give the servos time to settle into position
  delay(15);
}

/*
   MAIN - SETUP
*/
void setup() {
  //pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED, OUTPUT);

  display.ledValue = LOW;

  motors.state = STATE_NEUTRAL;
  motors.pos = POSITION_ORIGIN;
  motors.servo.attach(PIN_MOTOR); //controlling several in parallel off the same pin
  motors.servo.write(motors.pos);
}

/*
   MAIN - LOOP
*/

void loop() {


  updateMotorState();
  updateDisplay();
  //TODO: Show/update menu...

}
