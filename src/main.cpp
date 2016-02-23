#include <HT1632.h>
#include <font_8x4.h>
#include <font_5x4.h>
#include <images.h>
#include <Arduino.h>
#include <avr/sleep.h>

int wd;
char text [] = "ctag";
const char lumino[] = "123456789abcdefg";
//#define DEBUG

void blink_pwr()
{
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
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
  HT1632.begin(12,11,10); // Connect to matrix. CS, WR, DATA
  wd = HT1632.getTextWidth(text, FONT_8X4_END, FONT_8X4_HEIGHT);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

while (1)
{
  #ifdef DEBUG
    Serial.println("Running loop.");
    digitalWrite(13, HIGH);
  #endif

  // Setup screen
  delay(500);
  HT1632.clear();
  HT1632.setBrightness(1);
  HT1632.drawText(text, 7, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();

  // Fade in
  for (unsigned short int l = 2; l <= 16; l++)
  {
    delay(100);
    HT1632.setBrightness(l);
  }

  delay(500);
  // Keep lit while motion is detected
  while (digitalRead(2) == HIGH)
  {
    delay(100);
  }
  delay(500);

  // Fade out
  for (unsigned short int l = 16; l > 0; l--)
  {
    HT1632.setBrightness(l);
    delay(100);
  }
  HT1632.clear();
  HT1632.render();

  #ifdef DEBUG
    digitalWrite(13, LOW);
  #endif

  delay(2000);
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
