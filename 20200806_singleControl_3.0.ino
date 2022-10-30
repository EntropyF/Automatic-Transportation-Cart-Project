#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>
Servo myservo;
int pos = 10;
//PS2手柄引脚；
#define PS2_DAT        12  //14   
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK         9  //17
// 电机控制引脚；
#define IN1 2
#define IN2 3
#define IN3 7
#define IN4 8
int speedPinA = 5;
int speedPinB = 6;
int speed;
//speed =200;
int D2 = 13;
int D3 =  4;
int D0 = A0;
int D1 = A1;
int track = A2;
int beat = A3;
//int led = 4;
//int brightness = 0;
int startPushCounter = 0;
int selectPushCounter = 0;
int trianglePushCounter = 0;

//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false
PS2X ps2x; // create PS2 Controller Class
//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
  myservo.attach(A4);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);

  pinMode(track, INPUT);

  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  pinMode(beat, OUTPUT);
  //   pinMode(led, OUTPUT);

  Serial.begin(57600);
  delay(300) ; //added delay to give wireless ps2 module some time to startup, before configuring it
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  do {
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    delay(500);
  } while (error);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");

  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");


  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
  }
}
void turnLeft() //左转
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(20);
}
void turnRight()//右转
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  //  delay(20);
}
void backLeft() //左后
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  //  delay(20);
}
void backRight() //右后
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  //  delay(20);
}
void forward() // 前进
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  //  delay(20);
}
void stop() // 停止；
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  //  delay(20);
}
void back()  //后退
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void loop() {
  /* You must Read Gamepad to get new values and set vibration values
    ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
    if you don't enable the rumble, use ps2x.read_gamepad(); with no values
    You should call this at least once a second
  */
  //  analogWrite(led, brightness);

  if (error == 1) //skip loop if no controller found
    return;

  if (type == 2) //Guitar Hero Controller
    return;

  else  { //DualShock Controller
    ps2x.read_gamepad(false, 0); //read controller and set large motor to spin at 'vibrate' speed
    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      //      Serial.print("Stick Values:");
      //      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
      //      Serial.print(",");
      //      Serial.print(ps2x.Analog(PSS_LX), DEC);
      //      Serial.print(",");
      //      Serial.print(ps2x.Analog(PSS_RY), DEC);
      //      Serial.print(",");
      //      Serial.println(ps2x.Analog(PSS_RX), DEC);

      int LY = ps2x.Analog(PSS_LY);
      int LX = ps2x.Analog(PSS_LX);
      //      int RY = ps2x.Analog(PSS_RY);
      //      int RX = ps2x.Analog(PSS_RX);


      if (LY < 128) //前进
      {

        speed = 127 - LY;
        //        Serial.println(speed);
        analogWrite(speedPinA, speed);
        analogWrite(speedPinB, speed);
        forward();
      }
      //后退
      if (LY > 128)
      {
        speed = LY - 128;

        analogWrite(speedPinA, speed);
        analogWrite(speedPinB, speed);
        back();
      }
      //左转
      if (LX < 128)
      {
        speed = 127 - LX;

        analogWrite(speedPinA, speed);
        analogWrite(speedPinB, speed);
        turnLeft();
      }
      //右转
      if (LX > 128)
      {
        speed = LX - 128;

        analogWrite(speedPinA, speed);
        analogWrite(speedPinB, speed);
        turnRight();
      }
      //      //左转
      //      if (LX < 128 && LX < 128)
      //      {
      //        speed = 127 - LX;
      //
      //        analogWrite(speedPinA, speed);
      //        analogWrite(speedPinB, speed);
      //        backLeft();
      //      }
      //      //右转
      //      if (LX > 128 && LX < 128)
      //      {
      //        speed = LX - 128;
      //
      //        analogWrite(speedPinA, speed);
      //        analogWrite(speedPinB, speed);
      //        backRight();
      //      }
      //如果摇杆居中
      if (LY >= 128 && LY <= 128 && LX >= 128 && LX <= 128)
      {
        stop();

      }
      Serial.print(LX);
      Serial.print(" ,");
      Serial.println(LY);
    }
    if (ps2x.Button(PSB_L2) || ps2x.Button(PSB_R2))
    {
      stop();
    }

    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);

    if (ps2x.Button(PSB_PAD_UP)) {       //will be TRUE as long as button is pressed
      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
      Serial.println("Collect");
    }
    if (ps2x.Button(PSB_PAD_DOWN)) {
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
      Serial.println("Release");
    }

    //    int greyness;
    //    greyness = analogRead(2);
    //    if (greyness < 700) {
    //      Serial.println("Black");
    //    }

    //will be TRUE if any button changes state (on to off, or off to on)
    if (ps2x.Button(PSB_START)) {
      //        startPushCounter++;

      //        switch (startPushCounter % 2) {
      //          case 0:
      digitalWrite(D2, LOW);
      digitalWrite(D3, HIGH);
      //            analogWrite(D2, 0);
      //            analogWrite(D3, 80);
      delay(100);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      delay(100);
//      Serial.println("Belt ON");
      //            break;
      //          case 1:
    } else {
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
//      Serial.println("Belt OFF");
      //            break;
    }
  }

  digitalWrite(beat, LOW);
  //  Serial.println(digitalRead(A2));
//  if (ps2x.Button(PSB_SELECT)) {
    if (digitalRead(track) == 1) {
      Serial.println("Black");
      digitalWrite(beat, HIGH);
      delay(60);
      digitalWrite(beat, LOW);
      delay(60);
    } else {
      digitalWrite(beat, LOW);
    }
//  }
 if (ps2x.NewButtonState()) {
  if (ps2x.Button(PSB_TRIANGLE)) {
    
    trianglePushCounter++;
    switch (trianglePushCounter % 2) {
//    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//      // in steps of 1 degree
//      myservo.write(pos);              // tell servo to go to position in variable 'pos'
//      delay(3);                       // waits 15ms for the servo to reach the position
//    }
//    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//      myservo.write(pos);              // tell servo to go to position in variable 'pos'
//      delay(3);                       // waits 15ms for the servo to reach the position
//    }
    case 0:
//    myservo.write(pos == 90);
    Serial.println("Servo OPEN");    
//    delay(50);
//    myservo.write(pos == 0);
    for (pos = 10; pos <= 110; pos += 1) { // goes from 0 degrees to 180 degrees
//      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(7);                       // waits 15ms for the servo to reach the position
    }
    break;
    case 1:
//    myservo.write(pos == 0); 
    Serial.println("Servo CLOSE");  
//    delay(50);
//    myservo.write(pos == 90); 
    for (pos = 110; pos >= 10; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(1);                       // waits 15ms for the servo to reach the position
    }
    break;
    }
   }
  }
  
  delay(20);
}
