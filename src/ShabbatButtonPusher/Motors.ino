/*
 * Called in main setup
 */
void setupMotors() {
  pinMode(PIN_MOTOR, OUTPUT);

  motors.state = MOTOR_STATE_NEUTRAL;
  motors.servo.attach(PIN_MOTOR); //controlling several in parallel off the same pin
  motors.servo.write(MOTOR_POSITION_ORIGIN);
}

/*
 * Called in main loop
 */
void updateMotorState() {

  switch (motors.state) {
    //the hardcoded pos assignments should always be there inherently, it's just a sanity enforcement
    case MOTOR_STATE_FORWARD:
      motors.servo.write(MOTOR_POSITION_END);
      motors.state = MOTOR_STATE_REVERSE;
      break;

    case MOTOR_STATE_REVERSE:
      motors.servo.write(MOTOR_POSITION_ORIGIN);
      motors.state = MOTOR_STATE_NEUTRAL;
      chooseMenu(&menus[0]);
      showText("Finished Pushing Button!");
      delay(2000);
      visibleMenu->showDisplay();
      //loop forever - for testing
      //motors.state = MOTOR_STATE_START;
      break;
    case MOTOR_STATE_START:
      showText("Pushing Button...");
      motors.servo.write(MOTOR_POSITION_ORIGIN);
      motors.state = MOTOR_STATE_FORWARD;
      break;
    default:
      //always return early if motor wasn't already running here...
      return;
  }

  //give the servo time to settle into position
  delay(2000);
}


