/***************************************************************** 
      --Authors: Javier James
     --Date created: Sat 27 July 2013
    --file name: pref_suf.cpp
     --Descrition:   
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
//#define RUN_PTHREADS

#ifdef RUN_OPENMP 
#include <omp.h>  
#endif 

#ifdef RUN_PTHREADS 
#include <pthread.h>  

#endif 

int nthreads, tid;

/* --- Global shared memory data ---*/ 

void* rank(void *a);
void sort(void *a, void *b);

int *sorted_array;


void create_ordered_set (int *set, int num_set_elements);

int *S;  


int A [] = {58  ,89 , 32 ,73  ,131 ,156 ,30 ,29 ,
            141 ,37 ,133 ,151 ,88  ,53 ,122 ,126,
            131 ,49 ,130, 115 ,16  ,83 ,40  ,145,
            10  ,112 ,20 ,147 ,14 ,104 ,111 ,92 } ;

int min(int a, int b); 
void seq_function2(int* dataset, int* p_min, int* s_min, int d_length);
void* par_function_OMP(int* dataset, int* p_min, int* s_min, int d_length);
void* par_function_PTH(int* dataset, int* p_min, int* s_min, int d_length);


int mergesort(int *data, int data_size); 
int *mergesort2(int *data, int data_size);
int *mergesort3(int *data, int data_size);
int *merge(int *A, int A_size, int *B, int B_size); 
//int *merge2(int *A, int A_size, int *B, int B_size, int *C); 
int *merge2(int *data_left, int size_left, int *data_right, int size_right, int *data_result);
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

   /*create Lineair ordered set */ 
	int S_size= 12; 
        S= (int *) malloc (S_size*sizeof(int) );

	for(int i=0; i<S_size; i++)
	{	
		S[i] = i; 		
	} 
 
	//create_ordered_set (S, S_size); 
/*	for (int j=0; j<S_size; j++)
	{
		printf("S[%d]: %d \n  ", j, S[j]); 
	}
*/
	int A [] = { 28 ,36 ,40 ,61 ,68 ,71 ,123 ,149 } ; 
	int B [] = { 2, 5  , 18, 21, 24, 29, 31,  33, 
		     34, 35, 47, 48 , 49, 50, 52, 62, 
		    66, 70,  73, 80, 88,  89, 114, 124, 
		    125, 131, 143, 144, 145, 148, 155, 159 } ; 


	int test [] = { 40 ,61,  28 ,71 ,36 ,68 ,123 ,149 } ; 
	int test3 [] = { 40 ,61,  28 ,71} ; 

	/*Testing merge   */
	/*int merge_left []= {2,5,40,88,89,103} ; 
	int merge_right []= {1,6,39,110,150} ; 
	int left_size= sizeof(merge_left)/sizeof(int);
	int right_size= sizeof(merge_right)/sizeof(int);
	//int merge_out[left_size+right_size];
	int out_size= left_size + right_size;
	int merge_out[out_size];
	
	merge2(merge_left, left_size, merge_right, right_size,merge_out);
*/
/*
	printf("Debug merge output\n");
	//for(int i=0; i< left_size+right_size; i++) 
	for(int i=0; i< out_size; i++) 
	{
		printf("merge_out[%d]:%d \n",i,merge_out[i]); 	
	} 
*/ 

	int test_size= sizeof(test)/sizeof(int); 
//	mergesort(test, test_size);
	int *test2;

	 test_size= sizeof(test3)/sizeof(int); 
	test2= mergesort3(test3, test_size);

	   result.tv_sec = 0;

	result.tv_usec = 0;

//=======================================================

	/*Sequential2 algorithm */ 	
 	//result.tv_usec=0; 

        result.tv_sec = 0;
	result.tv_usec = 0;
	gettimeofday (&startt, NULL);

	//seq_function2(A, pref_min_log, suf_min_log, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
	

	/*print results  */
/*	cout<<"\n\n -------Sequential 2 Results------- "<<endl;
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	
	cout<<"\n \t\t A \t pref_min \t suf_min "<<endl;
	cout<<"---------------------------------------------------------------------- "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<A[f] <<"\t\t"<< pref_min_log[f]<<"\t\t"<<suf_min_log[f]<<endl;
	}
*/
//=======================================================

