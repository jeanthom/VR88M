/*
    Copyright (c) 2017 Jean THOMAS.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

struct OneByteInstruction {
  unsigned char opcode;
  const char *name;
  const char *format;
};

struct OneByteInstruction oneByteInstructions[] = {
  {0x00, "INC", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x01, "ADC", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x02, "TX0", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x03, "OR", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x04, "AND", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x05, "XOR", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x06, "ROL", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x07, "ROR", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x08, "DEC", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x09, "SBC", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x0A, "ADD", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x0B, "STP", "%04lx:\t%02x\t%s\t%d\n"},
  {0x0C, "BTT", "%04lx:\t%02x\t%s\t%d\n"},
  {0x0D, "CLP", "%04lx:\t%02x\t%s\t%d\n"},
  {0x0E, "T0X", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x0F, "CMP", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x10, "PSH", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x11, "POP", "%04lx:\t%02x\t%s\tR%d\n"},
  {0x15, "INT", "%04lx:\t%02x\t%s\t%d\n"},
  {0x16, "MUL", "%04lx:\t%02x\t%s\tR%d\n"},
};
