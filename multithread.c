/*
 * multithread.c
 *
 * Demonstrate use of a multi threading and scheduling using pthreads
 *
 * compile with
 cc multithread.c -o multithread -lrt -lpthread
 *
 */
#define _GNU_SOURCE
#define _REENTRANT      /* macro to ensure system calls are reentrant */
#include <pthread.h>    /* header file for pthreads */
#include <unistd.h>     /* header file for POSIX conformance */
#include <time.h>       /* header file for POSIX time management */
#include <sched.h>      /* header file for POSIX scheduling */
#include <stdio.h>      /* header file for standard input/outputlibrary */

void *threadA(void *);   /* predefine threadA routine */
void *threadB(void *);   /* predefine threadB routine */
void *threadC(void *);   /* predefine threadC routine */

pthread_t threadA_id,threadB_id,threadC_id,main_id; /* thread identifiers */
pthread_attr_t attrA,attrB,attrC;  /* thread attribute structures */
struct sched_param param;          /* scheduling structure for thread attributes */

int policy=SCHED_FIFO;
int priority_min,priority_max;     /* for range of priority levels */

/* main routine */
int main()
{
  struct timespec start;
  int status;                        /* check that system calls return ok */

  clock_gettime(CLOCK_REALTIME, &start);        /* get the time   */
  printf("Start time is: %d seconds %d nano_seconds\n",start.tv_sec,start.tv_nsec);

  /* Set processor affinity */ 
  cpu_set_t mask; CPU_ZERO(&mask); CPU_SET(0,&mask); /* use only 1 CPU core */
  unsigned int len = sizeof(mask);
  status = sched_setaffinity(0, len, &mask);
  if (status < 0) perror("sched_setaffinity");
  status = sched_getaffinity(0, len, &mask);
  if (status < 0) perror("sched_getaffinity");

  /* Find priority limits */
  
  priority_max = sched_get_priority_max(policy);
  priority_min = sched_get_priority_min(policy);
  
  /* Set priority and policy of main thread */

  main_id = pthread_self();
  param.sched_priority=priority_min+1;
  status = pthread_setschedparam(main_id, policy, &param);
  if (status != 0) perror("pthread_setschedparam"); /* error check */
  
  /* Create threadA */
  
  param.sched_priority = priority_min+1;
  pthread_attr_init(&attrA);
  status = pthread_attr_setschedpolicy(&attrA,policy);
  if (status != 0) perror("pthread_attr_setschedpolicy"); /* error check */
  status = pthread_attr_setschedparam(&attrA,&param);
  if (status != 0) perror("pthread_attr_setschedparam"); /* error check */
  status = pthread_create(&threadA_id, &attrA, threadA, NULL);
  if (status != 0) perror("pthread_create"); /* error check */
  status = pthread_setschedparam(threadA_id,policy,&param);
  if (status != 0) perror("pthread_setschedparam");

  /* Create threadB */
  
  param.sched_priority = priority_min+1;
  pthread_attr_init(&attrB);
  status = pthread_attr_setschedpolicy(&attrB,policy);
  if (status != 0) perror("pthread_attr_setschedpolicy"); /* error check */
  status = pthread_attr_setschedparam(&attrB,&param);
  if (status != 0) perror("pthread_attr_setschedparam"); /* error check */
  status = pthread_create(&threadB_id, &attrB, threadB, NULL);
  if (status != 0) perror("pthread_create"); /* error check */
  status = pthread_setschedparam(threadB_id,policy,&param);
  if (status != 0) perror("pthread_setschedparam");
  
  /* Create threadC */
  
  param.sched_priority = priority_min+1;
  pthread_attr_init(&attrC);
  status = pthread_attr_setschedpolicy(&attrC,policy);
  if (status != 0) perror("pthread_attr_setschedpolicy"); /* error check */
  status = pthread_attr_setschedparam(&attrC,&param);
  if (status != 0) perror("pthread_attr_setschedparam"); /* error check */
  status = pthread_create(&threadC_id, &attrC, threadC, NULL);
  if (status != 0) perror("pthread_create"); /* error check */
  status = pthread_setschedparam(threadC_id,policy,&param);
  if (status != 0) perror("pthread_setschedparam");

  /* Join threads - force main to wait for the thread to terminate */  
  printf("main() waiting for threads\n");
  
  status = pthread_join(threadA_id, NULL);
  if (status != 0) perror("pthread_join(threadA_id, NULL)"); /* error check */
  status = pthread_join(threadB_id, NULL);
  if (status != 0) perror("pthread_join(threadB_id, NULL)"); /* error check */
  status = pthread_join(threadC_id, NULL);
  if (status != 0) perror("pthread_join(threadC_id, NULL)"); /* error check */
  
  printf("\nmain() reporting that all threads have terminated\n");
  return(0);
  
}  /* end of main */

void *threadA(void *arg)
{
  int j;
  
  for(j=1;j<=10;j++){
	printf("a");
  }
  
  return (NULL);
}

void *threadB(void *arg)
{
  int j;
  
  for(j=1;j<=10;j++){
	printf("b");
  }
  return (NULL);
}

void *threadC(void *arg)
{
  int j;
  
  for(j=1;j<=10;j++){
	printf("c");
  }
  return (NULL);
}
