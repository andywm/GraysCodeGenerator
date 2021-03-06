/*------------------------------------------------------------------------------
	()      File:   metatypes.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Bit utilities
------------------------------
------------------------------
License Text - The MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------*/

#define SET_ENUM_BIT(x) 0x1 << 1
#define GET_ENUM_BIT(x) 0x1 >> 1
#define TRY_SET_BIT(DO, BIT) (DO? BIT : 0)

// move byte 3 to byte 0  // move byte 1 to byte 2 /// move byte 2 to byte 1 // byte 0 to byte 3
#define FLIP_ENDIAN(x) ( ((x >> 24) & 0xff) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) |  ((x << 24) & 0xff000000) )