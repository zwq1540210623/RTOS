#include <stdio.h>
#include "task.h"

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


/**
 * 函数功能：任务初始化函数
 * 入口参数：task 		 任务名
 * 			entry		任务入口函数
 * 			param		任务参数
 * 			stack		任务栈
 * 返回值： 无
*/
void tTaskInit(tTask * task, void (*entry)(void *), void * param, tTaskStack * stack)
{
	task->stack = stack; 
}

//任务切换函数
void tTaskSched(void)
{

}

//定义两个任务
tTask tTask1;
tTask tTask2;

//定义两个任务栈空间
tTaskStack task1Env[1024];
tTaskStack task2Env[1024];

//任务1入口函数
int task1Flag;
void task1(void * param)
{
	for(;;)
	{
		task1Flag = 1;
		delay(100);
		task1Flag = 0;
		delay(100);

		tTaskSched();
	}
}
//任务2入口函数
int task2Flag;
void task2(void * param)
{
	for(;;)
	{
		task2Flag = 1;
		delay(100);
		task2Flag = 0;
		delay(100);

		tTaskSched();
	}
}


int main(void)
{
	tTaskInit(&tTask1, task1, (void*)0x11111111, &task1Env[1024]);
	tTaskInit(&tTask1, task2, (void*)0x22222222, &task2Env[1024]);

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
