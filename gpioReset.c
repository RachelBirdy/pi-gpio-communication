#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if(argc == 1) {
		printf("Error. No. \n");
		return 1;
	}

	gpioInitialise();

	// Set pin input and output modes
	for(int i = 0; i < 8; i++) printf("%d", gpioSetMode(i, PI_OUTPUT));
	printf("\n");
	for(int i = 8; i < 16; i++) printf("%d", gpioSetMode(i, PI_INPUT));
	printf("\n");

	for(int i = 0; i < 8; i++) {
		gpioWrite(i, 0);
	}

	gpioTerminate();
	return 0;
}
