#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define TIME_TO_WAIT 1

#pragma GCC push_options
#pragma GCC optimize ("O0")
pthread_t tid[4];
const int loops=INT_MAX;
void *status;

long double samplei[600],samplef[600];
int s=0;
struct timeval start600,end600;

void *iops()
{
   int a=1,b=2,c=3;
   long int i;
   for(i=0; i< loops; i++)
   {
	a=b+c;
        b=1;

	a=b+c;
        b=1;

	a=b+c;
        b=1;
       
	a=b+c;
        b=1;

	a=b+c;
        b=1;

	a=b+c;
        b=1;

        a=b+c;
        b=1;

	a=b+c;
        b=1;

	a=b+c;
        b=1;
       
	a=b+c;
        b=1;

	a=b+c;
        b=1;

	a=b+c;
        b=1;
	
   }
}

void *flops()
{
   long int i;
   float a=1.0,b=2.0,c=3.0;
   
   for(i=0; i< loops; i++)
   {
	a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;
       
	a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;

        a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;
       
	a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;

	a=b+c;
        b=1.0;
	
   }
}


void *iopscal()
{
  int a=1,b=2,c=3;
  long int i;
  for(i=0; s<600; i++)
  {
	a=b+c;
	b=1;

	b=a+c;
	c=1;

	c=b+a;
	a=1;
	       
	a=b+c;
	b=1;

	b=a+c;
	c=1;

	c=b+a;
	a=1;

	a=b+c;
	b=1;

	b=a+c;
	c=1;

	c=b+a;
	a=1;
	       
	a=b+c;
	b=1;

	b=a+c;
	c=1;

	c=b+a;
	a=1;
	       
	samplei[s]++;
	gettimeofday(&end600, 0);
	if (end600.tv_sec>=(start600.tv_sec + TIME_TO_WAIT))
	{	
		s++;	
		gettimeofday(&start600, 0);	
	}
   }

}

void *flopscal()
{
  float a=1.0,b=2.0,c=3.0;
  long int i;
  for(i=0; s<600; i++)
  {
	a=b+c;
        b=1.0;

	b=a+c;
        c=1.0;

	c=b+a;
        a=1.0;
       
	a=b+c;
        b=1.0;

	b=a+c;
        c=1.0;

	c=b+a;
        a=1.0;

        a=b+c;
        b=1.0;

	b=a+c;
        c=1.0;

	c=b+a;
        a=1.0;
       
	a=b+c;
        b=1.0;

	b=a+c;
        c=1.0;

	c=b+a;
        a=1.0;
	       
	samplef[s]++;
	gettimeofday(&end600, 0);
	if (end600.tv_sec>=(start600.tv_sec + TIME_TO_WAIT))
	{	
		s++;	
		gettimeofday(&start600, 0);	
	}
   }

}


void iopsfor10()
{
   int i=0;
   int Threads[3] = {1,2,4};
   samplei[0]=0;

   while(s<600)
   {
    while(i<Threads[1])
   {
      pthread_create(&(tid[i]), NULL, &iopscal, NULL);
      i++;
   }
   gettimeofday(&start600, 0);
   i=0;
   while(i<Threads[1])
   {
      pthread_join(tid[i],&status);
      i++;
   }
   }
}

void flopsfor10()
{
   int i=0;
   int Threads[3] = {1,2,4};
   samplef[0]=0;


   while(s<600)
   {	
   while(i<Threads[1])
   {
      pthread_create(&(tid[i]), NULL, &flopscal, NULL);
      i++;
   }
   gettimeofday(&start600, 0);
   i=0;
   while(i<Threads[1])
   {
      pthread_join(tid[i],&status);
      i++;
   }
   }
 
}



int main()
{
   int i,j,k,e;
   long double x=loops,time,gflops,giops;

   clock_t start_t, end_t, total_t;
   struct timeval start, end;
   int Threads[3] = {1,2,4};
   
   
   for(k=0;k<3;k++)
   {

   printf("------------FLOPS For %d Thread------------ \n", Threads[k]);

   gettimeofday(&start, 0);
    i=1;
   while(i<=Threads[k])
   {
   e = pthread_create(&(tid[i]), NULL, &flops, NULL);
   i++;
   }
   i=1;
   while(i<=Threads[k])
   {
   pthread_join(tid[i],&status);
   i++;
   }
 
   gettimeofday(&end, 0);
   time = 1000.0 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000.0; // total time in ms
   unsigned long op = (Threads[k]*x*68);

   gflops = (op / (long double) (time / 1000)) / 1e9;

   printf("GigFLOPS %Lf \n",gflops);

   }
 


   for(k=0;k<3;k++)
   {

   printf("------------IOPS For %d Thread------------ \n", Threads[k]);
   
   gettimeofday(&start, 0);
    i=1;
   while(i<=Threads[k])
   {
   e = pthread_create(&(tid[i]), NULL, &iops, NULL);
   i++;
   }
   i=1;
   while(i<=Threads[k])
   {
   pthread_join(tid[i],&status);
   i++;
   }

   gettimeofday(&end, 0);


   time = 1000.0 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000.0; // total time in ms
   unsigned long op = (Threads[k]*x*68);
   giops = (op / (long double) (time / 1000)) / 1e9;
   printf("GigIOPS %Lf \n",giops);

   } 


   FILE *fp;
   fp = fopen("Sample IOPS.txt","w+");
   iopsfor10();
   unsigned long op = (73*4);
   for(j=0;j<600;j++)
   {
	samplei[j]= (long double) (samplei[j]*op) / (long double) 1e9;
	fprintf(fp,"%Lf \n",samplei[j]);
   }
 


   s=0;
   fp = fopen("Sample FLOPS.txt","w+");
   flopsfor10();
   for(j=0;j<600;j++)
   {
	samplef[j]= (long double) (samplef[j]*op) / (long double) 1e9;
	fprintf(fp,"%Lf \n",samplef[j]);
   }
  
   return 1;
}

#pragma GCC pop_options
