#include <HT1632.h>
#include <font_8x4.h>
#include <images.h>
#include <Arduino.h>
#include <avr/sleep.h>

int wd;
char text [] = "ctag";
const char lumino[] = "123456789abcdefg";

void blink_pwr()
{
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
}

void IRQ0 ()
{
  sleep_disable();
  blink_pwr();
}

int main()
{
  // Setup
  init(); // masked in normal setup()
  HT1632.begin(7,6,5); // Connect to matrix. CS, WR, DATA
  wd = HT1632.getTextWidth(text, FONT_8X4_END, FONT_8X4_HEIGHT);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

while (1)
{
  Serial.println("Running loop.");

  HT1632.clear();
  //HT1632.setBrightness(lumino[l]);
  HT1632.drawText(text, 7, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();
  while (digitalRead(2) == HIGH)
  {
    digitalWrite(13, HIGH);
    NOP();
    delay(100);
  }
  digitalWrite(13, LOW);
  HT1632.clear();
  HT1632.render();

  delay(50);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, IRQ0, RISING);
  sleep_mode();
  detachInterrupt(0);
  // delay(100);
}

  Serial.println("Exiting.");
  return(0);
}
