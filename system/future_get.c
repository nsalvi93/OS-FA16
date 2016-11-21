#include<xinu.h>
#include<future.h>

syscall future_get(future *f, int *value)
{
	
	intmask mask;
	mask = disable();
	
	struct procent *prptr;
	prptr = &proctab[currpid];
	
	if(f->flag == FUTURE_EXCLUSIVE)
	{
		if(f->state == FUTURE_VALID)
		{
			*value = *(f->value);
			f->state = FUTURE_EMPTY;
			
			restore(mask);
			return OK;
		}
		
		if(f->state == FUTURE_EMPTY)
		{
			f->pid = currpid;
			f->state = FUTURE_WAITING;
			
			suspend(currpid);
			if(f->state == FUTURE_VALID)
			{
				*value = *(f->value);
				restore(mask);	
				return OK;
			}
			restore(mask);
			return SYSERR;
		}
		
		restore(mask);
		return SYSERR;
		
	}
	
				
	if(f->flag == FUTURE_SHARED)
	{
		if(f->state == FUTURE_VALID)
		{
			
			if(isempty(f->get_queue))
			{
				f->state = FUTURE_EMPTY;
			}
			
			*value = *(f->value);
			restore(mask);
			return OK;
		}

		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
		{
			f->state = FUTURE_WAITING;
			insert(currpid, f->get_queue, prptr->prprio);
			suspend(currpid);
			if(f->state == FUTURE_VALID)
			{
				*value = *(f->value);
				if(isempty(f->get_queue))
				{
					f->state = FUTURE_EMPTY;
				}
				restore(mask);
				return OK;
			}
			restore(mask);
			return SYSERR;
		}
		
		restore(mask);
		return OK;
	}
	
	if ( f->flag == FUTURE_QUEUE )
	{
		if(f->state == FUTURE_VALID)
		{
			if(isempty(f->get_queue))
			{
				f->state = FUTURE_EMPTY;
			}
			else
			{
				f->state = FUTURE_WAITING;
			}
			*value = *(f->value);
			restore(mask);
			return OK;
		}
		
		if(f->state == FUTURE_EMPTY)
		{
			f->state = FUTURE_WAITING;
			insert(currpid, f->get_queue, prptr->prprio);

			if(nonempty(f->set_queue))
			{
				resume(dequeue(f->set_queue));
			}
			
			suspend(currpid);
			
			if(f->state == FUTURE_VALID)
			{
				*value = *(f->value);
				if(isempty(f->get_queue))
				{
					f->state = FUTURE_EMPTY;
				}
				else
				{
					f->state = FUTURE_WAITING;
				}
				restore(mask);
				return OK;
			}
			
			restore(mask);
			return SYSERR;
		}
		
		if (f->state == FUTURE_WAITING)
		{
			insert(currpid, f->get_queue, prptr->prprio);
			suspend(currpid);
			
			if(f->state == FUTURE_VALID)
			{
				if(isempty(f->get_queue))
				{
					f->state = FUTURE_EMPTY;
				}
				else
				{
					f->state = FUTURE_WAITING;
				}
				*value = *(f->value);
				restore(mask);
				return OK;
			}
			
			restore(mask);
			return SYSERR;

		}
		
		restore(mask);
		return SYSERR;
	}
	
	restore(mask);
	return SYSERR;
}
