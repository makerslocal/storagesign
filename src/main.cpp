#include <HT1632.h>
#include <font_8x4.h>
#include <font_5x4.h>
#include <images.h>
#include <Arduino.h>
#include <avr/sleep.h>

/*
 * Pin Mappings
 */
unsigned short int pinLightSensor = A0;
unsigned short int pinPowerLed = 13;
unsigned short int pinMotionInterrupt = 2;
unsigned short int pinSignCS = 12;
unsigned short int pinSignWR = 11;
unsigned short int pinSignDATA = 10;

/*
 * Global Variables
 */
int wd;
char text [] = "ctag";
unsigned short int lightLevel;
unsigned int fadeDelay = 150;
//const char lumino[] = "123456789abcdefg";

//#define DEBUG

void blink_pwr()
{
  digitalWrite(pinPowerLed, HIGH);
  delay(100);
  digitalWrite(pinPowerLed, LOW);
}

void IRQ0 ()
{
  sleep_disable();
  //blink_pwr();
}

int main()
{
  // Setup
  init(); // masked in normal setup()
  HT1632.begin(pinSignCS, pinSignWR, pinSignDATA); // Connect to matrix. CS, WR, DATA
  wd = HT1632.getTextWidth(text, FONT_8X4_END, FONT_8X4_HEIGHT);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  pinMode(pinMotionInterrupt, INPUT);
  pinMode(pinPowerLed, OUTPUT);
  //pinMode(12, OUTPUT);
  //digitalWrite(12, HIGH);

while (1)
{
  #ifdef DEBUG
    Serial.println("Running loop.");
    digitalWrite(pinPowerLed, HIGH);
  #endif

  // Setup screen
  delay(500);
  HT1632.clear();
  HT1632.setBrightness(1);
  HT1632.drawText(text, 7, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();

  // Fade in
  for (lightLevel = 2; lightLevel <= 16; lightLevel++)
  {
    delay(fadeDelay);
    HT1632.setBrightness(lightLevel);
    if ((lightLevel*60) >= (analogRead(pinLightSensor)))
    {
      break;
    }
  }

  delay(500);
  // Keep lit while motion is detected
  while (digitalRead(pinMotionInterrupt) == HIGH)
  {
    delay(100);
  }
  delay(500);

  // Fade out
  for (; lightLevel > 0; lightLevel--)
  {
    HT1632.setBrightness(lightLevel);
    delay(fadeDelay);
  }
  HT1632.clear();
  HT1632.render();

  #ifdef DEBUG
    digitalWrite(pinPowerLed, LOW);
  #endif

  delay(2000);

  if (digitalRead(pinMotionInterrupt) == HIGH)
    continue;

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, IRQ0, RISING);
  sleep_mode();
  detachInterrupt(0);
}

  #ifdef DEBUG
    Serial.println("Exiting.");
  #endif
  return(0);
}
