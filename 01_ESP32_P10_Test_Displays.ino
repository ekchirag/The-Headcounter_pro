//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 01_ESP32_P10_Test_Displays
//----------------------------------------Including the libraries.
#include <DMD32.h>  //--> DMD32 by Qudor-Engineer (KHUDHUR ALFARHAN) : https://github.com/Qudor-Engineer/DMD32
#include "fonts/SystemFont5x7.h"
#include "fonts/Arial_black_16.h"
//----------------------------------------

// Fire up the DMD library as dmd.
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

// Timer setup.
// create a hardware timer  of ESP32.
hw_timer_t * timer = NULL;

//________________________________________________________________________________IRAM_ATTR triggerScan()
//  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning,
//  this gets called at the period set in Timer1.initialize();
void IRAM_ATTR triggerScan() {
  dmd.scanDisplayBySPI();
}
//________________________________________________________________________________

//________________________________________________________________________________VOID SETUP()
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();

  delay(500);

  Serial.println();
  Serial.println("return the clock speed of the CPU.");
  // return the clock speed of the CPU.
  uint8_t cpuClock = ESP.getCpuFreqMHz();
  delay(500);

  Serial.println();
  Serial.println("Timer Begin");
  // Use 1st timer of 4.
  // devide cpu clock speed on its speed value by MHz to get 1us for each signal  of the timer.
  timer = timerBegin(0, cpuClock, true);
  delay(500);

  Serial.println();
  Serial.println("Attach triggerScan function to our timer.");
  // Attach triggerScan function to our timer.
  timerAttachInterrupt(timer, &triggerScan, true);
  delay(500);

  Serial.println();
  Serial.println("Set alarm to call triggerScan function.");
  // Set alarm to call triggerScan function.
  // Repeat the alarm (third parameter).
  timerAlarmWrite(timer, 300, true);
  delay(500);

  Serial.println();
  Serial.println("Start an alarm.");
  // Start an alarm.
  timerAlarmEnable(timer);
  delay(500);
}
//________________________________________________________________________________

//________________________________________________________________________________VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:

  // If you want to change the font, don't forget to use this line of code : dmd.selectFont . Example: dmd.selectFont(Arial_Black_16);

  //----------------------------------------Demo with "Arial_Black_16" font.
  dmd.selectFont(Arial_Black_16);

  //.................Running Text.
  String txt_1 = "ESP32 with P10 LED Display";
  char char_array_txt_1[txt_1.length() + 1];
  txt_1.toCharArray(char_array_txt_1, txt_1.length() + 1);

  dmd.clearScreen(true);
  delay(1000);
  dmd.drawMarquee(char_array_txt_1,txt_1.length(),(32*DISPLAYS_ACROSS)-1,0);
  long start_1=millis();
  long timer_1=start_1;
  boolean ret=false;
  while(!ret){
   if ((timer_1+30) < millis()) {
     ret=dmd.stepMarquee(-1,0);
     timer_1=millis();
   }
  }
  //.................

  //.................Display Text.
  dmd.clearScreen(true);
  delay(1000);
  dmd.drawString(0,0,"DMD", 3, GRAPHICS_NORMAL);  //--> dmd.drawString(x, y, Text, Number of characters in text, GRAPHICS_NORMAL);
  delay(3000);
  //.................
  //----------------------------------------

  //----------------------------------------Demo with "SystemFont5x7" font.
  // If you use the font "SystemFont5x7", then 1 panel P10 (32x16) can display text in 2 rows.
  dmd.selectFont(SystemFont5x7);

  //.................Display Text.
  dmd.clearScreen(true);
  delay(1000);
  dmd.drawString(0,0,"ESP32", 5, GRAPHICS_NORMAL);
  dmd.drawString(0,9,"P10", 3, GRAPHICS_NORMAL);
  delay(3000);
  //.................

  //.................The first row displays text and the second row displays running text.
  String txt_2 = "ESP32 with P10 LED Display";
  char char_array_txt_2[txt_2.length() + 1];
  txt_2.toCharArray(char_array_txt_2, txt_2.length() + 1);
  int scrl_long = (txt_2.length()*6) + (32*DISPLAYS_ACROSS);

  dmd.clearScreen(true);
  delay(1000);
  
  // Displays text in the first row.
  dmd.drawString(4,0,"UTEH", 4, GRAPHICS_NORMAL);
  
  long start_2=millis();
  long timer_2=start_2;
  int i = 32*DISPLAYS_ACROSS;
  while(true){
    if ((timer_2+30) < millis()) {
      // Displays running text on the second row.
      dmd.drawString(i, 9, char_array_txt_2, txt_2.length(), GRAPHICS_NORMAL);
      
      if (i > ~scrl_long) {
        i--;
      } else {
        break;
      }
    
      timer_2=millis();
    }
  }
  //.................

  //.................Displays Text and Numbers.
  int T = 29;
  int H = 73;
  char char_array_T[String(T).length() + 1];
  char char_array_H[String(H).length() + 1];
  String(T).toCharArray(char_array_T, String(T).length() + 1);
  String(H).toCharArray(char_array_H, String(H).length() + 1);

  dmd.clearScreen(true);
  delay(1000);
  
  dmd.drawString(0, 0, "T:", 2, GRAPHICS_NORMAL);
  dmd.drawString(11, 0, char_array_T, String(T).length(), GRAPHICS_NORMAL);
  dmd.drawCircle(24, 1, 1, GRAPHICS_NORMAL);
  dmd.drawString(27, 0, "C", 1, GRAPHICS_NORMAL);
  
  dmd.drawString(0, 9, "H:", 2, GRAPHICS_NORMAL);
  dmd.drawString(11, 9, char_array_H, String(H).length(), GRAPHICS_NORMAL);
  dmd.drawString(27, 9, "%", 1, GRAPHICS_NORMAL);

  delay(3000);
  //.................
  //----------------------------------------
}
//________________________________________________________________________________
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<





