/* Copyright 2019 Dilshan R Jayakody.
 *
 * Copyright (c) 2019 Dilshan R Jayakody.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */

#include "Arduino.h"
#include "dtmfgen.h"

#define CHAR_ZERO   48
#define CHAR_ONE    49
#define CHAR_TWO    50
#define CHAR_THREE  51
#define CHAR_FOUR   52
#define CHAR_FIVE   53
#define CHAR_SIX    54
#define CHAR_SEVEN  55
#define CHAR_EIGHT  56
#define CHAR_NINE   57
#define CHAR_HASH   35
#define CHAR_STAR   42

#define ROW_697   14
#define ROW_770   15
#define ROW_852   17
#define ROW_941   19

#define COL_1209  24
#define COL_1336  27
#define COL_1477  30

#define WAVE_TABLE_SIZE 200

static DTMFGenerator *generatorInstance = NULL;

unsigned char DTMFGenerator::dtmfRowPos = 0;
unsigned char DTMFGenerator::dtmfColPos = 0;

unsigned char DTMFGenerator::rowOffset = 0;
unsigned char DTMFGenerator::colOffset = 0;

unsigned char waveTable[WAVE_TABLE_SIZE] = { 
   64,  66,  68,  69,  71,  73,  75,  77,  79,  80,  82,  84,  86,  88,  89, 
   91,  93,  94,  96,  97,  99, 101, 102, 103, 105, 106, 107, 109, 110, 111, 
  112, 113, 114, 115, 116, 117, 118, 119, 120, 120, 121, 121, 122, 122, 123, 
  123, 123, 123, 124, 124, 124, 124, 124, 123, 123, 123, 123, 122, 122, 121, 
  121, 120, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 
  106, 105, 103, 102, 101,  99,  97,  96,  94,  93,  91,  89,  88,  86,  84, 
   82,  80,  79,  77,  75,  73,  71,  69,  68,  66,  64,  62,  60,  58,  56, 
   54,  53,  51,  49,  47,  45,  43,  42,  40,  38,  37,  35,  33,  32,  30, 
   28,  27,  26,  24,  23,  21,  20,  19,  18,  16,  15,  14,  13,  12,  11, 
   10,   9,   9,   8,   7,   7,   6,   6,   5,   5,   4,   4,   4,   4,   4, 
   04,   4,   4,   4,   4,   4,   5,   5,   6,   6,   7,   7,   8,   9,   9, 
   10,  11,  12,  13,  14,  15,  16,  18,  19,  20,  21,  23,  24,  26,  27, 
   28,  30,  32,  33,  35,  37,  38,  40,  42,  43,  45,  47,  49,  51,  53, 
   54,  56,  58,  60,  62 };

DTMFGenerator::DTMFGenerator()
{
  // Set R-2R ladder port as an output port.
  DTMF_DDR = 0xFF;

  // Update global object with current class instance.
  generatorInstance = this;

  // Setup global variables with initial values.
  dtmfRowPos = 0;
  dtmfColPos = 0;
}

void DTMFGenerator::generate(char key, unsigned long duration)
{
  unsigned long timerCycle = 0;
  
  // This library allows numerical characters, # and * only.
  if(((key >= CHAR_ZERO) && (key <= CHAR_NINE)) || (key == CHAR_HASH) || (key == CHAR_STAR))
  {
    // Distinguish the low-frequency and high-frequency components of the DTMF waveform.
    switch(key)
    {
      case CHAR_ONE:
        rowOffset = ROW_697;
        colOffset = COL_1209;
        break;
      case CHAR_TWO:
        rowOffset = ROW_697;
        colOffset = COL_1336;
        break;
      case CHAR_THREE:
        rowOffset = ROW_697;
        colOffset = COL_1477;
        break;
      case CHAR_FOUR:
        rowOffset = ROW_770;
        colOffset = COL_1209;
        break;
      case CHAR_FIVE:
        rowOffset = ROW_770; 
        colOffset = COL_1336;
        break;
      case CHAR_SIX:
        rowOffset = ROW_770;
        colOffset = COL_1477;
        break;
      case CHAR_SEVEN:
        rowOffset = ROW_852;
        colOffset = COL_1209;
        break;
      case CHAR_EIGHT:
        rowOffset = ROW_852;
        colOffset = COL_1336;
        break;
      case CHAR_NINE:
        rowOffset = ROW_852;
        colOffset = COL_1477;
        break;
      case CHAR_STAR:
        rowOffset = ROW_941;
        colOffset = COL_1209;
        break;
      case CHAR_ZERO:
        rowOffset = ROW_941;
        colOffset = COL_1336;
        break;
      case CHAR_HASH:
        rowOffset = ROW_941;
        colOffset = COL_1477;
        break;
    }

    // Start waveform from VCC/2.
    dtmfRowPos = 0;
    dtmfColPos = 0;

    // Start TIMER1 with frequency of 10KHz.
    noInterrupts();

    // Initialize TIMER1 registers to known state. 
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 = 0;

    // Set TIMER1 to 10KHz with reference to 16MHz Arduino clock. 
    OCR1A = 24;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // Enable TIMER1 interrupt.
    TIMSK1 |= (1 << OCIE1A);
    interrupts();

    // Wait for specified duration.
    while(timerCycle < duration)
    {
      timerCycle++;
      delay(1);
    }

    // Disable TIMER1 interrupt and clear all TIMER1 associated registers.
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 = 0;
    interrupts();
    
    // Reset DTMF output to VCC/2.
    DTMF_PORT = waveTable[0] + waveTable[0];
  }
}

void DTMFGenerator::waveGenerator()
{
  // Generate output waveform by mixing low-frequency and high-frequency waveforms.
  DTMF_PORT = waveTable[dtmfRowPos] + waveTable[dtmfColPos];

  dtmfRowPos += rowOffset;
  dtmfColPos += colOffset;

  // Check for end of low-frequency wave cycle.
  if(dtmfRowPos >= WAVE_TABLE_SIZE)
  {
    dtmfRowPos -= WAVE_TABLE_SIZE;
  }

  // Check for end of high-frequency wave cycle.
  if(dtmfColPos >= WAVE_TABLE_SIZE)
  {
    dtmfColPos -= WAVE_TABLE_SIZE;
  }
}

void DTMFGenerator::generateOutput()
{
  if(generatorInstance != NULL)
  {
    generatorInstance->waveGenerator();
  }
}

ISR(TIMER1_COMPA_vect)
{
  DTMFGenerator::generateOutput();
}
