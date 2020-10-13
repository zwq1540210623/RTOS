#include <stdio.h>

void delay(int count)
{
	while(--count>0);
}

int flag = 0;

int main(void)
{
	for(;;)
	{
		flag = 0;
		delay(100);
		flag = 1;
		delay(100);
	}
}
