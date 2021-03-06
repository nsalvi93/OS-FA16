#include<xinu.h>

#ifndef _FUTURE_H_
#define _FUTURE_H_

/* define states */
#define FUTURE_EMPTY		0
#define FUTURE_WAITING		1
#define FUTURE_VALID		2

/* modes of operation for future */
#define FUTURE_EXCLUSIVE 	1
#define FUTURE_SHARED	 	2
#define FUTURE_QUEUE	 	3

#define NFUT			5		//Maximum number of future that can be created at a time

typedef qid16 queue;

typedef struct futent
{
	int *value;
	int flag;
	int state;
	pid32 pid;
	queue set_queue;
	queue get_queue;
}future;

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
uint future_prod(future*);
uint future_cons(future*);

#endif /* _FUTURE_H_ */
