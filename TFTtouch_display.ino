
#include <Adafruit_GFX.h>    
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>


#include <SPI.h>

#include "DHT.h"

#define DHTPIN 40     

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
#define LCD_RESET A4 



#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define ILI9341_BLACK       0x0000      
#define ILI9341_NAVY        0x000F     
#define ILI9341_DARKGREEN   0x03E0      
#define ILI9341_DARKCYAN    0x03EF      
#define ILI9341_MAROON      0x7800      
#define ILI9341_PURPLE      0x780F      
#define ILI9341_OLIVE       0x7BE0      
#define ILI9341_LIGHTGREY   0xC618      
#define ILI9341_DARKGREY    0x7BEF      
#define ILI9341_BLUE        0x001F      
#define ILI9341_GREEN       0x07E0   
#define ILI9341_CYAN        0x07FF      
#define ILI9341_RED         0xF800     
#define ILI9341_MAGENTA     0xF81F      
#define ILI9341_YELLOW      0xFFE0      
#define ILI9341_WHITE       0xFFFF      
#define ILI9341_ORANGE      0xFD20      
#define ILI9341_GREENYELLOW 0xAFE5      
#define ILI9341_PINK        0xF81F

#define BUTTON_X 40
#define BUTTON_Y 100
#define BUTTON_W 60
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2


#define TEXT_X 10
#define TEXT_Y 10
#define TEXT_W 220
#define TEXT_H 50
#define TEXT_TSIZE 3
#define TEXT_TCOLOR ILI9341_MAGENTA

#define TEXT_LEN 12
char textfield[TEXT_LEN + 1] = "";
uint8_t textfield_i = 0;

#define YP A3  
#define XM A2  
#define YM 9   
#define XP 8   

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define STATUS_X 10
#define STATUS_Y 65



Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


Adafruit_GFX_Button buttons[15];
Adafruit_GFX_Button settingsButton;
Adafruit_GFX_Button backHomeButton;


char buttonlabels[15][5] = {"Ent.", "Clr.", "Esc.", "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#" };
uint16_t buttoncolors[15] = {ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED,
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE,
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE,
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE,
                             ILI9341_ORANGE, ILI9341_BLUE, ILI9341_ORANGE
                            };

bool numericScreenOn = false;
bool numericScreenInit = false;
bool homeScreenOn = true;
bool homeScreenInit = false;
bool settingsScreenOn = false;
bool settingsScreenInit = false;
bool numericScreenOnTemperature = false;
bool numericScreenOnHumidity = false;
String serialTemp;
String oldData = "";
String temperatureSP = "0";
String humiditySP = "0";

bool heaterOn = false;
bool fanOn = false;

