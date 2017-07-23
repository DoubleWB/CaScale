#include "HX711.h"
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"
#include "Time.h"

#define CALIBRATION_CONSTANT_GRAMS 1077.90 //Found using LoadCellSetup
#define DisplaySerial Serial

enum BrewState {beans, equipment, pour};

class Recipe {
  public:
    Recipe (int bg, int wg, int s) : bean_grams(bg), water_grams(wg), time_in_seconds(s), state(beans){};
    void display_recipe( Goldelox_Serial_4DLib *, int);
    int get_bean_grams() { return bean_grams; };
    int get_water_grams() { return water_grams; };
    void set_brew_state();
  private:
    int bean_grams;
    int water_grams;
    int time_in_seconds;
    BrewState state;
};


void Recipe::display_recipe( Goldelox_Serial_4DLib * screen, int seconds_passed){
  screen->txt_MoveCursor(3, 0); //Make sure we move the position in whatever unit we were using before
  screen->txt_Height(1);
  screen->txt_Width(1);
  /*if (seconds_passed = -2) {
    screen->putstr("Measure Beans!");
  }
  if (seconds_passed = -1) {
    screen->putstr("Add Equipment!");
  }*/
  if (seconds_passed == 0) {
    screen->putstr("Begin Pouring!");
  }
  else if (seconds_passed <= time_in_seconds) {
    screen->putstr("Keep Pouring!");
  }
  else {
    screen->putstr("STOP Pouring!");
  }
  screen->txt_Height(3) ;
  screen->txt_Width(3) ;
}

HX711 load_cell;
//use Serial0 to communicate with the display.
Goldelox_Serial_4DLib screen(&DisplaySerial);
const int button_pin = 12;
Recipe dummy(20, 300, 15);

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
  screen.gfx_BGcolour(WHITE) ; 
  screen.SSTimeout(0) ;
  screen.SSSpeed(0) ;
  screen.SSMode(0) ;
  screen.txt_BGcolour(WHITE) ;
  screen.txt_FGcolour(RED) ;
  screen.txt_Height(3) ;
  screen.txt_Width(3) ;

  //Set Up Button
  digitalRead(button_pin);
}

void loop() {
  float reading = load_cell.get_units(2);
  if ((reading >= (last_reading + .05)) || (reading <= (last_reading -.05))){
    char buffer[8];
    dtostrf(reading, 6, 2, buffer);
    screen.txt_MoveCursor(2, 0) ;
    screen.putstr(buffer);
    last_reading = reading;
  }
  
  if ((reading >= 1.0) && (first_tick == 0)) { //begin once appreciable amount of water is poured
    first_tick = now();
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
