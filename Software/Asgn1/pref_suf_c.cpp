/***************************************************************** 
      --Authors: Javier James
     --Date created: Mon 17 June 2013
    --file name: pref_suf.cpp
     --Descrition: This file provides a sequential implementation of the a prefix suffix pref_min in C  SW algorith for DNA sequence alignment using C code.  
      --References: 
     --Last update: Sat 27 July 2013
      --Note: 
   
 *******************************************************************/
  
//Includes
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <sys/time.h>
#include <math.h>
using namespace std;

//#define RUN_OPENMP
#define RUN_PTHREADS

#ifdef RUN_OPENMP 
#include <omp.h>  
#endif 

#ifdef RUN_PTHREADS 
#include <pthread.h>  

#endif 

int nthreads, tid;

/* --- Global shared memory data ---*/ 
int A [] = {58  ,89 , 32 ,73  ,131 ,156 ,30 ,29 ,
            141 ,37 ,133 ,151 ,88  ,53 ,122 ,126,
            131 ,49 ,130, 115 ,16  ,83 ,40  ,145,
            10  ,112 ,20 ,147 ,14 ,104 ,111 ,92 } ;

int min(int a, int b); 
void seq_function(int* dataset, int* p_min, int* s_min, int d_length);
void seq_function2(int* dataset, int* p_min, int* s_min, int d_length);
void* par_function_OMP(int* dataset, int* p_min, int* s_min, int d_length);
void* par_function_PTH(int* dataset, int* p_min, int* s_min, int d_length);


/*Pthreads declarations */

  void *copy_dataset_parallel(void* input);
  void *execute_parallel (void* input) ;
  void *execute_parallel_suf_min (void* input);
 
  typedef struct {
          int p_minima; 
          int s_minima; 
          int n; 
          int tid; 
          int nthreads;
          int *data;
          int celID; 
          int cel_jump_length; 
  }minima_args; 


int *pref_min_log_p ; // safe results of prefix minima  
int *suf_min_log_p ;  // safe results of suffix minima 


/* --- End of global memory shared data ---*/



int main(int argc, char** argv )
{
   	struct timeval startt, endt, result;
        
	int data_length;
	data_length = sizeof(A)/sizeof(int);
	
	int pref_min_log [data_length]; // safe results of prefix minima  
	int suf_min_log [data_length];  // safe results of suffix minima 

        result.tv_sec = 0;
	result.tv_usec = 0;

//=======================================================
	/*Sequential algorithm */ 	
/* 	result.tv_usec=0; 
	gettimeofday (&startt, NULL);

	seq_function(A, pref_min_log, suf_min_log, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
*/
	/*print results  */
/*	cout<<"\n\n -------Sequential Results------- "<<endl;
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	
	cout<<"\n \t\t A \t pref_min \t suf_min "<<endl;
	cout<<"---------------------------------------------------------------------- "<<endl;

	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<A[f] <<"\t\t"<< pref_min_log[f]<<"\t\t"<<suf_min_log[f]<<endl;
	}
*/
//=======================================================

	/*Sequential2 algorithm */ 	
 	//result.tv_usec=0; 
        result.tv_sec = 0;
	result.tv_usec = 0;
	gettimeofday (&startt, NULL);

	seq_function2(A, pref_min_log, suf_min_log, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
	
	/*print results  */
	cout<<"\n\n -------Sequential 2 Results------- "<<endl;
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	
	cout<<"\n \t\t A \t pref_min \t suf_min "<<endl;
	cout<<"---------------------------------------------------------------------- "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<A[f] <<"\t\t"<< pref_min_log[f]<<"\t\t"<<suf_min_log[f]<<endl;
	}

//=======================================================

#ifdef RUN_OPENMP 

	/*Parallel algorithm */ 	
	
 	//result.tv_usec=0; 
        result.tv_sec = 0;
	result.tv_usec = 0;
	gettimeofday (&startt, NULL);

	par_function_OMP(A, pref_min_log, suf_min_log, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);


	/*print results  */
	cout<<"\n\n -------Parallel OpenMP  Results------- "<<endl;
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	
	cout<<"\n \t\t A \t pref_min \t suf_min "<<endl;
	cout<<"---------------------------------------------------------------------- "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<A[f] <<"\t\t"<< pref_min_log[f]<<"\t\t"<<suf_min_log[f]<<endl;
	}

#endif 
//=======================================================

