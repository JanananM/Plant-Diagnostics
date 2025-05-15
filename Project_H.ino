#include <LiquidCrystal.h>
#include "DHT.h"
#include "pitches.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define RED 3

#define ENABLE 5
#define A 4
#define B 6

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DHT dht(DHTPIN, DHTTYPE);

/*
problems
led does not respond in setup
place the fan in*/
void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);

  /*int i;
  for (i = 0; i > 4; i++) {
    digitalWrite(RED, HIGH);
    delay(500);
    digitalWrite(RED, LOW);
    delay(500);
  }*/
  lcd.begin(16, 2);
  dht.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Thermostat Labs");
  delay(4000);
  lcd.clear();
  lcd.print("1.2.2018 Edition");
  delay(4000);

  pinMode(ENABLE,OUTPUT);
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RED, LOW);

  int currtime = millis();
  int setmain = 60000;

  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(f)) {
    lcd.setCursor(0, 0);
    lcd.print("ERROR");
    tone(13, NOTE_C5, 1000);
    digitalWrite(RED, HIGH);
    delay(1000);
    return;
  }

  float c = (5.00 / 9.00) * (f - 32.00);


  if (c >= 30.00) {
    digitalWrite(RED, HIGH);
    delay(1000);
    digitalWrite(RED, LOW);
    delay(1000);
    tone(13, NOTE_C7, 1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RM Temp. above  ");
    lcd.setCursor(0, 1);
    lcd.print("30*C. Cool Down.");
    delay(4000);

  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp          *C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity       %");
  lcd.setCursor(9, 0);
  lcd.print(c);
  lcd.setCursor(10, 1);
  lcd.print(h);

  delay(4000);

  fan(c,h);
  systemcool(currtime,setmain);

  if (c >= 20.00) {
    float netres = c - 20.00;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current RM Temp.");
    lcd.setCursor(0, 1);
    lcd.print("Diff. is:       ");
    lcd.setCursor(12, 1);
    lcd.print(netres);
    delay(4000);
  }

}

void fan(float c,float h){
  if(c >= 30.00){
    lcd.clear();
    lcd.print("Fan: ON");
    delay(1000);
    digitalWrite(ENABLE,HIGH);
    digitalWrite(A,HIGH);
    digitalWrite(B,LOW);
    if(c < 30.00){
      lcd.clear();
      lcd.print("Fan: OFF");
      delay(1000);
      int j;
      for(j = 255; j <= 0;j--){
        analogWrite(ENABLE,j);
        delay(200);
      }
    }
    
  }else if(h >= 70.00){
    lcd.clear();
    lcd.print("Fan: ON");
    delay(1000);
    digitalWrite(ENABLE,HIGH);
    digitalWrite(A,HIGH);
    digitalWrite(B,LOW);
    if(h < 70.00){
      lcd.clear();
      lcd.print("Fan: OFF");
      delay(1000);
      int k;
      for(k = 255; k <= 0;k--){
        analogWrite(ENABLE,k);
        delay(200);
      }
    }
  }else{
    lcd.clear();
    lcd.print("Fan: OFF");
    delay(1000);
    digitalWrite(ENABLE,LOW);
    delay(4000);
  }
}

void systemcool(int setmain,int currtime){
  if(currtime == setmain){
    digitalWrite(ENABLE,HIGH);
    digitalWrite(A,LOW);
    digitalWrite(B,HIGH);
    delay(10000);
    digitalWrite(B,LOW);
    delay(200);
    setmain += 60000;
  }
  
  
  
}

