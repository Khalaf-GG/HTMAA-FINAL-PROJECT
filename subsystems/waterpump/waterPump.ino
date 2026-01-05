const int PUMP_A_ENA = 6;
const int PUMP_A_IN1 = 8;
const int PUMP_A_IN2 = 9;

const int PUMP_B_ENB= 7;
const int PUMP_B_IN3 = 10;
const int PUMP_B_IN4 = 11;

const int WATER_SENSOR = A0;



int WATER_SPEED = 200;
int FOUNTAINSPEED = WATER_SPEED * 0.7;

void stopWaterPumps() {

  analogWrite(PUMP_A_IN1, LOW);
  analogWrite(PUMP_A_IN2, LOW);

  analogWrite(PUMP_B_IN3, LOW);
  analogWrite(PUMP_B_IN4, LOW);

  analogWrite(PUMP_A_ENA, 0);
  analogWrite(PUMP_B_ENB, 0);

}

void waterSystem() {

  int waterLevel = digitalRead(WATER_SENSOR);

  if (waterLevel == LOW) {
    fillWater();
  }else {
    runFountain();
  }
}

void fillWater() {
  analogWrite(PUMP_A_ENA, WATER_SPEED);
  analogWrite(PUMP_B_ENB, 0);

  digitalWrite(PUMP_A_IN1, HIGH);
  digitalWrite(PUMP_A_IN2, LOW);
  digitalWrite(PUMP_B_IN3, LOW);
  digitalWrite(PUMP_B_IN4, LOW);

  while (digitalRead(WATER_SENSOR) == LOW) {
    delay(100);
  }

  stopWaterPumps();
}

void runFountain() {

  analogWrite(PUMP_A_ENA, 0);
  analogWrite(PUMP_B_ENB, WATER_SPEED);

  digitalWrite(PUMP_A_IN1, LOW);
  digitalWrite(PUMP_A_IN2, LOW);
  digitalWrite(PUMP_B_IN3, LOW);
  digitalWrite(PUMP_B_IN4, HIGH);

  delay(2000);
  stopWaterPumps();
  delay(3000);

}

void setup() {

  pinMode(PUMP_A_ENA, OUTPUT);
  pinMode(PUMP_A_IN1, OUTPUT);
  pinMode(PUMP_A_IN2, OUTPUT);

  pinMode(PUMP_B_ENB, OUTPUT);
  pinMode(PUMP_B_IN3, OUTPUT);
  pinMode(PUMP_B_IN4, OUTPUT);

  pinMode(WATER_SENSOR, INPUT);

  stopWaterPumps();
}

void loop() {
 
  waterSystem();
  delay(500);

}
