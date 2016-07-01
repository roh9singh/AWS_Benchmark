#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <memory.h>
#include <stdlib.h>

#define loop 1000
#define memory 30*1024*1024

pthread_t tid[4];

void *status;

void *random_access(int* s)
{
  int i;
  int k = *s;
  long x;
  char *str1, *str2;
  str1 = (char *) malloc(memory);
  str2 = (char *) malloc(memory);
 
  for(i=0;i<loop;i++)
  {	
	x=(rand())%(memory-k);
  	memcpy(str2+x,str1+x,k); 
  }
  free(str1);
  free(str2);
}

void *sequential_access(int* s)
{
  int i;
  int k = *s;
  char *str1, *str2;
  str1 = (char *) malloc(k);
  str2 = (char *) malloc(k);
 
 
  for(i=0;i<loop;i++)
  	memcpy(str2,str1,k); 
  
  free(str1);
  free(str2);
}


int main()
{
   struct timeval start, end;  
   long double time;
   double lat, thr;

   int i=1,k,e,j;
   int m[3]={1,1024,1048576},Threads[2] = {1,2};
 

   printf("\n----------------------------For Sequential Memory Accesss----------------------------"); 
     
   for(j=0;j<3;j++)
   {

       for(k=0;k<2;k++)
       {
       printf("\n ------------For %d Thread------------ \n", Threads[k]);
       
       gettimeofday(&start, 0);
       i=1;
       while(i<=Threads[k])
       {
          e = pthread_create(&(tid[i]), NULL, &sequential_access, &m[j]);
          i++;
       }
       i=1;

       while(i<=Threads[k])
       {
          pthread_join(tid[i],&status);
          i++;
       }

	gettimeofday(&end, 0);

	time=((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / (long double) 1000000;
	lat= (double)(time*1000)/(double)(loop*Threads[k]);
	thr= ((double)(1000*m[j]) /(double)(1024*1024*lat));
   	printf("\n Block size: %d  Latency: %lfms   Throughput: %e",m[j],lat,thr);
       }
   }


   printf("\n---------------------------For Random Memory Accesss----------------------------");
   

   k=0;
   for(j=0;j<3;j++)
   {

       for(k=0;k<2;k++)
       {
       printf("\n ------------For %d Thread------------ \n", Threads[k]);
       
       gettimeofday(&start, 0);
       
       i=1;
           
       while(i<=Threads[k])
       {  
          e = pthread_create(&(tid[i]), NULL, &random_access, &m[j]);
          i++;
       }
       i=1;
       while(i<=Threads[k])
       {
          pthread_join(tid[i],&status);
          i++;
       }

	gettimeofday(&end, 0);

	time=((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / (long double) 1000000;
	lat= (double)(time * 1000)/(double)(loop*Threads[k]);
	thr= ((double)(1000*m[j]) /(double)(1024*1024*lat));
   	printf("\n Block size: %d  Latency: %lfms   Throughput: %e",m[j],lat,thr);
       }
   }
   return 1;

}
