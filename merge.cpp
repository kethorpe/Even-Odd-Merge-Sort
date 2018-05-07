#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merge.h"
#include "quickSort.h"
#include "mpi.h"

using namespace std;

merge::merge(int* locArr, int numProcessors, int numElements, int rank) {
    int adjArray[numElements];

    for(int i = 0; i < numProcessors; i++)
    {
       //run quickSort on all the elements
       quickSort qs(locArr, 0, numElements - 1);

       int adjProc;
       if(i%2 == 0) //We are on an even phase
       {
          if(rank%2 == 0) //We are an even process
          {
             adjProc = rank + 1;
          }
          else {
             adjProc = rank - 1;
          }
       }
       else  //We are on an odd phase
       {
          if(rank%2 == 0) //We are an even process
          {
             adjProc = rank - 1;
          }
          else {
             adjProc = rank + 1;
          }
       }

       if(adjProc >= numProcessors || adjProc < 0){
         continue;
       }

       if(rank % 2 == 0)
       {
          MPI_Send(locArr, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD);
          MPI_Recv(adjArray, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       }
       else
       {
          MPI_Recv(adjArray, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Send(locArr, numElements, MPI_INT, adjProc, 0, MPI_COMM_WORLD);
       }
       if(rank < adjProc)
       {

           while(true)
           {
               int minIndex = getMinIndex(adjArray, numElements);
               int maxIndex = getMaxIndex(locArr, numElements);
               if(adjArray[minIndex] < locArr[maxIndex]){
                  int swap = adjArray[minIndex];
                  adjArray[minIndex] = locArr[maxIndex];
                  locArr[maxIndex] = swap;
               }
               else
               {
                  break;
               }
            }
         }
         else
         {
            while(true)
            {
               int minIndex = getMinIndex(locArr, numElements);
               int maxIndex = getMaxIndex(adjArray, numElements);
               if(adjArray[maxIndex] > locArr[minIndex]){
                  int swap = adjArray[maxIndex];
                  adjArray[maxIndex] = locArr[minIndex];
                  locArr[minIndex] = swap;
               }
               else
               {
                  break;
               }
            }
        }
    }
}

/////////////////////////////////
/////*Gets minimum value in array*/
//////////////////////////////////
int merge::getMinIndex(int *arr, int length){

    int min = arr[0];
    int minIndex = 0;

    for(int i = 1; i < length; i++){
       if(arr[i] < min){
          min = arr[i];
          minIndex = i;
        }
    }
    return minIndex;
}

/////////////////////////////////
/*Gets maximum value in array*/
////////////////////////////////
int merge::getMaxIndex(int *arr, int length){

   int max = arr[0];
   int maxIndex = 0;

   for(int i = 1; i < length; i++){
      if(arr[i] > max){
        max = arr[i];
        maxIndex = i;
      }
   }
   return maxIndex;
}
