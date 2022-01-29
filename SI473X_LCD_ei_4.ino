/*
 SI4735-D60 / SI4732-A10 all in one with SSB Support

  This sketch has been successfully tested on:
  1) Pro Mini 3.3V;
  2) UNO (by using a voltage converter);
  3) Arduino Yún (by using a voltage converter);
  4) Arduino Micro (see the operating voltage of your Micro);
  5) Arduino Mega (by using a voltage converter); and
  6) Arduino DUE;

  This sketch uses I2C LiquidCrystal/LCD, buttons and  Encoder.

  This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
  together with this sketch) and LiquidCrystal I2C Library by Frank de Brabander (https://github.com/johnrickman/LiquidCrystal_I2C).
  Look for LiquidCrystal I2C on Manager Libraries.


  ABOUT DIGITAL pin 13 and INPUT PULL-UP on Arduino Pro Mini, UNO or similar:
  This pin has a LED and a resistor connected on the board. When this pin is set to HIGH the LED comes on. If you use the internal
  pull-up resistor of the pin 13, you might experiment problem due to the drop voltage caused by the LED circuit. 
  If this occurs in your project, change the circuit to use external pull-up on pin 13.     


  ABOUT SSB PATCH:  
  This sketch will download a SSB patch to your SI4735-D60/SI4732-A10 device (patch_init.h). It will take about 8KB of the Arduino memory.

  First of all, it is important to say that the SSB patch content is not part of this library. The paches used here were made available by Mr. 
  Vadim Afonkin on his Dropbox repository. It is important to note that the author of this library does not encourage anyone to use the SSB patches 
  content for commercial purposes. In other words, this library only supports SSB patches, the patches themselves are not part of this library.

  In this context, a patch is a piece of software used to change the behavior of the SI4735-D60 and SI4732-A10 devices.
  There is little information available about patching the SI4735-D60/SI4732-A10. The following information is the understanding of the author of
  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device.
  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
  Patches to the SI4735-D60/Si4732-A60 are distributed in binary form and have to be transferred to the internal RAM of the device by
  the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

  ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment.
  Given this, it is at your own risk to continue with the procedures suggested here.
  This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60 and SI4732-A10.
  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735-D60/SI4732-A10 or other devices that you are using.

  Features of this sketch:

  1) FM, AM (MW and SW) and SSB (LSB and USB);
  2) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4kHz;
  3) 22 commercial and ham radio bands pre configured;
  4) BFO Control; and
  5) Frequency step switch (1, 5 and 10kHz);

  Main Parts:
  Encoder with push button;
  Seven bush buttons;
  LCD20x4 with I2C device;
  Arduino Pro mini 3.3V;

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Nov 2019.

  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  PU2CLR Ricardo Lima Caratti tarafından yazılan programda bâzı değişiklikler ve ilâveler yapılmıştır. Bu değişiklik vew ilâveler şunlardır:
  DEĞİŞİKLİKLER:
  =============
  1) Baskı devre tasarımına göre tuşların Arduino'ya bağlandığı bacaklar değiştirilmiştir.
  2) Ekran tasarımı değiştirilmiştir. 4 satır, 20 karakterlik ekrana daha çok bilgi sığdırabilmek için bâzı ifadeler kısaltılmış (STEREO ST gibi),SNR, S Metre,
  Adım, Ses yerine simgeler kullanılmıştır.
  3) Band sayısı arttırılmış ve bandların başlangıç-bitiş frekansları yeniden belirlenmiş ve isimleri düzenlenmiştir.
  4) AM kipinde frekans adımlarına 9 kHz ve 100 kHz ilâve edilmiştir.
  İLAVELER:
  =======
 1) Devre, 2 adet Li Ion pille beslenmek üzere tasarlanmış olduğundan, pil geriliminin ölçülerek ekranda gösterilmesi için bir rutin ilave edilmiştir.
  GErilim ölçme devresindeki R1 ve R2 dirençlerinin değerini ölçerek aşağıdaki
  // Voltmetre değişkenleri kısmında OHM cinsinden r1 ve r2'nin değeri olarak yazınız. Benim devremde, R1 için 10k'lık bir direnç, R2 için 3,3k'lık bir
  direnç kullanılmı, direnç değerleri ölçülmüş ve 10k'lık direnç 9970 ohm, 3,3k'lık direnç ise 3260 ohm bulunmuş ve aşağıdaki şekilde
  programa tanıtılmıştır. 
  

float r1 = 9970.0;
float r2 = 3260.0;
Pil geriliminin ölçümü bu iki direnç değerinin hassasiyetine bağımlıdır.
DİKKAT!!!!!!
Arduino Pro Mini'nin girişlerine 3V'tan fazla gerilim uygulanmamalıdır. Bunun için R1 ve R2 ile yapacağınız gerilim bölücünün bölme oranını
hesaplarken buna dikkat ediniz. Bölme oranı şöyle hesaplanır:
Bölme oranı=(R1+R2)/R2
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SI4735.h>
#include <LiquidCrystal_I2C.h> // Version 1.1.4 by Frank de Brabander. Download this library on https://github.com/marcoschwartz/LiquidCrystal_I2C
#include "Rotary.h"

#include "patch_init.h" // SSB patch for whole SSBRX initialization string


/// özel karakterler ///////////////////
// hoparlör
byte hoparlor[8] = {
  B00010,
  B00110,
  B11110,
  B11110,
  B11110,
  B00110,
  B00010,
  B00000
};

// pil
byte pil[8] = {
  B01100,
  B11110,
  B10010,
  B10010,
  B11110,
  B11110,
  B11110,
  B00000
};
//anten
byte anten[8] = {
  B10101,
  B01010,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};
byte snr[8] = {
  B10101,
  B01010,
  B10101,
  B01010,
  B10101,
  B01010,
  B10101
};
byte adim[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B11111,
  B01110,
  B00100,
  B00000
};
/////////////////////////////////
const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN A1

// Enconder PINs
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// Buttons controllers
#define MODE_SWITCH      11   // KİP (Am/LSB/USB)
#define BANDWIDTH_BUTTON 10   // Band genişliği : 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz //////////////////////
#define VOL_UP           12   // Volume Up
#define VOL_DOWN          8   // Volume Down 14
#define BAND_BUTTON_UP    6   // Next band
#define BAND_BUTTON_DOWN  9   // Previous band
#define AGC_SWITCH        4   // Switch AGC ON/OF
#define STEP_SWITCH       7   // Used to select the increment or decrement frequency step (1, 5 or 10 KHz)
#define BFO_SWITCH        5   // Used to select the enconder control (BFO or VFO)

#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150

#define DEFAULT_VOLUME 45 // başlangıç ses değeri

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1

const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

bool bfoOn = false;
bool disableAgc = true;
bool ssbLoaded = false;
bool fmStereo = true;

int currentBFO = 0;
int previousBFO = 0;

long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedFrequency = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t currentStep = 1;
uint8_t currentBFOStep = 25;

uint8_t bwIdxSSB = 2;
const char *bandwidthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char *bandwidthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};
const char *bandAdi[] = {"3m", "LW", "H630m", "MW", "H160m", "120m", "90m", "H80m", "75m", "60m", "H60m", "49m", "H40m", "41m", "31m", "H30m", "25m", "22m", "H20m", "19m", "16m", "H17m", "15m", "H15m", "H12m", "CB", "H10m", "HEPSi"};

/*
   Band data structure
*/
typedef struct
{
  uint8_t bandType;     // Band type (FM, MW or SW)
  uint16_t minimumFreq; // Minimum frequency of the band
  uint16_t maximumFreq; // maximum frequency of the band
  uint16_t currentFreq; // Default frequency or current frequency
  uint16_t currentStep; // Defeult step (increment and decrement)
} Band;

