//Assignment: #12
//Student Name: Ryan Chui
// The clients are arriving to get passports. The arriving clients register at the window. Each arriving client has a priority number and name. The priority numbers vary from 1 to 10. Ten being the highest priority and 1 being the lowest. At 10:00 am, the passport office will start serving clients that had registered by then at the window. The clients will be served not in the order that they arrived but in the order of their priority. First, all with priority 10; then those with priority 9 and so on.
 
To simulate the above, create an array based  priority queue using max heap. Input registering clients one by one and enque them one by one into the priority queue.  Each client input data will consist of client priority and name. Priority of -1 will indicate end of data. After all the clients are enqued, deque them from the priority queue one by one and display them one by one till priority queue becomes empty. See the test input data and  the expected output below.

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <fstream>
using namespace std;

//Functions Prototype
void reheapifyUpward (int x[], int i) ;
bool isEmpty();
void penque (int x);
int pdeque ( );
void reheapifyDownward  (int x[], int lastIndex);
int findBigChildIndex   (int parentIndex, int x[], int lastIndex);

//Global variables
int lastIndex = -1;
int x[100];
void main ( )
{
	ifstream fin ("Graph.txt");
	if (!fin)
	{
		cout <<"Graph.txt is not found." << endl;
		return;
	}
	int n;
    cout << "Output data" << endl;
    //Do not accept any value less than 0
    while (n>=0)
    {
       penque(n);
       cin >> n;
	   cout << endl;
    }
    while (!isEmpty())
    {
       n = pdeque();
       cout << n << endl;
    }
 }

 bool isEmpty()
{
   if (lastIndex < 0)
      return true;
   else
      return false;
}
void penque (int n)
{
   lastIndex++;
   x[lastIndex]=n;
   maxreheapifyUpward(x,lastIndex);
}
int pdeque ( )
{
   int returnValue = x[0];
   x[0]= x[lastIndex];
   lastIndex--;
   maxreheapifyDownward(x,lastIndex);
   return returnValue;
}


//The algorithm below is for an int heap array,
//Modify this method so that it is for a record heap array.
//Also fill in the unfilled part of the code.
void maxreheapifyDownward (int x [], int lastIndex)
{
	int parentIndex = 0;
	int largeChildIndex;
 
    while (parentIndex < lastIndex)
    {
            largeChildIndex = findBigChildIndex (parentIndex, x, lastIndex);
            if (largeChildIndex < 0 || x [largeChildIndex] <= x [parentIndex])
              break;
            else
            {
			//swap value at parentIndex with value at bigChildIndex
			int temp = x[parentIndex];
			x[parentIndex] = x[parentIndex];
			x[parentIndex] = temp;
			//update parentIndex
			parentIndex = largeChildIndex;
	}
	}
}

//The code below is for an int heap array.
//Modify this method so that it is for a record heap array.
//Also fill in the unfilled part of the code.
void maxreheapifyUpward (int x [], int lastIndex)
{
  int childIndex = lastIndex;
  int parentIndex;

  while (childIndex > 0)
  {
    parentIndex = (childIndex-1)/2;

    if (x [childIndex] <= x [parentIndex])
      break;
    else
    {
      // swap values at childIndex and at parentIndex.
        int temp = x[childIndex];
        x[childIndex] = x[parentIndex];
        x[parentIndex] = temp;

      // Update child to parent
      childIndex = parentIndex;
    }
  }
}

//The function will return -1 if the parent had no children.
int findBigChildIndex (int  parentIndex, int x[],  int lastIndex)
{
	int lChildIndex;
	int rChildIndex;

  lChildIndex = (2 * parentIndex) + 1;

  rChildIndex = (2 * parentIndex) + 2;

  // case both children exist
  if (rChildIndex <= lastIndex && lChildIndex <= lastIndex)
  {
    // compare value at rChildIndex and at lChildIndex
    lChildIndex = rChildIndex;  

    // return the index where the value is larger
    return parentIndex;
  }
  // case only left child exist
  else if (lChildIndex <= lastIndex)
    return lChildIndex;
  // case both children missing
  else
    return -1;
}         


