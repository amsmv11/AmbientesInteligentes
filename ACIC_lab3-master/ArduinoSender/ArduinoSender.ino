/*==========================================================================
                                ACIC Lab03
                              Filipe Reynaud
                              Joao Saramago
  ===========================================================================*/

/* For temperature sensor */
const int sensorTempPin = A0;
const float sensorOffset = 0;  // Offset after sensor calibration

/* For potentiometer */
int const potPin = A3;
int potVal;
const int maxPotRead = 1023;  // Maximum value read after calibration
const int minPotRead = 0; // Minimum value read after calibration

/* Phototransistor */
const int photoTransistorPin = A1;
int maxPhotoRead = 1003;  // Maximum value read after calibration
int minPhotoRead = 0; // Minimum value read after calibration

/* Time variables */
unsigned int long currentMillis;
unsigned int long previousMillis;
unsigned int blinkingInterval;

float temperature;
int intensity;


void setup() {
  Serial.begin(115200);
}


/*
   readTemperatureTask
   This function prints to the Serial Monitor the current temperature
   using a temperature sensor.
*/
void readTemperatureTask() {
  // Get sensor value
  int sensorVal = analogRead(sensorTempPin);

  // Get the voltage associated with sensorVal
  float voltage = (sensorVal / 1024.0) * 5.0;

  // Get the temperature associated with sensorVal
  temperature = (voltage - 0.5) * 100 - sensorOffset;
}


/*
   blinkLEDTask
   This function regulates the period at which the LED blinks with
   the use of a potentiometer.
*/
void blinkLEDTask() {
  potVal = analogRead(potPin);

  blinkingInterval = map(potVal, minPotRead, maxPotRead, 200, 2000);

}


/*
   intensityLEDTask
   This function regulates the intensity of the light emitted by the LED
   with a Phototransistor.
*/
void intensityLEDTask() {
  int readValue = analogRead(photoTransistorPin);
  int mapped = map(readValue, minPhotoRead, maxPhotoRead, 0, 255);
  intensity = 255 - mapped;

}


/*
   sendToSerialTask
   This function sends the data from the sensors to Serial Monitor.
*/
void sendToSerialTask() {
  Serial.print(temperature);
  Serial.print(" ");
  Serial.print(blinkingInterval);
  Serial.print(" ");
  Serial.println(intensity);
  //delay(1000);
}


/*
   Main loop with Round-Robin software architecture
*/
void loop() {

  blinkLEDTask();
  readTemperatureTask();
  intensityLEDTask();
  sendToSerialTask();

}
