#include<xinu.h>
#include<future.h>

future* future_alloc(int future_flag)
{
	future *f;
	
	intmask mask;

	mask = disable();
	
	f = getmem(sizeof(future));
	
	if(f !=NULL)
	{
		f->value = getmem(sizeof(int *));
		printf("Location of value set to %d\n", f->value);
		f->flag = future_flag;
		f->state = FUTURE_EMPTY;
		
		if(f->flag != FUTURE_EXCLUSIVE)
		{
			f->get_queue = newqueue();
			printf("GQ: %d\n", f->get_queue);
			if(f->flag == FUTURE_QUEUE)
			{
				f->set_queue = newqueue();
				printf("SQ: %d\n", f->set_queue);
			}
		}
		
	}
	
	restore(mask);
	return f;
}
