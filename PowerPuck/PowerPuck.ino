/*
  Initial code for my PowerPuck project.
  Created by Albert Kravcov

*/
#include <EEPROM.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI


#define Voltagedetect          3.25 // Min. voltage for Detection
#define max_cellvoltage        4.25 // Max. cell voltage
#define R1                   18000  // Resistor 1
#define R2                    4700  // Resistor 2 (measured)
#define VOLT_SENS               A0  // Voltage Sensor
#define BUTTON_PIN               7  // Button
#define longpresstime         1000  // in ms
#define powertime             3000  // in ms
#define alarmADDR                1  // EEPROM Adress


float voltage;
int32_t lipo;
float alarmvalue = 3.40;
int32_t alarmvalueEEP;
float cellvoltage;
int32_t battery_health = 4;
int32_t VoltageByte;
int pressedbut = 0;
int i_butt = 0;
int mode;
int menuselect;
unsigned long runtime = 0;
unsigned long spendtime = 0;
unsigned long healthtime = 0;
int AlarmState = LOW;
long interval = 500;
long interval2 = 1000;
int MenuState = LOW;
long menuinterval = 100;
unsigned long updatetime = 0;
int32_t powermode = 1;
int32_t buttoncount = 0;
unsigned long presstime = 0;


void setup() {

  pinMode(3, OUTPUT); //LED red
  pinMode(4, OUTPUT); //LED green1
  pinMode(5, OUTPUT); //LED green2
  pinMode(6, OUTPUT); //LED green3
  pinMode(8, OUTPUT); //Buzzer
  pinMode(9, OUTPUT); //power
  digitalWrite(9, HIGH); //power on
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(13, OUTPUT); //ledbuidin

  Serial.begin(9600);

  alarmvalueEEP = EEPROM.read(alarmADDR);

  if (alarmvalueEEP != 0)
  {
    alarmvalue = (alarmvalueEEP / 10.0);
  }
  if (alarmvalueEEP > 250)
  {
    alarmvalue = 3.40;
  }
  else { }


  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);
  
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (50);
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (50);
  digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (50);
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (150);
  digitalWrite(6, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (50);
  digitalWrite(5, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (50);
  digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (50);
  digitalWrite(3, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (250);


  voltagetest();
  

  if (voltage > (Voltagedetect * 5.0))
  {
    lipo = 5;
    //beep_x(lipo);
  }
  else if (voltage > (Voltagedetect * 4.0))
  {
    lipo = 4;
    //beep_x(lipo);
  }
  else if (voltage > (Voltagedetect * 3.0))
  {
    lipo = 3;
    // beep_x(lipo);
  }
  else if (voltage > (Voltagedetect * 2.0))
  {
    lipo = 2;
    //beep_x(lipo);
  }
  else
  {
    lipo = 2;
    // beep_x(lipo);
  }



  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
}




void voltagetest()
{

  int sensorValue = analogRead(VOLT_SENS); // read the input on analog pin 0:

  voltage = sensorValue * (5.00 / 1023.0) * ((R1 + R2) / R2); // Convert the analog reading (which goes from 0 - 1023) to a voltage, considering the voltage divider:

  //voltage = round(voltage * 10) / 10.0; //round the result
  VoltageByte = voltage * 10;

  cellvoltage = voltage / lipo;

  float cellfull = (max_cellvoltage) - (alarmvalue); //determine 100% of travel scale
  float cellstate = (max_cellvoltage) - (cellvoltage); //determine the actual cell delta value
  float battery_state = 100 - (((cellstate) * 100) / (cellfull)); //determine cell charge left in percent

  if (battery_state >= 75)
  {
    unsigned long currenthealthtime = millis();

    if ((currenthealthtime - healthtime >= interval2) && (battery_state >= 75)) {
      healthtime = currenthealthtime;

      battery_health = 4;
    }
  }
  else if (battery_state >= 50 && battery_state < 75)
  {
    unsigned long currenthealthtime = millis();

    if ((currenthealthtime - healthtime >= interval2) && (battery_state >= 50 && battery_state < 75)) {
      healthtime = currenthealthtime;

      battery_health = 3;
    }
  }
  else if (battery_state >= 25 && battery_state < 50)
  {
    unsigned long currenthealthtime = millis();

    if ((currenthealthtime - healthtime >= interval2) && (battery_state >= 25 && battery_state < 50)) {
      healthtime = currenthealthtime;

      battery_health = 2;
    }
  }
  else if (battery_state >= 5 && battery_state < 25)
  {
    unsigned long currenthealthtime = millis();

    if ((currenthealthtime - healthtime >= interval2) && (battery_state >= 5 && battery_state < 25)) {
      healthtime = currenthealthtime;

      battery_health = 1;
    }
  }
  else if (battery_state >= 0)
  {
    unsigned long currenthealthtime = millis();

    if ((currenthealthtime - healthtime >= interval2) && (battery_state >= 0)) {
      healthtime = currenthealthtime;

      battery_health = 0;
    }
  }
  else {
    battery_health = 0;

  }
}



byte buttoncheck()
{
  int i_butt = 0;
  byte buttonz = 0;
  if (digitalRead(BUTTON_PIN) != 1)
  {
    while (digitalRead(BUTTON_PIN) != 1)
    {
      delay(2);
      i_butt++;
    }
    buttonz = 1;
    Serial.print(" button short");
    digitalWrite(8, HIGH);
    delay(10);
    digitalWrite(8, LOW);

    if (i_butt > (longpresstime / 2))
    {
      buttonz += 1;
      Serial.print(" button long");
      digitalWrite(8, HIGH);
      delay(200);
    }

    if (i_butt > (powertime / 2))
    {
      buttonz += 2;
    }
  }
  else { }


  pressedbut = buttonz;
  return buttonz;
}

void modecheck () {

  if (pressedbut == 1) {
    const long buttontimer = millis();
    if (buttontimer - presstime > 1500) {
      buttoncount = 0;
      presstime = buttontimer;
    }
    else {
      buttoncount += 1;
    }

    if  (buttoncount == 2) {
      if (menuselect != 1) {
        if (powermode == 1) {
          Serial.print(" button power");
          digitalWrite(9, LOW); // shutdown
          clearOLED();
          powermode = 0;
        }
        else {
          digitalWrite(9, HIGH); // turn on
          powermode = 1;
        }
      }
      else {
      }
    }
  }
}




void loop() {

  u8g.firstPage();
  do {
    
    u8g.setFont(u8g_font_unifont);
    //u8g.setFont(u8g_font_osb21);
    u8g.setPrintPos(0, 30);
    u8g.print(voltage);

    u8g.setPrintPos(50, 30);
    u8g.print(buttoncount);


    modecheck();
    buttoncheck();
    voltagetest();

    alarms();

    if (battery_health == 0) {
      digitalWrite(13, HIGH);
    }
    else {
      digitalWrite(13, LOW);
    }

    if (battery_health == 4)
    {
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
    }
    else if (battery_health == 3)
    {
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
    }
    else if (battery_health == 2)
    {
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
    }
    else if (battery_health == 1)
    {
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
    }

    else if (battery_health == 0)
    {
      if (AlarmState == LOW)
      {
        digitalWrite(3, LOW);
      }
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
    }
    else {
    }


    if (powermode == 0) {
      unsigned long currentruntime = millis();
      if (currentruntime - runtime >= 3000) {
        runtime = currentruntime;

        shadowmode();
      }
    }

    if (pressedbut == 2)
    {
      menuselect = 1;
      menu();
    }

    Serial.println();

  } while ( u8g.nextPage() );

}



void clearOLED() {

  u8g.firstPage();
  do {
  } while ( u8g.nextPage() );

}


void shadowmode() {

  while (powermode == 0) {
    
    modecheck();
    buttoncheck();
    voltagetest();
    alarms();


    if (AlarmState == LOW) {
      digitalWrite(3, LOW);
    }
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);


    if (pressedbut == 2)
    {
      menuselect = 1;
      menu();
    }

    if (powermode == 1)
    {
      return;
    }

  }
}


void alarms() {

  if (battery_health == 0)
  {
    //Buzzer Stuff
    unsigned long currenttime = millis();


    if (currenttime - spendtime >= interval) {
      spendtime = currenttime;

      if (AlarmState == LOW) {
        AlarmState = HIGH;
      }
      else {
        AlarmState = LOW;
      }

      digitalWrite(8, AlarmState);
      digitalWrite(3, AlarmState);


    }
  }
  else {
    digitalWrite(8, LOW);
  }
}

void menu() {
  while (menuselect == 1) {

    Serial.print(alarmvalue);
    Serial.println();

    buttoncheck();
    digitalWrite(8, LOW);
    unsigned long currenttime = millis();

    if (currenttime - spendtime >= menuinterval) {
      spendtime = currenttime;

      if (MenuState == LOW)
        MenuState = HIGH;
      else
        MenuState = LOW;

      digitalWrite(3, MenuState);
    }

    if (pressedbut == 2)
    {

      EEPROM.write(alarmADDR, alarmvalueEEP);
      menuselect = 0;
      return;
    }


    if (alarmvalue == 3.00)
    {
      if (pressedbut == 1)
      { alarmvalue = 3.20;
        alarmvalueEEP = alarmvalue * 10.0;
      }
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);


    }
    else if (alarmvalue == 3.20)
    {
      if (pressedbut == 1)
      { alarmvalue = 3.40;
        alarmvalueEEP = alarmvalue * 10.0;
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);

    }
    else if (alarmvalue == 3.40)
    {
      if (pressedbut == 1) {
        alarmvalue = 3.60;
        alarmvalueEEP = alarmvalue * 10.0;
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);

    }
    else if (alarmvalue == 3.60)
    {
      if (pressedbut == 1) {
        alarmvalue = 3.00;
        alarmvalueEEP = alarmvalue * 10.0;
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);

    }
  }

}

