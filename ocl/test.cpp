#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <CL/cl.h>
#include <unistd.h>

int main(void)
{
    cl_int ret;
    
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_mem memObj1 = NULL;
    cl_mem memObj2 = NULL;
    char *kernelSource = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    int *pInputBuffer1 = NULL;
    int *pInputBuffer2 = NULL;
    int *pOutputBuffer = NULL;
    int count = 5;  // 执行5次循环
    
    clGetPlatformIDs(1, &platform_id, NULL);
    if(platform_id == NULL)
    {
        printf("Get OpenCL platform failed! \n");
        return -1;
    }
    
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if(device_id == NULL)
    {
        printf("No GPU available as a compute device! \n");
        return -1;
    }
    
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    if(context == NULL)
    {
        printf("Context not established!");
        return -1;
    }
    
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    if(command_queue == NULL)
    {
        printf("Command queue cannot be created!");
        return -1;
    }
    

    
    const size_t contentLength = 4 * 1024 * 1024;
    printf("create buffer start \n");
    sleep(20);
    
    memObj1 = clCreateBuffer(context, CL_MEM_READ_WRITE, contentLength, NULL, &ret);
    if(memObj1 == NULL)
    {
        printf("Memory object1 failed to create! \n");
        return -1;
    }
    printf("create buffer end \n");
    pause();
    
    
    return 0;
}
