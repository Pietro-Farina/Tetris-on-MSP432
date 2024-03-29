#include <driverlib/timer_a.h>
#include <driverlib/interrupt.h>
#include "Energia.h"
#include "utils.h"
#include "graphic.h"
#include "tetris.h"


// --- Joystick Values --- //
#define joystickX 2
#define joystickY 26
uint16_t x, y;
uint32_t z;
// ----------------------- //


// --- Timer Dedicated Values and Functions --- //
uint8_t count;
bool freezed_S1;
bool freezed_S2;
bool freezed_joystick;
uint8_t unfreeze_S1;
uint8_t unfreeze_S2;
uint8_t unfreeze_joystick;
void setupTimer(unsigned Period);
void timerA3ISR(void);
// -------------------------------------------- //


// --- Game Phase Function Declaration --- //
Map mappa;
bool just_finished;
uint8_t playing;
void _enter_game();
void _start_game();
void _end_game();
// --------------------------------------- //


// --- Buttons Function Declaration --- //
#define ButtonS1 33
#define ButtonS2 32
void _setup_buttons();
void pressed_S1();
void pressed_S2();
// ------------------------------------ //

//void setup() {}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  _init_Random();
  _init_Screen();
  _setup_buttons();
  detachInterrupt(digitalPinToInterrupt(ButtonS1));
  _enter_game();
  attachInterrupt(digitalPinToInterrupt(ButtonS1), pressed_S1, RISING);

  // MSP432 14-bit set to 12-bit
#if defined(__MSP432P401R__)
  analogReadResolution(12);
#endif
}


void loop() {

  if (playing == 0) {
    if (!freezed_joystick) {
      x = map(analogRead(joystickX), 0, 4096, 0, 128);
      y = map(analogRead(joystickY), 0, 4096, 128, 0);
      if (x < 1)      x = 1;
      if (x > 126)    x = 126;
      if (y < 1)      y = 1;
      if (y > 126)    y = 126;

      if (y > 110) {

        TA3CCTL0 = ~BIT4;
        detachInterrupt(digitalPinToInterrupt(ButtonS1));
        detachInterrupt(digitalPinToInterrupt(ButtonS2));
        playing = move_Piece(&mappa, (Direction) 2);
        attachInterrupt(digitalPinToInterrupt(ButtonS1), pressed_S1, RISING );
        attachInterrupt(digitalPinToInterrupt(ButtonS2), pressed_S2, RISING);
        TA3CCTL0 = BIT4;

        // FREEZE IT
        freezed_joystick = true;
        unfreeze_joystick = (count + 2) % 4;
      } else if (x > 110) {

        TA3CCTL0 = ~BIT4;
        detachInterrupt(digitalPinToInterrupt(ButtonS1));
        detachInterrupt(digitalPinToInterrupt(ButtonS2));
        playing = move_Piece(&mappa, (Direction) 0);
        attachInterrupt(digitalPinToInterrupt(ButtonS1), pressed_S1, RISING);
        attachInterrupt(digitalPinToInterrupt(ButtonS2), pressed_S2, RISING);
        TA3CCTL0 = BIT4;

        // FREEZE IT
        freezed_joystick = true;
        unfreeze_joystick = (count + 2) % 4;
      } else if (x < 20) {

        TA3CCTL0 = ~BIT4;
        detachInterrupt(digitalPinToInterrupt(ButtonS1));
        detachInterrupt(digitalPinToInterrupt(ButtonS2));
        playing = move_Piece(&mappa, (Direction) 1);
        attachInterrupt(digitalPinToInterrupt(ButtonS1), pressed_S1, RISING );
        attachInterrupt(digitalPinToInterrupt(ButtonS2), pressed_S2, RISING);
        TA3CCTL0 = BIT4;

        // FREEZE IT
        freezed_joystick = true;
        unfreeze_joystick = (count + 2) % 4;
      }
    }
  } else if (just_finished) {
    just_finished = false;
    _end_game();
  }
}


void _setup_buttons() {
  // No need for INPUT_PULLUP
  pinMode(ButtonS1, INPUT);
  pinMode(ButtonS2, INPUT);

  attachInterrupt(digitalPinToInterrupt(ButtonS1), pressed_S1, RISING );
  attachInterrupt(digitalPinToInterrupt(ButtonS2), pressed_S2, RISING );

  freezed_S1 = false;
  freezed_S2 = false;
  unfreeze_S1 = 0;
  unfreeze_S2 = 0;
}


void _enter_game() {
  playing = 1;
  count = 0;
  just_finished = false;
  freezed_joystick = false;
  unfreeze_joystick = 0;
  print_Menu();
  setupTimer(333);
  _setup_buttons();
}

void _start_game() {
  detachInterrupt(digitalPinToInterrupt(ButtonS1));
  playing = 0;
  just_finished = true;
  print_Map();
  start_game(&mappa);
  TA3CCTL0 = BIT4;
  freezed_S1 = true;
  unfreeze_S1 = 1;
  attachInterrupt(digitalPinToInterrupt(ButtonS1), pressed_S1, RISING );
}

void _end_game() {
  detachInterrupt(digitalPinToInterrupt(ButtonS1));
  TA3CCTL0 = ~BIT4;
  playing = 1;
  count = 0;
  change_points(mappa.points);
  print_Menu();
  attachInterrupt(digitalPinToInterrupt(ButtonS1), pressed_S1, RISING );
}

void setupTimer(unsigned Period) {
  // Bits 9-8: Clock source select: set to ACLK (32kHz)
  // Bits 7-6: Input divider: set to 1
  // Bits 5-4: Mode control: Count up to TACCRO and reset
  // Bits 2: TACLR : set to initially clear timer system
  // Bit 1: Disabled interrupts from TA0
  // Bit 0: Interrupt (pending) flag : set to zero (initially)
  TA3CTL = 0b0000000100010100;
  
  TA3CCR0 = Period * 32;  // Set TACCR0 = Period (32kHz clock)
  TA3CCTL0 = ~BIT4;       // Enable interrupts when TAR = TACCR0

  // The vector number is 14 + 16  = 30 which is represented by the symbol INT_TA3_0
  Interrupt_registerInterrupt(INT_TA3_0, timerA3ISR);
}

void pressed_S1() {
  Serial.println("PRESSED");
  if (playing == 0) {
    if (!freezed_S1) {
      TA3CCTL0 = ~BIT4;
      rotate_Piece(&mappa);
      TA3CCTL0 = BIT4;

      // freeze it
      freezed_S1 = true;
      unfreeze_S1 = (count + 2) % 4;
    }
  } else {  // start game
    _start_game();
  }
}

void pressed_S2() {
  if ((playing == 0) && !freezed_S2) {
    TA3CCTL0 = ~BIT4;
    playing = choke_Piece(&mappa);
    TA3CCTL0 = BIT4;

    // freeze it
    freezed_S2 = true;
    unfreeze_S2 = (count + 2) % 4;
  }
}

void timerA3ISR(void) {
  if (playing == 0) {
    if (count >= 3) {
      noInterrupts();
      playing = move_Piece(&mappa, (Direction) 2);
      count = 0;
    } else {
      count++;
    }
    if (count == unfreeze_S1) {
      freezed_S1 = false;
    }
    if (count == unfreeze_S2) {
      freezed_S2 = false;
    }
    if (count == unfreeze_joystick) {
      freezed_joystick = false;
    }
    TA3CCTL0 &= ~1;
  } else {
    TA3CCTL0 ^= BIT4;
  }
}
