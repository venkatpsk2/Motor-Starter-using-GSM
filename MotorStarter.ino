#include <SIM800L.h>
#include <EEPROM.h>
#define relayPin1 5
#define relayPin2 6

SIM800L sim800l(2, 3);  //Rx, Tx

int motorstate = 0;

void handleSMS(String number, String message) {
  Serial.println("number: " + number + "\nMessage " + message);
  if(message=="10")
  {
    int motorstatus;
    EEPROM.get(0, motorstatus);
    if(motorstatus==0)
    {
      sim800l.sendSMS(number, "Motor OFF");
    }
    else
    {
      sim800l.sendSMS(number, "Motor on");
    }
  }
}

void handleCall(String number) {
  Serial.println("New call from " + number);
  if (number == "+917538870336" || number == "+918667696587") {
    int motorstatus;
    EEPROM.get(0, motorstatus);
    if (motorstatus== 0) {
      motoron();
      Serial.println("Motor started");
      sim800l.sendSMS(number, "Motor on");
    } 
    else if (motorstatus ==1) {
      motoroff();
      Serial.println("Motor stopped");
      sim800l.sendSMS(number, "Motor off");
    }
  }
}

void setup() {
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);
  sim800l.begin(9600);

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  sim800l.setSMSCallback(handleSMS);
  sim800l.setCallCallback(handleCall);
  delay(30000);

  int motorstatus;
  EEPROM.get(0, motorstatus);
  if(motorstatus==1)
  {
    motoron();
    sim800l.sendSMS("+917538870336", "Current on reason: Motor on");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sim800l.listen();
}

void motoron() {
  Serial.println("Motor ON");
  digitalWrite(relayPin1, LOW);
  delay(3000);
  digitalWrite(relayPin1, HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN, HIGH);
  motorstate=1;
  EEPROM.put(0, motorstate);
}
void motoroff() {
  Serial.println("Motor OFF");
  digitalWrite(relayPin2, LOW);
  delay(3000);
  digitalWrite(relayPin2, HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  motorstate = 0;
  EEPROM.put(0, motorstate);
}
