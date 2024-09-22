#include <Arduino.h>
// Define the number of sliders
const int noSliders = 8;
const int ledPin = LED_BUILTIN; // LED_BUILTIN represents the onboard LED pin


// Struct to store slider information
struct Slider {
  int analogPin;
  int jitterTolerance;
  int currentReading;
};

// Store information about each slider: analog pin, jitter tolerance, and current reading
Slider sliders[noSliders] = {
  {A0, 1, 0},  
  {A1, 1, 0},
  {A2, 1, 0},
  {A3, 1, 0},
  {A4, 1, 0},
  {A5, 1, 0},
  {A6, 1, 0},
  {A7, 1, 0}
};

// Constants for readability
const int maxAnalogReading = 1020;
const int minAnalogReading = 2;
const int readingDelay = 6;

// MIDI channel to send control change messages
const int channel = 1;


// put function declarations here:
bool readAxis(int);
bool readAxisData(); 
bool readAxis(int slider);
void outputData();

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // Set the LED pin as an output
}

void loop() {
  // Main loop
  
  // Check if there's new sensor data
  if (readAxisData()) {
    // If there's new data, send MIDI control change messages
    outputData();
  }
  
  // Clear MIDI input queue
  while (usbMIDI.read()) {
    // controllers must call .read() to keep the queue clear even if they are not responding to MIDI
  }
}

// Function to check if there's new sensor data
bool readAxisData() {
  bool returnValue = false;
  
  for (int i = 0; i < noSliders; i++) {
    if (readAxis(i)) {
      returnValue = true;
    }
  }
  
  return returnValue; 
}

// Function to read data from a single slider
bool readAxis(int slider) {
  int currentReading = analogRead(sliders[slider].analogPin);
   
  bool returnValue = false;

  // Ensure that readings near the lower and upper bounds are clamped
  if (currentReading > maxAnalogReading) {
    currentReading = 1023;
  }

  if (currentReading < minAnalogReading) {
    currentReading = 0;
  }
  
  // Limit jitter and send MIDI control change message
  if (
    currentReading < sliders[slider].currentReading - sliders[slider].jitterTolerance || 
    currentReading > sliders[slider].currentReading + sliders[slider].jitterTolerance
  ) 
  {
    sliders[slider].currentReading = currentReading;
    byte data = currentReading >> 3;    
    usbMIDI.sendControlChange(slider, data, channel);
    returnValue = true;
  }
 
  // Delay to avoid excessive reading
  delay(readingDelay);
  
  return returnValue; 
}

// Function to output sensor readings to the serial monitor
void outputData() {
  digitalWrite(ledPin, HIGH);   // Turn the LED on
  
  
  for (int i = 0; i < noSliders - 1; i++) {
    Serial.print(sliders[i].currentReading);
    Serial.print(" ");
  }
  Serial.print(sliders[noSliders - 1].currentReading);
  Serial.print("\n");
  digitalWrite(ledPin, LOW);    // Turn the LED off
}
