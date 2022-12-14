//CPE 301 Final Project
//Owen Boxx

#include <DHT.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <TimeLib.h>

#define DHTPIN 22     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

volatile unsigned char *pinB =     (unsigned char *) 0x23;
volatile unsigned char *portDDRB = (unsigned char *) 0x24;
volatile unsigned char *portB =    (unsigned char *) 0x25;
volatile unsigned char *pinH =     (unsigned char *) 0x100;
volatile unsigned char *portDDRH = (unsigned char *) 0x101;
volatile unsigned char *portH =    (unsigned char *) 0x102;
volatile unsigned char *pinE =     (unsigned char *) 0x2C;
volatile unsigned char *portDDRE = (unsigned char *) 0x2D;
volatile unsigned char *portE =    (unsigned char *) 0x2E;

Servo myservo;
LiquidCrystal lcd(8,9,4,5,6,7);



/*
----States----
[Disabled - 0] [Idle - 1] [Running - 2] [Error - 3]
*/

DHT dht(22,DHT11);
int VRy = A1;
int ventA = 0;
int yPos = 0;
int mapY = 0;
int fan = A7;

int state = 0;

int button;
int buttonStore;
long debounce = 200;
long time = 0;

int waterLevel = 0;
int waterThresh = 200;
int respin = A5;

int temp = 0;
int tempThresh = 75;

void displayLCD(float h, float t, int s);
void leds(int s);
void buttonPress();

void setup()
{
    Serial.begin(9600);
    dht.begin();
    lcd.begin(16,2);

    myservo.attach(3);

    pinMode(VRy,INPUT);
    pinMode(fan,OUTPUT);
}

void loop(){
  //tests
  Serial.print("State: ");
  Serial.println(state);
  
  //update all values before switch statement
  delay(2000);
  time_t t = now();
  waterLevel = analogRead(respin);
  float temp = dht.readTemperature(true);
  float hum = dht.readHumidity();

  Serial.println(temp);
  Serial.println(hum);
  Serial.println(waterLevel);
  
  leds(state);
  buttonPress();


  switch(state){
    case 0: //Disabled
      break;
    case 1: //Idle
      displayLCD(hum,temp,state);

      if(waterLevel <= waterThresh){
        Serial.println("The water level is low.");
        state = 3;
      }else if(temp > tempThresh){
        Serial.println("The temperature is above the threshold.");
        state = 2;
      }
      break;

    case 2: //Running
      displayLCD(hum,temp,state);

      digitalWrite(fan, HIGH);
      Serial.write("Fan turned on at :");
      Serial.write(day());
      Serial.write("d");
      Serial.write(hour());
      Serial.write("h");
      Serial.write(minute());
      Serial.write("m");
      Serial.write(second());
      Serial.write("s Into Runtime.");
      Serial.write('\n');

      if(waterLevel < waterThresh){
        digitalWrite(fan, LOW); //DO THIS WITHOUT DWRITE 
        Serial.write("Fan turned off at :");
        Serial.write(day(t));
        Serial.write("d");
        Serial.write(hour(t));
        Serial.write("h");
        Serial.write(minute(t));
        Serial.write("m");
        Serial.write(second(t));
        Serial.write("s Into Runtime.");
        Serial.write('\n');

        Serial.println("The water level is low.");
        state = 3;
      }else if(temp <= tempThresh){
        digitalWrite(fan, LOW); //DO THIS WITHOUT DWRITE 
        Serial.write("Fan turned off at :");
        Serial.write(day(t));
        Serial.write("d");
        Serial.write(hour(t));
        Serial.write("h");
        Serial.write(minute(t));
        Serial.write("m");
        Serial.write(second(t));
        Serial.write("s Into Runtime.");
        Serial.write('\n');

        Serial.println("The temperature is below the threshold.");
        state = 1;
      }
      break;

    case 3: //Error
      displayLCD(hum,temp,state);
      break;

    default:
      state = 1;
      break;
  }
  yPos = analogRead(VRy);
  mapY = -1 * map(yPos, 0, 1023,-512,512);
  if(mapY >= 100 && ventA < 90){
    ventA = ventA + 15;
  }else if(mapY <= -100 && ventA > 0){
    ventA = ventA -15;
  }else{
    switch(ventA){
      case 90:
        Serial.println("Vent Angle: 90");
        break;
      case 0:
        Serial.println("Vent Angle: 0");
        break;
      default:
        Serial.println("Vent is open");
        break;
    }
  }
  myservo.write(ventA);
}

void displayLCD(float h, float t, int s){
  lcd.clear();
  if(s == 1){
    lcd.setCursor(0,0);
    lcd.print("IDLE");
  }
  if(s == 3){
    lcd.setCursor(0,0);
    lcd.print("Error:     ");
    lcd.setCursor(0,1);
    lcd.print("Water Level Low");
    return;
  }
  lcd.setCursor(0,0);
  lcd.print("HUMID: ");
  lcd.print('\t' + h);
  lcd.setCursor(0,1);
  lcd.print("TEMP: ");
  lcd.print('\t' + t);
  return;
}

void leds(int s){
  switch(s){
    case 0:
      //Disabled
      *portB |= 0b00010000;
      *portB &= 0b11101111;
      break;
    case 1:
      //Idle
      *portB |= 0b00100000;
      *portB &= 0b11011111;
      break;
    case 2:
      //Running
      *portB |= 0b01000000;
      *portB &= 0b10111111;
      break;
    case 3:
      //Error
      *portB |= 0b10000000;
      *portB &= 0b01111111;
      break;
    default:
      return;
      break;
  }
}

void buttonPress(){
  button = digitalRead(2);
  if(button == HIGH && buttonStore == LOW && millis()-time > debounce){
    if(state){
      state = 0;
    }else{
      state = 1;
    }
    time = millis();
  }
  buttonStore = button;
}

