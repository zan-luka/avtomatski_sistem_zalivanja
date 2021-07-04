#include <DHT.h>

#define SOIL A5
#define PUMP 4
#define TEMPERATURE 5
#define WATER0 A0
#define WATER1 A1
#define WATER2 A2
#define WATER3 A3
#define BUTTON 2
#define BUZZ 6
#define LED0 7
#define LED1 8
#define LED2 9
#define LED3 10
#define DHTTYPE DHT11 // temperature / humidity sensor

DHT dht(TEMPERATURE, DHTTYPE);

float temp;
float hum;
float soil;
long water0, water1, water2, water3;
long current, debounce = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SOIL, INPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(WATER0, INPUT);
  pinMode(WATER1, INPUT);
  pinMode(WATER2, INPUT);
  pinMode(WATER3, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON, INPUT);
  dht.begin();
}

void waterLevel() {
  water0, water1, water2, water3 = 0;
    for (int i=0; i<100; i++) {
      water0 = water0 + analogRead(WATER0);
      delay(10);
    }

    for (int i=0; i<100; i++) {
      water1 = water1 + analogRead(WATER1);
      delay(10);
    }

    for (int i=0; i<100; i++) {
      water2 = water2 + analogRead(WATER2);
      delay(10);
    }

    for (int i=0; i<100; i++) {
      water3 = water3 + analogRead(WATER3);
      delay(10);
    }
    
    water0 = water0 / 100;
    water1 = water1 / 100;
    water2 = water2 / 100;
    water3 = water3 / 100;
}

void waterLevelDisplay() {
  if (water3 >= 870) {
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
    }
    else if (water2 >= 870) {
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
    }
    else if (water1 >= 870) {
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);
    }
    else if (water0 >= 870) {
      digitalWrite(LED0, HIGH);
    }
    else {
      for (int i=0; i<8; i++) {
          tone(BUZZ, 1000);
          digitalWrite(LED0, HIGH);
          delay(750);
          noTone(BUZZ);
          digitalWrite(LED0, LOW);
          delay(750);
        }
    }
}

void ledOff() {
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void measureSoil() {
  while (true) {
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    
    soil = 0.0;
    for (int i=0; i<100; i++) {
      soil = soil + analogRead(SOIL);
      delay(1);
    }
    soil = soil/100.0;
  
    if (temp > 6.0 && soil >= 712) {
      if (water0 >= 870) {
        digitalWrite(PUMP, HIGH);
      }
      else {
        break;
      }
      Serial.print("OK");
      delay(5000);
    }
    else {
      digitalWrite(PUMP, LOW);
      break;
    }
  }
}

void printData() {
  Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

    
    Serial.print("\nSoil: ");
    Serial.print(soil);
    
    Serial.print("\nWater: ");
    Serial.print(water0);
    Serial.print("\n");
}

void loop() {
  waterLevel();
  waterLevelDisplay();
  delay(5000);
  ledOff();
  measureSoil();
  delay(60000);
}
