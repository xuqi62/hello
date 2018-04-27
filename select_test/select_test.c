#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>

static int read_command(char* buf, int size)
{
	fd_set readfd;
	int nMaxfds;
	int ret;
	int read_size = 0;
		
	printf("\ninput:");
	fflush(stdout);
	nMaxfds = 0;
	FD_ZERO(&readfd);
	FD_SET(STDIN_FILENO, &readfd);
	ret = select(nMaxfds+1, &readfd, NULL, NULL, NULL);

	if(ret > 0)
	{
		read_size = read(STDIN_FILENO, buf, size );
		if(read_size == size)
		{
			buf[size-1] = '\0';
		}
		printf("%s", buf);
	}

}

#define COM_SIZE 1024
int main()
{
	int quit = 0;
	char command[COM_SIZE];
	printf("hello\n");
	while(!quit)
	{
		
		read_command((char*)&command, COM_SIZE);
		if(strncmp((char*)command, "quit", 4) == 0)
			quit = 1;
	}
return 0;
}
