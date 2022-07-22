
const int noSliders = 8;

int sliders[8][3] {
{A0,1,0},  
{A1,1,0},
{A2,1,0},
{A3,1,0},
{A4,1,0},
{A5,1,0},
{A6,1,0},
{A7,1,0}
};

const int channel = 1; // MIDI channel



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}


void loop() {
  // put your main code here, to run repeatedly: 

  
  if (readAxisData()==true)
  {
    outputData();
  }
  
  while (usbMIDI.read()) {
     // controllers must call .read() to keep the queue clear even if they are not responding to MIDI
  }
}


bool readAxisData()
{
  bool returnValue = false;
  
  for (int i=0; i < noSliders; i++)
  {
    
    if (readAxis(i))
    {
      returnValue = true;
    }
  }

  return returnValue; 
}


bool readAxis(int slider)
{
   int currentReading =  analogRead(sliders[slider][0]);

   
   bool returnValue = false;

   if(currentReading > 1023)
   {
      currentReading=1023;
   }

   if(currentReading < 0)
   {
      currentReading = 0;
   }
   
   // limit jitter
   if (currentReading < sliders[slider][2] - sliders[slider][1] || currentReading > sliders[slider][2] + sliders[slider][1])
   {
      sliders[slider][2] = currentReading;
      byte data;
      data = currentReading>>3;    
      usbMIDI.sendControlChange(slider,data, channel);
      returnValue = true;
   }
 
   delay(6);
   return returnValue; 
}

void outputData()
{
  for (int i =0; i < noSliders-1; i++)
  {
    Serial.print(sliders[i][2]);
    Serial.print(" ");
  }
  Serial.print(sliders[noSliders-1][2]);
  Serial.print("\n");
}