#ifdef RUN_PTHREADS 
	/*Parallel algorithm */ 	


        pref_min_log_p = (int  *) malloc(data_length* sizeof(int))      ;//create argument data for each thread.   
        suf_min_log_p = (int  *) malloc(data_length* sizeof(int))      ;//create argument data for each thread.   
	
 	//result.tv_usec=0; 
        result.tv_sec = 0;
	result.tv_usec = 0;
	gettimeofday (&startt, NULL);

	par_function_PTH(A, pref_min_log_p, suf_min_log_p, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);


	/*print results  */
	cout<<"\n\n -------Parallel PTHREADS  Results------- "<<endl;
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	
	cout<<"\n \t\t A \t pref_min \t suf_min "<<endl;
	cout<<"---------------------------------------------------------------------- "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<A[f] <<"\t\t"<< pref_min_log_p[f]<<"\t\t"<<suf_min_log_p[f]<<endl;
	}


        free(pref_min_log_p);
	free(suf_min_log_p);   
#endif
//=======================================================




}//end main




void seq_function(int* dataset, int* p_min, int* s_min, int d_length){

      

	/*calculate prefix minima */ 
	
	int pref_min;    					// T(n) = O(1)
	pref_min= dataset[0];					// T(n) = O(1)
	p_min[0]= pref_min;                                  	// T(n) = O(1)
	
	for(int i=1; i<d_length; i++)				// T(n) = O(d_length))= O(n)
	{
		pref_min = min(pref_min, dataset[i] ); 	// T(n) = O(1)
		p_min[i]= pref_min; 
	}

     
	 /*calculate sufix  minima */ 
	int suf_min;

	for(int i=0; i<d_length; i++)
	{
		suf_min= dataset[i];
		for(int t=i; t<d_length; t++)
		{
			suf_min= min(suf_min, dataset[t] );
		}
			s_min[i]= suf_min;
		
	}

}

//---------------------------------


void seq_function2(int* dataset, int* p_min, int* s_min, int d_length){
/* --- Thread local memory data --- */

int n = d_length; 
int num_stages= log(n) / log(2); 
int g; 
int mini[n]; 
int tid; 


/* --- End thread local memory data --- */


//printf("Initial values\n\n");
//printf("n: %d | stages:%d  | \n\n", n, num_stages);


/*copy data set */
// printf("\n \t\t----Start copy --- \n\n");
        for(g=0; g<n; g++)
	{
		p_min[g] = dataset[g];
		s_min[g] = dataset[g];
        }
 
// printf("\n \t\t----Done copy --- \n\n");


 /*calc pref_min  */
 int r=1;
 int y;

 for(int q=1; q<= num_stages; q++ ){  //log(n) stages
 
	int l=0; //restart left value for min operation 
 
        for(y=r; y<n; y++)
        {
//		tid = omp_get_thread_num();
         	p_min[y]= min ( p_min[y-r],p_min[y] );
//		printf("Thread#: %d ==> \t ==> \t |  Y:%d  R: %d | min(%d, %d) \t \t ==> \    t mini(%d):%d  \n",tid, y, r,  p_min[y-r], p_min[y], y,p_min[y]  );
 
        }
 
        r=r*2; //double length of comparison starting point 
  }


 /*print results */
/*printf("\n\n\n------------Results of prefix ------- \n\n " );
 for( int  j=0; j<n; j++)
 {
         printf("p_min[%d}:\t%d\n ",j,p_min[j] );
 
 }
*/




/************************
* Suffix minima 
*************************/

 r=1;



/*calculate suffix minima  */
 for(int q=1; q<= num_stages; q++ ){  //log(n) stages
 
	int l=n-1; //restart left value for min operation  ex. 7  
 
        for(y=l-r; y>=0; y--)  //calculate n-1 blocks   6>=y>=0,  5>=y>=0,  3>=y>=0 
        {
	//	tid = omp_get_thread_num();
         	s_min[y]= min ( s_min[y+r ],s_min[y] );
//		printf("Thread#: %d ==> \t ==> \t |  Y:%d  R: %d | min(%d, %d) \t \t ==> \    t mini(%d):%d  \n",tid, y, r,  s_min[y-r], s_min[y], y,s_min[y]  );

        }
 
        r=r*2; //double length of comparison starting point 
  }


 /*print results */
/*printf("\n\n\n------------Results of sufix ------- \n\n " );
 for( int  j=0; j<n; j++)
 {
         printf("s_min[%d}:\t%d\n ",j,s_min[j] );
 }
*/


} //end par_function_OMP



//--------------------------


#ifdef RUN_OPENMP 