#ifdef RUN_OPENMP 

	/*Parallel algorithm */ 	
	
 	//result.tv_usec=0; 
  /*      result.tv_sec = 0;
	result.tv_usec = 0;
	gettimeofday (&startt, NULL);

	par_function_OMP(A, pref_min_log, suf_min_log, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
*/

	/*print results  */
/*	cout<<"\n\n -------Parallel OpenMP  Results------- "<<endl;
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	
	cout<<"\n \t\t A \t pref_min \t suf_min "<<endl;
	cout<<"---------------------------------------------------------------------- "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<A[f] <<"\t\t"<< pref_min_log[f]<<"\t\t"<<suf_min_log[f]<<endl;
	}

*/
#endif 
//=======================================================

#ifdef RUN_PTHREADS 
	/*Parallel algorithm */ 	
/*

        pref_min_log_p = (int  *) malloc(data_length* sizeof(int))      ;//create argument data for each thread.   
        suf_min_log_p = (int  *) malloc(data_length* sizeof(int))      ;//create argument data for each thread.   
	
 	//result.tv_usec=0; 
        result.tv_sec = 0;
	result.tv_usec = 0;
	gettimeofday (&startt, NULL);

	par_function_PTH(A, pref_min_log_p, suf_min_log_p, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);

*/
	/*print results  */
/*	cout<<"\n\n -------Parallel PTHREADS  Results------- "<<endl;
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	
	cout<<"\n \t\t A \t pref_min \t suf_min "<<endl;
	cout<<"---------------------------------------------------------------------- "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<A[f] <<"\t\t"<< pref_min_log_p[f]<<"\t\t"<<suf_min_log_p[f]<<endl;
	}


        free(pref_min_log_p);
	free(suf_min_log_p);   

*/
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


void create_ordered_set (int *set, int num_set_elements)
{
     /* Generate a seed input */
          srand ( time(NULL) );
          for(int k=0; k<num_set_elements; k++){
                 set[k] = rand();
          }


} 

void create_sordered_set (int *set, int num_set_elements)
{
     /* Generate a seed input */
          srand ( time(NULL) );
          for(int k=0; k<num_set_elements; k++){
                 set[k] = rand();
          }


}

int mergesort(int *data, int data_size)
{
	
//	static int *data_copy=data; //copy data and allow address to be accessed once function is left for recurssion 
       
//	int *data_copy = data;
	static int count=0;  
	int B[data_size/2]; 
	int C[data_size/2]; 

	int left=0;
	int right=0;

//	int *left_data, *right_data; 
	int middle=data_size/2;
	int left_data[middle], right_data[middle]; 
	int *p_data; 
	int results;		
//printf("\t\t--------------Start Call ------------Debugging \n\n");
//	printf("count value: %d: \n ", count++);   
	if( data_size==1)
	{ 
//		printf("-----Size 1 data--- \n  ");   
		return data[0];
//		left= left_data[0]; 
//		right= right_data[0]; 
		
	//	return(10);		
	}
	else{	
//		printf("Data is greater than 1\n  ");   

		/*Devide data  */
		/*Devide and Conquer approach */
		for(int i=0; i<middle; i++)
		{
			left_data[i]= data[i]; 		
			right_data[i]= data[i+middle]; 		
			//left_data[i]= data_copy[i]; 		
			//right_data[i]= data_copy[i+middle]; 		
		}
   //  printf("leftside recursive call\n"); 
	left= mergesort(left_data, middle); 
     // printf("right side recursive call\n"); 
   	right= mergesort(right_data, middle); 	

	
	printf("left:%d \t right:%d  \n", left, right);
      	return(2); 
	//return (merge(left,right)); 
	} 

/*printf("Debugging : Results \n");
for(int i=0; i<middle; i++)
	printf("left_data[%d]:%d \t right_data[%d]:%d  \n",i, left_data[i], i+middle, right_data[i]);


printf("\t\t--------------Call done ------------Debugging \n\n");
*/

	/* */


//	if( data_size==1) 
//		return A[1];
//	else 
//	{
	//	B := mergesort(&A[1], n/2);
	//	C := mergesort(A&[n/2+1] .. n]);
	//	return merge(B, n/2, C, n/2)
//	} 

/*

merge( X, m, Y, n)
if m = 1 and n=1 then return min(X[1], Y[1]) ++ max(X[1],Y[1])
else
if X[1] ≤ Y[1] then return X[1]++merge(X[2..m],m-1,Y,n
else
return Y[1]++merge(X,m,Y[2..n-1], n-1)
*/
}

