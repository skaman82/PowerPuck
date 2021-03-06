/*
  Initial code for my PowerPuck project.
  Created by Albert Kravcov

*/
#include <EEPROM.h>

// uncomment this if you are usind an OLED screen
#define OLED

#define Voltagedetect          3.25 // Min. voltage for Detection
#define max_cellvoltage        4.25 // Max. cell voltage
#define R1                   17800  // Resistor 1
#define R2                    4700  // Resistor 2 (measured)
#define VOLT_SENS               A1  // Voltage Sensor
#define BUTTON_PIN               7  // Button
#define longpresstime         1000  // in ms
#define alarmADDR                1  // EEPROM Adress


#define NOTE_C4                 262  // C4
#define NOTE_A6                 1760 // A6

float voltage;
int32_t lipo;
float alarmvalue = 3.40;
int32_t alarmvalueEEP;
float cellvoltage;
int32_t battery_health = 4;
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
int32_t powermode = 1;
int32_t buttoncount = 0;
unsigned long presstime = 0;

unsigned long updatetime = 0;
int8_t counter = 0;
int32_t oldvoltage = 0;
int32_t samplevoltage = 0;


#ifdef OLED 
#include "U8glib.h"
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI
#endif



void setup() {

 
  analogReference(DEFAULT);

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
  delay (100);
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (100);
  digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (100);
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (250);
  digitalWrite(6, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (100);
  digitalWrite(5, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (100);
  digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (100);
  digitalWrite(3, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (250);

 
  voltagetest();
  lipo = 2;
  

#ifdef OLED 

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

  clearOLED();
  
  #endif
}




void voltagetest()
{

  int sensorValue = analogRead(VOLT_SENS); // read the input on analog pin 0:

    unsigned long currentrunthtime = millis();
    if (currentrunthtime - updatetime >= 100) {
      updatetime = currentrunthtime;
      counter += 1;
      samplevoltage += sensorValue; 
    }
    
    if (counter >= 10) {   // if we have 10 samples            
          oldvoltage = (samplevoltage / 10);
          samplevoltage = 0;
          counter = 0;
    }
    
  voltage = oldvoltage * (6.05 / 1023.0) * ((R1 + R2) / R2); // Convert the analog reading (which goes from 0 - 1023) to a voltage, considering the voltage divider:
  // use 6.00 for direct lipo and 6.05 for packs with bms and extension cable

  cellvoltage = voltage / 2;

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
  else if (battery_state >5 && battery_state < 25)
  {
    unsigned long currenthealthtime = millis();

    if ((currenthealthtime - healthtime >= interval2) && (battery_state >= 5 && battery_state < 25)) {
      healthtime = currenthealthtime;

      battery_health = 1;
    }
  }
  else if (battery_state >= 5)
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
          tone(8, NOTE_A6, 200);
          delay(500);
          tone(8, NOTE_C4, 500);
          
          #ifdef OLED 
          clearOLED();
          #endif
          
          powermode = 0;
        }
        else {
          digitalWrite(9, HIGH); // turn on
          delay(500);
          digitalWrite(8, HIGH);
          delay(100);
          digitalWrite(8, LOW);
          powermode = 1;
        }
      }
      else {
      }
    }
  }
}




void loop() {

#ifdef OLED 
    u8g.firstPage();
    do {
    
       u8g.setFont(u8g_font_helvR14r);
       u8g.setPrintPos(5, 26);
       u8g.print(voltage);
       u8g.print("v");

      
       u8g.drawFrame(59,9,64,22);
       u8g.drawBox(122,16,4,8);

       if (battery_health == 4) {
       u8g.drawBox(62,12,13,16); //25%
       u8g.drawBox(77,12,13,16); //50%
       u8g.drawBox(92,12,13,16); //75%
       u8g.drawBox(107,12,13,16); //100%
       }
       else if (battery_health == 3) {
       u8g.drawBox(62,12,13,16); //25%
       u8g.drawBox(77,12,13,16); //50%
       u8g.drawBox(92,12,13,16); //75%
       }
        else if (battery_health == 2) {
       u8g.drawBox(62,12,13,16); //25%
       u8g.drawBox(77,12,13,16); //50%

       }
        else if (battery_health == 1) {
       u8g.drawBox(62,12,13,16); //25%

       }
       else if (battery_health == 0) {

       
       u8g.setFont(u8g_font_lucasfont_alternater);
       u8g.setPrintPos(70, 23);
       u8g.print("LOW BAT.");
       
       }
       
#endif

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
      AlarmState = LOW;
    }
    else if (battery_health == 3)
    {
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      AlarmState = LOW;
    }
    else if (battery_health == 2)
    {
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      AlarmState = LOW;
    }
    else if (battery_health == 1)
    {
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      AlarmState = LOW;
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

 #ifdef OLED 
 } while ( u8g.nextPage() );
#endif
}


#ifdef OLED 
  void clearOLED() {

    u8g.firstPage();
    do {
    } while ( u8g.nextPage() );

  }
#endif

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
      #ifdef OLED 
      clearOLED();
      #endif
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

    // Serial.print(alarmvalue);
    // Serial.println();
 #ifdef OLED 
  
  u8g.firstPage();
    do
        {
        u8g.setFont(u8g_font_lucasfont_alternater);
        u8g.setPrintPos(5, 17);
        u8g.print("ALARM");
        u8g.setPrintPos(5, 28);
        u8g.print("SETTING");
        
        u8g.setFont(u8g_font_helvR14r);
        u8g.setPrintPos(58, 26);
        u8g.print("> ");
        u8g.print(alarmvalue);
        u8g.print("v");
        }
        while (u8g.nextPage());
 #endif
    
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

