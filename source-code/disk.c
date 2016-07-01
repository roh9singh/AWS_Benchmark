#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <memory.h>
#include <stdlib.h>

#define memory 1024*1024*100
#define loop 1000

//char path[]="/home/ec2-user/File.txt";



pthread_t tid[4];
void *status;

void *random_write(int* s)
{
  int i,l=1,c=0;
  int k = *s;
  long x;
  
  FILE *fp;
  fp = fopen("File.txt","w+");
  for(i=0,l=0;i<loop;i++)
  {	
	x=(rand())%(memory-k);
  	fseek(fp,x,SEEK_SET);
	while(l++<k)        
	   fprintf(fp,"A");
  }
  fflush(fp);
}

void *random_read(int* s)
{
  int i=0,l=0;
  int k = *s;
  long x;
  char *str = (char *) malloc(memory);
  FILE *fp;
  fp = fopen("File.txt","r+");

  while(i++<loop)
  {
       l=0;
       fseek(fp,rand()%(k),SEEK_SET);
       fgets(str,k,fp);

       
  }
  fflush(fp);
}

void *sequential_write(int* s)
{
  int i,c=0;
  int k = *s;
  FILE *fp;
  char str='A';

  fp = fopen("File.txt","w+");
		 
  for(i=0,c=0;i<loop;i++) 
  {
	while(c++<k)
     	   fprintf(fp,"%c",str);
  }
  fflush(fp);
}

void *sequential_read(int* s)
{
  int i=0,c=0;
  int k = *s;
  FILE *fp;
  //char *str = (char *) malloc(memory);
  char str=' ';
  
  fp = fopen("File.txt","r+");

  for(i=0;i<loop;i++) 
  {
      while(c++<k)
        fscanf(fp,"%c",&str);
   //fseek(fp,c,SEEK_SET);
   //fgets(str,k,fp);
   //c=i*k;   
  }
 //free(str);
 fflush(fp);
}

int main()
{
   struct timeval start, end;  
   double time,lat,thr;
   int i=1,k,e,j;
   int m[3]={1,1024,1048576},Threads[2] = {1,2};
		 

   printf("\n----------------------------For Sequential Memory Write----------------------------"); 
     
   for(j=0;j<3;j++)
   {

       for(k=0;k<2;k++)
       {
	       printf("\n ------------For %d Thread------------ \n", Threads[k]);
		       
	       gettimeofday(&start, 0);
	       i=1;
	       while(i<=Threads[k])
	       {
		  e = pthread_create(&(tid[i]), NULL, &sequential_write, &m[j]);
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
		lat=(double)(time * 1000)/(double)(Threads[k]);
	        thr= (double)(1000*m[j]) /(double)(1024*1024*lat);
	        printf("\n Memory: %d  Latency: %ems   Throughput: %e",m[j],lat,thr);
       }
   }


   printf("\n----------------------------For Sequential Memory Read----------------------------"); 
     
   for(j=0;j<3;j++)
   {

       for(k=0;k<2;k++)
       {
	       printf("\n ------------For %d Thread------------ \n", Threads[k]);
		       
	       gettimeofday(&start, 0);
	       i=1;
	       while(i<=Threads[k])
	       {
		  e = pthread_create(&(tid[i]), NULL, &sequential_read, &m[j]);
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
		lat=(double)(time*1000)/(double)(Threads[k]);
	        thr= (double)(1000*m[j]) /(double)(1024*1024*lat);
	        printf("\n Memory: %d  Latency: %ems   Throughput: %e",m[j],lat,thr);
       }
   }

   printf("\n---------------------------For Random Memory Write----------------------------");
	
   for(j=0;j<3;j++)
   {
       for(k=0;k<2;k++)
       {
  	       printf("\n ------------For %d Thread------------ \n", Threads[k]);
	       
               gettimeofday(&start, 0);
		       
	       i=1;
			   
	       while(i<=Threads[k])
	       {  
		  e = pthread_create(&(tid[i]), NULL, &random_write, &m[j]);
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
	       lat=(double)(time * 1000)/(double)(Threads[k]);
	       thr= (double)(1000*m[j]) /(double)(1024*1024*lat);
	       printf("\n Memory: %d  Latency: %ems   Throughput: %e",m[j],lat,thr);
	}
   }

   printf("\n---------------------------For Random Memory Read----------------------------");
	
   for(j=0;j<3;j++)
   {
       for(k=0;k<2;k++)
       {
  	       printf("\n ------------For %d Thread------------ \n", Threads[k]);
	       
               gettimeofday(&start, 0);
		       
	       i=1;
			   
	       while(i<=Threads[k])
	       {  
		  e = pthread_create(&(tid[i]), NULL, &random_read, &m[j]);
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
	       lat=(double)(time * 1000)/(double)(Threads[k]);
	       thr= (double)(1000*m[j]) /(double)(1024*1024*lat);
	       printf("\n Memory: %d  Latency: %ems   Throughput: %e",m[j],lat,thr);
	}
   }

   return 1;
}
