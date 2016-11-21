/* xsh_prodcons.c - xsh_prodcons */

#include <xinu.h>
#include <prodcons.h>
#include<future.h>

/*------------------------------------------------------------------------
 * xsh_prodcons - create producer and consumer processes
 *------------------------------------------------------------------------
 */

int n;

sid32 produced, consumed;

future *f_exclusive, *f_shared, *f_queue;

shellcmd xsh_prodcons(int nargs, char *args[]) {


	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s [option]\n\n", args[0]);
		printf("Description:\n");
		printf("\tCreates two processes, producer and consumer\n\n");
		printf("[options]:\n\n");
		printf("\t<number>:\tThis will force the program to use semaphores\n\n");
		printf("\t-f:\tThis will force the program to use futures\n\n");
		return 0;
	}

	/* Check argument count */

	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}

	produced = semcreate(0);
	consumed = semcreate(1);
	
	int count = 2000;	


	if(nargs == 2)
	{

		if(strncmp(args[1], "-f", 3) == 0)
		{
			semdelete(produced);
			semdelete(consumed);
			
			f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
			f_shared = future_alloc(FUTURE_SHARED);
			f_queue = future_alloc(FUTURE_QUEUE);
			
			//Test FUTURE_EXCLUSIVE
			
			resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
			resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );
			
			//Test FUTURE_SHARED
			
			resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
			resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
			resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) );
			resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
			resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared) );
		
                       // Test FUTURE_QUEUE
			resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
			resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
			resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
			resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
			resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue) );
			resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue) );
			resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue) );
			resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue) );

			return 0;
				
		}
		else
		{
			/*Logic to read a number if argument is provided*/
			int i = 0;
			while(args[1][i] != '\0')
			{
				char ch = args[1][i];
				/*Check id argument is a number*/
				/*return error if not a number*/
				if(ch < '0' || ch > '9')
				{
					fprintf(stderr, "%s: argument should be a number\n", args[0]);
					fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
					return 1;
				}
				i++;
			}	
			/*logic to convert input argument into a number*/
			--i;
			int num = 0;
			int mul = 1;

			while(i >= 0)
			{
				num += ((int)args[1][i] - (int)'0') * mul;
				mul *= 10;
				--i;
			}
	
			count = num;
		
		
		}
		
	}	
	
	resume( create(producer, 1024, 20, "producer", 1, count));
	resume( create(consumer, 1024, 20, "consumer", 1, count));
		
	return 0;
}