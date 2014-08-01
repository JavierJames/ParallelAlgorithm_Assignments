/***************************************************************** 
      --Authors: Javier James
     --Date created: Mon 17 June 2013
    --file name: pref_suf.cpp
     --Descrition: This file provides a sequential implementation of the a prefix suffix pref_min in C  SW algorith for DNA sequence alignment using C code.  
      --References: 
     --Last update: Mon 17 13 June 2013
      --Note: 
   
 *******************************************************************/
  
//Includes
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <sys/time.h>
using namespace std;

#include <omp.h>  

int nthreads, tid;

//data set in global memory 
int A [32]= {58,89,32,73,131,156,30,29,141,37,133,151,88,53,122,126,131,49,130,115,16,83,40,145,10,112,20,147,14,104,111,92 } ;


void seq_function(int* dataset, int* p_min, int* s_min, int d_length);

void* par_function_OMP(int* dataset, int* p_min, int* s_min, int d_length);

int min(int a, int b); 


void seq_function2(int* dataset, int* p_min, int* s_min, int d_length);

int main(int argc, char** argv )
{
   	struct timeval startt, endt, result;
        
	int data_length;
	data_length = sizeof(A)/sizeof(int);
	
	int pref_min_log [data_length];
	int suf_min_log [data_length];

        result.tv_sec = 0;
	result.tv_usec = 0;

	/*Generate a seed input */
/*	srand ( time(NULL) );
        int test[6];
	for(int k2=0; k2<6; k2++) {
		test[k2] = rand();
		cout<<test[k2]<<endl;
	} 
*/

	/*Sequential algorithm */ 	
 	result.tv_usec=0; 
	gettimeofday (&startt, NULL);

	seq_function(A, pref_min_log, suf_min_log, data_length);

	gettimeofday (&endt, NULL);
        result.tv_usec = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
 	cout<<"time: \t " <<result.tv_usec/1000000<<"\t"<<result.tv_usec%1000000 <<endl;	





	/*Parallel algorithm */ 	

// for(int j=1; j< )
/*
#pragma omp parallel private(A) 
{ 
	int tid = omp_get_thread_num();
}

     int nCPU = omp_get_num_procs(); 
     cout<< "nubmer of CPU:  \t  " <<nCPU<<endl;
*/
	cout<<"i \t pref_min \t suf_min "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<pref_min_log[f]<<"\t\t"<<suf_min_log[f]<<endl;
	}





}//end main



void seq_function2(int* dataset, int* p_min, int* s_min, int d_length){
 /*calc pref_min  */
 
 int r=1;
 int y;
int mini= p_min; 
 n= d_length; 
 num_stages= log(n) / log(2);
 
 for(int q=1; q<= num_stages; q++ ){  //log(n) stages
 
         int l=0; //restart left value for min operation 
 
         for(y=r; y<n; y++)
         {
                 int minimum;
 
		minimum= min( mini[y-r], mini[y] ) ;
               //  if (mini[y-r]< mini[y])  minimum= mini[y-r];
                // else minimum= mini[y];
 
                 mini[y]= minimum;
                 printf("Thread#: %d ==> \t ==> \t |  Y:%d  R: %d | min(%d, %d) \t \t ==> \    t mini(%d):%d  \n",tid6, y, r,  mini[y-r], mini[y], y,mini[y]  );
 
 
         }
 
         r=r*2;
} 


}//end  fun

void seq_function(int* dataset, int* p_min, int* s_min, int d_length){

      

	/*calculate prefix minima */ 
	
	int pref_min;    					// T(n) = O(1)
	pref_min= dataset[0];					// T(n) = O(1)
	p_min[0]= pref_min;                                  	// T(n) = O(1)
	
	for(int i=1; i<d_length; i++)				// T(n) = O(d_length))= O(n)
	{
	//	if(dataset[i]<pref_min) pref_min= dataset[i];
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
		//	if(dataset[t]<suf_min) suf_min= dataset[t];
			suf_min= min(suf_min, dataset[t] );
		}
			s_min[i]= suf_min;
		
	}

}


void* par_function_OMP(int* dataset, int* p_min, int* s_min, int d_length){

	
     int nCPU = omp_get_num_procs(); 
     cout<< "nubmer of CPU:  \t !!!!!!! " <<endl;
	/*calculate prefix minima */ 
	
	int pref_min;
	pref_min= dataset[0];
	p_min[0]= pref_min;
	
	for(int i=1; i<d_length; i++)
	{
		if(dataset[i]<pref_min) pref_min= dataset[i];
		p_min[i]= pref_min;
	}

     
	 /*calculate sufix  minima */ 
	int suf_min;

	for(int i=0; i<d_length; i++)
	{
		suf_min= dataset[i];
		for(int t=i; t<d_length; t++)
		{
			if(dataset[t]<suf_min) suf_min= dataset[t];
		}
			s_min[i]= suf_min;
		
	}

}


//void* par_function_OMP(void* a){
	/* The code for threaded computation */
	// Perform operations on B
//}


void* par_function_PTH(void* a){
	/* The code for threaded computation */
	// Perform operations on B
}



int min(int a, int b)
{
	if ( a< b) return a; // a<b 
	else return b; // b<a ^ a=b  
} 
