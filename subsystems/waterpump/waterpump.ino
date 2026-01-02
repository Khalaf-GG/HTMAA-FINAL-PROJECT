const int WATER_ENA = 6;
const int WATER_ENB = 7;
const int WATER_IN1 = 8;
const int WATER_IN2 = 9;
const int WATER_IN3 = 10;
const int WATER_IN4 = 11,
const int WATER_SENSOR = A0;

int WATER_SPEED = 200;

void setup() {
  pinMode(WATER_ENA, OUTPUT);
  pinMode(WATER_ENB, OUTPUT);
  pinMode(WATER_IN1, OUTPUT);
  pinMode(WATER_IN2, OUTPUT);
  pinMode(WATER_IN3, OUTPUT);
  pinMode(WATER_IN4, OUTPUT);
  pinMode(WATER_SENSOR, INPUT);
  
  stopWaterPumps();
}

void loop() {
  manageWaterSystem();
  delay(500);
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
  digitalWrite(WATER_IN1, HIGH);
  digitalWrite(WATER_IN2, LOW);
  digitalWrite(WATER_IN3, LOW);
  digitalWrite(WATER_IN4, LOW);
  
  while (digitalRead(WATER_SENSOR) == LOW) {
    delay(100);
  }
  
  stopWaterPumps();
}

void runFountain() {
  analogWrite(WATER_ENA, 0);
  analogWrite(WATER_ENB, WATER_SPEED * 0.7);
  digitalWrite(WATER_IN1, LOW);
  digitalWrite(WATER_IN2, LOW);
  digitalWrite(WATER_IN3, LOW);
  digitalWrite(WATER_IN4, HIGH);
  
  delay(2000);
  stopWaterPumps();
  delay(3000);
}

void stopWaterPumps() {
  digitalWrite(WATER_IN1, LOW);
  digitalWrite(WATER_IN2, LOW);
  digitalWrite(WATER_IN3, LOW);
  digitalWrite(WATER_IN4, LOW);
  analogWrite(WATER_ENA, 0);
  analogWrite(WATER_ENB, 0);
}