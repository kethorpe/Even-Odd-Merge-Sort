#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "merge.h"
#include "mpi.h"

using namespace std;

int MAX = 640;

void Write(int *sortedArray){
   ofstream myfile;

   myfile.open("out.txt");
   if(myfile.fail()){
        cout << "Unable to open file to write";
        return;
   }

   for(int i = 0; i < MAX; i++){
        myfile << sortedArray[i] << " ";
   }

   myfile.close();
}


void RandomWrite(){
  /* initialize random seed: */
  srand(time(NULL));

   int random;
   ofstream myfile;

   myfile.open("in.txt");
   if(myfile.fail()){
        cout << "Unable to open file to write";
        return;
   }

   myfile << MAX << " "; //Write MAX as first number in file
   for(int i = 0; i < MAX; i++){
        random = rand() % 100;
        myfile << random << " ";
   }

   myfile.close();
}

int ReadInFile(int *arr){
   //cout << "inside ReadInFile function" << endl;
   int num;
   ifstream din;

    din.open("in.txt");
    if (din.fail()) {
        cout << "Unable to open file to read";
        return 0;
    }

  din >> num; // reads in first number MAX

   int i = 0;
   for(i = 0; i < MAX; i++){
      din >> num;
      arr[i] = num;
   }

  din.close();

  return i;
}

main(int argc, char* argv[])
{
  int my_rank;       /* rank of process      */
  int p;             /* number of processes  */
  int source;        /* rank of sender       */
  int dest;          /* rank of receiver     */
  int tag = 0;       /* tag for messages     */
  MPI_Status status;        /* return status for receive */
  int elementsPerProc; //length of local array
  int *array, *sortedArray, *localArray;   //local array for each processor
  int numElements;   //total Num of elements in final array

  /* Start up MPI */
  MPI_Init(&argc, &argv);

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  elementsPerProc = MAX/p;

  if(my_rank == 0){
    array = (int*)malloc(sizeof(int) * MAX);
    sortedArray = (int*)malloc(sizeof(int) * MAX);

    RandomWrite();

    numElements = ReadInFile(array);

  //elementsPerProc = numElements / p;
    }

    //Scatter the data w/ Process 0
    localArray = (int*)malloc(sizeof(int) * elementsPerProc);
     MPI_Scatter(array, elementsPerProc, MPI_INT, localArray, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);

    //even odd sort between adjacent processors
    merge m(localArray, p, elementsPerProc, my_rank);

    //Gather the data w/ Process 0
    MPI_Gather(localArray, elementsPerProc, MPI_INT, sortedArray, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);

   if(my_rank == 0){
     for(int i = 0; i < MAX; i++){
        cout << sortedArray[i] << " ";
     }
     Write(sortedArray);
   }

  /* Shut down MPI */

   MPI_Finalize();

} /* end main */
