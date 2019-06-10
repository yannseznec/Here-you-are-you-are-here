# Here You Are, You Are Here
GPS based granular audio playback system

How does it work?

A GPS antenna looks for signal from satellites to designate a location. When a location has been found the software uses this location to control a set of parameters to play back a sound using granular synthesis. 
There are two knobs and a button. The knobs control pitch and volume, and the button plays the sound according to the parameters dictated by the listener's location.
The system is battery powered, as it necessarily needs to be outside to function. The instrument only works when a satellite signal has been located. 

This instrument uses the following hardware:
- Teensy LC
- Bela (with BeagleBoard)
- Momentary Button Switch
- 1 LED
- Ublox NEO-6M 56 Channel GPS Receiver
- USB battery pack
- 2 potentiometers

And the following software:
- Pure Data (PD)
- TinyGPS++ Library for Arduino/Teensy
Some notes on each of these...

Teensy LC
The GPS module is attached to the Teensy and converts the location data received from satellites into MIDI data. This is sent via USB to the Bela and into the Pure Data patch running on there.

Bela
The Bela is running the PD patch that creates all the sound. The sound comes out the headphone output. The granular synthesis system seems to take up quite a lot of CPU, which means that when it is running the Bela sometimes becomes inaccessible. To fix this I use the SSH technique described here: https://github.com/BelaPlatform/Bela/wiki/Recover-from-stuck-in-reboot-loop 

Momentary Button Switch
The button creates the sound. Two of the parameters set by the location are the attack and release times, so the button will act different depending on where you are.

LED
The LED flashes when location data is received. I originally used a small screen to show the location data, but this was distracting so it has been replaced by this somewhat more abstract approach. The Teensy code still has all the necessary things to include this screen, so if you'd like to use it you just need to get this: https://www.sparkfun.com/products/14048

Ublox NEO-6M 56 Channel GPS Receiver
I suspect there are better GPS systems than this, but I managed to get it working with the Teensy so I didn't want to push my luck. I'm not hugely happy with it, mainly because it seems to be unusually challenging to consistently receive satellite signals, and in the future I would like to find a better solution. http://www.hobbytronics.co.uk/ublox-gps-neo6m

USB Battery pack
I use this one, which seems to last a long time and provide enough current for the Bela to run happily: https://www.amazon.co.uk/gp/product/B00F5Q4F0U
I find when the battery pack is below about half charge things start to go wrong, but this still provides several hours of functionality. 

Potentiometers
I use 1kΩ linear potentiometers with solder lugs, each with a .1µf capacitor for smoothing. These are the ones:
https://www.rapidonline.com/taiwan-alpha-rv16af-1k-linear-16mm-metal-case-solder-lug-potentiometer-65-0751
I connect the outer two pins to 5v and Ground on the Bela, and the middle pin to the analog input pins (0,1).
The capacitor connects between the ground and middle pin of each potentiometer. 

Pure Data
The Bela includes Pure Data so there's nothing special to install. The patch that creates the sound is taking a sound recording and applying granular synthesis techniques. This means that it plays a number of tiny fragments of the sound on loop, crossfading between them, with some amounts of randomness to stop it sounding repetitive. Thge location data changes a number of the audio parameters in the patch (attack/release, grain size, amount of random panning, location within the sound, amount of randomness within the sound, number of grains playing, etc), and two parameters are controlled manually (pitch and volume). 
The PD patches can be downloaded and used on their own, without the Bela or the GPS, if you're simply interested in the granular synthesis part of things. The patches contain a number of vestigial features from the early stages of developement which may be of interest, including the ability to record your own audio into the buffer live, and the ability to have multiple instances running at once. 

Arduino/Teensy
I tried a number of different GPS libraries to get the Ublox GPS receiver working, and the only one that worked with any consistency was the TinyGPS++ library. I don't pretend to understand why, or indeed how it works, but it generally does.