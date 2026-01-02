
const uint8_t IR_PIN = 4;
const uint8_t MOTOR_IN1 = 5;
const uint8_t MOTOR_IN2 = 6;
const uint8_t MOTOR_ENA = 9;

const int rollTime = 3000;
const int motorSpeed = 200;


bool bowlIsOut = false;

void rollBowlOut() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_ENA, motorSpeed);
  delay(rollTime);
  stopMotor();
  bowlIsOut = true;
}

void rollBowlIn() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  analogWrite(MOTOR_ENA, 150);
  delay(rollTime);
  stopMotor();
  bowlIsOut = false;
}

void stopMotor() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_ENA, 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  stopMotor();
}

void loop() {
  bool irDetected = (digitalRead(IR_PIN) == LOW); 
  if (irDetected && !bowlIsOut) {
    rollBowlOut();
  }
  if (!irDetected && bowlIsOut) {
    delay(2000); 
    rollBowlIn();
  }
  delay(500);
}