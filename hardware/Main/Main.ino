#include "HX711.h"
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"
#include "Time.h"

#define CALIBRATION_CONSTANT_GRAMS 1077.90 //Found using LoadCellSetup
#define DisplaySerial Serial
/*
 * Load Cell Pins are:
 *    DOUT in 3
 *    SCK in 2
 * LCD Pins Are:
 *    TX into 0 (rx) Note: This must be unplugged for an upload to the arduino to work
 *    RX into 1 (tx)
 *    RES into 4 (include a 1 kilo-ohm resistor in this connection)
 * Button Pins are:
 *    SIGNAL PIN into 12
 *    
 */

enum BrewState {beans, equipment, pour};
enum GrindLevel {very_course, course, medium_course, fine, espresso_fine}; 

char * get_string(GrindLevel g) {
  switch(g) {
    case(very_course):
      return "Very Course";
    case(course):
      return "Course";
    case(medium_course):
      return "Medium Course";
    case(fine):
      return "Fine";
    case(espresso_fine):
      return "Espresso Fine";
  }
}

class Recipe {
  public:
    Recipe (int bg, int wg, int s, GrindLevel g) : bean_grams(bg), water_grams(wg), time_in_seconds(s), state(beans), grind(g){};
    void display_recipe( Goldelox_Serial_4DLib *, int);
    int get_bean_grams() { return bean_grams; };
    int get_water_grams() { return water_grams; };
    void set_brew_state (BrewState b) { state = b; };
    BrewState get_brew_state() { return state; };
  private:
    int bean_grams;
    int water_grams;
    int time_in_seconds;
    BrewState state;
    GrindLevel grind;
};


void Recipe::display_recipe( Goldelox_Serial_4DLib * screen, int seconds_passed){
  char buff[3]; //used to print int values to LCD;
  screen->txt_MoveCursor(4, 0); //Make sure we move the position in whatever unit we were using before
  screen->txt_Height(1);
  screen->txt_Width(1);
  switch (state) { //Extra spaces so that text isn't left unpainted over
    case beans:
      screen->putstr("Measure ");
      screen->putstr(buff);
      sprintf(buff, "%d", bean_grams);
      screen->putstr("g of Beans!      ");
      break;
    case equipment:
      screen->putstr("Grind ");
      screen->putstr(get_string(grind));
      screen->putstr(" and bush button when ready to brew");
      break;
    case pour:
      if (seconds_passed == 0) {
        screen->putstr("Begin Pouring!          ");
      }
      else if (seconds_passed <= time_in_seconds) {
        screen->putstr("Keep pouring to ");
        sprintf(buff, "%d", water_grams);
        screen->putstr(buff);
        screen->putstr("g!           ");
      }
      else {
        screen->putstr("STOP Pouring!           ");
      }
      break;
  }
  screen->txt_Height(3) ;
  screen->txt_Width(3) ;
}

HX711 load_cell;
//use Serial0 to communicate with the display.
Goldelox_Serial_4DLib screen(&DisplaySerial);
const int button_pin = 12;
Recipe dummy(20, 300, 15, medium_course);

float last_reading = -10.0;
long first_tick = 0;

void setup() {
  // Set up Scale
  load_cell.begin(3, 2);
  load_cell.tare();
  load_cell.set_scale(CALIBRATION_CONSTANT_GRAMS);

  //For handling errors
  screen.Callback4D = mycallback ;  
  screen.TimeLimit4D = 5000;
  DisplaySerial.begin(9600);

  //Let Screen Start Up
  delay(2000);

  // Set up Style of screen
  screen.gfx_ScreenMode(LANDSCAPE);
  screen.gfx_BGcolour(BLACK) ; 
  screen.SSTimeout(0) ;
  screen.SSSpeed(0) ;
  screen.SSMode(0) ;
  screen.txt_BGcolour(BLACK) ;
  screen.txt_FGcolour(GREEN) ;
  screen.txt_Height(3) ;
  screen.txt_Width(3) ;

  //Set Up Button
}

void loop() {
  //Basic Readout - Should be everpresent
  float reading = load_cell.get_units(2);
  if ((reading >= (last_reading + .05)) || (reading <= (last_reading -.05))){ //Make sure that the scale has a noticeable change before changing the display, to prevent flickering 
    char buffer[8];
    dtostrf(reading, 6, 2, buffer);
    screen.txt_MoveCursor(2, 0) ;
    screen.putstr(buffer);
    last_reading = reading;
  }

  //Recipe Instructions - should change as needed
  switch (dummy.get_brew_state()) {
    case beans:
      if (reading >= dummy.get_bean_grams()) {
        dummy.set_brew_state(equipment);
      }
      break;
    case equipment:
      if (digitalRead(button_pin) == LOW) {
        load_cell.tare();
        dummy.set_brew_state(pour);
      }
      break;
    case pour:
      if ((reading >= 1.0) && (first_tick == 0)) { //begin once appreciable amount of water is poured
        first_tick = now();
      }
      break;
  }

  if (first_tick == 0) {
    dummy.display_recipe(&screen, 0);
  }
  else {
    dummy.display_recipe(&screen, now() - first_tick);
  }
}

void mycallback(int ErrCode, unsigned char Errorbyte)
{
  // Pin 13 has an LED connected on most Arduino boards. Just give it a name
  int led = 13;
  pinMode(led, OUTPUT);
  while(1)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                // wait for 200 ms
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(200);                // wait for 200 ms
  }
}