void setup(void) {

  dht.begin();
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
  if (identifier == 0x0101)
    identifier = 0x9341;

  if (identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if (identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  } else if (identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if (identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if (identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if (identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.4\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
}


void status(const __FlashStringHelper *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}

void status(char *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}


#define MINPRESSURE 10
#define MAXPRESSURE 1000

void NumericKeyboardScreen(TSPoint p) {
  if (!numericScreenInit) {
    tft.fillScreen(BLACK);

    // create buttons
    for (uint8_t row = 0; row < 5; row++) {
      for (uint8_t col = 0; col < 3; col++) {
        buttons[col + row * 3].initButton(&tft, BUTTON_X + col * (BUTTON_W + BUTTON_SPACING_X),
                                          BUTTON_Y + row * (BUTTON_H + BUTTON_SPACING_Y), // x, y, w, h, outline, fill, text
                                          BUTTON_W, BUTTON_H, ILI9341_WHITE, buttoncolors[col + row * 3], ILI9341_WHITE,
                                          buttonlabels[col + row * 3], BUTTON_TEXTSIZE);
        buttons[col + row * 3].drawButton();
      }
    }

    // create 'text field'
    tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, ILI9341_WHITE);
    numericScreenInit = true;
  }

  
  for (uint8_t b = 0; b < 15; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      
      buttons[b].press(true);  
    } else {
      buttons[b].press(false);  
    }
  }

  
  for (uint8_t b = 0; b < 15; b++) {
    if (buttons[b].justReleased()) {
    
      buttons[b].drawButton();  
    }

    if (buttons[b].justPressed()) {
      buttons[b].drawButton(true);  

     
      if (b >= 3) {
        if (textfield_i < TEXT_LEN) {
          textfield[textfield_i] = buttonlabels[b][0];
          textfield_i++;
          textfield[textfield_i] = 0; 
        }
      }

      
      if (b == 1) {

        textfield[textfield_i] = 0;
        if (textfield > 0) {
          textfield_i--;
          textfield[textfield_i] = ' ';
        }
      }

      
      Serial.println(textfield);
      tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
      tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
      tft.setTextSize(TEXT_TSIZE);
      tft.print(textfield);

      
      if (b == 2) 
      {
        if (numericScreenOnTemperature) {
          //temperatureSP = textfield;
          settingsScreenOn = true;
          settingsScreenInit = false;
          numericScreenOnTemperature = false;
        }

        if (numericScreenOnHumidity) {
          //temperatureSP = textfield;
          settingsScreenOn = true;
          settingsScreenInit = false;
          numericScreenOnHumidity = false;
        }

        for (int i = 0; i <= 12; i++) {
          textfield[i] = ' ';
        }
        textfield_i = 0;
      }
     
      if (b == 0) 
      {  
        if (numericScreenOnTemperature) {
          temperatureSP = textfield;
          settingsScreenOn = true;
          settingsScreenInit = false;
          numericScreenOnTemperature = false;
        }

        if (numericScreenOnHumidity) {
          humiditySP = textfield;
          settingsScreenOn = true;
          settingsScreenInit = false;
          numericScreenOnHumidity = false;
        }

        for (int i = 0; i <= 12; i++) {
          textfield[i] = ' ';
        }
        textfield_i = 0;
      }

      delay(100); 
    }
  }

}

void HomeScreen(String temperature, String humidity, String tempSP, String humSP, bool hOn, bool fOn) {
  if (!homeScreenInit) {
    tft.fillScreen(BLACK);
    settingsButton.initButton(&tft, 120, 20,   // x, y, w, h, outline, fill, text
                              230, 30, ILI9341_WHITE, ILI9341_DARKGREEN, ILI9341_WHITE,
                              "Settings", BUTTON_TEXTSIZE);
    settingsButton.drawButton();
    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Temp. ");
    tft.setTextColor(ILI9341_ORANGE);
    tft.print("(SP: " + tempSP + " C)");

    tft.setCursor(10, 130);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, BLACK);
    tft.print("Heat~~");

    tft.setCursor(10, 200);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Humidity ");
    tft.setTextColor(ILI9341_BLUE);
    tft.print("(SP: " + humSP + "%)");

    tft.setCursor(10, 280);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, BLACK);
    tft.print("Fan~~");

    homeScreenInit = true;
  }
  tft.setCursor(10, 80);
  tft.setTextSize(5);
  tft.setTextColor(ILI9341_WHITE, BLACK);
  tft.print(temperature + " C");



  if (hOn) {
    tft.drawCircle(100, 135, 10, GREEN);
    tft.fillCircle(100, 135, 10, GREEN);
  } else {
    tft.drawCircle(100, 135, 10, RED);
    tft.fillCircle(100, 135, 10, RED);
  }



  tft.setCursor(10, 230);
  tft.setTextSize(5);
  tft.setTextColor(ILI9341_WHITE, BLACK);
  tft.print(humidity + " %");



  if (fOn) {
    tft.drawCircle(100, 285, 10, GREEN);
    tft.fillCircle(100, 285, 10, GREEN);
  } else {
    tft.drawCircle(100, 285, 10, RED);
    tft.fillCircle(100, 285, 10, RED);
  }

}

void SettingsScreen(String temp, String hum) {
  if (!settingsScreenInit) {
    tft.fillScreen(BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Temp. Setpoint");
    tft.drawRect(10, 30, TEXT_W, TEXT_H, ILI9341_WHITE);
    tft.setCursor(10, 90);
    tft.print("Humidity Setpoint");
    tft.drawRect(10, 120, TEXT_W, TEXT_H, ILI9341_WHITE);

    backHomeButton.initButton(&tft, 120, 300,   // x, y, w, h, outline, fill, text
                              230, 30, ILI9341_WHITE, ILI9341_LIGHTGREY, ILI9341_WHITE,
                              "Back Home", BUTTON_TEXTSIZE);
    backHomeButton.drawButton();

    settingsScreenInit = true;
  }
  tft.setCursor(TEXT_X + 2, 40);
  tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
  tft.setTextSize(TEXT_TSIZE);
  tft.print(temp);

  tft.setCursor(TEXT_X + 2, 130);
  tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
  tft.setTextSize(TEXT_TSIZE);
  tft.print(hum);


}

void initAllScreens() {
  numericScreenInit = false;
  homeScreenInit = false;
  settingsScreenInit = false;
}


void loop(void) 
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
 
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
  
  }




  if (homeScreenOn) {
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    if (t < temperatureSP.toFloat()) {
      heaterOn = true;
    } else {
      heaterOn = false;
    }

    if (h > humiditySP.toFloat()) {
      fanOn = true;
    } else {
      fanOn = false;
    }
    //initAllScreens();
    HomeScreen(String(t), String(h), temperatureSP, humiditySP, heaterOn, fanOn);
    
  }

  if (settingsScreenOn) {
   
    SettingsScreen(temperatureSP, humiditySP);
  }
 
  if (numericScreenOnTemperature || numericScreenOnHumidity) {
    NumericKeyboardScreen(p);
  }


  if (homeScreenOn && p.x > 10 && p.x < 230 && p.y > 0 && p.y < 30 ) 
  {
    settingsScreenOn = true;
    numericScreenOn = false;
    homeScreenOn = false;
    numericScreenOnTemperature = false;
    initAllScreens();

  }

  
  if (settingsScreenOn && p.x > 10 && p.x < 230 && p.y > 260 && p.y < 280) 
  {
    numericScreenOnTemperature = false;
    settingsScreenOn = false;
    numericScreenOn = false;
    homeScreenOn = true;
    initAllScreens();
  }

  
  if (settingsScreenOn && p.x > 10 && p.x < 230 && p.y > 40 && p.y < 80) 
  {
    numericScreenOnTemperature = true;
    numericScreenOnHumidity = false;
    settingsScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }

  
  if (settingsScreenOn && p.x > 10 && p.x < 230 && p.y > 130 && p.y < 170) 
  {
    numericScreenOnTemperature = false;
    numericScreenOnHumidity = true;
    settingsScreenOn = false;
    homeScreenOn = false;
    initAllScreens();
  }



}
