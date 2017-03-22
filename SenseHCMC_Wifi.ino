#include "UbidotsMicroESP8266.h"
#define TOKEN  "XTkUpLeBjnCkDcKs9Xq6Jg4t02bTI1"  // Put here your Ubidots TOKEN
#define VARIABLE_ID "58cffabe7625427aeab97082" // Put her your varibale ID
#define WIFISSID "FabPrivate"   // Wireless SSID
#define PASSWORD "11235813vip"    // Wireless Password

#define SAMPLING_TIME 280
#define DELTA_TIME 40
#define SLEEP_TIME 9680
#define VOLTAGE_REF 3.3

Ubidots client(TOKEN);

const uint8_t measurePin = A0;
const uint8_t ledPower = 2;   // Pin D4 on NodeMCU

void setup() {
  Serial.begin(115200);
  delay(10);
  client.wifiConnection(WIFISSID, PASSWORD);  // connect to wireless network

  pinMode(ledPower, OUTPUT);
}
void loop() {
  float voltageMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;

  // Read dust sensor value
  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(SAMPLING_TIME);

  voltageMeasured = analogRead(measurePin); // read dust density value

  delayMicroseconds(DELTA_TIME);
  digitalWrite(ledPower, HIGH); // power off the LED
  delayMicroseconds(SLEEP_TIME);

  calcVoltage = voltageMeasured * (VOLTAGE_REF / 1024); // mapped 3.3V scale to 0-1023
  // calibration http://www.howmuchsnow.com/arduino/airquality/
  if(calcVoltage >= 0.59) {
    dustDensity = 0.17 * calcVoltage - 0.1;
  } else {
    dustDensity = 0;
  }

  // print out dust values
  Serial.print("Raw value (0-1023): ");
  Serial.print(voltageMeasured);
  Serial.print(" - Voltage (0-3.3V): ");
  Serial.print(calcVoltage);
  Serial.print(" - Dust Density (0-0.8mg/m3): ");
  Serial.println(dustDensity);

  // post data to Ubidots
  //client.add(VARIABLE_ID, dustDensity);
  //client.sendAll(true);

  delay(10000); // delay 10 seconds
}
