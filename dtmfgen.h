/* Copyright 2019 Dilshan R Jayakody.
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

#ifndef DTMF_GENERATOR_H
#define DTMF_GENERATOR_H

#if !defined(__AVR_ATmega328P__)
  #error Unsupported board. This library support ATmega328P based Arduino Uno boards only.	
#endif

class DTMFGenerator
{
  public:
    DTMFGenerator();
    void generate(char key, unsigned long duration);
    static void generateOutput();
    
  private:
    void waveGenerator();
    
  private:
    static unsigned char dtmfRowPos;
    static unsigned char dtmfColPos;

    static unsigned char rowOffset;
    static unsigned char colOffset;
};

// Port with R-2R ladder circuit. By default use PORTD (Digital 0 to 7).
#ifndef DTMF_PORT
#define DTMF_PORT PORTD
#endif

// Data direction register which associated with above R-2R ladder port.
#ifndef DTMF_DDR
#define DTMF_DDR DDRD
#endif

#endif
