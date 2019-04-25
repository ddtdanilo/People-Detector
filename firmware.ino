#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 32
#define SHT_LOX2 33

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

bool detection1 = false;
bool detection2 = false;

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println("Shutdown pins inited...");

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  Serial.println("Both in reset mode...(pins are low)");


  Serial.println("Starting...");
  setID();

}

void loop() {
  read_dual_sensors();

}

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }
}

void read_dual_sensors() {

  int dist1 = 0;
  int dist2 = 0;

  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  /*Serial.print("1: ");

    if(measure1.RangeStatus != 4) {     // if not out of range
    dist1 = measure1.RangeMilliMeter;
    Serial.print(dist1);
    } else {
    Serial.print(dist1);
    //dist1 = -1;
    }

    Serial.print(" ");

    // print sensor two reading
    Serial.print("2: ");
    if(measure2.RangeStatus != 4) {
    dist2 = measure2.RangeMilliMeter;
    Serial.print(dist2);
    } else {
    Serial.print(dist2);
    }
    Serial.println();
  */

  dist1 = measure1.RangeMilliMeter;
  dist2 = measure2.RangeMilliMeter;


  if (dist1 < 800 && dist1 > 0) {
    detection1 = true;
    Serial.println("detection1 = true");
    if (detection2) {
      Serial.println("************Salida!");
      delay(2000);
      detection1 = false;
      detection2 = false;
      Serial.println("FALSE");
      dist2 = 1000;
    }
  }


  if (dist2 < 800 && dist2 > 0) {
    detection2 = true;
    Serial.println("detection2 = true");
    if (detection1) {
      Serial.println("************Entrada!");
      delay(2000);
      detection1 = false;
      detection2 = false;
      Serial.println("FALSE");
      dist1 = measure1.RangeMilliMeter;
      dist2 = measure2.RangeMilliMeter;
    }
  }
}