int *mergesort2(int *data, int data_size)
{
	
	static int *data_copy=data; //copy data and allow address to be accessed once function is left for recurssion 
 
	int B[data_size/2]; 
	int C[data_size/2]; 

//	int *left_data, *right_data; 
	int middle=data_size/2;
	int left_data[middle], right_data[middle]; 

	
	if( data_size==1) 
		return &A[1];
	
	else{	
		printf("Data is greater than 1\n  ");   

		/*Devide data  */
		for(int i=0; i<middle; i++)
		{
			//left_data[i]= data[i]; 		
			//right_data[i]= data[i+middle]; 		
			left_data[i]= data_copy[i]; 		
			right_data[i]= data_copy[i+middle]; 		
		}
        
	 mergesort(left_data, middle); 
       mergesort(right_data, middle); 	
	} 

printf("Debugging : Results \n");
for(int i=0; i<middle; i++)
	printf("left_data[%d]:%d \t right_data[%d]:%d  \n",i, left_data[i], i+middle, right_data[i]);


}



int *mergesort3(int *data, int data_size)
{
	
//	static int *data_copy=data; //copy data and allow address to be accessed once function is left for recurssion 
       
//	int *data_copy = data;
	static int count=0;  
//	int B[data_size/2]; 
//	int C[data_size/2]; 

	int left=0;
	int right=0;

//	int *left_data, *right_data; 

/*
	int middle; 
	if(data_size=1) middle =1; 
	else middle=data_size/2; 
*/

	printf("\\-------------------------------------------------------\\ \n "); 
	printf("\t\t  new call \n");
	printf("Call ID:  %d \n  ", count++); 
	printf("-------------------------------------------------------\n\n "); 
	printf("\t\t Debugging intput........\n "); 
	printf("input size: %d \n  ", data_size); 
	for(int i=0; i<data_size; i++) 
	{
		printf("data[%d]: %d \n  ", i, data[i]); 
	}

	printf("\t\t Debugging intput end \n "); 




//	printf("\t\t +++++++Debugging datasize+++++++\n "); 
//	printf("\t\t size before middle:%d \n", data_size); 
	int middle=data_size/2;
	
	printf("\t\t size of middle :%d \n", middle);
 
	int left_data[middle], right_data[middle]; 
	int *p_data, *p_left, *p_right; 

	int results;		


	//printf("\n-------------------------------------------------------\n\n ");	
	printf("Start evaluation....\n "); 
	/*devide until there is 1 element left */
	if(data_size>1)
	{  
		printf("size>1\n "); 	
		printf("Deviding.....\n\n "); 	
		/*Devide and Conquer approach */
		/*copy data */
		for(int i=0; i<middle; i++)
		{
			left_data[i]= data[i]; 		
			right_data[i]= data[i+middle]; 		
			//left_data[i]= data_copy[i]; 		
			//right_data[i]= data_copy[i+middle]; 		
		}
		printf("Deviding End\n "); 	

		printf("\n\nDebug devision results....\n "); 
	
		printf("****************************Debugging left data....****************************\n "); 
//		printf("\n\n\t\tDebugging left data.... \n "); 
		for(int i=0; i<middle; i++)
			printf("left_data[%d]:%d\n", i, left_data[i]); 
		

		printf("\n\n****************************Debugging right data....****************************\n "); 

//		printf("\n\n\t\tDebugging right data.... \n "); 
		for(int i=0; i<middle; i++)
			printf("right_data[%d]:%d\n", i, right_data[i]); 


		printf("\n\n****************************Debugging end....****************************\n "); 
			
       	//	p_left=(int *) malloc(middle*2*sizeof(int)); 		
       	 	//p_right=(int *) malloc(middle*2*sizeof(int)); 		

		printf("Starting left recursion...\n ");
		printf("Debug data for function....\n");  
		
		for(int i=0; i<middle; i++) 
			printf("output to function.. left_data[%d]:%d\n",i, left_data[i]); 
		printf("Debug data for function end\n");  
		printf("Calling function now\n\n");  
		
		/*recursive calls */
		printf("\t\t !!--Left call [%d] start--\n\n ", count); 
		p_left= mergesort3(left_data, middle); 
		printf("\t\t !!--Left call[%d]  end--\n\n ", count);
		
	//	printf("\t\t ~~~~~~~~~~~~~~~~~~~`\n ", count); 
	 
		printf("Starting right recursion...\n ");
		printf("Debug data for function....\n");  
		
		for(int i=0; i<middle; i++) 
			printf("output to function.. right_data[%d]:%d \n",i, right_data[i]); 
		printf("Debug data for function end\n");  
		printf("Calling function now\n\n");  
		
		printf("\t\t !!--Right call [%d] start--\n\n ", count); 
		p_right= mergesort3(right_data, middle); 
		printf("\t\t !!--Right call[%d]  end--\n\n ", count);
		
		printf("End of Recursive calls\n "); 
		printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"); 
		//printf("\t\t----Debugging Results p_left & p_right....\n\n "); 
/*		printf("\t\t----Debugging Results left_data & right_data ....\n\n "); 
		
		printf("Debugging : Results \n");
		for(int i=0; i<middle; i++)
		printf("left_data[%d]:%d \t right_data[%d]:%d  \n",i, left_data[i], i+middle, right_data[i]);
		//printf("left_data[%d]:%d \t right_data[%d]:%d  \n",i, p_left[i], i+middle, p_right[i]);

*/
 
		printf("\n\n............. \t............... \t ...........\n"); 
		printf("\n\nMerge elements....\n\n"); 
		printf("Debug merge data ....\n");  
		
		for(int i=0; i<middle; i++) 
			printf("left_data[%d]: %d \t.. right_data[%d]:%d \n",i, left_data[i], i, right_data[i]); 
		printf("Debug merge data end \n");  
		printf("Merge now\n\n");  
		printf("\n\n............. \t............... \t ...........\n"); 
	
//		int * merge_results= (int *) malloc(2*middle*sizeof(int));
	


/*	int left_size= sizeof(left_data)/sizeof(int);
	int right_size= sizeof(merge_right)/sizeof(int);
	int merge_out[left_size+right_size];
	merge2(merge_left, left_size, merge_right, right_size,merge_out);
*/	 		
	int merge_out[2*middle];
	merge2(left_data, middle, right_data, middle,merge_out);
	
/*	printf("Debug merge output\n");
	for(int i=0; i< 2*middle; i++) 
	{
		printf("merge_out[%d]: %d \n",i,merge_out[i]); 	
	} 
	printf("End Debug merge output\n");
*/

//	merge2(p_left, middle, p_right,middle, merge_results); 
//		int merge_results2[2*middle]; 
//		merge2(left_data, middle, right_data,middle, merge_results2); 
	//	free(p_left); 
	//	free(p_right);
//count++;
	
	}
//	else return &data[0];  

		printf("size<1\n ");
		 	
//printf("\t\t--------------Start Call ------------Debugging \n\n");
//	printf("count value: %d: \n ", count++);   
/*	if( data_size==1)
	{ 
//		printf("-----Size 1 data--- \n  ");   
		return data[0];
//		left= left_data[0]; 
//		right= right_data[0]; 
		
	//	return(10);		
	}
	else{	
//		printf("Data is greater than 1\n  ");   
*/
		/*Devide data  */
		/*Devide and Conquer approach */
/*		for(int i=0; i<middle; i++)
		{
			left_data[i]= data[i]; 		
			right_data[i]= data[i+middle]; 		
			//left_data[i]= data_copy[i]; 		
			//right_data[i]= data_copy[i+middle]; 		
		}
   //  printf("leftside recursive call\n"); 
	left= mergesort(left_data, middle); 
     // printf("right side recursive call\n"); 
   	right= mergesort(right_data, middle); 	

	
	printf("left:%d \t right:%d  \n", left, right);
      	return(2); 
	//return (merge(left,right)); 
	} 
*/
/*printf("Debugging : Results \n");
for(int i=0; i<middle; i++)
	printf("left_data[%d]:%d \t right_data[%d]:%d  \n",i, left_data[i], i+middle, right_data[i]);
*/


}

