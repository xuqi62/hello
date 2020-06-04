/*
 * 验证申请内存的 copy on write 行为
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("sleep 20s, run cat /proc/%d/smaps > start.txt \n", getpid());
    sleep(20);

    char* a = (char*)malloc(1024*1024);

    printf("malloc end, run cat /proc/%d/smaps > malloc.txt \n", getpid());
    sleep(20);

    memset(a, 0, 1024*1024);

    printf("memset , run cat /proc/%d/smaps > memset.txt \n", getpid());
    sleep(20);


    return 0;
}