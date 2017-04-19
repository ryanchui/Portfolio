// Assignment: #4
// Name: Ryan Chui
/**
Create a software simulator that will simulate a world consisting of workers serving customers. There will be N workers. N will be a simulation parameter provided as an input to the simulator. There will be several customers.  They will arrive one by one each at a different time. 

All workers will be available for service from the beginning at time zero in simulation. The first customer will arrive at minute zero or any minute there after. Others will arrive after that one at a time. Each customer will arrive at a different minute in time. For each customer, its arrival time and the time required for service will be furnished as an input to the simulator. For example, the arrival time provided for customer 1 may be minute 4 and time required for service 10 minutes. For customer 2, the arrival time provided may be minute 8 and the required service time 6 minutes etc.

For simulating its world, the simulator will provide three queues namely Time queue, Idle queue, and Wait queue. These queues are described below.
*/
#pragma once
struct NodeType;
typedef NodeType * NodePtr;
struct NodeItem
{
	int type;
	int id;
	int time;
	int data;
};

struct NodeType
{
	int type;
	int id;
	int time;
	int data;
	NodePtr next;
};

class SimQue
{
private:
	NodePtr head;
	NodePtr tail;
public:
	SimQue(void);
	~SimQue(void);
	void insert(NodeItem item);
	void enque(NodeItem item);
	NodeItem deque();
	bool isEmpty();
};


#include "SimQue.h"
#include <iostream>
#include <fstream>
using namespace std;

SimQue::SimQue(void)
{
	head= NULL;
	tail= NULL;
}


SimQue::~SimQue(void)
{
	while (head)
	{
		NodeType * tmp=head->next;
		delete head;
		head=tmp;
	}
}


void SimQue::insert(NodeItem item)
{
	NodePtr cur=NULL;
	NodePtr prev=NULL;
	NodePtr newPtr= new NodeType;
	newPtr->type=item.type;
	newPtr->id=item.id;
	newPtr->time=item.time;
	newPtr->data=item.data;
	newPtr->next=NULL;
	if (head == NULL || item.time <head->time)
	{
		newPtr->next = head;
		head =newPtr;
	}
	else
	{
		for (prev=head,cur=head->next;cur!=NULL;prev=prev->next,cur=cur->next)
		{
			if (item.time <cur->time)
				break;
		}
		newPtr->next=prev->next;
		prev->next =newPtr;
	}

}

void SimQue::enque(NodeItem item)
{
	NodePtr newPtr = new NodeType;
	newPtr->type=item.type;
	newPtr->id=item.id;
	newPtr->time=item.time;
	newPtr->data=item.data;
	newPtr->next=NULL;
	if (head==NULL && tail==NULL)
	{
		head = newPtr;
		tail=head;
	}
	else 
	{
		tail->next=newPtr;
		tail=tail->next;
	}


}

NodeItem SimQue::deque()
{
	NodePtr delPtr;
	NodeItem delItem;
	if (head != NULL)
	{
		delPtr=head;
		head=head->next;
		if (head==NULL)
			tail=NULL;
	}
	delItem.type=delPtr->type;
	delItem.id=delPtr->id;
	delItem.time=delPtr->time;
	delItem.data=delPtr->data;
	delete delPtr;
	return delItem;
}

bool SimQue::isEmpty()
{
	if (head==NULL)
		return true;
	else
		return false;
}

void main()
{
	//variable declaration
	int numberOfWorkers,numberOfCustomers,index,currentTime;
	SimQue timeQ,idleQ,waitQ;
	NodeItem item,worker,customer;
	ifstream fin ("input.txt");
	if (!fin.good()) throw "I/O error";

	//initialization
	fin >> numberOfWorkers;
	int * idleTime = new int [numberOfWorkers];

	for (index=1;index<=numberOfWorkers;index++)
	{
		worker.type=1;
		worker.id=index;
		worker.time=0;
		worker.data=0;
		idleTime[index-1]=0;
		idleQ.enque(worker);
	}
	
	numberOfCustomers=0;
	while (fin.good())
	{
		numberOfCustomers++;
		fin >> customer.time >> customer.data;
		customer.type=0;
		customer.id=numberOfCustomers;
		timeQ.insert(customer);
	}
	int * waitTime = new int [numberOfCustomers];

	while (!timeQ.isEmpty())
	{
		item=timeQ.deque();
		currentTime=item.time;

		if (item.type==0 && !(idleQ.isEmpty()))
		{
			worker=idleQ.deque();
			waitTime[item.id-1]=0;
			idleTime[worker.id-1]+=currentTime-worker.time;
			worker.time=currentTime+item.data;
			worker.data=item.id;
			timeQ.insert(worker);
			cout << currentTime << ": Worker #" << worker.id 
				<< " started with customer #" << item.id << "." << endl;
		}
		else if (item.type==0 && idleQ.isEmpty())
		{
			waitQ.enque(item);
			cout << currentTime << ": Customer #"<< item.id
				<< " put on wait queue." << endl;
		}
		else if (item.type==1 && !(waitQ.isEmpty()))
		{
			customer=waitQ.deque();
			cout << currentTime << ": Worker #" << item.id
				<< " finished with customer #" << item.data << "." << endl;
			item.time=currentTime+customer.data;
			item.data=customer.id;
			timeQ.insert(item);
			waitTime[customer.id-1]=currentTime-customer.time;
			cout << currentTime << ": Worker #" << item.id 
				<< " started with customer #" << customer.id << "." << endl;
		}
		else if (item.type==1 && (waitQ.isEmpty()))
		{
			cout << currentTime << ": Worker #" << item.id
				<< " finished with customer #" << item.data << "." << endl;
			idleQ.enque(item);
			cout << currentTime << ": Worker #" << item.id 
				<< " put on idle queue." << endl;
		}
	}
	cout << endl << endl << "Summary: " << endl << endl;
	for (index=0;index<numberOfCustomers;index++)
		cout << "Customer #" << index+1 << " wait time " << waitTime[index] << endl;

	for (index=0;index<numberOfWorkers;index++)
	{
		item=idleQ.deque();
		idleTime[item.id-1]+= currentTime-item.time;
	}

	cout << endl;
	for (index=0;index<numberOfWorkers;index++)
		cout << "Worker #" << index+1 << " total idle time " << idleTime[index] << endl;
	cin >> currentTime;
}