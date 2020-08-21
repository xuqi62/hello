#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

struct ion_func{
    int (*open)();
    int (*legacy)(int fd);
    int (*alloc_fd)(int fd, size_t len, size_t align, unsigned int heap_mask,
              unsigned int flags, int *handle_fd);
    int (*close)(int fd);
};

enum ion_heap_ids {
    INVALID_HEAP_ID = -1,
    ION_CP_MM_HEAP_ID = 8,
    ION_SECURE_HEAP_ID = 9,
    ION_SECURE_DISPLAY_HEAP_ID = 10,
    ION_CP_MFC_HEAP_ID = 12,
    ION_CP_WB_HEAP_ID = 16, /* 8660 only */
    ION_CAMERA_HEAP_ID = 20, /* 8660 only */
    ION_SYSTEM_CONTIG_HEAP_ID = 21,
    ION_ADSP_HEAP_ID = 22,
    ION_PIL1_HEAP_ID = 23, /* Currently used for other PIL images */
    ION_SF_HEAP_ID = 24,
    ION_SYSTEM_HEAP_ID = 25,
    ION_PIL2_HEAP_ID = 26, /* Currently used for modem firmware images */
    ION_QSECOM_HEAP_ID = 27,
    ION_AUDIO_HEAP_ID = 28,

    ION_MM_FIRMWARE_HEAP_ID = 29,

    ION_HEAP_ID_RESERVED = 31 /** Bit reserved for ION_FLAG_SECURE flag */
};
#define ION_HEAP(bit) (1 << (bit))
#define ION_FLAG_CACHED 1
#define ION_FLAG_CACHED_NEEDS_SYNC 2

static int save_file(const char* filename)
{
    FILE* fp = NULL;
    char cmd[512];
    sprintf(cmd, "cat /proc/%d/smaps > %s", getpid(), filename);
    printf("cmd: %s \n", cmd);
    fp = popen(cmd, "r");
    if(fp == NULL)
    {
        return -1;
    }
    pclose(fp);
    printf("==== save file \n");
    return 0;
}

int main(int argc, char * argv[])
{
    save_file("start.txt");
    void* dl_ion = NULL;
    dl_ion = dlopen("libion.so", RTLD_NOW);
	if (!dl_ion) {
        printf("failed to load library libion.so! \n");
        printf("dlerror=%s\nerrno=%d\nstrerror=%s\n", dlerror(), errno, strerror(errno));
    }
    printf("====== dlopen ion success");

    struct ion_func ion_ops;

    printf("st_night_pid=%d \n", getpid());


    ion_ops.open = dlsym(dl_ion, "ion_open");
	if (!ion_ops.open) {
		printf("Can't found function: ion_open \n");
        return -1;
	}

    ion_ops.close = dlsym(dl_ion, "ion_close");
	if (!ion_ops.close) {
		printf("Can't found function: ion_close \n");
        return -1;
	}

    ion_ops.legacy = dlsym(dl_ion, "ion_is_legacy");
	if (!ion_ops.legacy) {
		printf("Can't found function: ion_legacy \n");
        return -1;
	}

    ion_ops.alloc_fd = dlsym(dl_ion, "ion_alloc_fd");
	if (!ion_ops.alloc_fd) {
		printf("Can't found function: ion_alloc_fd \n");
        return -1;
	}

    int ion_fd = ion_ops.open();
    if(ion_ops.legacy(ion_fd) == 1)
    {
        printf("===> ion legacy \n");
    }
    save_file("open_ion_fd.txt");

    int len = 4096*1024;
    unsigned int heap_id_mask = ION_HEAP(ION_SYSTEM_HEAP_ID);
    unsigned int flags = ION_FLAG_CACHED;
    int map_fd;
    ion_ops.alloc_fd(ion_fd, len, 0, heap_id_mask, flags, &map_fd);

    char* addr = (char*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0);
    save_file("alloc_fd.txt");

    munmap(addr, len);
    close(map_fd);
    save_file("close_map_fd.txt");
    ion_ops.close(ion_fd);
    save_file("close_ion.txt");

    dlclose(dl_ion);
    save_file("dlclose.txt");

    return 0;
}
