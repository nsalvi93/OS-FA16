#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
	
	int i;

	for(i = 1; i <= count; i++)
	{
		wait(consumed);
		n = i;
		signal(produced);
	}
	
}



