#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern "C" bool debug_dump_heap(const char* file_name);
int main()
{
	int c;
	char* p = (char*)malloc(1);
	printf("p: %p \n", p);
	p[2000] = 1;
	free(p);
	//char a = p[5000];
	//char* b = (char*)malloc(20000);
//	pause();
	//printf("a: %x \n", a);
	//free(b);

	debug_dump_heap("/sdcard/debug_mem/test.txt");
	return 0;
}
