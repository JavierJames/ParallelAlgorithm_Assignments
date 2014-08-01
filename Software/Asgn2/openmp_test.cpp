#include <omp.h>
#include <stdio.h>
#include <stdio.h>
#include <math.h>


int main (int argc, char *argv[]) {

int nthreads, tid;

int A[8] = {25, 54, 30, 22, 43, 1, 5, 65 } ; 

/* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nthreads, tid)
 {

 	/* Obtain thread number */
 	tid = omp_get_thread_num();
 	printf("Hello World from thread = %d\n", tid);

 	/* Only master thread does this */
 	if (tid == 0) 
 	{
  		nthreads = omp_get_num_threads();
 		printf("Number of threads = %d\n", nthreads);
 	}	

 } /* All threads join master thread and disband */

printf("\n\n\n\n ");  

	int NCPU,tid2,NPR,NTHR;
 #pragma omp parallel private(tid2)
  {

    	/* get the total number of CPUs/cores available for OpenMP */
    	NCPU = omp_get_num_procs();
    	/* get the current thread ID in the parallel region */
    	tid2 = omp_get_thread_num();
    	/* get the total number of threads available in this parallel region */
    	NPR = omp_get_num_threads();
    	/* get the total number of threads requested */
    	NTHR = omp_get_max_threads();
    	
	/* only execute this on the master thread! */
    	if (tid2 == 0) {
		printf("message from thread %d \n", tid2);      
		printf("%i : NCPU\t= %i\n",tid2,NCPU);
      	
	printf("%i : NTHR\t= %i\n",tid2,NTHR);
      	printf("%i : NPR\t= %i\n",tid2,NPR);
	printf("message from thread %d Finished\n\n\n", tid2);      
    	}
    	
	printf("%i : hello multicore user! I am thread %i out of %i\n",tid2,tid2,NPR);
  }




/*

int max= log(8);
int tid3; 
int B[6]; 
//for(int u=1; u<= max; u++ ){

	#pragma omp parallel  private(tid3) shared(B) 
	{   
		tid3 = omp_get_thread_num();	
		#pragma omp for 
	    	for(int d=0; d <4; d++){

		B[tid3]= A[d] + tid3; 	
	
	} 	 	
//            printf("id: %d  \t sum: %d\n",tid3, tid3 +10 ); 
            printf("id: %d  \t B[%d] = %d \n",tid3, tid3, B[tid3] ); 

	 } 
	
//} 
*/



/*
int A2[4] = {1,2,43,45 }; 
int tid4; 
int C[6];
int par_elem= 4; // length of array. going to apply Kogge-stone 

max= log(4);


  
for(int u=1; u<= max; u++ ){

	#pragma omp parallel  private(tid4) shared(A,B,max) 
	{   
		tid4 = omp_get_thread_num();
		max--;
		
		#pragma omp for 
	    	for(int s=0; s <max; d++){

		B[tid3]= A[d] + tid3; 	
	
	} 	 	
//            printf("id: %d  \t sum: %d\n",tid3, tid3 +10 ); 
            printf("id: %d  \t B[%d] = %d \n",tid3, tid3, B[tid3] ); 

	 } 


	
}

*/




//---------------------------------------------------------------


//int A6[8] = {58,89,32,73, 131, 156, 30, 29 };       
//int A6[7] = {58,89,32,73, 131, 30, 29 };       

int data[] = {58  ,89 , 32 ,73  ,131 ,156 ,30 ,29,
	        141 ,37 ,133 ,151 ,88  ,53 ,122 ,126, 
		131 ,49 ,130, 115 ,16  ,83 ,40  ,145,
		10  ,112 ,20 ,147 ,14 ,104 ,111 ,92 } ;
int tid6; 				   
/*
int mini[7];
int n=7;
int num_stages= log(7)/log(2);
int g; 
*/
int par_elem= 4; // length of array. going to apply Kogge-stone 


// int data []= {58,89,32,73,131,156,30,29,141,37,133,151,88,53,122,126,131,49,130,
//115,16,83,40,145,10,112,20,147,14,104,111,92 } ;

int n= sizeof(data) /sizeof(int);
int num_stages= log(n)/log(2);
int g; 
int mini[n];

printf("Initial values\n\n"); 
printf("n: %d | stages:%d  | \n\n", n, num_stages); 

//printf("Value of max: %d \n\n ", num_stages); 
int d6;

/*copy data set */
//#pragma omp parallel for private (g,tid6) shared(A6, B6, n)
//#pragma omp parallel for private (g,tid6) shared(A6, mini, n)
#pragma omp parallel for private (g,tid6) shared(data, mini, n)
 	for(g=0; g<n; g++) 
  	{
		//mini[g] = A6[g]; 
		mini[g] = data[g]; 
//		printf("Thread#: %d ==> \t ==> \t mini(%d):%d  \t A(%d): %d  \n",omp_get_thread_num(),g,mini[g],g, A6[g]  ); 
		printf("Thread#: %d ==> \t ==> \t mini(%d):%d  \t data(%d): %d  \n",omp_get_thread_num(),g,mini[g],g, data[g]  ); 
				
	}

printf("\n\n Done copyig !! \n\n");
/*print dataset */
/*for(int j=0; j<n; j++)
{
	printf("A[%d}:\t%d\n ",j, A6[j] ); 

} 
 */

/*calc pref_min  */
 
int r=1;
int y;  
for(int q=1; q<= num_stages; q++ ){  //log(n) stages
	
	int l=0; //restart left value for min operation 

	#pragma omp parallel for private(y,tid6) shared(mini,r)
	for(y=r; y<n; y++) 
	{
		int minimum; 
	
		tid6 = omp_get_thread_num();
	//	mini[y]= min ( (y-r),y );
		if (mini[y-r]< mini[y])  minimum= mini[y-r]; 
		else minimum= mini[y]; 

		mini[y]= minimum; 
		printf("Thread#: %d ==> \t ==> \t |  Y:%d  R: %d | min(%d, %d) \t \t ==> \t mini(%d):%d  \n",tid6, y, r,  mini[y-r], mini[y], y,mini[y]  ); 
		

	}
	
	r=r*2;  


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
	
}

 
printf("\n\n\n------------Results of prefix ------- \n\n " ); 
/*print results */
for( int  j=0; j<n; j++)
{
	printf("mini[%d}:\t%d\n ",j, mini[j] ); 

} 
//---------------------------------------------------------------

}//end main

