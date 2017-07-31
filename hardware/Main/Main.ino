#include "HX711.h"
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"
#include "Time.h"

#define CALIBRATION_CONSTANT_GRAMS 1090 //Found using LoadCellSetup
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

class Step {
  protected:
    char * info;
  public:
    Step() : info("Default Text") {};
    Step(char * i) : info(i) {};
    virtual boolean add_to_display (Goldelox_Serial_4DLib * screen, double reading, int elapsed_seconds) { return true; };
    void clear_from_display (Goldelox_Serial_4DLib * screen) {
      screen->txt_MoveCursor(4, 0); //Make sure we move the position in whatever unit we were using before
      screen->txt_Height(1);
      screen->txt_Width(1);
      screen->putstr("                                                                                "); // At least the length of a line haha
      screen->txt_Height(3);
      screen->txt_Width(3);
    }
};

class WeightDoneStep : public Step {
  public:
    WeightDoneStep(int g, char * i): grams(g), Step(i) {}
    boolean add_to_display (Goldelox_Serial_4DLib * screen, double reading, int elapsed_seconds) {
      screen->txt_MoveCursor(4, 0); //Make sure we move the position in whatever unit we were using before
      screen->txt_Height(1);
      screen->txt_Width(1);
      screen->putstr(Step::info);
      screen->txt_Height(3);
      screen->txt_Width(3);
      
      return (reading >= grams);
    }
  protected: 
    double grams;  
};

class TimeDoneStep : public Step { 
  public:
    TimeDoneStep(int s, char * i): seconds(s), Step(i) {}
    boolean add_to_display (Goldelox_Serial_4DLib * screen, double reading, int elapsed_seconds) {
      screen->txt_MoveCursor(4, 0); //Make sure we move the position in whatever unit we were using before
      screen->txt_Height(1);
      screen->txt_Width(1);
      screen->putstr(Step::info);
      screen->txt_Height(3);
      screen->txt_Width(3);

      return (elapsed_seconds >= seconds);
    }
  protected:
    int seconds;
};

class WeightTimeDoneStep : public WeightDoneStep {
  public:
    WeightTimeDoneStep(int g, int s, char * i): WeightDoneStep(g, i), seconds(s) {}
    boolean add_to_display (Goldelox_Serial_4DLib * screen, double reading, int seconds_elapsed) {
      screen->txt_MoveCursor(4, 0); //Make sure we move the position in whatever unit we were using before
      screen->txt_Height(1);
      screen->txt_Width(1);
      screen->putstr(WeightDoneStep::info);
      screen->txt_Height(3);
      screen->txt_Width(3);

      return (seconds_elapsed >= seconds) && (reading >= WeightDoneStep::grams);
    }
    private:
      int seconds;
};

class Recipe {
  public:
    Recipe(): currentStep(0), numSteps(0) {}
    void add_to_display (Goldelox_Serial_4DLib * screen, double reading, int seconds_elapsed) {
      if (currentStep == numSteps) {
        screen->txt_MoveCursor(4, 0); //Make sure we move the position in whatever unit we were using before
        screen->txt_Height(1);
        screen->txt_Width(1);
        screen->putstr("                                                                  ");
        screen->txt_Height(3);
        screen->txt_Width(3);
      }
      else if (steps[currentStep]->add_to_display(screen, reading, seconds_elapsed)) {
        steps[currentStep]->clear_from_display(screen);
        currentStep++;
      }
    }
    void add_step(Step* s) {
      steps[numSteps] = s;
      numSteps++;
    }
    int get_step() { return currentStep; }
  private:
    Step* steps[10]; //limiting at 10 steps right now
    int currentStep;
    int numSteps;
};

HX711 load_cell;
//use Serial0 to communicate with the display.
Goldelox_Serial_4DLib screen(&DisplaySerial);
const int button_pin = 12;
Recipe dummy;
float last_reading = -10.0;
long last_checkpoint = 0;
int last_step = -1;
WeightDoneStep w = WeightDoneStep(30, "30g of Beans!");
WeightTimeDoneStep wt = WeightTimeDoneStep(30, 30, "30g Water for 30 Sec");  
TimeDoneStep t = TimeDoneStep(10, "Wait 10 Seconds");

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

  dummy.add_step(&w);
  dummy.add_step(&wt);
  dummy.add_step(&t);
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
  if(dummy.get_step() > last_step) { // make sure each step has it's own timing bracket
    last_checkpoint = now(); 
    last_step = dummy.get_step();
  }
  
  dummy.add_to_display(&screen, reading, now() - last_checkpoint); 
  
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
