#include <pthread_utils.h>
#include <libsem.h>
#include <pthread.h>


// Macro que incluye el código de la instrucción máquina xchg
#define atomic_xchg(A,B) 	__asm__ __volatile__(	\
							"   lock xchg %1,%0 ;\n"	\
							: "=ir"  (A)			\
							: "m"  (B), "ir" (A)		\
							);

int g=0;


void initsem(SEMAPHORE *s,int val)
{
	s->count = val;
	initqueue(&s->queue);
}


void waitsem(SEMAPHORE *s)
{
	int l = 1;
	do{atomic_xchg(g,l)}while(l);
	if(--(s->count) < 0)
	{
		enqueue(&s->queue,pthread_self());
		g = 0;
		block_thread();
	}
	else
		g = 0;
		
}

void signalsem(SEMAPHORE *s)
{
	int l = 1;
	do{atomic_xchg(g,l)}while(l);
	if(++(s->count) <= 0)
	{
		unblock_thread(dequeue(&s->queue));
	}
	g = 0;
	
}








