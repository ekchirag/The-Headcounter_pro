#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
#define LOX4_ADDRESS 0x33

int th=1000;
long startTime1 = 0; // Variable to store the start time
long endTime1 = 0;
long startTime2 = 0; // Variable to store the start time
long endTime2 = 0;
long resettime1=0;
long resettime2=0; 
int count =0; 
// set the pins to shutdown
#define SHT_LOX1 34
#define SHT_LOX2 33
#define SHT_LOX3 4
#define SHT_LOX4 2

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;
/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);


  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);
  if(!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while(1);
  }
  digitalWrite(SHT_LOX4, HIGH);
  // initing LOX4
  if(!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot fourth VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX4
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);
}

void read_dual_sensors() {
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false);
  lox4.rangingTest(&measure4, false);

  if (measure1.RangeMilliMeter<th || measure3.RangeMilliMeter<th ){// print sensor one reading
  Serial.print(F("1: "));
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("3: "));
  if(measure2.RangeStatus != 4) {
    Serial.print(measure3.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  }
  Serial.print(F(" "));
  Serial.println(F(" "));
if (measure2.RangeMilliMeter<th || measure4.RangeMilliMeter<th ){
  // print sensor three reading
  Serial.print(F("2: "));
  if(measure3.RangeStatus != 4) {     // if not out of range
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.print(F(" "));

   // print sensor four reading
  Serial.print(F("4: "));
  if(measure4.RangeStatus != 4) {
    Serial.print(measure4.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
}
  Serial.println(F(" "));

}
void Funky1 (int a,int b){
  if  (a<th){
    startTime1=millis();  
  }if (b<th){
    endTime1=millis();
  }
}
void Funky2 (int a,int b){
  if  (a<th){
    startTime2=millis();  
  }if (b<th){
    endTime2=millis();
  }
}
// to resolve error
void reset(int a,int b,int c,int d){
  if (a<th|| b<th || c<th || d<th){
    resettime1=millis();
  }
  else{
    resettime2=millis();
  }
  if ((resettime2-resettime1)>750){
    startTime1 = 0; 
    endTime1 = 0;
    startTime2 = 0; 
    endTime2 = 0;
    // Serial.print("reset");
  }
}
// count function
void counter(){
  Serial.print("                      ");
  long elapsedTime1 = endTime1 - startTime1;
  if(endTime1!=0 && startTime1 !=0){
  if (elapsedTime1>0){
    endTime1=0;
    startTime1=0;
    count++;
      // Serial.print("count");
      // Serial.print(count);

    // delay(10);
  }
  if(elapsedTime1<0){
    endTime1=0;
    startTime1=0;
    count--;
      // Serial.print("count");
      // Serial.print(count);

    // delay(10);
  }}  
    long elapsedTime2 = endTime2 - startTime2;
  if(endTime2!=0 && startTime2 !=0){
  if (elapsedTime2>0){
    endTime2=0;
    startTime2=0;
    count++;
      // Serial.print("count");
      // Serial.print(count);

    // delay(10);
  }
  if(elapsedTime2<0){
    endTime2=0;
    startTime2=0;
    count--;
      // Serial.print("count  :");
      // Serial.print(count);

    // delay(10);
  }}
  Serial.println("count");
  Serial.println("");
  Serial.print("                      ");
  Serial.print(count);
  
  Serial.println("");
}
void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();
 
}

void loop() {
   
  read_dual_sensors();
  Funky1(measure1.RangeMilliMeter,measure3.RangeMilliMeter);
  Funky2(measure2.RangeMilliMeter,measure4.RangeMilliMeter);
  counter();
  reset(measure1.RangeMilliMeter,measure3.RangeMilliMeter,measure2.RangeMilliMeter,measure4.RangeMilliMeter);
  delay(10);
}
