//
// NB! This is a file generated from the .4Dino file, changes will be lost
//     the next time the .4Dino file is built
//
#define RESETLINE     4

#define DisplaySerial Serial


// uncomment to log SPE errors messages to Serial (serial monitor)
//#define LOG_MESSAGES



#include <Servo.h>
#include "Diablo_Serial_4DLib.h"
#include "Diablo_Const4D.h"

Diablo_Serial_4DLib Display(&DisplaySerial);
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
unsigned long myTime;
int A = 10000;
int pos = 100;    // variable to store the servo position
int iter = 0;
int s1 = 1;
int delta = 20;
int loops = 0 ;
String ATresponse ;     // response from last AT command, does not include "OK" or "ERROR" and will be partial if command timed out

// routine to handle ESP Serial errors
void mycallback(int ErrCode, unsigned char Errorbyte)
{
#ifdef LOG_MESSAGES
  const char *Error4DText[] = {"OK\0", "Timeout\0", "NAK\0", "Length\0", "Invalid\0"} ;
  Serial.print(F("Serial 4D Library reports error ")) ;
  Serial.print(Error4DText[ErrCode]) ;
  if (ErrCode == Err4D_NAK)

  {
    Serial.print(F(" returned data= ")) ;
    Serial.println(Errorbyte) ;
  }
  else
    Serial.println(F("")) ;
  while (1) ; // you can return here, or you can loop
#else
  // Pin 13 has a LED connected on most Arduino boards. Just give it a name
#define led 13
  while (1)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(200);                // wait for a second
  }
#endif
}
// end of routine to handle Serial errors

word Bstate ;
word x, y ;
int n ;

void setup()
{
// Ucomment to use the Serial link to the PC for debugging
//  Serial.begin(115200) ;        // serial to USB port
// Note! The next statement will stop the sketch from running until the serial monitor is started
//       If it is not present the monitor will be missing the initial writes
//    while (!Serial) ;             // wait for serial to be established

  pinMode(RESETLINE, OUTPUT);       // Picaso reset pin
digitalWrite(RESETLINE, 1);       // Reset Display, using shield
  delay(100);                       // wait for it to be recognised
digitalWrite(RESETLINE, 0);       // Release Display Reset, using shield
  delay(3000) ;                     // give display time to startup

  // now start display and ESP as Serial lines should have 'stabilised'
  DisplaySerial.begin(9600) ;     // Hardware serial to Display, same as SPE on display is set to
  Display.TimeLimit4D = 5000 ;      // 5 second timeout on all commands
  Display.Callback4D = mycallback ;

  Display.gfx_ScreenMode(PORTRAIT) ; // change manually if orientation change
  // put your setup code here, to run once:
  Display.touch_Set(TOUCH_ENABLE);                            // enable the touch screen
  Bstate = BUTTON_UP ;
  Display.gfx_Button(Bstate, 44, 190, GREEN, WHITE, FONT3, 2, 2, "ON-OFF") ;
  Display.gfx_MoveTo(20,260) ;
  Display.putstr("OFF") ;
  //Display.touch_Set(TOUCH_ENABLE);                            // enable the touch screen
  n = 0 ;
  Display.gfx_Slider(SLIDER_RAISED, 28, 28, 203, 63, TURQUOISE, 100, n) ;
  Display.gfx_MoveTo(20,100) ;
  Display.putstr("Period (min):") ;
  Display.gfx_MoveTo(140,100) ;
  Display.putstr("000") ;
  Display.gfx_MoveTo(20,125) ;
  Display.putstr("Oscillations:") ;
  Display.gfx_MoveTo(140,125) ;
  Display.putstr("000") ;
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos);
} // end Setup **do not alter, remove or duplicate this line**

void loop()
{
  byte state ;
  state = Display.touch_Get(TOUCH_STATUS);               // get touchscreen status
  //-----------------------------------------------------------------------------------------
  if((state == TOUCH_PRESSED) || (state == TOUCH_MOVING))                       // if there's a press, or it's moving
  {
    x = Display.touch_Get(TOUCH_GETX);
    y = Display.touch_Get(TOUCH_GETY);
  }

  //-----------------------------------------------------------------------------------------
  if(state == TOUCH_RELEASED)                      // if there's a release
  {
    if ((x >= 44) && (x <= 184) && (y >= 203) && (y <= 236))
    {
      Bstate = !Bstate ;
      Display.gfx_Button(Bstate, 44, 190, GREEN, WHITE, FONT3, 2, 2, "ON-OFF") ;  // Width=141 Height=33
      Display.gfx_MoveTo(20,260) ;
      if (Bstate)
        Display.putstr("OFF") ;
      else
        Display.putstr("ON ") ;
    }
  }

  byte state2 ;
  word x2, y2 ;
  char nx[20] ;
  char ix[20] ;
  state2 = Display.touch_Get(TOUCH_STATUS);               // get touchscreen status
  //-----------------------------------------------------------------------------------------
  if((state2 == TOUCH_PRESSED) || (state2 == TOUCH_MOVING))                       // if there's a press, or it's moving
  {
    x2 = Display.touch_Get(TOUCH_GETX);
    y2 = Display.touch_Get(TOUCH_GETY);
    if ((x2 >= 28) && (x2 <= 203) && (y2 >= 28) && (y2 <= 63))
    {
      n = Display.gfx_Slider(SLIDER_RAISED, 28, 28, 203, 63, TURQUOISE, 100, -x2);  // use slider to scale x to n
      //Display.gfx_MoveTo(20,100) ;
      //Display.putstr("Period (min):") ;
      Display.gfx_MoveTo(140,100) ;
      sprintf(nx,"%3.3d", n) ;
      Display.putstr(nx) ;

    }
  }


  // put your main code here, to run repeatedly:
  if (!Bstate){
    
  if (A>n*60){

  if (iter ==0){
      for (pos = 100; pos <= 115; pos += 1) { // goes from 100 degrees to 115 degrees
  // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(delta);                       // waits delta ms for the servo to reach the position
      }
  }
  else{
    if (s1 == 1){
    for (pos = 115; pos >= 80; pos -= 1) { // goes from 115 degrees to 80 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(delta);                       // waits delta ms for the servo to reach the position
    }
    s1=2;
    }
    else{
    for (pos = 80; pos <= 115; pos += 1) { // goes from 80 degrees to 115 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(delta);                       // waits delta ms for the servo to reach the position
    }
    s1=1;
    }
  }
  iter +=1;
  A = 0;
  }
  else{
    delay(1000);
    A += 1;
    }
  }
  else{
    A = 10000;
  }
  //Display.gfx_MoveTo(20,150) ;
  //Display.putstr("Oscillations:") ;
  Display.gfx_MoveTo(140,125) ;
  sprintf(ix,"%3.3d", iter) ;
  Display.putstr(ix) ;
}
