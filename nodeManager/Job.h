/*
 * =====================================================================================
 *
 *       Filename:  Job.h
 *
 *    Description:  execute "jobs" on a seperate thread
 *
 *        Version:  1.0
 *        Created:  07/14/2016 06:29:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rice Shelley
 *   Organization:  NONE 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void create_job(char* cmd, char* ip_addr, int jRPort) {
	pid_t pid = fork();
	if (pid == 0)
	{
		// do sys call send output to file
		strcat(cmd, " > output");
		system(cmd);
		// get output of system call
		FILE* outputf = fopen("output", "r");
		char output[60000];
		memset(output, '\0', 60000);
		strcpy(output, "SYSJDONE>");
		fread(&output[5], sizeof(char), 59990, outputf);
		fclose(outputf);
		if (jRPort != -1)
		{
			// attempt connection to socket at suplied address 
			struct sockaddr_in server_addr;
			int sockID = socket(AF_INET, SOCK_STREAM, 0);
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(jRPort);
			if (inet_pton(AF_INET, (const char*) ip_addr, &server_addr.sin_addr.s_addr) <= 0)
			{
				printf("inet_pton error\n");
			}
			if (connect(sockID, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
			{
				perror("failed to connect to job server");
				close(sockID);
				exit(0);
			}

			// print output <- test
			send(sockID, (const char*) output, strlen(output), 0); 
			close(sockID);	
		}
		exit(0);
	}
}