void* par_function_OMP(int* dataset, int* p_min, int* s_min, int d_length){

/* --- Thread local memory data --- */

int n = d_length; 
int num_stages= log(n) / log(2); 
int g; 
int mini[n]; 
int tid; 


/* --- End thread local memory data --- */


//printf("Initial values\n\n");
//printf("n: %d | stages:%d  | \n\n", n, num_stages);


/*copy data set */
// printf("\n \t\t----Start copy --- \n\n");
#pragma omp parallel for private (g,tid) shared(dataset, p_min, n)
        for(g=0; g<n; g++)
	{
		p_min[g] = dataset[g];
		s_min[g] = dataset[g];
  
 //             printf("Thread#: %d ==> \t ==> \t p_min(%d):%d  \t data(%d): %d  \n",omp_get_thread_num(),g,p_min[g],g, dataset[g]  );
 
        }
 
// printf("\n \t\t----Done copy --- \n\n");


 /*calc pref_min  */
 int r=1;
 int y;

 for(int q=1; q<= num_stages; q++ ){  //log(n) stages
 
	int l=0; //restart left value for min operation 
 
        //#pragma omp parallel for private(y,tid) shared(p_mini,r)
        #pragma omp parallel for private(y,tid) shared(p_min,r)
        for(y=r; y<n; y++)
        {
		tid = omp_get_thread_num();
         	p_min[y]= min ( p_min[y-r],p_min[y] );
//		printf("Thread#: %d ==> \t ==> \t |  Y:%d  R: %d | min(%d, %d) \t \t ==> \    t mini(%d):%d  \n",tid, y, r,  p_min[y-r], p_min[y], y,p_min[y]  );
 
        }
 
        r=r*2; //double length of comparison starting point 
  }


 /*print results */
/*printf("\n\n\n------------Results of prefix ------- \n\n " );
 for( int  j=0; j<n; j++)
 {
         printf("p_min[%d}:\t%d\n ",j,p_min[j] );
 
 }
*/




/************************
* Suffix minima 
*************************/

 r=1;



/*calculate suffix minima  */
 for(int q=1; q<= num_stages; q++ ){  //log(n) stages
 
	int l=n-1; //restart left value for min operation  ex. 7  
 
        //#pragma omp parallel for private(y,tid) shared(p_mini,r) //changed, but worked before. ultimate last change b4 things was working. name of change F4
        #pragma omp parallel for private(y,tid) shared(p_min,r)
        for(y=l-r; y>=0; y--)  //calculate n-1 blocks   6>=y>=0,  5>=y>=0,  3>=y>=0 
        //for(y=0; y<n; y++)
        {
		tid = omp_get_thread_num();
         	s_min[y]= min ( s_min[y+r ],s_min[y] );
//		printf("Thread#: %d ==> \t ==> \t |  Y:%d  R: %d | min(%d, %d) \t \t ==> \    t mini(%d):%d  \n",tid, y, r,  s_min[y-r], s_min[y], y,s_min[y]  );

        }
 
        r=r*2; //double length of comparison starting point 
  }


 /*print results */
/*printf("\n\n\n------------Results of sufix ------- \n\n " );
 for( int  j=0; j<n; j++)
 {
         printf("s_min[%d}:\t%d\n ",j,s_min[j] );
 }
*/


} //end par_function_OMP

#endif 


//================================================================

#ifdef RUN_PTHREADS 
void* par_function_PTH(int* dataset, int* p_min, int* s_min, int d_length){

int nthreads= 4; 
int *pref_min_log;
 int n=d_length; 
pthread_t threads[nthreads], threads2[n]; 
int num_stages= log(n)/log(2);



 minima_args *thread_args = (minima_args *) malloc(nthreads* sizeof(minima_args))      ;//create argument data for each thread.  

 minima_args *thread_args2 = (minima_args *) malloc(n* sizeof(minima_args))      ;//create argument data for each thread.  
printf("\t\t---------Make copy--------\n ");
for(int i=0; i<nthreads; i++)
 {
 
         thread_args[i].data=dataset; //give thread data to copy 
         thread_args[i].n=n;
         thread_args[i].tid=i;
         thread_args[i].nthreads=nthreads;
 
 
 //      pthread_create(&threads[i] ,NULL,f_minima_wrapper ,&thread_args[i]);
         pthread_create(&threads[i] ,NULL,copy_dataset_parallel ,&thread_args[i]);
 }

 for(int j=0; j<nthreads; j++)
 {
 
         pthread_join (threads[j] , NULL);
 }

printf("\t\t---------Finished copy--------\n ");

free(thread_args); 
//free(thread_args2); 

printf("\n\n");


/*execute prefix minima */

 int r=1;
 int y;

printf("Debugging ........\n ");
for(int i=0; i<n; i++)
{
	printf("pref_min_log_p[%d]:%d \n ", i, pref_min_log_p[i]);
	
}
 
 for(int y=1; y<= num_stages; y++ ){  //log(n) stages
 
         int l=0; //restart left value for min operation 
	printf("stage: %d\n ",y);  
 //      #pragma omp parallel for private(y,tid6) shared(mini,r)
 

/*create threads */
         for(int q=r; q<n; q++) // create r to n-1 threads //workign now
         {
 
                 thread_args2[q].data=pref_min_log_p;    
		  thread_args2[q].n=n;
                 thread_args2[q].tid=q;
                 thread_args2[q].nthreads=nthreads;
                 thread_args2[q].celID=q;
                 thread_args2[q].cel_jump_length=r;
 
//some where here is prob
          pthread_create(&threads2[q] ,NULL,execute_parallel,&thread_args2[q]);
         }//end parallel loop
 
 
         for(int j=r; j<n; j++)
         {
                 pthread_join (threads2[j] , NULL);
         }
 
        r=r*2;
	

 
 }//end sequential loop

//free(thread_args2); 

/*execute suffix minima  */

  r=1;
 
 for(int y=1; y<= num_stages; y++ ){  //log(n) stages
 
         int l=n-1; //restart left value for min operation 
 //      #pragma omp parallel for private(y,tid6) shared(mini,r)
 

/*create threads */
         for(int q=l-r; q>=0; q--) // create r to n-1 threads //workign now
         {
 
                 thread_args2[q].data=suf_min_log_p; //give thread data to copy     thread_args2[q].n=n;
                 thread_args2[q].tid=q;
                 thread_args2[q].nthreads=nthreads;
                 thread_args2[q].celID=q;
                 thread_args2[q].cel_jump_length=r;
 
 
          pthread_create(&threads2[q] ,NULL,execute_parallel_suf_min,&thread_args2[q]);
         }//end parallel loop
 
 
         for(int j=l-r; j>=0; j--)
         {
                 pthread_join (threads2[j] , NULL);
         }
 
        r=r*2;
	
 
 }//end sequential loop





}//end pthread parallelfunction
#endif



