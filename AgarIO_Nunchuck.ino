/*
   NunchuckPrint

   2007 Tod E. Kurt, http://todbot.com/blog/

   Change log:

      Mark Tashiro -    Changed Wire.read to Wire.write
                        Changed Wire.receive to Wire.read
                        Added code for servos

*/
//parameters for wii's joystick.  This varies for every nunchuck, so use serial monitor to check.

#define xMin 25
#define xMax 226
#define yMin 36
#define yMax 224
int centerX = 120;
int centerY = 137;
#define radius 100 //radius for square
const int deadzone = 10;
//If you want to use the two buttons on the nunchuck as keyboard inputs
char zButton = ' ';
char cButton = 'w';

#include <Wire.h>
#include <Mouse.h>
#include <Servo.h>
#include <Keyboard.h>
Servo myServo;
unsigned long pMillis = 0;
unsigned long secMillis = 0;
unsigned long mouseioMillis = 0;
unsigned long scrollXMillis = 0;
unsigned long scrollYMillis = 0;
unsigned long hitMillis = 0;
bool scrollMode = false;
bool scrollModeTwo = false;
int counting = 1;
bool change = false;
bool previous = false;
bool escape = false;
// parameters for reading the joystick:
// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
const int mouseioDelay = 7;
const int gameioDelay = 1;
int responseDelay = gameioDelay;        // response delay of the mouse, in ms
int threshold = range / 4;  // resting threshold
int center = range / 2;     // resting position value


boolean mouseIsActive = true;    // whether or not to control the mouse


bool c = false;
bool z = false;
int countTwo = 0;
static uint8_t nunchuck_buf[6];   // array to store nunchuck data,
unsigned long dataMillis = 0;
int xReading;
int yReading;
bool action1;
bool action2;
unsigned long action1Millis = 0;
unsigned long action2Millis = 0;
unsigned long allActionMillis = 0;

bool game1 = false;
bool game2 = false;
unsigned long game1Millis = 0;
unsigned long game2Millis = 0;
unsigned long allGameMillis = 0;
bool slitherio = false;

bool mouse1 = false;
bool mouse2 = false;
unsigned long mouse1Millis = 0;
unsigned long mouse2Millis = 0;
unsigned long allMouseMillis = 0;
bool mouseio = false;
void setup()
{
  Serial.begin(115200);  nunchuck_init(); // send the initilization handshake
  Serial.println ("Finished setup\n");
  Mouse.begin();
  myServo.attach(10);
  Keyboard.begin();

}

void loop()
{
  unsigned long cMillis = millis();
  nunchuck_get_data();
  int xAxis = nunchuck_buf[0];
  int yAxis = nunchuck_buf[1];
  int xReading = readAxis(xAxis, cMillis);
  change = true;
  int yReading = readAxis(yAxis, cMillis);
  change = false;
  if (cMillis - pMillis >= responseDelay) {
    if (!scrollModeTwo && !scrollMode && mouseio && mouseIsActive) {
      Mouse.move(xReading, yReading);
    }
    else if (!mouseio) {
      Mouse.move(xReading, yReading);
    }
    pMillis = cMillis;
  }
  if (!scrollMode && !scrollModeTwo && !mouseio && (cMillis - hitMillis > 0.5) && (counting == 0 ) && (sqrt(pow((centerX - xAxis), 2) + pow((centerY - yAxis), 2)) < 50)) {
    Keyboard.press(KEY_ESC);
    Keyboard.release(KEY_ESC);
    Keyboard.press(KEY_RIGHT_CTRL);
    Keyboard.press(KEY_RIGHT_ALT);
    Keyboard.press('s');
    Keyboard.release(KEY_RIGHT_CTRL);
    Keyboard.release(KEY_RIGHT_ALT);
    Keyboard.release('s');
    counting++;
  }
  else if (!scrollModeTwo && !scrollMode && (sqrt(pow((centerX - xAxis), 2) + pow((centerY - yAxis), 2)) > 50)) {
    counting = 0;
    hitMillis = cMillis;
  }
  if (cMillis - dataMillis >= 250) {
    // un-comment next line to print data to serial monitor
    nunchuck_print_data();
    dataMillis = cMillis;
  }
  checkButtons();
  if (!scrollModeTwo && !scrollMode && !mouseio) {
    differentGame(cMillis);
    Mouse.release(MOUSE_LEFT);
    Mouse.release(MOUSE_RIGHT);
  }
  if (!scrollModeTwo && scrollMode && mouseio) {
    if ((cMillis - scrollXMillis) >= 30) {
      if ((xAxis - 10) > centerX) {
        Keyboard.release(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, -1);
      }
      else if ((xAxis + 10) < centerX) {
        Keyboard.release(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, 1);
      }
      scrollXMillis = cMillis;
    }
    if ((cMillis - scrollYMillis) >= 30) {
      if ((yAxis - 10) > centerY) {
        Keyboard.press(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, -1);
      }
      else if ((yAxis + 10) < centerY) {
        Keyboard.press(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, 1);
      }
      scrollYMillis = cMillis;
    }
  }
  if (scrollModeTwo && mouseio && !scrollMode) {
    if ((cMillis - scrollXMillis) >= 5) {
      if ((xAxis - 10) > centerX) {
        Keyboard.release(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, 10);
      }
      else if ((xAxis + 10) < centerX) {
        Keyboard.release(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, -10);
      }
      scrollXMillis = cMillis;
    }
    if ((cMillis - scrollYMillis) >= 15) {
      if ((yAxis - 10) > centerY) {
        Keyboard.press(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, 10);
      }
      else if ((yAxis + 10) < centerY) {
        Keyboard.press(KEY_LEFT_SHIFT);
        Mouse.move(0, 0, -10);
      }
      scrollYMillis = cMillis;
    }

  }
  myServo.write(90);
  scrollMotion(cMillis);
  MouseMode(cMillis);
  if (escape) {
    shakeEsc(cMillis);
  }
}


