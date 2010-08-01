#include <stdio.h>
#include <stdlib.h>
#include "iface.h"

#include "fakeiface.h"

int main(int argc, char *argv[])
{
	int i;

	for(i = 1; i < argc; i++) {
		char b1[256];
		char *infile = argv[i];
		char *b;
		
		b = basename(infile);
		snprintf(b1, 256, "../OUTPUTS/recv-01-%s", b);
		process_infile(infile, b1);
	}
	exit(0);
}


