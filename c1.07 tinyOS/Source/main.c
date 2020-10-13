#include <stdio.h>

#define NVIC_INT_CTRL 	0xE000ED04 //NVIC_INT_CTRL寄存器
#define NVIC_PENDSVSET 	0x10000000 //往NVIC_INT_CTRL寄存器中存放的值
#define NVIC_SYSPRI2	0xE000ED22 //NVIC_SYSPRI2寄存器
#define NVIC_PENDSV_PRI 0x000000FF //往NVIC_PENDSV_PRI寄存器中存放的值

//寄存器写值的宏
#define MEM32(addr) *(volatile unsigned long *)(addr)
#define MEM8(addr)  *(volatile unsigned char *)(addr)

//触发PendSVC挂起异常的函数
void triggerPendSVC(void)
{
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}


//定义任务块结构体
typedef struct _BlockType_t
{
	unsigned long * stackPtr; //任务块栈指针
}BlockType_t;  

BlockType_t *blockPtr;

//延时函数
void delay(int count)
{
	while(--count>0);
}


int flag = 0;

//栈数组
unsigned long stackBuffer[1024];
//定义一个任务块
BlockType_t block;


int main(void)
{
	block.stackPtr = &stackBuffer[1024]; //栈是满减栈，让任务块的栈指针指向栈数组的最后一个元素
	blockPtr = &block;
	
	for(;;)
	{
		flag = 0;
		delay(100);
		flag = 1;
		delay(100);

		triggerPendSVC();
	}
}