//
// Nunchuck functions
//

// Uses port C (analog in) pins as power & ground for Nunchuck
// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
void nunchuck_init()
{
  Wire.begin();                      // join i2c bus as master
  Wire.beginTransmission(0x52);     // transmit to device 0x52
  Wire.write(0x40);            // sends memory address
  Wire.write(0x00);            // sends sent a zero.
  Wire.endTransmission();     // stop transmitting
}

// Send a request for data to the nunchuck
// was "send_zero()"
void nunchuck_send_request()
{
  Wire.beginTransmission(0x52);     // transmit to device 0x52
  Wire.write(0x00);            // sends one byte
  Wire.endTransmission();     // stop transmitting
}

// Receive data back from the nunchuck,
int nunchuck_get_data()
{
  int cnt = 0;
  Wire.requestFrom (0x52, 6);     // request data from nunchuck
  while (Wire.available ()) {
    // receive byte as an integer
    nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.read());
    cnt++;
  }
  nunchuck_send_request();  // send request for next data payload
  // If we recieved the 6 bytes, then go print them
  if (cnt >= 5) {
    return 1;   // success
  }
  return 0; //failure
}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void nunchuck_print_data()
{
  static int i = 0;
  int joy_x_axis = nunchuck_buf[0];
  int joy_y_axis = nunchuck_buf[1];

  int accel_x_axis = nunchuck_buf[2]; // * 2 * 2;
  int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
  int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;


  int z_button = 0;
  int c_button = 0;

  // byte nunchuck_buf[5] contains bits for z and c buttons
  // it also contains the least significant bits for the accelerometer data
  // so we have to check each bit of byte outbuf[5]
  if ((nunchuck_buf[5] >> 0) & 1) {
    z_button = 1;
  }
  if ((nunchuck_buf[5] >> 1) & 1) {
    c_button = 1;
  }
  if ((nunchuck_buf[5] >> 2) & 1)
    accel_x_axis += 2;
  if ((nunchuck_buf[5] >> 3) & 1)
    accel_x_axis += 1;

  if ((nunchuck_buf[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((nunchuck_buf[5] >> 5) & 1)
    accel_y_axis += 1;

  if ((nunchuck_buf[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((nunchuck_buf[5] >> 7) & 1)
    accel_z_axis += 1;

  Serial.print(i, DEC);
  Serial.print("\t");

  Serial.print("joy:");
  Serial.print(joy_x_axis, DEC);
  Serial.print(",");
  Serial.print(joy_y_axis, DEC);
  Serial.print("  \t");

  Serial.print("acc:");
  Serial.print(accel_x_axis, DEC);
  Serial.print(",");
  Serial.print(accel_y_axis, DEC);
  Serial.print(",");
  Serial.print(accel_z_axis, DEC);
  Serial.print("\t");

  Serial.print("but:");
  Serial.print(z_button, DEC);
  Serial.print(",");
  Serial.print(c_button, DEC);

  Serial.print("\r\n");  // newline
  i++;
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char nunchuk_decode_byte (char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}
void shakeMouse(long cM) {
  if (cM - action1Millis > 50) {
    if (nunchuck_buf[2] > 150) {
      action1 = true;
    }
    action1Millis = cM;
  }
  if (action1)
    if (cM - action2Millis > 90) {
      if (nunchuck_buf[2] < 50) {
        action2 = true;
      }
      action2Millis = cM;
    }
  if (action1 && action2 && c) {
    if (mouseIsActive)
      Serial.println("Joystick off");
    else
      Serial.println("Joystick on");
    mouseIsActive = !mouseIsActive;
    allActionMillis -= 400;
  }
  if (cM - allActionMillis > 300) {
    action1 = false;
    action2 = false;
    allActionMillis = cM;
  }
}
void shakeEsc(long cM) {
  if (cM - action1Millis > 50) {
    if (nunchuck_buf[2] > 150) {
      action1 = true;
    }
    action1Millis = cM;
  }
  if (action1)
    if (cM - action2Millis > 90) {
      if (nunchuck_buf[2] < 50) {
        action2 = true;
      }
      action2Millis = cM;
    }
  if (action1 && action2 && c) {
    Serial.println("Escaping");
    Keyboard.write(KEY_ESC);
    Keyboard.write(KEY_RETURN);
    allActionMillis -= 400;
  }
  if (cM - allActionMillis > 300) {
    action1 = false;
    action2 = false;
    allActionMillis = cM;
  }
}
void differentGame(long cM) {
  if (game2)
    if (cM - game1Millis > 30) {
      if (nunchuck_buf[4] > 190) {
        game1 = true;
      }
      game1Millis = cM;
    }
  if (cM - game2Millis > 60) {
    if (nunchuck_buf[4] < 105) {
      game2 = true;
    }
    game2Millis = cM;
  }
  if (game1 && game2 && c && !mouseio) {
    if (slitherio) {
      Serial.println("Agar.io mode");
      previous = true;
    }
    else {
      Serial.println("Slither.io mode");
      previous = false;
    }
    slitherio = !slitherio;
    allGameMillis -= 400;
  }
  if (cM - allGameMillis > 300) {
    game1 = false;
    game2 = false;
    allGameMillis = cM;
  }
}

void MouseMode(long cM) {
  if (mouseio) {
    shakeMouse(cM);
    escape = false;
  }
  else {
    escape = true;
  }
  if (mouse2)
    if (cM - mouse1Millis > 30) {
      if (nunchuck_buf[4] > 190) {
        mouse1 = true;
      }
      mouse1Millis = cM;
    }
  if (cM - mouse2Millis > 60) {
    if (nunchuck_buf[4] < 105) {
      mouse2 = true;
    }
    mouse2Millis = cM;
  }
  if (mouseio && ((cM - mouseioMillis) >= 400)) {
    c = false;
    mouseioMillis = cM;
  }
  if (mouse1 && mouse2 && !c) {
    if (mouseio) {
      Serial.println("Mouse mode off");
      responseDelay = gameioDelay;
    }
    else {
      Serial.println("Mouse mode on");
      responseDelay = mouseioDelay;
    }
    Keyboard.write(KEY_ESC);
    mouseio = !mouseio;
    if (!previous)
      slitherio = false;
    else
      slitherio = true;
    allMouseMillis -= 400;
  }
  if (cM - allMouseMillis > 300) {
    mouse1 = false;
    mouse2 = false;
    allMouseMillis = cM;
  }
}
void checkButtons() {
  if ((nunchuck_buf[5] >> 1) & 1) {
    if (!c)
      c = true;
    if (mouseio) {
      Mouse.release(MOUSE_LEFT);
    }
    else if (slitherio)
      Keyboard.release(cButton);
  }
  else {
    if (c) {
      if (mouseio) {
        Mouse.press(MOUSE_LEFT);
      }
      else {
        Keyboard.press(cButton);
        if (!slitherio) {
          c = false;
          Keyboard.release(cButton);
        }
      }
    }
  }
  if ((nunchuck_buf[5] >> 0) & 1) {
    if (!z)
      z = true;
    if (mouseio) {
      Mouse.release(MOUSE_RIGHT);
    }
    else if (slitherio)
      Keyboard.release(zButton);
  }
  else {
    if (z) {
      if (mouseio) {
        Mouse.press(MOUSE_RIGHT);
      }
      else {
        Keyboard.press(zButton);
        if (!slitherio) {
          z = false;
          Keyboard.release(zButton);
        }
      }
    }
  }
}
void scrollMotion(long cM) {
  if (nunchuck_buf[2] > 170) {
    scrollMode = true;
    scrollModeTwo = false;
    countTwo = 0;
  }
  else if ((nunchuck_buf[2] > 70) && (nunchuck_buf[2] < 90)) {
    scrollModeTwo = true;
    scrollMode = false;
    countTwo = 0;
  }
  else {
    scrollMode = false;
    scrollModeTwo = false;
    if (countTwo == 0)
      Keyboard.release(KEY_LEFT_SHIFT);
    countTwo++;
  }
}

int readAxis(int thisAxis, long cM) {
  // read the analog input:
  int reading = thisAxis;

  if (change)
    reading = map(reading, yMin, yMax, range, 0);  //y-axis
  else
    reading = map(reading, xMin, xMax, 0, range);  //x-axis

  // if the output reading is outside from the
  // rest position threshold,  use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }
  // return the distance for this axis:
  return distance;
}
