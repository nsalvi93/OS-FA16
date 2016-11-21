#include <xinu.h>
#include <prodcons.h>

void consumer(int count) {
	
	int i;

	for(i = 0; i < count; i++)
	{
		wait(produced);
		printf("Consumed: %d\n", n);
		signal(consumed);
	}
	
}

