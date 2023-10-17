#include <SIM800L.h>
#include <EEPROM.h>
#define relayPin1 5
#define relayPin2 6
#define ledpinoff 9
#define ledpinon 12

SIM800L sim800l(2, 3);     //Rx, Tx

int motorstate = 0;

void handleSMS(String number, String message) {
  // Serial.println("number: " + number + "\nMessage " + message);
  if (message == "10") {
    int motorstatus;
    EEPROM.get(0, motorstatus);
    if (motorstatus == 0) {
      sim800l.sendSMS(number, "Motor OFF");
    } else {
      sim800l.sendSMS(number, "Motor on");
    }
  }
}

void handleCall(String number) {
  // Serial.println("New call from " + number);
  if (number == "+917538870336" || number == "+918667696587" || number == "+919994008886") {
    int motorstatus;
    EEPROM.get(0, motorstatus);
    if (motorstatus == 0) {
      delay(10000);
      sim800l.tryATcommand("ATH");
      motoron();
      // Serial.println("Motor started");
      sim800l.sendSMS(number, "Motor on");
    } else if (motorstatus == 1) {
      delay(4500);
      sim800l.tryATcommand("ATH");
      motoroff();
      // Serial.println("Motor stopped");
      sim800l.sendSMS(number, "Motor off");
    }
  } else {
    delay(4000);  
    String caller = number;
    sim800l.tryATcommand("ATH");  
    delay(2000);  
    sim800l.sendSMS("+918667696587", "this number calling you:" + caller);
  }
}

void setup() {
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);

  pinMode(ledpinon, OUTPUT);
  pinMode(ledpinoff, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  // Serial.begin(9600);
  delay(60000);
  sim800l.begin(9600);

  sim800l.tryATcommand("AT+CMGD=1,4");
  delay(2000);

  sim800l.setSMSCallback(handleSMS);
  sim800l.setCallCallback(handleCall);

  EEPROM.get(0, motorstate);
  if (motorstate == 1) {
    motoron();
    delay(2000);
    sim800l.sendSMS("+918667696587", "Current on reason: Motor on");
  }
}

void loop() {
  sim800l.listen();
  if (motorstate == 0) {
    digitalWrite(ledpinoff, HIGH);
  }
  if (motorstate == 1) {
    digitalWrite(ledpinon, HIGH);
    delay(700);
    digitalWrite(ledpinon, LOW);
    delay(700);
  } else {
    digitalWrite(ledpinoff, LOW);
  }
}

void motoron() {
  // Serial.println("Motor ON");
  digitalWrite(relayPin1, LOW);
  delay(3000);
  digitalWrite(relayPin1, HIGH);
  delay(3000);

  motorstate = 1;
  EEPROM.put(0, motorstate);
}
void motoroff() {
  // Serial.println("Motor OFF");
  digitalWrite(relayPin2, LOW);
  delay(3000);
  digitalWrite(relayPin2, HIGH);
  delay(3000);
  motorstate = 0;
  EEPROM.put(0, motorstate);
}
