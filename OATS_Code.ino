/*
 * TO DO:
 *      1) Warning/error lights & documentation
 *      2) Data collection duration (Sort of? Just turn it off.... ajustable duration BACKLOG)
 *      3) Ajustable sample rate  (BACKLOG)
 *      4) Output of csv file for excel analysis
 *      
 *      PINS
 *         SD card attached to SPI bus as follows:
 *         MOSI - pin 11
 *         MISO - pin 12
 *         CLK - pin 13
 *         CS - pin 4
 *      Feedback Lights   
 *         Green - pin 3
 *         Yellow - pin 9
 *         Orange - pin 5
 *         Red - pin 6
 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
int i =1;
//Pin varables
const int GREEN = 3;
const int YELLOW = 9;
const int ORANGE = 5;
const int RED = 6;
const int LONG_DELAY = 60000;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1){
      digitalWrite(RED,HIGH);
    }
  }
  Serial.println("card initialized.");
  Serial.println("Starting Datalog!");
  
}

void loop() {
  for(int count=0;count<60;count++){
    delay(1000);
    Serial.println(count);
  }
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  int sensor = analogRead(A0);
  float reading;
  reading = sensor*(3300/1024);
  reading = (reading-215.0)/10;
    
  dataString += "Min: ";
  dataString += String(i);
  dataString += ",";
  dataString += " Deg C: ";
  dataString += String(reading);
  dataString += ",";

  i++;
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    while(true){
      digitalWrite(ORANGE,HIGH);
    }
  }
}


/*                          //OLD CODE
// include the SD library:
#include <SPI.h>
#include <SD.h>

//Pin for SD card reader/writer
const int chipSelect = 4;

//Pin varables
const int GREEN = 3;
const int YELLOW = 9;
const int ORANGE = 5;
const int RED = 6;
const int LONG_DELAY = 600;
const int TEMP = A0;

//Other varables
int brightness =0;
int fadeAmount =5;
int sensor = -1;
float deg_C = 0.0;
int i,j,k;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

void setup() {
  //Start serial communication and wait for it to connect
  Serial.begin(9600);

  //Define PinMode
//OUTPUT
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(RED, OUTPUT);
//INPUT
  pinMode(TEMP, INPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //Show stuff
  Serial.print("\nInitializing SD card...");

  //Is the SD card good?
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1){
      digitalWrite(RED,HIGH);
      delay(250);
      digitalWrite(RED,LOW);
      delay(250);
    }
    
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // Is the partition good? - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1){
      digitalWrite(YELLOW,HIGH);
      delay(1000);
      digitalWrite(YELLOW,LOW);
      delay(250);;
    }
  }
  i=0;
  while(i<20){
    // set the brightness of pin 9:
    analogWrite(ORANGE, brightness);

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;
    
    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
      i++;  //Add one to counter
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
  j=1;
  i=1;
    
  //Open the file. ONLY 1 FILE CAN BE OPEN AT A TIME! Close when done.
  File dataFile = SD.open("TWAT_reading.txt", FILE_WRITE);
  
} //End setup

//-----------------------------------------------------------------------------------------

//Varables
bool EVEN;
String data_LN = "";

void loop() {
  data_LN = "";
  delay(LONG_DELAY);
  
//Read the sensor
  sensor = analogRead(TEMP);
  delay(50);  //Wait for results

//Translate from reading to temprature
  sensor = sensor*(3300/1024);
  deg_C = (sensor-215.0)/10;

//Make a data line to write to file/serial
  data_LN +="Min,";
  data_LN +=String(i);
  data_LN+=",";
  data_LN+="Temp,";
  data_LN+=String(deg_C);
  data_LN+=",";
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(data_LN);
    dataFile.close();
    // print to the serial port too:
    Serial.println(data_LN);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }
  i++;
}
*/
