#include "Adafruit_HX711.h"

const uint8_t IR_SENSOR = 2;          
const uint8_t BOWL_ENA = 3;          
const uint8_t BOWL_IN1 = 8;          
const uint8_t BOWL_IN2 = 9;          
const uint8_t LOADCELL_DOUT_PIN = 4;  
const uint8_t LOADCELL_SCK_PIN = 5;   
const uint8_t WATER_ENA = 6;  
const uint8_t WATER_ENB = 7;         
const uint8_t WATER_IN1_IN3 = 12;  
const uint8_t WATER_IN2_IN4 = 13;

const uint8_t STEP_PIN = 10; 
const uint8_t DIR_PIN = 11; 


const uint8_t WATER_SENSOR = A0; 
const int FOOD_TARGET_WEIGHT = 100.0; 
const int FOOD_MIN_WEIGHT = 5.0;    
const int BOWL_SPEED = 200;            
const int WATER_SPEED = 200;           


float calibrationFactor = -7050.0;
int32_t tareOffset = 0;

Adafruit_HX711 hx711(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

float currentWeight = 0;
bool bowlExtended = false;
bool petPresent = false;
unsigned long lastFeedTime = 0;
const unsigned long FEED_COOLDOWN = 300000; // 5 min

void setup() {
  Serial.begin(9600);
  hx711.begin();
  delay(1000);
  
  int32_t tareSum = 0;
  for (int i = 0; i < 10; i++) {
    tareSum += hx711.readChannelRaw(CHAN_A_GAIN_128);
    delay(100);
  }
  tareOffset = tareSum / 10;
  
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(BOWL_ENA, OUTPUT);
  pinMode(BOWL_IN1, OUTPUT);
  pinMode(BOWL_IN2, OUTPUT);
  pinMode(WATER_ENA, OUTPUT);
  pinMode(WATER_ENB, OUTPUT);
  pinMode(WATER_IN1_IN3, OUTPUT);
  pinMode(WATER_IN2_IN4, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  pinMode(WATER_SENSOR, INPUT);
  
  stopBowlMotor();
  stopWaterPumps();
  
  delay(2000);
}

void loop() {
  petPresent = (digitalRead(IR_SENSOR) == HIGH);
  currentWeight = readWeight();
  
  if (petPresent && !bowlExtended) {
    if (millis() - lastFeedTime > FEED_COOLDOWN || lastFeedTime == 0) {
      
      if (currentWeight < FOOD_TARGET_WEIGHT) {
        dispenseFoodToTarget();
      }

      extendBowl();
      bowlExtended = true;
      lastFeedTime = millis();
    }
  }
  else if (bowlExtended && currentWeight <= FOOD_MIN_WEIGHT) {
    retractBowl();
    bowlExtended = false;
    
    int32_t tareSum = 0;
    for (int i = 0; i < 10; i++) {
      tareSum += hx711.readChannelRaw(CHAN_A_GAIN_128);
      delay(100);
    }
    tareOffset = tareSum / 10;
  }
  
  manageWaterSystem(); 
  delay(500);
}

float readWeight() {
  int32_t rawSum = 0;
  for (int i = 0; i < 10; i++) {
    rawSum += hx711.readChannelRaw(CHAN_A_GAIN_128);
    delay(10);
  }
  int32_t rawAverage = rawSum / 10;
  float weight = (rawAverage - tareOffset) / calibrationFactor;
  return weight;
}

void dispenseFoodToTarget() {
  digitalWrite(DIR_PIN, HIGH);
  
  while (currentWeight < FOOD_TARGET_WEIGHT) {
    for (int i = 0; i < 200; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(1000);
    }
    
    currentWeight = readWeight();
    
    if (currentWeight > FOOD_TARGET_WEIGHT + 20) {
      break;
    }
  }
}

void extendBowl() {
  analogWrite(BOWL_ENA, BOWL_SPEED);
  digitalWrite(BOWL_IN1, HIGH);
  digitalWrite(BOWL_IN2, LOW);
  delay(3000);
  stopBowlMotor();
}

void retractBowl() {
  analogWrite(BOWL_ENA, BOWL_SPEED);
  digitalWrite(BOWL_IN1, LOW);
  digitalWrite(BOWL_IN2, HIGH);
  delay(3000);
  stopBowlMotor();
}
void stopBowlMotor() {
  digitalWrite(BOWL_IN1, LOW);
  digitalWrite(BOWL_IN2, LOW);
  analogWrite(BOWL_ENA, 0);
}

void manageWaterSystem() {
  int waterLevel = digitalRead(WATER_SENSOR);

  if (waterLevel == LOW) {
    fillWater();
  } else {
    runFountain();
  }
}

void fillWater() {
  analogWrite(WATER_ENA, WATER_SPEED);
  analogWrite(WATER_ENB, 0);  
  digitalWrite(WATER_IN1_IN3, HIGH);
  digitalWrite(WATER_IN2_IN4, LOW);
  while (digitalRead(WATER_SENSOR) == LOW) {
    delay(100);
  }
  stopWaterPumps();
}

void runFountain() {
  analogWrite(WATER_ENA, 0);  
  analogWrite(WATER_ENB, WATER_SPEED * 0.7);
  digitalWrite(WATER_IN1_IN3, LOW);
  digitalWrite(WATER_IN2_IN4, HIGH);
  delay(2000);
  stopWaterPumps();
  delay(3000);
}

void stopWaterPumps() {
  digitalWrite(WATER_IN1_IN3, LOW);
  digitalWrite(WATER_IN2_IN4, LOW);
  analogWrite(WATER_ENA, 0);
  analogWrite(WATER_ENB, 0);
}