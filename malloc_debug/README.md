1. libc_malloc_debug.so是android bionic的debug库，库或可执行文件链接该so就可以测试内存泄露或内存越界
    源码在android sdk以下路径： android/bionic/libc/malloc_debug
2. 在程序退出时，使用debug_dump_heap 保存堆栈信息，用于最后生成内存报告
3. 编译并把库和可执行文件推到手机
 使用ndk编译需要 -g 参数，否则在生成report时无法定位函数位置

4. 并把库和可执行文件推到手机，执行
   adb shell stop 
   
adb shell setprop libc.debug.malloc.options backtrace 

adb shell setprop libc.debug.malloc.program test 

adb shell export LIBC_DEBUG_MALLOC_OPTIONS=backtrace 

adb shell export LIBC_DEBUG_MALLOC_ENABLE=1 

adb shell start 


5. 运行完毕后，会生成相应的文件，adb pull 到本地pc
如：adb pull /sdcard/debug_mem/   ./ 

 并把库和可执行文件放到pc相应路径下，比如，so在手机的/data/目录下，需要把so拷出来放到pc的/data/目录下，
 因为生成的文件记录的动态库路径在/data下。（这里的库必须是带-g编译选项的，android sdk的out/xxx/symbol下的库）

6. 执行native_heapdump_viewer.py 工具，即可将相应的文件生成report 
如：
python  native_heapdump_viewer.py --verbose --reverse debug_mem/mem.txt > ${OUTPUT_DIR}/mem_report.txt 

或者（生成html的报告）
python  native_heapdump_viewer.py --verbose --reverse --html  debug_mem/mem.txt > ${OUTPUT_DIR}/mem.html 


7. 检查内存越界，输入以下命令，并执行程序，从log中会打印 
export LIBC_DEBUG_MALLOC_OPTIONS=backtrace 

