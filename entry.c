//entry.c
#include "minicrt.h"

#ifdef WIN32
#include <Windows.h>
#endif 

extern int main(int argc,char* argv[]);
void exit(int);

static void crt_fatal_error(const char* msg)
{
	// printf("fatal error: %s",msg);
	exit(1);
}

void mini_crt_entry(void)
{
	int ret;
	// printf("ENTRY mini_crt_entry\n");

#ifdef WIN32
	int flag=0;
	int argc=0;
	char* argv[16];   // 最多16个参数
	char* cl=GetCommandLineA();   // 对于Windows系统来说，用GetCommandLine这个API来返回整个命令行参数字符串

	// 解析命令行，即将整个命令行字符串分割成若干个参数，以符合argc和argv的格式
	argv[0]=cl;
	++argc;
	while (*cl)
	{
		if (*cl=='\"')   // 字符串中的空格是有效的空格，所以需要设置flag
		{
			if (flag==0) flag=1;
			else flag=0;
		}
		else if (*cl==' ' && flag==0)   // 否则剔除空格，但只能处理单空格
		{
			if (*(cl+1))
			{
				argv[argc]=cl+1;
				++argc;
			}
			*cl='\0';
		}
		++cl;
	}

#else
	int argc;
	char** argv;

	char* ebp_reg=0;
	// ebp_reg=%ebp;
	asm("movl %%ebp,%0  \n\t":"=r"(ebp_reg));

	argc=*(int*)(ebp_reg+4);
	argv=(char**)(ebp_reg+8);
	
#endif
	if (!mini_crt_init_heap())
		crt_fatal_error("heap initialize failed");

	if (!mini_crt_init_io())
		crt_fatal_error("IO initialize failed");

	ret=main(argc,argv);
	exit(ret);
}

void exit(int exitCode)
{
	// mini_crt_call_exit_routine();   // 启动退出时，调用注册的全局对象析构函数
#ifdef WIN32
	ExitProcess(exitCode);
#else
	asm("movl %0,%%ebx \n\t"
	    "movl $1,%%eax \n\t"
	    "int $0x80	   \n\t"
	    "hlt	   \n\t"::"m"(exitCode));
#endif
}
