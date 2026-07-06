#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd1(0x27,16,2);
LiquidCrystal_I2C lcd2(0x26,16,2);   // Change if address differs

Servo entryServo;
Servo exitServo;

//---------------Pins---------------//
#define ENTRY_IR D7
#define EXIT_IR  D0

#define SLOT1 RX
#define SLOT2 TX
#define SLOT3 D3
#define SLOT4 D4

#define ENTRY_SERVO D6
#define EXIT_SERVO  D8

//----------------------------------//

int totalSlots = 4;
int availableSlots = 4;

bool slot1,slot2,slot3,slot4;

void setup()
{
  Serial.begin(115200);

  pinMode(ENTRY_IR,INPUT);
  pinMode(EXIT_IR,INPUT);

  pinMode(SLOT1,INPUT);
  pinMode(SLOT2,INPUT);
  pinMode(SLOT3,INPUT);
  pinMode(SLOT4,INPUT);

  entryServo.attach(ENTRY_SERVO);
  exitServo.attach(EXIT_SERVO);

  entryServo.write(0);
  exitServo.write(0);

  lcd1.init();
  lcd1.backlight();

  lcd2.init();
  lcd2.backlight();

  updateSlots();
  displayLCD();
}

void loop()
{
  updateSlots();

  //================ ENTRY =================//

  if(digitalRead(ENTRY_IR)==LOW)
  {
      if(availableSlots>0)
      {
          entryServo.write(90);
          delay(3000);
          entryServo.write(0);
      }
      else
      {
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("PARKING FULL");
          delay(2000);
      }

      while(digitalRead(ENTRY_IR)==LOW);
  }

  //================ EXIT =================//

  if(digitalRead(EXIT_IR)==LOW)
  {
      exitServo.write(90);
      delay(3000);
      exitServo.write(0);

      while(digitalRead(EXIT_IR)==LOW);
  }

  displayLCD();

  delay(200);
}

void updateSlots()
{
    slot1 = digitalRead(SLOT1);
    slot2 = digitalRead(SLOT2);
    slot3 = digitalRead(SLOT3);
    slot4 = digitalRead(SLOT4);

    availableSlots = 0;

    if(slot1==HIGH) availableSlots++;
    if(slot2==HIGH) availableSlots++;
    if(slot3==HIGH) availableSlots++;
    if(slot4==HIGH) availableSlots++;
}

void displayLCD()
{
    lcd1.clear();

    lcd1.setCursor(0,0);
    lcd1.print("Available:");
    lcd1.print(availableSlots);

    lcd1.setCursor(0,1);
    lcd1.print("Occupied:");
    lcd1.print(totalSlots-availableSlots);

    lcd2.clear();

    lcd2.setCursor(0,0);
    lcd2.print("S1:");
    lcd2.print(slot1?"Empty":"Full");

    lcd2.setCursor(9,0);
    lcd2.print("S2:");
    lcd2.print(slot2?"Empty":"Full");

    lcd2.setCursor(0,1);
    lcd2.print("S3:");
    lcd2.print(slot3?"Empty":"Full");

    lcd2.setCursor(9,1);
    lcd2.print("S4:");
    lcd2.print(slot4?"Empty":"Full");
}
