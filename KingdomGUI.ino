#include "application.h"

#include "Adafruit_mfGFX.h"   // Core graphics library
#include "RGBmatrixPanel.h" // Hardware-specific library
#include "math.h"

//SYSTEM_MODE(SEMI_AUTOMATIC);

//#include "spark_disable_wlan.h"
//#include "spark_disable_cloud.h"

#if defined(SPARK)
/* is in header
  #define CLK D6
  #define OE  D7
  #define LAT A4
  #define A   A0
  #define B   A1
  #define C   A2
  #define D   A3
*/
#else
 #define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
 #define LAT A3
 #define OE  9
 #define A   A0
 #define B   A1
 #define C   A2
#endif


// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
//RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);

// 32x32
//RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

//about one 3 letter word per second
#define SECONDS_PER_CHARACTER ((1.0/5.0) * 1000.0)
#define CHAR_HEIGHT 8
#define CHAR_WIDTH  6

#define MILLISECONDS_BETWEEN_MESSAGES 0  // 5000


String message;
int xpos = 0;
int textMin;

uint8_t  fontSize = 1;
uint16_t fontColor = 0x00FF;
uint16_t backColor = 0x0000;

int setText(String text);
int setFontSize(String size);
int setColor(String col);

void setup()
{
  Spark.function("setText", setText);
  Spark.function("setFontSize", setFontSize);
  Spark.function("setColor", setColor);

  Time.zone(+1);  // Timezone UTC +1h

  while (NULL == message || !message.length())
    Spark.process();

//  Spark.disconnect();

  matrix.begin();
  matrix.fillScreen(backColor);

  matrix.setTextWrap(false);
  matrix.swapBuffers(false);
  matrix.setFont(COMICS_8);
  textMin = 1 + message.length() * CHAR_WIDTH * fontSize * -1;
//  resetMessage();

//    WiFi.on();
//    Spark.connect();
}

uint8_t resubscribe = 1;
uint8_t bkcol = 0;


void loop()
{
  matrix.fillScreen(backColor);

  matrix.setTextColor(matrix.Color333(3,3,3));
  matrix.setTextSize(fontSize);
  matrix.setCursor(0, -1);
  if (Time.hour() < 10)
    matrix.print("0");
  matrix.print(Time.hour());
  matrix.print(":");
  if (Time.minute() < 10)
    matrix.print("0");
  matrix.print(Time.minute());
  matrix.print(":");
  if (Time.second() < 10)
    matrix.print("0");
  matrix.print(Time.second());


  //if (Time.second() > 50)
  //{
  //  matrix.setTextColor(matrix.Color333(3,0,0));
  //  matrix.setCursor((matrix.width() - CHAR_WIDTH * fontSize) / 2, matrix.height() - CHAR_HEIGHT * fontSize - 1);
  //}
  //else
  //{
  //  matrix.setTextColor(matrix.Color333(0,3,0));
  //  matrix.setCursor(matrix.width() / 2 - CHAR_WIDTH * fontSize, matrix.height() - CHAR_HEIGHT * fontSize - 1);
  //}
  //matrix.print(String(59 - Time.second()));

  matrix.fillRect(Time.second()+1, matrix.height()-6, matrix.width()-1, 5,
                  Time.second() > 50 ? matrix.Color444(10,0,0) : matrix.Color444(0,2,0));
  matrix.drawRect(0, matrix.height()-7, matrix.width(), 7,
                  Time.second() > 50 ? matrix.Color444(10,0,0) : matrix.Color444(2,2,0));

  matrix.setTextColor(fontColor);
  matrix.setTextSize(fontSize*2);
  matrix.setCursor(xpos, matrix.height()/2 - CHAR_HEIGHT * fontSize - 1);
  if ((--xpos) <= 2*textMin)
    xpos = matrix.width();
  matrix.print(message);


  delay(SECONDS_PER_CHARACTER / (CHAR_WIDTH * fontSize));
  //updateMarqueeText();

}

int setText(String text)
{
  if (NULL == text)
    delay(10000);  // give us 10sec

  message = text.c_str();

  return text.length();
}

int setFontSize(String size)
{
  int fs = size.toInt();

  if (0 < fs && fs < 5)
  {
    fontSize = fs;
  }
  else
    fs = -1;

  return fs;
}

int setColor(String col)
{
  if (col.length())
    if (col.startsWith("-"))
      backColor = (uint16_t)(-1 * col.toInt());
    else
      fontColor = (uint16_t)col.toInt();
  else
    return ++backColor;

  return col.toInt();
}


//
//  Update our message params, coordinates, sizes, etc.
//
void resetMessage() {
/*
//  RGB.control(false);
//  Spark.connect();

  xpos = matrix.width();

  //figure out how many pixels long it is
  textMin = 1 + message.length() * CHAR_WIDTH * fontSize * -1;

  // size 1 == 8 pixels high
  // size 2 == ?
  matrix.setTextSize(fontSize);
  matrix.setTextColor(fontColor, 0);

  delay(MILLISECONDS_BETWEEN_MESSAGES);

//  Spark.disconnect();
//  RGB.control(true);
//  RGB.color((fontColor & 0x0F00) >> 8, (fontColor & 0x00F0) >> 4, (fontColor & 0x000F));
*/
}

void updateMarqueeText() {
/*
    matrix.fillScreen(0);
    matrix.setRotation(0);
    matrix.setCursor(xpos, 0);   // start at top left, with one pixel of spacing
    matrix.print(message);
    matrix.setRotation(2);
    //matrix.setCursor(matrix.width() - xpos + textMin, matrix.height() - CHAR_HEIGHT * fontSize);
    matrix.setCursor(xpos, 0);   // start at top left, with one pixel of spacing
    matrix.print(message);

    // Update display
    //matrix.swapBuffers(false);
    delay(SECONDS_PER_CHARACTER / (CHAR_WIDTH * fontSize));

    if ((--xpos) <= textMin)
      resetMessage();
*/
}