//int *merge2(int *A, int A_size,int *B, int B_size, int *C)
int *merge2(int *data_left, int size_left, int *data_right, int size_right, int *data_result)
{
	int p1=0; 
	int p2=0; 
	int p3=0;
	
	int i=0; 
	int j=0; 
	int k=0; 
	
	int i_left=0; 
	int i_right=0; 
	int i_result=0;

	int size_result= size_left + size_right;  

	printf("\n\nEvaluate merge input.........\n ");
	for(int i=0; i<size_left; i++) 
		printf("left_data[%d]: %d \n ",i, data_left[i]);   
	printf("\n"); 
	for(int i=0; i<size_right; i++) 
		printf("right_data[%d]: %d\n ",i, data_right[i]);   
	printf("\n"); 
	for(int i=0; i<size_result; i++) 
		printf("result_data[%d]: %d\n ",i, data_result[i]);   
	printf("End evaulation intput  merge\n ");

	/*merge in sort order as long  any array isnt fully sorted */
	while( (i_left<size_left) && (i_right<size_right) ) 
	{
		
/*printf("´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´\n "); 
        printf("Debug all data....\n\n "); 
  	printf("left size: %d \n left index:%d \n left data[%d]%d \n\n ", size_left, i_left, i_left, data_left[i_left]); 	
  	printf("Right size: %d \n Right index:%d \n Right data[%d]%d \n\n ", size_right, i_right, i_right, data_right[i_right]); 	
  	printf("result size: %d \n result index:%d \n result data[%d]%d \n\n ", size_result, i_result, i_result, data_result[i_result]); 	

printf("´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´\n "); 
			
*/

		printf("inside while loop\n show data to compare\n\n  "); 			
		printf("data_left[%d]:%d & data_right[%d]: %d \n  ",i_left, data_left[i_left], i_right,data_right[i_right]); 			

		if (data_left[i_left]<data_right[i_right])
		{
			data_result[i_result] = data_left[i_left];
			i_left++;
		}
		else 
		{
			data_result[i_result] = data_right[i_right];
			i_right++;
		
		}
		printf("Inside merge. result[%d]:%d\n",i_result,data_result[i_result]); 
		i_result++; 
	
	}//end while loop	

		
		printf("Merge the rest data ........\n  "); 			
		printf("i_left:%d \t i_right:%d \t i_result:%d\n \n", i_left,  i_right, i_result); 
		printf("size_left:%d \t size_right:%d \t size_result:%d\n \n",size_left,  size_right, size_result); 

		

	int i_sync=0; //sync position of copy target with that of element index to copy
	/*copy remaining data */
	if(i_left==size_left) //leftside finished 
	{
	//	int i_sync=0; //sync position of copy target with that of element index to copy
	 
		printf("Start copy remaining right...........\n"); 
		/*start from up following element to copy */
		for(int t=i_right; t<size_right; t++) //copy remaining right
		{
			 
		printf("data to copy. \t data_right[%d]:%d \n ",t, data_right[t]); 
		//	data_result[i_result+t-1]= data_right[t];
			//data_result[i_result+t]= data_right[t];
			data_result[i_result+i_sync]= data_right[t];
			i_sync++;
		//	printf("data_result[%d]: %d\n ",t,data_right ); 
			//printf("Copy right data. result[%d]:%d\n",i_result,data_result[i_result]); 
//			printf("Copy right data. result[%d]:%d\n",t,data_result[t]); 
		} 
		printf("End copy remaining right\n\n"); 
	}
	else 
	{
		printf("Start copy remaining left...........\n"); 
		for(int t=i_left; t<size_left; t++) //copy remaining right
		{ 	
	//			data_result[i_result+i-1]= data_left[i_left];
			//data_result[i_result+i]= data_left[i_left];
			data_result[i_result+i_sync]= data_left[t];
			i_sync++;
			printf("Copy left data. result[%d]:%d\n",i_result,data_result[i_result]); 
			
		}
		printf("End copy remaining left\n\n"); 
	}


for(int i=0; i<size_left+size_right; i++)
printf("c results C[%d]: %d \n", i , data_result[i]); 

}

 
int *merge(int *A, int A_size,int *B, int B_size)
{
	//int *pA,*pB;
	int p1,p2; 
	int value;
       
	int merge_size= A_size + B_size; 	
	p1=1; 
	p2=1; 

	for(int i=0; i< merge_size; i++)
	{

		if(A[p1]<=B[p2]){
			value = A[p1];
			p1++;
		}
		else{ 
			value = B[p2]; 
			p2++;	
		} 
	}
} 
