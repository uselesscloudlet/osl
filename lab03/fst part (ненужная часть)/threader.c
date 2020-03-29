#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void *print_message_function(void *ptr)
{
     int retval = 4;
     char *message = (char *) ptr;
     printf("%s\n", message);
     return (void *)(++retval);
}
int main()
{
     pthread_t thread1, thread2;
     const char *message1 = "Thread 1";
     const char *message2 = "Thread 2";
     int res1 = pthread_create(&thread1, NULL, print_message_function, (void*) message1);
     int res2 = pthread_create(&thread2, NULL, print_message_function, (void*) message2);
     int iret1, iret2;
     pthread_join(thread1, (void **)&iret1);
     pthread_join(thread2, (void **)&iret2); 
     
     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
     
     return 0;
}

