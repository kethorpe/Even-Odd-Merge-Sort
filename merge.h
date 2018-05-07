#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;
class merge
{
        public:
                merge();    // Default Constructor
                merge(int* locArr, int numProcessors, int numElements, int rank);

                int getMinIndex(int *arr, int length);
                int getMaxIndex(int *arr, int length);
};
