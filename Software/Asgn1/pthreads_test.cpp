//#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



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

int *pref_min_log;

void *copy_dataset_parallel(void* input) {

	minima_args *in= (minima_args*)input;

	int data_length, thread_id;  
	int chunck_size, start, end; 

	
	/*fetch input data */
	data_length= in->n; 
	thread_id= in ->tid ; 

	//int out_data[data_length]; 
	chunck_size= data_length / in->nthreads;


printf("\n-----------------------------------------\n");
printf("chunck_size: %d \t n: %d \t threads:%d\n", chunck_size, data_length, in->nthreads);

start=thread_id*chunck_size; 
end= start+chunck_size;


printf("start: %d \t  end :%d\n", start, end);

/*fetch input data */
in ->n = data_length; 
in ->tid = thread_id; 



for(int i=start; i<end; i++)
{
	
//	out_data[i]= *( (in->data)+i);
	pref_min_log [i]= *( (in->data)+i);

	//printf("ThreadID:%d \t in_data[%d]: %d \t out_data[%d]: %d \t \n ",in->tid, i,*( (in->data)+i), i,out_data[i] );
	printf("ThreadID:%d \t in_data[%d]: %d \t out_data[%d]: %d \t \n ",in->tid, i,*( (in->data)+i), i,pref_min_log[i] );
}

printf("\n-----------------------------------------\n");


}//end function parallel data set copy
//---------------------------------------------------



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
	int r= 	cel_jump_length_s;

//	int out_data[data_length]; 
	chunck_size= data_length / in->nthreads;
	
	int minimum; 

//	printf("ThreadID:%d \t in_data[%d]: %d \t pref_log[%d]: %d \t \n ",in->tid, i,*( (in->data)+....), i,pref_min_log[i] );
	


	
	//	tid6 = omp_get_thread_num();
	//	mini[y]= min ( (y-r),y );
		if (pref_min_log[y-r]< pref_min_log[y])  minimum= pref_min_log[y-r]; 
		else minimum= pref_min_log[y]; 

		pref_min_log[y]= minimum; 
//		printf("Thread#: %d ==> \t ==> \t |  Y:%d  R: %d | min(%d, %d) \t \t ==> \t mini(%d):%d  \n",tid, y, r,  mini[y-r], mini[y], y,mini[y]  ); 
		

/*





//printf("\n-----------------------------------------\n");
//printf("chunck_size: %d \t n: %d \t threads:%d\n", chunck_size, data_length, in->nthreads);

start=thread_id*chunck_size; 
end= start+chunck_size;


//printf("start: %d \t  end :%d\n", start, end);
*/
/*fetch input data */
/*in ->n = data_length; 
in ->tid = thread_id; 








for(int i=start; i<end; i++)
{
	
//	out_data[i]= *( (in->data)+i);
	pref_min_log [i]= *( (in->data)+i);

	//printf("ThreadID:%d \t in_data[%d]: %d \t out_data[%d]: %d \t \n ",in->tid, i,*( (in->data)+i), i,out_data[i] );
//	printf("ThreadID:%d \t in_data[%d]: %d \t out_data[%d]: %d \t \n ",in->tid, i,*( (in->data)+i), i,pref_min_log[i] );
}

//printf("\n-----------------------------------------\n");
*/


}//end function parallel data set copy



//--------------------------------------------------

void *f_minima_wrapper(void* input) {

	int pmin, smin, data_length, thread_id;  
	int chunck_size, start, end; 
	int minimum;
 	

	minima_args *in= (minima_args*)input;

//	printf("\n\n Inside of f_minima_Wrapper\n ") ; 
//	printf("\nThread ID: %d /t pmin value: %d, /t smin value: %d, /t n value: %d\n\n\n", in->tid, in->p_minima, in->s_minima, in->n) ; 

/*pmin= ( ( minima_args*) input)->p_minima+1; 
smin= ( ( minima_args*) input)->s_minima+1; 
data_length= ( ( minima_args*) input)->n+1; 
thread_id= ( ( minima_args*) input)->tid +1; 
*/


pmin= in->p_minima+1; 
smin= in ->s_minima+1; 
//data_length= in->n+1; //this works for nthreads=2
data_length= in->n; 
thread_id= in ->tid ; 


int out_data[data_length]; 

chunck_size= data_length / in->nthreads;

/*printf("\n-----------------------------------------\n");
printf("chunck_size: %d \t n: %d \t threads:%d\n", chunck_size, data_length, in->nthreads);

start=thread_id*chunck_size; 
end= start+chunck_size;


printf("start: %d \t  end :%d\n", start, end);
*/

in->p_minima = pmin; 
in ->s_minima = smin; 
in ->n = data_length; 
in ->tid = thread_id; 

//printf("\n\n Inside of f_minima_Wrapper, after changes\n ") ; 
//printf("\nThread ID: %d \t pmin value: %d, \t smin value: %d, \t n value: %d\n\n\n", in->tid, in->p_minima, in->s_minima, in->n) ; 
 
//printf("self thread Id :%d\n  ", (int) pthread_self()); 
//printf("self thread Id :%d\n  ", (int) pthread_getunique_np()); 

/*for(int i=start; i<end; i++)
{
	
	out_data[i]= *( (in->data)+i);
	printf("ThreadID:%d \t in_data[%d]: %d \t out_data[%d]: %d \t \n ",in->tid, i,*( (in->data)+i), i,out_data[i] );
//	printf("in_data[%d]: %d \t \n\n\n ", i,*( (in->data)+i) );
}

printf("\n-----------------------------------------\n");
*/

}//end of wrapper function 










