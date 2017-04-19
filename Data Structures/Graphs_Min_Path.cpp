// Name: Ryan Chui
// Data Structure
// Write a program that will display the minimum path between any two vertices in a given graph. The program will input the graph from a file provided by the user. It will prompt the user for entering the name of the start vertex and the end vertex. It will find the minimum path between the two vertices. It will display the minimum path showing all the vertices along the path. It will also display the total distance along the minimum path.

#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
struct NodeType;
typedef NodeType * NodePtr;
struct NodeType{
	int vertix;
	NodePtr link;
};
struct ItemNodeType;
typedef ItemNodeType * ItemNodePtr;
struct ItemType{
	int dist;
	int path[20];
	int pathLength;
};
struct ItemNodeType{
	int dist;
	int path[20];
	int pathLength;
	ItemNodePtr link;
};
class IntQue
{
private:
	NodePtr head;
	NodePtr tail;
public:
	IntQue(void);
	~IntQue(void);
	bool isEmpty();
	void enque (int vertix);
	int deque();
};
class IntStack{
private:
	NodePtr head;
public:
	IntStack(void);
	~IntStack(void);
	bool isEmpty();
	void push (int vertix);
	int pop();
};
class Graph{
	private:
	string vNames[20];
	int vEdges[20][20];
	bool vVisited[20];
	int gSize;
	void getAdjacent(int vertix,IntQue & iQue);
	void getAdjacent(int vertix,IntStack & iStack);
public:
	Graph(void);
	~Graph(void);
	void addVertix(string vName);
	void addEdge(int fromV,int toV,int weight);
	void breadthFirst(int startV);
	void depthFirst(int startV);
	void findMinPath(int fromV,int toV);
};

class ItemPque{
private:
	ItemNodePtr head;
public:
	ItemPque (void);
	bool isEmpty();
	void enque(ItemType item); //its insert
	ItemType deque();

};



#include "Graph.h"

Graph::Graph(void){
	gSize=0;
	// initialize visited array and edges to zero
	int i,j;
	for (i=0;i<20;i++)
		vVisited[i]=false;
	for (i=0;i<20;i++)
		for (j=0;j<20;j++)
			vEdges[i][j]=0;
	
}

Graph::~Graph(void){
}

//vNames are recieve in ascending order starting from 0
//it ensures that there will be no unspecified vertix names in the array
void Graph::addVertix(string vName){
	vNames[gSize]=vName;
	gSize++;
}

void Graph::addEdge (int fromV,int toV,int weight){
	vEdges[fromV][toV]=weight;
	vEdges[toV][fromV]=weight;
}

void Graph::breadthFirst(int startV){
	for (int i=0;i<20;i++)
		vVisited[i]=false;
	//create travesal que
	IntQue travQue;
	IntQue adjQue;
	//initialize travQue
	travQue.enque(startV);
	cout << "Breadth First Traversal: ";
	while(!travQue.isEmpty()){
		int v= travQue.deque();
		if (!vVisited[v]){
			vVisited[v]=true;
			cout << vNames[v] << " ";
			getAdjacent(v,adjQue);
			while(!adjQue.isEmpty()){
				int adjV=adjQue.deque();
				if (!vVisited[adjV])
					travQue.enque(adjV);
			}
		}
	}
}

void Graph::depthFirst(int startV){
	for (int i=0;i<20;i++)
		vVisited[i]=false;
	//create travesal que
	IntStack travStack;
	IntStack adjStack;
	travStack.push(startV);
	cout << "Depth First Traversal: ";
	while (!travStack.isEmpty())
	{
		int v= travStack.pop();
		if (!vVisited[v]){
			vVisited[v]=true;
			cout << vNames[v] << " ";
			//Get a list of all its adjacent vertices
			getAdjacent(v,adjStack);
			while(!adjStack.isEmpty()){
				int adjV=adjStack.pop();
				if (!vVisited[adjV])
					travStack.push(adjV);
			}
		}
	}
}

     


void Graph::getAdjacent(int vertix,IntQue & adjQue){
	for (int col=0;col<gSize;col++)
		if (vEdges[vertix][col]>0)
			adjQue.enque(col);
}

void Graph::getAdjacent(int vertix,IntStack & adjStack){
	for (int col=0;col<gSize;col++)
		if (vEdges[vertix][col]>0)
			adjStack.push(col);
}

