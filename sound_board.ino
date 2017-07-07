#include "config.h"

#include <Adafruit_VS1053.h>

/************************ Music Maker definitions ***************************/
// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
 
// Feather ESP8266
#define VS1053_CS      16     // VS1053 chip select pin (output)
#define VS1053_DCS     15     // VS1053 Data/command select pin (output)
#define CARDCS          2     // Card chip select pin
#define VS1053_DREQ     0     // VS1053 Data request, ideally an Interrupt pin
 
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
AdafruitIO_Feed *SoundFeed = io.feed("sound_effect");

void waitForMusicPlayerInit() {
    if(!musicPlayer.begin()) { // initialise the music player
        while (1);
    }
}

void playTestTone() {
    musicPlayer.sineTest(0x44, 500);
}

void waitForSDCardInit() {
    if(!SD.begin(CARDCS)) {
        while (1);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.print("Connecting to Adafruit IO");
    io.connect();

    SoundFeed->onMessage(handleSoundFeedMessage);

    while(io.status() < AIO_CONNECTED) {
        Serial.print(io.status());
        Serial.print(" ");
        Serial.println(io.statusText());

        delay(2000);
    }

    waitForMusicPlayerInit();
    waitForSDCardInit();
    playTestTone();

    musicPlayer.setVolume(20,20); 
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
}

void loop()
{
	io.run();
}

void handleSoundFeedMessage(AdafruitIO_Data *data) {
    String fileName = data->toString() + ".mp3";
    musicPlayer.startPlayingFile(fileName.c_str());
}
