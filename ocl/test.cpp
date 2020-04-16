#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

static int64_t get_now_us()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)tv.tv_usec + tv.tv_sec*1000000ll;
}

static int print_meminfo_t(const char* str)
{
    printf("************** [%s] begin print memory **************\n", str);
    static int pid = 0;
    if (pid == 0) {
        pid = getpid();
        printf("pid: %d \n", pid);
    }
    // sleep(2);
    FILE *fp = NULL;
    char result[256] = {0};
    char CMD[100] = {0};
    int end = sprintf(CMD, "dumpsys meminfo ");
    end += sprintf(CMD + end, "%d", pid);
    sprintf(CMD + end, " |grep -E 'TOTAL:|Native Heap:|Graphics:|Code:'");
    printf("CMD: %s \n", CMD);
    if ((fp = popen(CMD, "r")) != NULL) {
        printf("=======>memory used: \n");
        while (fgets(result, sizeof(result), fp)) {
            printf("%s \n", result);
        }
        pclose(fp);
    }
    printf("************** [%s] end print memory ************** \n", str);
    //sleep(2);
}

const char hello_kernel[] =
    "__kernel void helloworld(__global float *pIn1, __global float *pIn2, __global float *pOut)\n"
    "{      \n"
    "   int i = get_global_id(0); \n"
    "   float4 layer1 = vload4(i, pIn1); \n"
    "   float4 layer2 = vload4(i, pIn2); \n"
    "   float4 tmp = exp(layer2); \n"
    "   vstore4(tmp/(exp(layer1) + tmp), i, pOut); \n"
    "}              \n";

// const char hello_kernel[] =
//     "__kernel void helloworld(__global float *pIn1, __global float *pIn2, __global float *pOut)\n"
//     "{      \n"
//     "   int i = get_global_id(0); \n"
//     "   float tmp = exp(pIn2[i]); \n"
//     "   pOut[i] = tmp / (exp(pIn1[i]) + tmp); \n"
//     "}              \n";

