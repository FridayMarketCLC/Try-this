
#if defined(SPARK)
#include "Adafruit_mfGFX.h"
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#define PROGMEM
#define pinLO(_pin)	(PIN_MAP[_pin].gpio_peripheral->BRR = PIN_MAP[_pin].gpio_pin)
#define pinHI(_pin)	(PIN_MAP[_pin].gpio_peripheral->BSRR = PIN_MAP[_pin].gpio_pin)
#define pinSet(_pin, _hilo) (_hilo ? pinHI(_pin) : pinLO(_pin))
#define portSet(_port, _word) (_port->ODR = _word)
#define portSetMasked(_port, _word, _mask) (_port->BSRR = (_mask << 16) | (_word & _mask))


// A full PORT register is required for the data lines, though only the
// top 6 output bits are used.  For performance reasons, the port # cannot
// be changed via library calls, only by changing constants in the library.
// For similar reasons, the clock pin is only semi-configurable...it can
// be specified as any pin within a specific PORT register stated below.

//#define FASTER
#define FASTEST

#if defined(FASTEST)
  #define CLK D5
  #define LAT D6
  #define OE  D7
  #define CTRLPORT GPIOA
  #define CTRLMASK 0x6000

  #define A   D4
  #define B   D3
  #define C   D2
  #define D   A6           // not needed for 16x32 panels
  #define ABCSHIFT 3
  #define ABCDPORT GPIOB
  #define ABCDMASK 0x0039  // 0x0038 for 16x32 panels

  #define R1	TX		// bit 2 = RED 1
  #define G1	RX		// bit 3 = GREEN 1
  #define B1	A2		// bit 4 = BLUE 1
  #define R2	A3		// bit 5 = RED 2
  #define G2	A4		// bit 6 = GREEN 2
  #define B2	A5		// bit 7 = BLUE 2
  #define RGBPORT GPIOA
  #define RGBMASK 0x00FC
#elif defined(FASTER)
  #define CLK D6
  #define OE  D7
  #define LAT A4
  #define A   A0
  #define B   A1
  #define C   A2
  #define D   A3

  #define R1	A6		// bit 2 = RED 1
  #define G1	D4		// bit 3 = GREEN 1
  #define B1	D3		// bit 4 = BLUE 1
  #define R2	D2		// bit 5 = RED 2
  #define G2	D1		// bit 6 = GREEN 2
  #define B2	D0		// bit 7 = BLUE 2
#else
  #define CLK D5
  #define LAT D6
  #define OE  D7

  #define A   D4
  #define B   D3
  #define C   D2
  #define D   A6

  #define R1	TX		// bit 2 = RED 1
  #define G1	RX		// bit 3 = GREEN 1
  #define B1	A2		// bit 4 = BLUE 1
  #define R2	A3		// bit 5 = RED 2
  #define G2	A4		// bit 6 = GREEN 2
  #define B2	A5		// bit 7 = BLUE 2
#endif

  //#define signalPIN	D7	// Use pin D7 for oscilloscope or analyzer timing

  #define numPanels	2

#else
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 #include "pins_arduino.h"
#endif
#include "Adafruit_GFX.h"
#endif



class RGBmatrixPanel : public Adafruit_GFX {

 public:

  // Constructor for 16x32 panel:
  RGBmatrixPanel(uint8_t a, uint8_t b, uint8_t c,
    uint8_t sclk, uint8_t latch, uint8_t oe, boolean dbuf);

  // Constructor for 32x32 panel (adds 'd' pin):
  RGBmatrixPanel(uint8_t a, uint8_t b, uint8_t c, uint8_t d,
    uint8_t sclk, uint8_t latch, uint8_t oe, boolean dbuf);

  void
    begin(void),
    drawPixel(int16_t x, int16_t y, uint16_t c),
    fillScreen(uint16_t c),
    updateDisplay(void),
    swapBuffers(boolean),
    dumpMatrix(void);
  uint8_t
    *backBuffer(void);
  uint16_t
    Color333(uint8_t r, uint8_t g, uint8_t b),
    Color444(uint8_t r, uint8_t g, uint8_t b),
    Color888(uint8_t r, uint8_t g, uint8_t b),
    Color888(uint8_t r, uint8_t g, uint8_t b, boolean gflag),
    ColorHSV(long hue, uint8_t sat, uint8_t val, boolean gflag);

 private:

  uint8_t         *matrixbuff[2];
  uint8_t          nRows;
  volatile uint8_t backindex;
  volatile boolean swapflag;

  // Init/alloc code common to both constructors:
  void init(uint8_t rows, uint8_t a, uint8_t b, uint8_t c,
    uint8_t sclk, uint8_t latch, uint8_t oe, boolean dbuf);


#if defined(SPARK)
  uint8_t	_sclk, _latch, _oe, _a, _b, _c, _d;
#else
  // PORT register pointers, pin bitmasks, pin numbers:
  volatile uint8_t
    *latport, *oeport, *addraport, *addrbport, *addrcport, *addrdport;
  uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin,
    _sclk, _latch, _oe, _a, _b, _c, _d;
#endif

  // Counters/pointers for interrupt handler:
  volatile uint8_t row, plane;
  volatile uint8_t *buffptr;
};