/*
   Band table
*/
Band band[] = {
  {FM_BAND_TYPE, 8700, 10800, 10280, 5},
  {LW_BAND_TYPE, 100, 510, 150, 1},        // Uzun dalga bandı
  //{LW_BAND_TYPE, 440, 480, 455, 1},        // IF girişi
  {LW_BAND_TYPE, 471, 480, 472, 1},        // 630m amatör bandı
  {MW_BAND_TYPE, 522, 1719, 540, 9},       // Orta dalga yayın bandı
  {SW_BAND_TYPE, 1800, 2000, 1900, 1},     // 160 metre amatör bandı
  {SW_BAND_TYPE, 2300, 2500, 2300, 5},    // 120metre yayın bandı
  {SW_BAND_TYPE, 3200, 3400, 3200, 5},     // 90 metre yayın bandı
  {SW_BAND_TYPE, 3500, 4000, 3500, 1},     // 80 metre amatör bandı
  {SW_BAND_TYPE, 3900, 4000, 3900, 5},     // 75 metre yayın bandı
  {SW_BAND_TYPE, 4750, 5000, 4750, 5},     // 60 metre yayın bandı
  {SW_BAND_TYPE, 5250, 5250, 5440, 1},     // 60 metre amatör bandı
  {SW_BAND_TYPE, 5900, 6200, 6000, 5},     // 49 metre yayın bandı
  {SW_BAND_TYPE, 7000, 7300, 7010, 1},     // 40 metre amatör bandı
  {SW_BAND_TYPE, 7200, 7450, 7200, 5},     // 41 metre yayın bandı
  {SW_BAND_TYPE, 9400, 9900, 9500, 5},     // 31 metre yayın bandı
  {SW_BAND_TYPE, 10090, 10169, 10100, 1},  // 30 metre amatör bandı
  {SW_BAND_TYPE, 11600, 12100, 11940, 5},  // 25m yayın bandı
  {SW_BAND_TYPE, 13500, 13950, 13600, 5},  // 22m yayın bandı
  {SW_BAND_TYPE, 13990, 14360, 14100, 1},  // 20 metre amatör bandı
  {SW_BAND_TYPE, 15090, 15950, 15300, 5},  // 19 metre yayın bandı
  {SW_BAND_TYPE, 17350, 17950, 17600, 5},  // 16 metre yayın bandı
  {SW_BAND_TYPE, 18000, 18300, 18100, 1},  // 17 metre amatör bandı
  {SW_BAND_TYPE, 18890, 19150, 19000, 5},  // 15 metre yayın bandı
  {SW_BAND_TYPE, 20990, 21470, 21200, 1},  // 15 metre amatör bandı
  {SW_BAND_TYPE, 24880, 25000, 24940, 1},  // 12 metre amatör bandı
  {SW_BAND_TYPE, 26200, 27900, 27500, 5},  // CB bandı (11 metre)
  {SW_BAND_TYPE, 28000, 29690, 28400, 1},  // 10 metre  amatör bandı
  {SW_BAND_TYPE, 150, 29990, 15000, 5}     // Bütün band
}; // 10 meters

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Voltmetre değişkenleri
float temp = 0.0;
float r1 = 9970.0;
float r2 = 3260.0;
float input_voltage = 0.0;
// buraya kadar

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
LiquidCrystal_I2C display(0x27, 20, 4); // please check the address of your I2C device
SI4735 si4735;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(BANDWIDTH_BUTTON, INPUT_PULLUP);
  pinMode(BAND_BUTTON_UP, INPUT_PULLUP);
  pinMode(BAND_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);
  pinMode(BFO_SWITCH, INPUT_PULLUP);
  pinMode(AGC_SWITCH, INPUT_PULLUP);
  pinMode(STEP_SWITCH, INPUT_PULLUP);
  pinMode(MODE_SWITCH, INPUT_PULLUP);

  display.init();

  delay(500);

  // Splash - Change it for your introduction text.
  display.backlight();
  display.setCursor(6, 0);
  display.print("SI4735        ");
  display.setCursor(0, 1);
  display.print("  AM-SSB-FM RADYO   ");
  delay(500);
  display.setCursor(2, 2);
  display.print("v1.5.1.1 PU2CLR  ");
  delay(500);
  display.setCursor(4, 3);
  display.print("Mod. TA2EI     ");
  delay(2000);
  // end Splash

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, FM_BAND_TYPE);

  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();
  delay(200);
  currentFrequency = previousFrequency = si4735.getFrequency();

  si4735.setVolume(volume);
  display.clear();
  showStatus();

  ////
  display.createChar(1, hoparlor);
  display.createChar(2, pil);
  display.createChar(3, anten);
  display.createChar(4, snr);
  display.createChar(5, adim);
}

// Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();
  if (encoderStatus)
  {
    if (encoderStatus == DIR_CW)
    {
      encoderCount = 1;
    }
    else
    {
      encoderCount = -1;
    }
  }
}


void clearLine4() {
  display.setCursor(0, 2);
  display.print("                    ");
}

// Show current frequency

void showFrequency()
{
  String freqDisplay;
  String unit;
  String bandMode;
  int divider = 1;
  int decimals = 3;
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    divider = 100;
    decimals = 2;
    unit = "MHz";
  }
  else if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
  {
    divider = 1;
    decimals = 0;
    unit = "kHz";
  }
  else
  {
    divider = 1000;
    decimals = 3;
    unit = "kHz";
  }

  if ( !bfoOn )
    freqDisplay = String((float)currentFrequency / divider, decimals);
  else
    freqDisplay = ">" + String((float)currentFrequency / divider, decimals) + "<";

  display.setCursor(4, 0);
  display.print("        ");
  display.setCursor(4, 0);
  display.print(freqDisplay);

  if (currentFrequency < 520 )
    bandMode = "LW  ";
  else
    bandMode = bandModeDesc[currentMode];

  display.setCursor(0, 0);
  display.print(bandMode);

  display.setCursor(11, 0);
  display.print(unit);
}

/*
    Show some basic information on display
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showStatus()
{

  showFrequency();

  display.setCursor(15, 0);
  display.print("      ");
  display.setCursor(15, 0);
  display.write(byte(5));
  display.print(currentStep);

  display.setCursor(0, 3);
  display.print("       ");
  display.setCursor(0, 3);

  if (currentMode == LSB || currentMode == USB)
  {
    display.print("BW:");
    display.print(String(bandwidthSSB[bwIdxSSB]));
    display.print("k");
    showBFO();
  }
  else if (currentMode == AM)
  {
    display.print("BW:");
    display.print(String(bandwidthAM[bwIdxAM]));
    display.print("k");
  }

  // Show AGC Information /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  si4735.getAutomaticGainControl();
  display.setCursor(13, 1);
  display.print((si4735.isAgcEnabled()) ? "A" : "K");
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  showRSSI();
  showVolume();
  volc();
  display.setCursor(0, 1);
  display.print("     ");
  display.setCursor(0, 1);
  // bandAdiGoster();
  display.print(String (bandAdi[bandIdx]));


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// PİL GERİLİMİ GÖSTER ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void volc()
{
  int analog_value = analogRead(A0);
  temp = (analog_value * 3.3) / 1024.0;
  input_voltage = temp / (r2 / (r1 + r2));
  delay(100);
  display.setCursor(6, 1);
  display.print("      ");
  display.setCursor(6, 1);
  display.write(byte(2));
  display.print(input_voltage);
  display.print("v");
}

/////Shows RSSI status ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showRSSI()
{
  //int bars = ((rssi / 10.0) / 2.0) + 1;

  display.setCursor(13, 3);
  display.print("      ");
  display.setCursor(13, 3);
  display.write(byte(3));
  // display.print("S:");
  display.print(rssi);
  display.print("uV");
  /////////////////////////////////
  display.setCursor(15, 1);
  display.print("     ");
  display.setCursor(15, 1);
  display.write(byte(4));
  display.print(si4735.getCurrentSNR());
  display.print("dB");
  /*
    if ( bars > 5 )  {
    bars = 5;
    }
    for (int i = 0; i < bars; i++)
    display.print("|");
  */
  if ( currentMode == FM) {
    display.setCursor(0, 3);
    display.print((si4735.getCurrentPilot()) ? "ST" : "MO");
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////   Shows the volume level on LCD /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showVolume()
{
  display.setCursor(9, 3);
  display.print("   ");
  display.setCursor(9, 3);
  display.write(byte(1));
  display.print(si4735.getCurrentVolume());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// Shows the BFO current status.( Must be called only on SSB mode) ////////////////////////////////////////////////////////////////////
void showBFO()
{

  String bfo;

  if (currentBFO > 0)
    bfo = "+" + String(currentBFO);
  else
    bfo = String(currentBFO);

  display.setCursor(0, 2);
  display.print("         ");
  display.setCursor(0, 2);
  display.print("BFO:");
  display.print(bfo);
  display.print("Hz ");

  display.setCursor(13, 2);
  display.print("       ");
  display.setCursor(13, 2);
  display.print("A:  ");
  display.print(currentBFOStep);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
   Goes to the next band (see Band table)
*/
void bandUp()
{
  // save the current frequency for the band
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;

  if (bandIdx < lastBand)
  {
    bandIdx++;
  }
  else
  {
    bandIdx = 0;
  }
  useBand();
}

/*
   Goes to the previous band (see Band table)
*/
void bandDown()
{
  // save the current frequency for the band
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;
  if (bandIdx > 0)
  {
    bandIdx--;
  }
  else
  {
    bandIdx = lastBand;
  }
  useBand();
}

/*
   This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
   SSB mode.
*/
void loadSSB()
{
  display.setCursor(0, 2);
  display.print("SSB kipine geciliyor");

  si4735.reset();
  si4735.queryLibraryId(); // Is it really necessary here?  Just powerDown() maigh work!
  si4735.patchPowerUp();
  delay(50);
  si4735.setI2CFastMode(); // Recommended
  // si4735.setI2CFastModeCustom(500000); // It is a test and may crash.
  si4735.downloadPatch(ssb_patch_content, size_content);
  si4735.setI2CStandardMode(); // goes back to default (100kHz)
  clearLine4();

  // delay(50);
  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2kHz (default); 1=2.2kHz; 2=3kHz; 3=4kHz; 4=500Hz; 5=1kHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  si4735.setSSBConfig(bwIdxSSB, 1, 0, 0, 0, 1);
  delay(25);
  ssbLoaded = true;
  display.clear();
}

/*
   Switch the radio to current band.
   The bandIdx variable points to the current band.
   This function change to the band referenced by bandIdx (see table band).
*/
void useBand()
{
  clearLine4();
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    si4735.setTuneFrequencyAntennaCapacitor(0);
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    bfoOn = ssbLoaded = false;

  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
      si4735.setTuneFrequencyAntennaCapacitor(0);
    else
      si4735.setTuneFrequencyAntennaCapacitor(1);

    if (ssbLoaded)
    {
      si4735.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, currentMode);
      si4735.setSSBAutomaticVolumeControl(1);
      si4735.setSsbSoftMuteMaxAttenuation(0); // Disable Soft Mute for SSB
    }
    else
    {
      currentMode = AM;
      si4735.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      si4735.setAutomaticGainControl(1, 0);
      si4735.setAmSoftMuteMaxAttenuation(0); // // Disable Soft Mute for AM
      bfoOn = false;
    }

  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  showStatus();
}


void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (bfoOn)
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
    }
    else
    {
      if (encoderCount == 1)
        si4735.frequencyUp();
      else
        si4735.frequencyDown();

      // Show the current frequency only if it has changed
      currentFrequency = si4735.getFrequency();
    }
    encoderCount = 0;
  }

  // Check button commands
  if ((millis() - elapsedButton) > MIN_ELAPSED_TIME)
  {
    // check if some button is pressed
    if (digitalRead(BANDWIDTH_BUTTON) == LOW)
    {
      if (currentMode == LSB || currentMode == USB)
      {
        bwIdxSSB++;
        if (bwIdxSSB > 5)
          bwIdxSSB = 0;
        si4735.setSSBAudioBandwidth(bwIdxSSB);
        // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
        if (bwIdxSSB == 0 || bwIdxSSB == 4 || bwIdxSSB == 5)
          si4735.setSBBSidebandCutoffFilter(0);
        else
          si4735.setSBBSidebandCutoffFilter(1);
      }
      else if (currentMode == AM)
      {
        bwIdxAM++;
        if (bwIdxAM > 6)
          bwIdxAM = 0;
        si4735.setBandwidth(bwIdxAM, 1);
      }
      showStatus();
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(BAND_BUTTON_UP) == LOW)
      bandUp();
    else if (digitalRead(BAND_BUTTON_DOWN) == LOW)
      bandDown();
    else if (digitalRead(VOL_UP) == LOW)
    {
      si4735.volumeUp();
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(VOL_DOWN) == LOW)
    {
      si4735.volumeDown();
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(BFO_SWITCH) == LOW)
    {
      if (currentMode == LSB || currentMode == USB) {
        bfoOn = !bfoOn;
        if (bfoOn)
          showBFO();
        showStatus();
      } else if (currentMode == FM) {
        si4735.seekStationUp();
        delay(30);
        currentFrequency = si4735.getFrequency();
      }
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(AGC_SWITCH) == LOW)
    {
      disableAgc = !disableAgc;
      // siwtch on/off ACG; AGC Index = 0. It means Minimum attenuation (max gain)
      si4735.setAutomaticGainControl(disableAgc, 1);
      showStatus();
    }
    else if (digitalRead(STEP_SWITCH) == LOW) /////////////////////////STEP butonu basılı ise ////////////////////////////////////
    {
      if ( currentMode == FM) {
        fmStereo = !fmStereo;
        if ( fmStereo )
          si4735.setFmStereoOn();
        else
          si4735.setFmStereoOff(); // It is not working so far.
      } else {

        // This command should work only for SSB mode
        if (bfoOn && (currentMode == LSB || currentMode == USB))
        {
          currentBFOStep = (currentBFOStep == 25) ? 10 : 25;
          showBFO();
        }
        else
        {
          if (currentStep == 1)
            currentStep = 5;
          else if (currentStep == 5)
            currentStep = 9;
          ///////////////////ekleme
          else if (currentStep == 9)
            currentStep = 100;
          //////////////ekleme
          else
            currentStep = 1;
          si4735.setFrequencyStep(currentStep);
          band[bandIdx].currentStep = currentStep;
          showStatus();
        }
        delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
      }
    }
    /////////////////////////////////////////STEP butonu ile ilgili kısmın sonu /////////////////////////////////////////////////////////////////////////
    else if (digitalRead(MODE_SWITCH) == LOW)
    {
      if (currentMode != FM ) {
        if (currentMode == AM)
        {
          // If you were in AM mode, it is necessary to load SSB patch (avery time)
          loadSSB();
          currentMode = LSB;
        }
        else if (currentMode == LSB)
        {
          currentMode = USB;
        }
        else if (currentMode == USB)
        {
          currentMode = AM;
          ssbLoaded = false;
          bfoOn = false;
        }
        // Nothing to do if you are in FM mode
        band[bandIdx].currentFreq = currentFrequency;
        band[bandIdx].currentStep = currentStep;
        useBand();
      }
    }
    elapsedButton = millis();
  }

  // Show the current frequency only if it has changed
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showFrequency();
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 12)
  {
    si4735.getCurrentReceivedSignalQuality();
    int aux = si4735.getCurrentRSSI();
    if (rssi != aux)
    {
      rssi = aux;
      showRSSI();
    }
    elapsedRSSI = millis();
  }

  // Show volume level only if this condition has changed
  if (si4735.getCurrentVolume() != volume)
  {
    volume = si4735.getCurrentVolume();
    showVolume();
  }

  if (currentMode == LSB || currentMode == USB)
  {
    if (currentBFO != previousBFO)
    {
      previousBFO = currentBFO;
      si4735.setSSBBfo(currentBFO);
      showBFO();
    }
  }

  delay(10);
}
////////////// EKLEMELER
/*
  void bandAdiGoster()
  {
  if (bandIdx == 1) { bandAdi=
*/
///////////// PİL GERİLİMİNİ ÖLÇME RUTİNİ /////////////////////////////
//////////////////////////////////////////////////////////////////////
