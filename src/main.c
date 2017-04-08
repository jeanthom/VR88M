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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include "main.h"

char signExtend(unsigned char inputChar) {
  return *((char *)&inputChar);
}

int main(int argc, char *argv[]) {
  char *filename = NULL;
  int getoptReturn;
  unsigned int baseAddr = 0;
  unsigned char c;
  unsigned char opcode;
  unsigned char instructionBuffer[3];
  unsigned char bufPos = 0;
  int i;
  FILE* fh;

  /* Parsing arguments */
  while ((getoptReturn = getopt(argc, argv, "i:b:")) != -1) {
    switch (getoptReturn) {
    case 'i':
      filename = optarg;
      break;
    case 'b':
      if (sscanf(optarg, "%x", &baseAddr) != 1) {
	fprintf(stderr, "Error : Wrong value for base address. Make sure you are using a hexadecimal value, without \"0x\" prefix or \"h\" suffix.\n");
	return EXIT_FAILURE;
      }
      break;
    case '?':
      printf("Usage:\n\tvr88m -i inputfile\n\tvr88m -i inputfile -b baseaddress\n");
      return EXIT_FAILURE;
      break;
    default:
      abort();
    }
  }

  /* No file provided */
  if (filename == NULL) {
    printf("Please provide a file to disassemble with -i option.\n");
    printf("Example : vr88m -i my_file.bin\n");
    return EXIT_SUCCESS;
  }

  /* Opening input file */
  fh = fopen(filename, "rb");
  if (fh == NULL) {
    perror("Error");
    return EXIT_FAILURE;
  }

  /* Where the magic happens… The machine code is read a byte at a time.
     The instructions are handled in two ways :
     - One byte instructions are directly handled. Most of the one byte instructions are configured in main.h and use very few lines of code because they are pretty similar
     - Multiple bytes instructions are handled through a buffer (unsigned char instructionsBuffer[3])
   */
  fread(&c, 1, 1, fh);
  while (!feof(fh)) {
    if (bufPos > 0) {
      opcode = instructionBuffer[0] >> 3;
      
      /* BR0 */
      if (opcode == 0x12) {
	printf("%04lx:\t%02x %02x\tBR0\t%d, %d\n",
	       (long)ftell(fh)-2+baseAddr,
	       instructionBuffer[0],
	       c,
	       instructionBuffer[0] & 0x7,
	       signExtend(c));
	bufPos = 0;
      }

      /* BR1 */
      else if (opcode == 0x13) {
	printf("%04lx:\t%02x %02x\tBR1\t%d, %d\n",
	       (long)ftell(fh)-2+baseAddr,
	       instructionBuffer[0],
	       c,
	       instructionBuffer[0] & 0x7,
	       signExtend(c));
 	bufPos = 0;
      }

      /* DBNZ */
      else if (opcode == 0x14) {
	printf("%04lx:\t%02x %02x\tDBNZ\tR%d, 0x%x\n",
	       (long)ftell(fh)-2+baseAddr,
	       instructionBuffer[0],
	       c,
	       instructionBuffer[0] & 0x7,
	       c);
	bufPos = 0;
      }

      /* JSR */
      else if (instructionBuffer[0] == 0xBF) {
	if (bufPos == 1) {
	  instructionBuffer[1] = c;
	  bufPos++;
	} else {
	  printf("%04lx:\t%02x %02x %02x\tJSR\t0x%04x\n",
		 (long)ftell(fh)-3+baseAddr,
		 instructionBuffer[0],
		 instructionBuffer[1],
		 c,
		 instructionBuffer[1] | c << 8);
	  bufPos = 0;
	}
      }

      /* LDA */
      else if (opcode == 0x1D) {
	if (bufPos == 1) {
	  instructionBuffer[1] = c;
	  bufPos++;
	} else {
	  printf("%04lx:\t%02x %02x %02x\tLDA\tR%d, 0x%04x\n",
		 (long)ftell(fh)-3+baseAddr,
		 instructionBuffer[0],
		 instructionBuffer[1],
		 c,
		 instructionBuffer[0] & 0x07,
		 instructionBuffer[1] | c << 8);
	  bufPos = 0;
	}
      }

      /* LDI */
      else if (opcode == 0x1C) {
	printf("%04lx:\t%02x %02x\tLDI\tR%d, 0x%02x\n",
	       (long)ftell(fh)-3+baseAddr,
	       instructionBuffer[0],
	       c,
	       instructionBuffer[0] & 0x07,
	       c);
	bufPos = 0;
      }

      /* STA */
      else if (opcode == 0x19) {
	if (bufPos == 1) {
	  instructionBuffer[1] = c;
	  bufPos++;
	} else {
	  printf("%04lx:\t%02x %02x %02x\tSTA\tR%d, 0x%02x\n",
		 (long)ftell(fh)-3+baseAddr,
		 instructionBuffer[0],
		 instructionBuffer[1],
		 c,
		 instructionBuffer[0] & 0x07,
		 instructionBuffer[1] | c << 8);
	  bufPos = 0;
	}
      }

      /* JMP */
      else if (instructionBuffer[0] == 0xBC) {
	if (bufPos == 1) {
	  instructionBuffer[1] = c;
	  bufPos++;
	} else {
	  printf("%04lx:\t%02x %02x %02x\tJMP\t0x%04x\n",
		 (long)ftell(fh)-3+baseAddr,
		 instructionBuffer[0],
		 instructionBuffer[1],
		 c,
		 instructionBuffer[1] | c << 8);
	  bufPos = 0;
	}
      }
    } else {
      opcode = c >> 3;
      bool handled = false;

      for (i = 0; i < sizeof(oneByteInstructions)/sizeof(struct OneByteInstruction); i++) {
	if (oneByteInstructions[i].opcode == opcode) {
	  printf(oneByteInstructions[i].format,
		 ftell(fh)-1+baseAddr,
		 c,
		 oneByteInstructions[i].name,
		 c & 0x7);
	  handled = true;
	  break;
	}
      }

      if (!handled) {
	/* BR0 */
	if (opcode == 0x12) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* BR1 */
	else if (opcode == 0x13) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* DBNZ */
	else if (opcode == 0x14) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* JSR */
	else if (c == 0xBF) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* RTI */
	else if (c == 0xBA) {
	  printf("%04lx:\t%02x\tRTI\n",
		 ftell(fh)-1+baseAddr,
		 c);
	}

	/* LDA */
	else if (opcode == 0x1D) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* LDI */
	else if (opcode == 0x1C) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* STA */
	else if (opcode == 0x19) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* JMP */
	else if (c == 0xBC) {
	  bufPos++;
	  instructionBuffer[0] = c;
	}

	/* RTS */
	else if (c == 0xB9) {
	  printf("%04lx:\t%02x\tRTS\n",
		 ftell(fh)-1+baseAddr,
		 c);
	}

	/* STX */
	else if (opcode == 0x1A) {
	  if (c & 0x1) {
	    printf("%04lx:\t%02x\tSTX\tR%d[++]\n",
		   ftell(fh)-1+baseAddr,
		   c,
		   (c & 0x6) > 1);
	  } else {
	    printf("%04lx:\t%02x\tSTX\tR%d\n",
		   ftell(fh)-1+baseAddr,
		   c,
		   (c & 0x6) > 1);
	  }
	}

	else {
	  printf("%04lx:\tUnhandled instruction : %02x\n",
		 ftell(fh)-1+baseAddr,
		 c);
	}
      }
    }

    fread(&c, 1, 1, fh);
  }
  
  return EXIT_SUCCESS;
}
