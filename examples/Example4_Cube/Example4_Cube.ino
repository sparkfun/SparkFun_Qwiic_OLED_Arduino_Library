/*
  MicroOLED_Cube.ino
  Rotating a 3-D Cube on the MicroOLED Breakout
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: October 27, 2014

  This sketch uses the MicroOLED library to draw a 3-D projected
  cube, and rotate it along all three axes.

  Hardware Connections:
    This example assumes you are using Qwiic. See the SPI examples for
    a detailed breakdown of connection info.

  This code is beerware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Distributed as-is; no warranty is given.
 */

//#define MICRO
//#define NARROW


#if defined(MICRO)
#include <qwiic_oledmicro.h> 
QwOLEDMicro myOLED;
const char * deviceName = "Micro OLED";

#elif defined(NARROW)
#include <qwiic_olednarrow.h> 
QwOLEDNarrow myOLED;
const char * deviceName = "Narrow OLED";

#else
#include <qwiic_oledtransp.h>
QwOLEDTransparent myOLED;
const char * deviceName = "Transparent OLED";
#endif

QwI2C i2cBus;


int width;
int height;

uint draw_total_time;
uint n_draws;

float d = 3;
float px[] = {
    -d, d, d, -d, -d, d, d, -d};
float py[] = {
    -d, -d, d, d, -d, -d, d, d};
float pz[] = {
    -d, -d, -d, -d, d, d, d, d};

float p2x[] = {
    0, 0, 0, 0, 0, 0, 0, 0};
float p2y[] = {
    0, 0, 0, 0, 0, 0, 0, 0};

float r[] = {
    0, 0, 0};

#define SHAPE_SIZE 400
// Define how fast the cube rotates. Smaller numbers are faster.
// This is the number of ms between draws.
#define ROTATION_SPEED 00

// I2C is great, but will result in a much slower update rate. The
// slower framerate may be a worthwhile tradeoff, if you need more
// pins, though.
void setup()
{
    delay(100);   //Give display time to power on
    Serial.begin(115200);
    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Test #1 on: ");
    Serial.println(String(deviceName));


    i2cBus.init();
    myOLED.set_comm_bus(i2cBus, myOLED.default_address);
    if(!myOLED.init()){

        Serial.println("Init Failed");
        while(1);
    }

    Serial.println("- Init Success");

    width = myOLED.getWidth();
    height = myOLED.getHeight();

    // for frame rate calc
    draw_total_time =0;
    n_draws=0;

    // set a template for our framerate display
    Serial.print("- Frame Rate: 00.00"); 
}

void loop()
{
  drawCube();
  //delay(ROTATION_SPEED);
}

void drawCube()
{
  r[0] = r[0] + PI / 180.0; // Add a degree
  r[1] = r[1] + PI / 180.0; // Add a degree
  r[2] = r[2] + PI / 180.0; // Add a degree
  if (r[0] >= 360.0 * PI / 180.0)
    r[0] = 0;
  if (r[1] >= 360.0 * PI / 180.0)
    r[1] = 0;
  if (r[2] >= 360.0 * PI / 180.0)
    r[2] = 0;

  for (int i = 0; i < 8; i++)
  {
    float px2 = px[i];
    float py2 = cos(r[0]) * py[i] - sin(r[0]) * pz[i];
    float pz2 = sin(r[0]) * py[i] + cos(r[0]) * pz[i];

    float px3 = cos(r[1]) * px2 + sin(r[1]) * pz2;
    float py3 = py2;
    float pz3 = -sin(r[1]) * px2 + cos(r[1]) * pz2;

    float ax = cos(r[2]) * px3 - sin(r[2]) * py3;
    float ay = sin(r[2]) * px3 + cos(r[2]) * py3;
    float az = pz3 - 150;

    p2x[i] = width / 2 + ax * SHAPE_SIZE / az;
    p2y[i] = height / 2 + ay * SHAPE_SIZE / az;
    //printf("CUBE (%d, %d)\n", p2x[i], p2y[i]);
  }

  // Calculate draw time...
  uint milStart = millis();

  myOLED.erase();
  for (int i = 0; i < 3; i++)
  {
    myOLED.line(p2x[i], p2y[i], p2x[i + 1], p2y[i + 1]);
    myOLED.line(p2x[i + 4], p2y[i + 4], p2x[i + 5], p2y[i + 5]);
    myOLED.line(p2x[i], p2y[i], p2x[i + 4], p2y[i + 4]);
  }
  myOLED.line(p2x[3], p2y[3], p2x[0], p2y[0]);
  myOLED.line(p2x[7], p2y[7], p2x[4], p2y[4]);
  myOLED.line(p2x[3], p2y[3], p2x[7], p2y[7]);
  myOLED.display();

  draw_total_time += millis() - milStart;
  n_draws++;

  // output framerate?
  if(n_draws % 120 == 0){
      // backspace over old number 
      Serial.print("\b\b\b\b\b");  // backspace over old number
      Serial.print(((float)draw_total_time)/n_draws);

      if(n_draws > 1000){ // reset after a bit...
          n_draws = 0;
          draw_total_time=0;
      }
  }

}
