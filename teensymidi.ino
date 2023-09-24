// Define the number of sliders
const int noSliders = 8;
const int ledPin = LED_BUILTIN; // LED_BUILTIN represents the onboard LED pin

// Store information about each slider: analog pin, jitter tolerance, and current reading
int sliders[8][3] {
  {A0, 1, 0},  
  {A1, 1, 0},
  {A2, 1, 0},
  {A3, 1, 0},
  {A4, 1, 0},
  {A5, 1, 0},
  {A6, 1, 0},
  {A7, 1, 0}
};

// MIDI channel to send control change messages
const int channel = 1;

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
  int currentReading = analogRead(sliders[slider][0]);
   
  bool returnValue = false;

  // Ensure that readings near the lower and upper bounds are clamped
  if (currentReading > 1020) {
    currentReading = 1023;
  }

  if (currentReading < 2) {
    currentReading = 0;
  }
  
  // Limit jitter and send MIDI control change message
  if (currentReading < sliders[slider][2] - sliders[slider][1] || currentReading > sliders[slider][2] + sliders[slider][1]) {
    sliders[slider][2] = currentReading;
    byte data = currentReading >> 3;    
    usbMIDI.sendControlChange(slider, data, channel);
    returnValue = true;
  }
 
  // Delay to avoid excessive reading
  delay(6);
  
  return returnValue; 
}

// Function to output sensor readings to the serial monitor
void outputData() {
  digitalWrite(ledPin, HIGH);   // Turn the LED on
  
  
  for (int i = 0; i < noSliders - 1; i++) {
    Serial.print(sliders[i][2]);
    Serial.print(" ");
  }
  Serial.print(sliders[noSliders - 1][2]);
  Serial.print("\n");
  digitalWrite(ledPin, LOW);    // Turn the LED off
}
