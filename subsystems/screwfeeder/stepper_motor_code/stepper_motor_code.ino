  const int step_pin = 10;
  const int dir_pin = 11;

  const int steps_for_100g = 500; // total step to reach 10 seconds at 20ms delay
  const int speed_delay = 20; // speed in miliseconds 


void setup() {
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  digitalWrite(dir_pin, LOW);


  for(int i = 0; i < steps_for_100g; i++) { 
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(step_pin, LOW);
    delay(speed_delay);
  }

  delay(1000);


  digitalWrite(dir_pin, HIGH);

  for(int i = 0; i < steps_for_100g; i++) { 
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(step_pin, LOW);
    delay(speed_delay);
  }

  

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  delay(10000);
}
