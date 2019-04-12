#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//TeensyView
#include <TeensyView.h>
#define PIN_RESET 15
#define PIN_DC    5
#define PIN_CS    10
#define PIN_SCK   13
#define PIN_MOSI  11
TeensyView oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);

const int channel = 1;
int const values = 2; // number of values to check for changes
int newValues[values];
int oldValues[values];
int latitude;
int longitude;
int latCurrent;
int longCurrent;
int sats;
int masterCurrent;
int master = 127; // the CC value that controls whether there is sound or not based on whether there is a satellite signal

int const numCC = 8; //  number of CC
int newVal[numCC];
int gpsCC[] = {  
  10,11,12,13,14,15,16,17   // which CC to use
};

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.setFontType(3);  // Set font to type 1
  oled.setCursor(0, 0); // move cursor
//  oled.print("TUB50FATBALLS");  // Display what's in the buffer (splashscreen)
  oled.display(); 
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.println(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
      latitude = int(gps.location.lat()*10000)%127;
      longitude = int(gps.location.lng()*10000)%127;
      if (latCurrent != latitude || longCurrent != longitude || masterCurrent != master) {
      usbMIDI.sendControlChange(master, 1, channel); 
      usbMIDI.sendControlChange(75, latitude, channel); 
      usbMIDI.sendControlChange(76, longitude, channel); 
      latCurrent = latitude;
      longCurrent = longitude;
      masterCurrent = master;
      }

  oled.setFontType(0);  // Set font to type 1
  oled.setCursor(0, 0); // move cursor
  oled.print("Longitude ");
  oled.print(gps.location.lng(),8);  // Write a byte out as a character
  oled.setCursor(0, oled.getLCDHeight() / 3); // move cursor
  oled.print("Latitude ");
  oled.print(gps.location.lat(),8);  // Write a byte out as a character
  oled.setCursor(0, oled.getLCDHeight() * .66); // move cursor
  oled.print("Satellites ");
  oled.print(ss.available());  // Write a byte out as a character
  oled.display();  // Send the PAGE to the OLED memory
  }
  
  else
  {
    Serial.println(F("INVALID"));
    oled.setFontType(0);  // Set font to type 1
    oled.setCursor(0, 0); // move cursor
    oled.print("no GPS data available.");
    oled.setCursor(0, oled.getLCDHeight() / 3); // move cursor
    oled.print("go outside. or wait. or both.");
    oled.display();  // Send the PAGE to the OLED memory
    
    usbMIDI.sendControlChange(master, 0, channel); 

  }


}
