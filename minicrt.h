// minicrt.h
#ifndef __MINI_CRT_H__
#define __MINI_CRT_H__

/*定义C++相关的函数，以使得函数的兼容性得到满足，但是依旧按照C的修饰规则来进行导出函数名修饰，即使用extern "C"*/
#ifdef __cplusplus
extern "C" {
#endif

// malloc
#ifndef NULL
#define NULL (0)
#endif

void free(void* ptr);
void* malloc(unsigned size);
static int brk(void* end_data_segment);
int mini_crt_init_heap();

// 字符串
char* itoa(int n, char* str, int radix);
int strcmp(const char* src, const char* dst);
char* strcpy(char* dest, const char* src);
unsigned strlen(const char* sr);

// 文件与IO
typedef int FILE;

#define EOF (-1)

/*FILE* 这个类型在Windows下实际上是内核句柄，要通过GetStdHandle的Windows API获得
而在Linux下则是文件描述符，标准输入输出是0,1,2，并不是指向FILE结构的地址*/
#ifdef  WIN32
#define stdin  ((FILE*) (GetStdHandle(STD_INPUT_HANDLE)))
#define stdout ((FILE*) (GetStdHandle(STD_OUTPUT_HANDLE)))
#define stderr ((FILE*) (GetStdHandle(STD_ERROR_HANDLE)))
#else
#define stdin  ((FILE*)0)
#define stdout ((FILE*)1)
#define stderr ((FILE*)2)
#endif

int mini_crt_init_io();   // 省略了缓冲等诸多内容，miniCRT的IO基本无需初始化（即无需给打开文件列表进行空间分配），故而这个函数其实就是个空函数
FILE* fopen(const char* filename,const char* mode);
int fread(void* buffer,int size,int count,FILE* stream);
int fwrite(const void* bufffer,int size,int count,FILE* stream);
int fclose(FILE* fp);
int fseek(FILE* fp,int offset,int set);

// printf
int fputc(int c,FILE* stream);
int fputs(const char* str,FILE* stream);
int printf(const char* format,...);
int fprintf(FILE* stream,const char* format,...);

// internal
void do_global_ctors();
void mini_crt_call_exit_routine();

// atexit
typedef void (*atexit_func_t) (void);
int atexit(atexit_func_t func);

#ifdef  __cplusplus
}
#endif

#endif   // __MINI_CRT_H
