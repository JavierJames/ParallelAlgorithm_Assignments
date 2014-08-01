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
  
int A [32]= {58,89,32,73,131,156,30,29,141,37,133,151,88,53,122,126,131,49,130,115,16,83,40,145,10,112,20,147,14,104,111,92  } ;


int main(int argc, char** argv )
{
	int data_length;
	int pref_min;
	int suf_min;
	

	data_length = sizeof(A)/sizeof(int);
	int pref_min_log [data_length];
	int suf_min_log [data_length];
	pref_min= A[0];
	pref_min_log[0]= pref_min;
        
        /*calculate prefix minima */ 
	for(int i=1; i<data_length; i++)
	{
		if(A[i]<pref_min) pref_min= A[i];
		pref_min_log[i]= pref_min;
	}

        /*calculate sufix  minima */ 
	for(int i=0; i<data_length; i++)
	{
		suf_min= A[i];
		for(int t2=i; t2<data_length; t2++)
		{
			if(A[t2]<suf_min) suf_min= A[t2];
		}
			suf_min_log[i]= suf_min;
		
	}

	cout<<"i \t pref_min \t suf_min "<<endl;
	for(int f=0; f<data_length; f++)
	{
		cout<<f+1<<"\t\t"<<pref_min_log[f]<<"\t\t"<<suf_min_log[f]<<endl;
	}


}//end main

void seq_function(int m){
	/* The code for sequential algorithm */
	// Perform operations on B
}


void* par_function_OMP(void* a){
	/* The code for threaded computation */
	// Perform operations on B
}


void* par_function_PTH(void* a){
	/* The code for threaded computation */
	// Perform operations on B
}
