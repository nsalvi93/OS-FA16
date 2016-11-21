/*Global variable for producer consumer*/
extern int n; /*Dedclaration*/

/*function prototypes*/
void consumer(int count);
void producer(int count);

/*semaphore declarations*/
extern sid32 produced, consumed;
