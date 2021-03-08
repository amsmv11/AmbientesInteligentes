/*==========================================================================
                                ACIC Lab03
                              Filipe Reynaud
                              Joao Saramago
  ===========================================================================*/

/* For temperature task */
const float baselineTemp =  30.0;
const int ledTempPin = 4; // LED related to temperature sensor
const int ledTempTesterPin = A4; // analogue pin that tests the led circuit voltage
bool ledTempLastHealth = true; // health reported if the led is off
float temperature;

/* For blink LED task */
int blinkingInterval = 1000;
const int ledPotPin = 2;  // LED related to potentiometer
const int ledPotTesterPin = A2; // analogue pin that tests the led circuit voltage
bool ledPotLastHealth = true; // health reported if the led is off


/* For LED intensity task */
const int ledPhotoPin = 3; // LED related to Light Sensor
const int ledPhotoTesterPin = A3; // analogue pin that tests the led circuit voltage
bool ledPhotoLastHealth = true; // health reported if the led is off
int intensity;

/* Time variables */
unsigned int long currentMillis;
unsigned int long previousMillis;


void setup() {
  Serial.begin(115200);

  for (int pinNumber = 2; pinNumber < 5; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }

  currentMillis = millis();
  previousMillis = currentMillis;
}


/*
   readTemperatureTask
   This function turns on a led depending on the received temperature information and if it is above a given baseline.
*/

void readTemperatureTask() {
  if (temperature > baselineTemp) {
    digitalWrite(ledTempPin, HIGH);
    // As this LED may not be always on, we need to be sure to test it when it is turned ON
    ledTempLastHealth = getLedHealth(ledTempTesterPin, ledTempPin, ledTempLastHealth);
  }
  else {
    digitalWrite(ledTempPin, LOW);
  }
}


/*
   blinkLEDTask
   This function regulates the period at which the LED blinks with
   the blinking interval received from the Serial.
*/
void blinkLEDTask() {
  currentMillis = millis();

  // If the blinkingInterval has passed, keep LED ON. Only turn LED OFF
  // in the next call of this task. No delays were used.
  if (currentMillis - previousMillis > blinkingInterval/2) {
    digitalWrite(ledPotPin, HIGH);
    if (currentMillis - previousMillis > blinkingInterval) {
      previousMillis = currentMillis;
      // As this LED is blinking, we need to be sure to test it when it is turned ON
      ledPotLastHealth = getLedHealth(ledPotTesterPin, ledPotPin, ledPotLastHealth);
    }
  }
  else {
    digitalWrite(ledPotPin, LOW);
  }
}


/*
   intensityLEDTask
   This function regulates the intensity of the light emitted by the LED
   with the intesity information received from the Serial.
*/
void intensityLEDTask() {
  analogWrite(ledPhotoPin, intensity);
}


/*
   Read serial with sensor's information coming from the PC
*/
void readSerialTask()
{
  String readString;

  // Try to read something from Serial on char at a time
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one char from serial buffer
      readString += c; //makes the string readString
    }
  }

  // If something was read from the Serial then extract the information from it
  if (readString.length() > 0) {
      
      String sensor_name = getValue(readString,':',0);
      String sensor_value = getValue(readString,':',1);
      
      if (sensor_name == "int") {
        intensity = sensor_value.toInt();
        ledPhotoLastHealth = getLedHealth(ledPhotoTesterPin, ledPhotoPin, ledPhotoLastHealth);
        delay(10);  
        Serial.println(ledPhotoLastHealth);
      } else if (sensor_name == "pot") {
        blinkingInterval = sensor_value.toInt();
        delay(10);  
        Serial.println(ledPotLastHealth);
      } else if (sensor_name == "temp") {
        temperature = sensor_value.toFloat();
        delay(10);  
        Serial.println(ledTempLastHealth);
      }
      
    // reset String so it is ready to get a new String from Serial
    readString="";
  }
}

int int_led_zero_counter = 0;

boolean getLedHealth(int probePin, int ledPin, bool lastHealth)
{
  int value = analogRead(probePin);
  if (ledPin != ledPhotoPin && 200 < value && value < 850) {
    return true;
  }
  else if (ledPin == ledPhotoPin && intensity > 0) {
    long start = millis();

    // for intensity 1 we need 300 cycles to get a stable health report, for intensity 100 we need 15 cycles
    int cycles = 300 - 80 * log10(5*(intensity-0.8));
    // Serial.println(cycles);
    for (int i = 0; i < cycles; i++) {
      value = analogRead(probePin);
      if (value > 100 && value < 900) {
        
        int elapsed = millis() - start;
        // Serial.print("Time ");
        // Serial.println(elapsed);
        
        return true;
      }
    }
    
    int elapsed = millis() - start;
    // Serial.print("Time ");
    // Serial.println(elapsed);
    
    return false;
    /*
    if (value == 0 && int_led_zero_counter < 100) {
      int_led_zero_counter++;
      return lastHealth;
    } else if (value == 0 && int_led_zero_counter >= 100) {
      int_led_zero_counter = 0;
      return false;
    } else if (value == 1023) {
      return false;
    }
    int_led_zero_counter = 0;
    return true;
    */
  }
  else {
    return false;
  }
}


/*
   From the received String get a value from a certain index after spliting the String by a specific character
*/
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


/*
   Main loop with Round-Robin software architecture
*/
void loop() {
  readSerialTask();
  blinkLEDTask();
  readTemperatureTask();
  intensityLEDTask();

}
