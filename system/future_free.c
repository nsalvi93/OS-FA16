#include<xinu.h>
#include<future.h>

syscall future_free(future *f)
{
	intmask mask;
	mask = disable();
	if(f->flag != FUTURE_EXCLUSIVE)
	{
		while(nonempty(f->get_queue))
		{
			dequeue(f->get_queue);
		}
		
		if(f->flag == FUTURE_QUEUE)
		{
			while(nonempty(f->set_queue))
			{
				dequeue(f->set_queue);	
			}
		}
	}
	freemem(f->value, sizeof(int *));
	freemem(f, sizeof(future));
	restore(mask);
	return OK;
}
