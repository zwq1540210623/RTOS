#ifndef _TASK_H_
#define _TASK_H_
#include <stdint.h>

//定义堆栈类型
//Cortex-M的堆栈单元类型:堆栈单元的大小为32位，所以使用uint32_t
typedef uint32_t tTaskStack;

//定义任务类型
typedef struct _tTask{
    // 任务所用的当前堆栈指针。每个任务都有他自己的堆栈，用于在运行过程中存储临时变量等一些环境参数
    // 在tinyOS运行该任务前，会从stack指向的位置处，会读取堆栈中的环境参数恢复到CPU寄存器中，
    // 然后开始运行。在切换至其他任务时，会将当前CPU寄存器值保存到堆栈中，等待下一次运行该任务时再恢复。
    // stack保存了最后保存环境参数的地址位置，用于后续恢复。
    uint32_t * stack;
}tTask;

#endif
