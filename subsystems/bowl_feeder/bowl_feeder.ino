#include "Adafruit_HX711.h"

const uint8_t IR_SENSOR 2

const uint8_t BOWL_ENA 3
const uint8_t BOWL_IN1 8
const uint8_t BOWL_IN2 9

const uint8_t LOADCELL_DOUT_PIN 4
const uint8_t LOADCELL_SCK_PIN 5

const int FOOD_TARGET_WEIGHT 100.0
const int FOOD_MIN_WEIGHT 5.0
const int BOWL_SPEED 200

float calibrationFactor = -7050.0;
int32_t tareOffset = 0;

Adafruit_HX711 hx711(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

float currentWeight = 0;
bool bowlExtended = false;
bool petPresent = false;

void setup() {
  hx711.begin();
  delay(1000);
  
  int32_t tareSum = 0;
  for (int i = 0; i < 10; i++) {
    tareSum += hx711.readChannelRaw(CHAN_A_GAIN_128);
    delay(100);
  }
  tareOffset = tareSum / 10;
  
  pinMode(BOWL_ENA, OUTPUT);
  pinMode(BOWL_IN1, OUTPUT);
  pinMode(BOWL_IN2, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  
  stopBowlMotor();
}

void loop() {
  petPresent = (digitalRead(IR_SENSOR) == HIGH);
  currentWeight = readWeight();
  
  if (petPresent && !bowlExtended) {
    extendBowl();
    bowlExtended = true;
  }
  else if (bowlExtended && currentWeight <= FOOD_MIN_WEIGHT) {
    retractBowl();
    bowlExtended = false;
    retareScale();
  }
  
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

void retareScale() {
  int32_t tareSum = 0;
  for (int i = 0; i < 10; i++) {
    tareSum += hx711.readChannelRaw(CHAN_A_GAIN_128);
    delay(100);
  }
  tareOffset = tareSum / 10;
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
