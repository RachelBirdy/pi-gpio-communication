#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void sendChar(char *currentChar) {
	// Write the output pins
	for(int i = 0; i < 7; i++) {
                if(*currentChar & (int) pow(2, i)) {
                        gpioWrite(i, 1);
                } else {
                        gpioWrite(i, 0);
                }
        }
	return;
}

void getChar(char *recvChar) {
	for(int i = 8; i < 15; i++) {
                *recvChar += pow(2, i - 8) * gpioRead(i);
        }
	return;
}

void flipSignal(char *signalBit) {
	gpioWrite(7, *signalBit ? 0 : 1);
	*signalBit = *signalBit ? 0 : 1;
	return;
}

void communication(char *sendString, char *recvString) {
	char sendSignalBit = 0;
	char recvSignalBit = gpioRead(15);


	if(recvSignalBit) {
                for (int i = 0; i < 81920; i++) {
                        sendChar(&sendString[i]);
                        flipSignal(&sendSignalBit);
                        getChar(&recvString[i]);
                        while(gpioRead(15) == recvSignalBit);
                        recvSignalBit = recvSignalBit ? 0 : 1;
                }
        } else {
                for (int i = 0; i < 81920; i++) {
			flipSignal(&sendSignalBit);
                        sendChar(&sendString[i]);
                        while(gpioRead(15) == recvSignalBit);
                        recvSignalBit = recvSignalBit ? 0 : 1;
                        getChar(&recvString[i]);
                }
        }
        flipSignal(&sendSignalBit);
        return;
}

int main(int argc, char *argv[]) {
	if(argc == 1) {
		printf("Error. No. \n");
		return 1;
	}

	char *sendString = calloc(81920, sizeof(char));
	strcpy(sendString, argv[1]);
	char *recvString = calloc(81920, sizeof(char));
	gpioInitialise();

	// Set pin input and output modes
	for(int i = 0; i < 8; i++) gpioSetMode(i, PI_OUTPUT);
	for(int i = 8; i < 16; i++) gpioSetMode(i, PI_INPUT);
	gpioWrite(7,0);

	clock_t start, end;
        double cpu_time_used;
	start = clock();
	communication(sendString, recvString);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("%s\n",recvString);
	printf("Took %f seconds\n",cpu_time_used);
	gpioWrite(7,0);
	gpioTerminate();
	return 0;
}
