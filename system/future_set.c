#include<xinu.h>
#include<future.h>

syscall future_set(future *f, int *value)
{
	intmask mask;
	mask = disable();
	
	struct procent *prptr;
	prptr = &proctab[currpid];
	
	

	if(f->flag == FUTURE_EXCLUSIVE)
	{
				
		if(f->state == FUTURE_EMPTY)
		{
			*(f->value) = *value;
			f->state = FUTURE_VALID;
			restore(mask);
			return OK;
		}
		
		if(f->state == FUTURE_WAITING)
		{
			*(f->value) = *value;
			f->state = FUTURE_VALID;
			resume(f->pid);
			restore(mask);
			return OK;	
		}
		restore(mask);
		return SYSERR;
	}
	
	if(f->flag == FUTURE_SHARED)
	{
		if(f->state == FUTURE_EMPTY)
		{
			*(f->value) = *value;
			f->state = FUTURE_VALID;
			restore(mask);
			return OK;
		}
		
		if(f->state == FUTURE_WAITING)
		{
			*(f->value) = *value;
			f->state = FUTURE_VALID;
			while(nonempty(f->get_queue))
			{
				resume(dequeue(f->get_queue));
			}
			restore(mask);
			return OK;
		}
		restore(mask);
		return SYSERR;
	}
	
	if(f->flag == FUTURE_QUEUE)
	{
		if ( f->state == FUTURE_EMPTY ) 
		{
			insert(currpid, f->set_queue, prptr->prprio);
			suspend(currpid);
			if(f->state == FUTURE_WAITING)
			{
				*(f->value) = *value;
				f->state = FUTURE_VALID;
				resume(dequeue(f->get_queue));
				restore(mask);
				return OK;
			}
			restore(mask);
			return SYSERR;
		}

		if( f->state == FUTURE_WAITING )
		{
			*(f->value) = *value;
			f->state = FUTURE_VALID;
			resume(dequeue(f->get_queue));
			restore(mask);
			return OK;
		} 

		restore(mask);
		return SYSERR;
	} 
	
	restore(mask);
	return SYSERR;
}