int main(int argc, char* argv[])
{
    cl_int			status = 0;				// 函数返回状态
    cl_uint			uiNumPlatforms = 0;				// 平台个数
    cl_platform_id	platform;				// 选择的平台
    size_t			uiSize = 0;				// 平台版本名字字节数
    cl_int			iErr = 0;				// 返回参数
    char			*pName = NULL;				// 平台版本名
    cl_uint			uiNumDevices = 0;				// 设备数量
    cl_device_id	devices;				// 设备
    cl_context		Context = NULL;				// 设备环境
    cl_command_queue	CommandQueue = NULL;			// 命令队列
    const char		*pFileName = "hello.cl";	// cl文件名
    const char		*pSource;							// 代码字符串指针
    size_t 			source_size;			// 代码字符串长度
    cl_program		Program = NULL;				// 程序对象
    const char		*pInput = "gdkknvnqkc";		// 输入字符串
    size_t			uiStrlength = strlen(hello_kernel);	// 输入字符串长度
    char			*pOutput = NULL;				// 输出字符串
    cl_mem			mem_input1 = NULL;				// 输入内存对象
    cl_mem			mem_input2 = NULL;				//
    cl_mem			mem_output = NULL;				// 输出内存对象
    cl_kernel		Kernel = NULL;				// 内核对象
    size_t			uiGlobal_Work_Size[1] = { 0 };		// 用于设定内核分布
    int   data_size = 4000* 3000;

    //-------------------1. 获得并选择可用平台-----------------------------
    // 查询可用的平台个数，并返回状态
	status = clGetPlatformIDs(1, &platform, NULL);
	if (0 != status)
	{
		printf("Error: Getting platforms error \n");
		return -1;
	}

	//--------------2. 查询GPU设备，并选择可用设备------------------------
	// 获得GPU设备数量
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &devices, NULL);
	if(0 != status)
	{
        printf("get devices failed \n");
        return -1;
	}

	// -------------------3.创建设备环境---------------------------------
	// 创建设备环境
	Context = clCreateContext(NULL, 1, &devices, NULL, NULL, &status);
	if (NULL == Context)
	{
        printf("Error: Can not create context \n");
		return -1;
	}

	// -------------------4.创建命令队列--------------------------------------
	// 创建第1个设备的命令队列
	CommandQueue = clCreateCommandQueue(Context, devices, 0, NULL);
	if (NULL == CommandQueue)
	{
		printf("Error: Can not create command queue \n");
		return 0;
	}

    /*
     * Query the device to find out it's prefered integer vector width.
     * Although we are only printing the value here, it can be used to select between
     * different versions of a kernel.
     */
    cl_uint integerVectorWidth;
    clGetDeviceInfo(devices, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(cl_uint), &integerVectorWidth, NULL);
    printf("Prefered vector width for integers: %d \n", integerVectorWidth);


	// ----------------------5. 创建程序对象------------------------------
	pSource = hello_kernel;			// 获得strSource指针
	source_size = strlen(pSource);	// 字符串大小

	// 创建程序对象
	Program = clCreateProgramWithSource(Context, 1, &pSource, &source_size, NULL);
	if (NULL == Program)
	{
		printf("Error: Can not create program \n");
		return 0;
	}


	// -----------------------------6. 编译程序--------------------------------
	// 编译程序
	status = clBuildProgram(Program, 1, &devices, NULL, NULL, NULL);
	if (CL_SUCCESS != status)	// 编译错误
	{
		printf("Error: build program failed \n");
		char szBuildLog[16384];
		clGetProgramBuildInfo(Program, devices, CL_PROGRAM_BUILD_LOG, sizeof(szBuildLog), szBuildLog, NULL);

		printf("log: %s \n", szBuildLog);
		clReleaseProgram(Program);

		return 0;
	}

	//-------------------------7. 并创建输入输出内核内存对象--------------------------------

	// 创建输入内存对象
	mem_input1 = clCreateBuffer(
		Context,
		CL_MEM_READ_ONLY,  // 输入内存为只读，并可以从宿主机内存复制到设备内存
		data_size * sizeof(float),		  // 输入内存空间大小
		NULL,
		NULL);

    // 创建输入内存对象
	mem_input2 = clCreateBuffer(
		Context,
		CL_MEM_READ_ONLY,  // 输入内存为只读，并可以从宿主机内存复制到设备内存
		data_size * sizeof(float),		  // 输入内存空间大小
		NULL,
		NULL);

	// 创建输出内存对象
	mem_output = clCreateBuffer(
		Context,
		CL_MEM_WRITE_ONLY,					// 输出内存只能写
		data_size * sizeof(float),	// 输出内存空间大小
		NULL,
		NULL);


        print_meminfo_t("create buffer");

	//--------------------------8. 创建内核对象-------------------------------------
	Kernel = clCreateKernel(Program,
		"helloworld",  // cl文件中的入口函数
		NULL);
	if (NULL == Kernel)
	{
        printf("create kernel failed");
		return 0;
	}

	//----------------------------9. 设置内核参数----------------------------------
	status = clSetKernelArg(Kernel, 0, sizeof(cl_mem),(void *)&mem_input1);
    status = clSetKernelArg(Kernel, 1, sizeof(cl_mem),(void *)&mem_input2);
	status = clSetKernelArg(Kernel, 2, sizeof(cl_mem), (void *)&mem_output);

    float* in_layer1 = (float*)malloc(data_size* sizeof(float));
    float* in_layer2 = (float*)malloc(data_size* sizeof(float));
    float* out       = (float*)malloc(data_size* sizeof(float));
    for(int i=0; i<data_size; i++)
    {
        in_layer1[i] = 1.0;
        in_layer2[i] = 1.0;
    }
    clEnqueueWriteBuffer(CommandQueue, mem_input1, CL_TRUE, 0,
        data_size * sizeof(float), in_layer1, 0, NULL, NULL);
    clEnqueueWriteBuffer(CommandQueue, mem_input2, CL_TRUE, 0,
        data_size * sizeof(float), in_layer1, 0, NULL, NULL);
	// --------------------------10.运行内核---------------------------------
	uiGlobal_Work_Size[0] = data_size / 4;

    int64_t start = get_now_us();
	// 利用命令队列使将再设备上执行的内核排队
	status = clEnqueueNDRangeKernel(
		CommandQueue,
		Kernel,
		1,
		NULL,
		uiGlobal_Work_Size,  // 确定内核在设备上的多个处理单元间的分布
		NULL,				 // 确定内核在设备上的多个处理单元间的分布
		0,
		NULL,
		NULL);


	if (CL_SUCCESS != status)
	{
		cout << "Error: Can not run kernel" << endl;
		return 0;
	}

	// ----------------------------11. 将输出读取到主机内存

	status = clEnqueueReadBuffer(
		CommandQueue,		// 命令队列
		mem_output,	// 输出内存对象
		CL_TRUE,			// 内核读取结束之前该函数不会返回
		0,
		data_size * sizeof(float),
		out,
		0,
		NULL,
		NULL);

	if (CL_SUCCESS != status)
	{
		printf("Error: Can not reading result buffer, %d\n", status);
		return 0;
	}
    int64_t end = get_now_us();

    printf("ocl time: %lld \n", end-start);

    for(int i=0; i<data_size; i++)
    {
        if(out[i] != 0.5)
        {
            printf(" error, out[%d], %f", i, out[i]);
            return -1;
        }
    }

    print_meminfo_t("1111111111");



	// ---------------------12--输出计算结果---------------
	printf(" %f, %f, %f \n", in_layer1[0], in_layer2[0], out[0]);

    start = get_now_us();
    float tmp;
    for(int i=0; i<data_size; i++)
    {
        tmp = exp(in_layer2[i]);
        out[i] = tmp /(tmp + exp(in_layer1[i]));
    }
    end = get_now_us();
    printf(" cpu time: %lld \n", end-start);
    printf(" %f, %f, %f \n", in_layer1[0], in_layer2[0], out[0]);
 print_meminfo_t("222222");
	// -------------------------------13. 释放资源--------------------------------
	clReleaseKernel(Kernel);
	clReleaseProgram(Program);

    clReleaseMemObject(mem_input1);
    clReleaseMemObject(mem_input2);
	clReleaseMemObject(mem_output);
	clReleaseCommandQueue(CommandQueue);
	clReleaseContext(Context);

	free(in_layer1);
    free(in_layer2);
    free(out);
    print_meminfo_t("33333");

    pause();

	return 0;
}