void Graph::findMinPath(int fromV,int toV){

	//create the main priority que and adjacent regular que
	ItemPque mainQ;
	IntQue adjQue;
	for (int i=0;i<20;i++)
		vVisited[i]=false;
	ItemType item;
	item.dist=0;
	item.pathLength=1;
	item.path[0]=fromV;
	mainQ.enque(item);
	
	while (!mainQ.isEmpty()){
		item=mainQ.deque();
		int pVertix = item.path[item.pathLength-1];
		int pPathLength=item.pathLength;
		int pDist=item.dist;
		if (pVertix==toV)
		{
			cout << "Total Distance: " << pDist << endl;
			for (int i=0;i<pPathLength;i++)
				cout << vNames[item.path[i]] << " ";
			cout << endl << endl;
			break;
		}
		if (!vVisited[pVertix]){
			//mark it visited
			vVisited[pVertix]=true;
			//enque neighbors
			getAdjacent(pVertix,adjQue);
			while (!adjQue.isEmpty()){
				int adjVertix = adjQue.deque();
				//update item values
				item.dist= pDist + vEdges[pVertix][adjVertix];
				item.path[pPathLength] = adjVertix;
				item.pathLength=pPathLength + 1;
				mainQ.enque(item);
			}

		}
	}


}
ItemPque::ItemPque (void){
	head=NULL;
}

bool ItemPque::isEmpty(){
	return ((head==NULL)? true: false);
}

void ItemPque::enque(ItemType item){
	ItemNodePtr newPtr= new ItemNodeType;
	newPtr->dist=item.dist;
	newPtr->pathLength=item.pathLength;
	for (int i=0;i<20;i++)
		newPtr->path[i]=item.path[i];
	newPtr->link=NULL;
	//insert the node
	//head insertion
	if (head==NULL || item.dist < head->dist){
		newPtr->link=head;
		head=newPtr;
	}
	else
	{
		//mid insetion
		ItemNodePtr prev,cur;
		for (prev=head,cur=head->link;cur!=NULL;cur=cur->link,prev=prev->link)
			if (item.dist <= cur->dist)
				break;
		newPtr->link=prev->link;
		prev->link=newPtr;
	}
}

ItemType ItemPque::deque(){
	ItemType item;
	if (isEmpty()){
		item.dist=-1;
		item.pathLength=-1;
		return item;
	}
	item.dist= head->dist;
	item.pathLength=head->pathLength;
	for (int i=0;i<20;i++)
		item.path[i]=head->path[i];
	head=head->link;
	return item;
}

IntQue::IntQue(void){
	head=NULL;
	tail=NULL;
}


IntQue::~IntQue(void)
{
}

bool IntQue::isEmpty(){
	if (head==NULL)
		return true;
	else
		return false;
}

void IntQue::enque(int vertix){
	//create a new node
	NodePtr newPtr = new NodeType;
	newPtr->vertix=vertix;
	newPtr->link=NULL;
	//enque the node
	if (head==NULL){
		head=newPtr;
		tail=newPtr;
	}
	else
	{
		tail->link=newPtr;
		tail=tail->link;
	}
}

int IntQue::deque(){
	if (isEmpty())
		return -1;
	int vertix;
	vertix=head->vertix;
	head=head->link;
	if (head==NULL)
		tail=NULL;
	return vertix;
}
IntStack::IntStack(void){
	head=NULL;
}

IntStack::~IntStack(void){
}

bool IntStack::isEmpty(){
	if (head==NULL)
		return true;
	else
		return false;
}

void IntStack::push (int vertix){
	NodePtr newPtr = new NodeType;
	newPtr->vertix=vertix;
	newPtr->link=head;
	head=newPtr;
}

int IntStack::pop(){
	if (isEmpty())
		return -1;
	int vertix=head->vertix;
	head=head->link;
	return vertix;
}

void main(){
	Graph distGraph;
	ifstream fin ("Graph.txt");
	if (!fin)
	{
		cout <<"Graph.txt is not found/opened" << endl;
		return;
	}
	string vName;
	//input vertix names
	getline(fin,vName);

	while(vName!="//")
	{
		distGraph.addVertix(vName);
		getline(fin,vName);
	}
	int fromV,toV,weight;
	while (true){
		fin >> fromV;
		if (fromV==-1)
			break;
		fin >> toV;
		fin >> weight;
		distGraph.addEdge(fromV,toV,weight);
	}
	int startV;
	while (true){
		cout << "Enter start vertix for min path" << endl;
		cin >> startV;
		if (startV < 0)
			break;
		cout << "Enter dest vertix for min path" << endl;
		cin >> toV;
		distGraph.findMinPath(startV,toV);
	}
	cout <<"\nBye." << endl;

	char ch;
	cin >> ch;
}



