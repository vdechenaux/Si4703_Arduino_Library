#include <Si4703_Breakout.h>

const int resetPin = 2;
const int SDIO = A4;
const int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);
int channel;
int volume = 1;

void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nSi4703_Breakout Test Sketch");
  Serial.println("===========================");  
  Serial.println("a b     Favourite stations");
  Serial.println("+ -     Volume (max 15)");
  Serial.println("u d     Seek up / down");
  Serial.println("r       Listen for RDS Data (15 sec timeout)");
  Serial.println("Send me a command letter.");
  

  radio.powerOn();
  radio.setVolume(volume);
}

void loop()
{
  radio.readRDS();

  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'u') 
    {
      channel = radio.seekUp();
      displayInfo();
    } 
    else if (ch == 'd') 
    {
      channel = radio.seekDown();
      displayInfo();
    } 
    else if (ch == 'm') 
    {
      Serial.println(radio.toggleMute() ? "MUTE" : "Sound is back");
    } 
    else if (ch == '+') 
    {
      volume ++;
      if (volume == 16) volume = 15;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == '-') 
    {
      volume --;
      if (volume < 0) volume = 0;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == 'a')
    {
      channel = 930; // Rock FM
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'b')
    {
      channel = 974; // BBC R4
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'r')
    {
      Serial.println(radio.getRdsInfo().radioText);
      Serial.println(radio.getRdsInfo().stationName);
      Serial.println(radio.getRdsInfo().programIdentificationCode, HEX);

      for (int i=0; i<radio.getRdsInfo().alternateFrequenciesCount; i++) {
        Serial.print(radio.getRdsInfo().alternateFrequencies[i] / 10.0);
        Serial.println("Mhz");
      }
    }
  }
}

void displayInfo()
{
   Serial.print("Channel: "); Serial.println(channel); 
   Serial.print("Volume: "); Serial.println(volume); 
   Serial.println(radio.isStereo() ? "STEREO" : "MONO");
   Serial.print("RSSI: "); Serial.println(radio.getSignalStrength()); // in dBµV
}