int main (int argc, char *argv[]) {


int nthreads=4; 

pthread_t threads[nthreads], thread;  
  
minima_args *thread_args = (minima_args *) malloc(nthreads* sizeof(minima_args))  ;//create argument data for each thread.   


/*create threads */
//test
/*for(int i=0; i<nthreads; i++)
{

	thread_args[i].p_minima=i+2; 
	thread_args[i].s_minima=i+2; 
	thread_args[i].n=i+2; 
       	thread_args[i].tid=i;
       	thread_args[i].nthreads=nthreads;
	
	pthread_create(&threads[i] ,NULL,f_minima_wrapper ,&thread_args[i]);
*/
/*
	test.p_minima=+3; 
	test.s_minima=+3; 
	test.n=+3; 
       	test.tid=i;
   	
	pthread_create(&threads[i] ,NULL,f_minima_wrapper ,&test );
}

*/

/*
for(int j=0; j<nthreads; j++)
{
	
	pthread_join (threads[j] , NULL); 
}
free(thread_args);
*/

//	pthread_create(&thread ,NULL,f_minima_wrapper ,(void *) &test );
//	pthread_join (thread, NULL); 


/*printf("\n\n Inside of main\n ") ; 
printf("\nThread ID: %d /t pmin value: %d, /t smin value: %d, /t n value: %d\n\n\n  ", test.tid, test.p_minima, test.s_minima, test.n) ; 
*/
int tid;


int data[] = {58  ,89 , 32 ,73  ,131 ,156 ,30 ,29} ; /*,
	        141 ,37 ,133 ,151 ,88  ,53 ,122 ,126, 
		131 ,49 ,130, 115 ,16  ,83 ,40  ,145,
		10  ,112 ,20 ,147 ,14 ,104 ,111 ,92 } ;
*/

int n= sizeof(data) /sizeof(int);

printf("data set size: %d \n  ", n);

int num_stages= log(n)/log(2);
int g; 
int mini[n];


/*
printf("Initial values\n\n"); 
printf("n: %d | stages:%d  | \n\n", n, num_stages); 

//printf("Value of max: %d \n\n ", num_stages); 
int d6;
*/

 pref_min_log= (int *) malloc(n* sizeof(int))  ;//create argument data for each thread.   

/* -----copy data set ----- */
/*create threads */

//#pragma omp parallel for private (g,tid6) shared(data, mini, n)
 	

/*create threads */
for(int i=0; i<nthreads; i++)
{

	thread_args[i].data=data; //give thread data to copy 
	thread_args[i].n=n; 
       	thread_args[i].tid=i;
       	thread_args[i].nthreads=nthreads;

	
//	pthread_create(&threads[i] ,NULL,f_minima_wrapper ,&thread_args[i]);
	pthread_create(&threads[i] ,NULL,copy_dataset_parallel ,&thread_args[i]);
}

for(int j=0; j<nthreads; j++)
{
	
	pthread_join (threads[j] , NULL); 
}

//printf("Thread finish. thread ID:%d  thread_args[%d].n:%d\n\n ",thread_args[1].tid, 1, thread_args[1].n);
free(thread_args);

//printf("Thread finish. thread ID:%d  thread_args[%d].n:%d\n\n ",thread_args[1].tid, 1, thread_args[1].n);


/*now perform prefix minima */


	/*for(g=0; g<n; g++) 
  	{
		mini[g] = data[g]; 
		printf("Thread#:==> \t ==> \t mini(%d):%d  \t data(%d): %d \n",g,mini[g],g, data[g]  ); 
				
	}
*/
//printf("\n\n Done copyig !! \n\n");
/*print dataset */
/*for(int j=0; j<n; j++)
{
	printf("pref_min_log[%d]:\t%d\n ",j, pref_min_log[j] ); 

} 
*/


/*calc pref_min  */
 
int r=1;
int y;  

pthread_t threads2[n] ; 
minima_args *thread_args2 = (minima_args *) malloc(n* sizeof(minima_args))  ;//create argument data for each thread.   

for(int y=1; y<= num_stages; y++ ){  //log(n) stages
	
	int l=0; //restart left value for min operation 

//	#pragma omp parallel for private(y,tid6) shared(mini,r)

/*create threads */

	for(int q=r; q<n; q++) // create r to n-1 threads //workign now

	{

		thread_args2[q].data=pref_min_log; //give thread data to copy 
		thread_args2[q].n=n; 
    		thread_args2[q].tid=q;
       		thread_args2[q].nthreads=nthreads;
       		thread_args2[q].celID=q;
      	 	thread_args2[q].cel_jump_length=r;

	
	 pthread_create(&threads2[q] ,NULL,execute_parallel,&thread_args2[q]);
	}//end parallel loop


	for(int j=r; j<n; j++)
//	for(int j=0; j<n; j++)

	{
		pthread_join (threads2[j] , NULL); 
	}

	
	r=r*2;  

}//end sequential loop



/*	
	#pragma omp parallel  private(tid6) shared(A6,B6,max6) 
	{   
		tid6 = omp_get_thread_num();
		max6--;
		
		#pragma omp for 
	    	for(int s=0; s <max6; d6++){

		B6[tid6]= A[d6] + tid6; 	
	
	}
//            printf("id: %d  \t sum: %d\n",tid3, tid3 +10 ); 
   //         printf("id: %d  \t B[%d] = %d \n",tid3, tid3, B[tid3] ); 

	 } 


*/ 	 	
	
//}

 
printf("\n\n\n------------Results of prefix ------- \n\n " ); 
/*print results */
for( int  j=0; j<n; j++)
{
	printf("mini[%d}:\t%d\n ",j, pref_min_log[j] ); 

} 


//---------------------------------------------------------------


free(pref_min_log);
}//end main