/*Pthread functions */
 void *copy_dataset_parallel(void* input) {
  
          minima_args *in= (minima_args*)input;
  
          int data_length, thread_id;
          int chunck_size, start, end;
  
  
          /*fetch input data */
          data_length= in->n;
          thread_id= in ->tid ;
  
          //int out_data[data_length]; 
          chunck_size= data_length / in->nthreads;
  
  
  //printf("\n-----------------------------------------\n");
  //printf("chunck_size: %d \t n: %d \t threads:%d\n", chunck_size, data_length, in->    nthreads);
  

 start=thread_id*chunck_size;
  end= start+chunck_size;
  
  
 // printf("start: %d \t  end :%d\n", start, end);
  
  /*fetch input data */
  in ->n = data_length;
  in ->tid = thread_id;
  
  
  
  for(int i=start; i<end; i++)
  {
  
         pref_min_log_p [i]= *( (in->data)+i);
         suf_min_log_p [i]= *( (in->data)+i);
 
         //printf("ThreadID:%d \t in_data[%d]: %d \t out_data[%d]: %d \t \n ",in->    tid, i,*( (in->data)+i), i,out_data[i] );

         /////printf("ThreadID:%d \t in_data[%d]: %d \t out_data[%d]: %d \t \n ",in->ti    d, i,*( (in->data)+i), i,pref_min_log[i] );
  }
  
// printf("\n-----------------------------------------\n");
  
  
  }//end function parallel data set copy
 //---------------------------------------------------


void *execute_parallel_suf_min (void* input) {

         int data_length, thread_id;
         int chunck_size, start, end;
         int celID_s;
         int cel_jump_length_s;
 
         minima_args *in= (minima_args*)input;
 
         /*fetch input data */
         data_length= in->n;
         thread_id= in ->tid ;
         celID_s = in ->celID ;
         cel_jump_length_s= in ->cel_jump_length ;
 
 
         int y = celID_s;
         int r=  cel_jump_length_s;

         chunck_size= data_length / in->nthreads;
 
       ( in->data)[y]= min( ( in->data)[y+r] , ( in->data)[y] ) ;
   //    ( in->data)[y]=43; 

   //      pref_min_log_p[y]= min (   pref_min_log_p[y-r] ,  pref_min_log_p[y] ) ;



} 



void *execute_parallel (void* input) {
 
 
         int data_length, thread_id;
         int chunck_size, start, end;
         int celID_s;
         int cel_jump_length_s;
 
         minima_args *in= (minima_args*)input;
 
         /*fetch input data */
         data_length= in->n;
         thread_id= in ->tid ;
         celID_s = in ->celID ;
         cel_jump_length_s= in ->cel_jump_length ;
 
 
         int y = celID_s;
         int r=  cel_jump_length_s;

         chunck_size= data_length / in->nthreads;
 
     ( in->data)[y]= min( ( in->data)[y-r] , ( in->data)[y] ) ;

     //   pref_min_log_p[y]= min (   pref_min_log_p[y-r] ,  pref_min_log_p[y] ) ;


    
 
 }//end function parallel data set copy




 
 
 

 
int min(int a, int b)
{
	if ( a< b) return a; // a<b 
	else return b; // b<a ^ a=b  
}
