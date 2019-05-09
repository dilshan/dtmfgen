# Arduino DTMF Generator
 
This is a library to generate DTMF tones only using *Arduino Uno*. It is designed to work with R-2R ladder based DAC to produce DTMF tones.
 
For more information about this library please check jayakody2000lk.blogspot.com.

## DTMF Generator API

### DTMFGenerator class
Try to create only single instance of DTMFGenerator class in global space. For example:

    #include "dtmfgen.h"
    
    DTMFGenerator dtmf;
    
### Class methods

#### generate
**Description:** Generate DTMF tone for specified symbol or number.

**Syntax:**

    generate(key, duration)
**Parameters:**

 - *key*: Symbol or digit as a character. 
 - *duration*: Length of the tone in milliseconds.

**Returns:** none.

**Example:**

    dtmf.generate('3', 100);

Above example generate DTMF tone relates to digit 3 for 100 milliseconds.

## License

Copyright (c) 2019 Dilshan R Jayakody.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
