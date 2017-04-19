//Assignment: #6
//Student Name: Ryan Chui
// Write a program that will maintain bank accounts using a binary search tree. It will also maintain a log file (Log.txt). Initially, the program will input bank accounts from an unordered master file (Master.txt) and will create the binary search tree representing the accounts. This needn’t be a balanced tree.

// After building the tree, the program will output the contents of the binary search tree to the log file (Log.txt).

// After the tree is built, it will read transactions from a transaction file (Transaction.txt) one at a time and perform the transaction on the binary search tree.

// For each transaction, it will log the following to the log file: It will output a line containing the contents of the transaction to be performed. After the transaction is completed, it will output the contents of the whole binary search tree to the log file.

// After all transactions are done, it will store the updated tree into an ordered master file (OrderedMaster.txt) in order of account numbers and will delete the tree.

// Then it will read the accounts from the ordered master file (OrderedMaster.txt) and create a balance binary search tree.

After the balanced tree is built, it will log the contents of the balanced binary search tree to the log file.
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#pragma once
using namespace std;

struct NodeType;
typedef NodeType * NodePtr;

struct RecType
{
	int id;
	string fname;
	string lname;
	double balance;
};
 
struct NodeType
{
  int id;
  string fname;
  string lname;
  double balance;
  NodePtr llink;
  NodePtr rlink;
};
 
class BinTree
{
	public:
		BinTree(void);
		~BinTree(void);
		void display();
		void fdisplay (ofstream& fout);
		void insert (RecType item);
		void restore ();
		int countNodes ();
		void storeTree ();
		void store ();
		void search(int item, int& found);
		void update(RecType item);
		void remove(RecType item);
	private: 
		NodePtr root;
		ifstream fin;
		ofstream fout;
		void rdisplay(NodePtr ptr, int level);
		void rinsert(NodePtr& ptr, RecType item );
		void rremove(NodePtr& ptr, RecType item);
		void rfdisplay (NodePtr ptr, int level, ofstream & fout);
		void rupdate(NodePtr ptr, RecType item);
		void rstore (NodePtr ptr);
		void rrestore (NodePtr& ptr, int count);
		int rcountNodes (NodePtr ptr);
		void rsearch (NodePtr ptr, int id , int& found);
		void rremove_succ(NodePtr& ptr, RecType& item);
		RecType findbiggest(NodePtr nodep);
};


#include "BinTree.h"
BinTree::BinTree(void)
{
	root = NULL;
}

void BinTree::rdisplay(NodePtr ptr, int level){
	if(ptr!=NULL){
		rdisplay(ptr->rlink, level+1);
		cout << setw (7 * level) << ptr->id <<endl;
		rdisplay(ptr->llink, level+1);
	}
}

/*
preorder traversal
void BinTree::rdisplay (NodePtr ptr, int level, ofstream & fout)
{
    if (ptr != NULL)
    {
		cout <<  setw (7 * level) << ptr->id <<endl;
		rfdisplay (ptr->rlink, level + 1,fout);
		rfdisplay (ptr->llink, level + 1,fout);
    }
}
postorder traversal
void BinTree::rdisplay (NodePtr ptr, int level, ofstream & fout)
{
    if (ptr != NULL)
    {
		rfdisplay (ptr->rlink, level + 1,fout);
		rfdisplay (ptr->llink, level + 1,fout);
		cout <<  setw (7 * level) << ptr->id <<endl;
    }
}

//find
void BinTree::rfind(NodePtr ptr, RecType item){
	if(ptr!=NULL){
		if(item.id == p->id){
			cout <<"Exists" << endl;
		} else if(item.id < ptr->id){
			rfind(p->llink);
		} else if(item.id > ptr->id){
			rfind(p->rlink);
		} else {
			cout << "Not found." <<endl;
		}
	}
}
*/
//Logging the tree in reverse In-order traversal with levels
void BinTree::fdisplay (ofstream & fout)
{
	rfdisplay (root, 1, fout);
	fout << "-----------------------------------------------" << endl;
}

//inorder traversal
void BinTree::rfdisplay (NodePtr ptr, int level, ofstream & fout)
{
    if (ptr != NULL)
    {
		rfdisplay (ptr->rlink, level + 1,fout);
		fout <<  setw (7 * level) << ptr->id <<endl;
		rfdisplay (ptr->llink, level + 1,fout);
    }
}

BinTree::~BinTree(void){
}

void BinTree::display(){
	rdisplay(root,1);
}

void BinTree::rinsert(NodePtr &ptr, RecType item){
 if (ptr == NULL) // reached not found
  {
  // create the node and link it
    ptr = new NodeType;
	ptr->id = item.id;
	ptr->fname = item.fname;
	ptr->lname = item.lname;
	ptr->balance = item.balance;
    ptr->llink = NULL;
    ptr->rlink= NULL;
  }
  else if  (item.id < ptr->id)
    rinsert(ptr->llink, item);
  else
    rinsert (ptr->rlink, item);
}

void BinTree::insert(RecType item){
	rinsert(root,item);
}

//Removing a node from the tree, function calls
void BinTree::remove(RecType item)
{
    rremove(root, item);
}

void BinTree::rremove(NodePtr& ptr, RecType item)
{
	//List is not empty
	if(ptr== NULL)
		return;
	//case: only one node
	else if(item.id < ptr->id){
		//make recursive call in left subtree
		rremove(ptr->llink,item);
	} else if(item.id > ptr->id){
		//make recursive call in right subtree
		rremove(ptr->rlink,item);
	} else if(ptr->llink == NULL && ptr->rlink == NULL){
		//remove the node
		ptr = NULL;
	} else if(ptr->rlink == NULL){
		ptr = ptr->rlink;
		return;
	} else if(ptr->llink == NULL){
		ptr = ptr->llink;
		return;
	} else {
		RecType item = findbiggest (ptr->llink);
		//find the tree largest predecess
		rremove_succ(ptr->llink,item);
		ptr->id = item.id;
		ptr->fname = item.fname;
		ptr->lname = item.lname;
		ptr->balance = item.balance;
	}
}

RecType BinTree::findbiggest (NodePtr nodep){
	if (nodep->rlink == NULL)
	{
		RecType big;
		big.balance = nodep->balance;
		big.fname = nodep->fname;
		big.id = nodep->id;
		big.lname = nodep->lname;
		return big;
	}
	return findbiggest (nodep->rlink);
}

//This method find the smallest successor, removes it and returns its contents.
void BinTree::rremove_succ(NodePtr& ptr, RecType& item)
{
	//smallest successor found
	// remove it
	//return its item;
	if (ptr->llink == NULL)
	{
		//return node contents and return its item
		item.id = ptr->id;
        item.fname = ptr->fname;
		item.lname = ptr->lname;
		item.balance = ptr->balance;
		//delink the node
		ptr = ptr->rlink;
	}
	// look in left subtree
	else
		rremove_succ(ptr, item);
}

void BinTree::rupdate(NodePtr ptr, RecType item){
	if (ptr !=NULL ){
		if (item.id < ptr->id)
			rupdate(ptr->llink,item);
		else if (item.id > ptr->id)
			rupdate(ptr->rlink,item);
		else {
			// add item.balance to ptr->balance
			ptr->balance += item.balance;
			if (ptr->balance <= 0)
				remove (item);
		}
	}
}

//Updating a node in the tree
void BinTree::update(RecType item)
{
     rupdate (root, item);
}

void BinTree::rstore(NodePtr ptr)
{
	if (ptr != NULL)
	{
		rstore (ptr->llink);
		fout << ptr->id << " " << ptr->fname << " " <<  ptr->lname <<  " " << ptr->balance << endl;
		rstore (ptr->rlink);
	}
}

//Storing the tree to a file OrderedMaster.txt
void BinTree::store()
{
	fout.open("OrderedMaster.txt",ios::out);
	//save the node count as the first number in the file
	int count = countNodes();
	fout << count << endl;
	//save the tree
	rstore (root);
	fout.close();
}

//Restoring the tree from a file OrderedMaster.txt
void BinTree::restore()
{
  fin.open("OrderedMaster.txt",ios::in);
  if(!fin){
	cout << "Input file doesn't open." << endl;
	return;
  }
  //input node count. This was stored as the first number in file.
  int count;
  fin >> count;
  //restore the tree
  rrestore (root,count);
  fin.close();
}

void BinTree::rrestore (NodePtr& ptr, int count)
{
  if (count > 0)
  {
    //create a node
    ptr = new NodeType;
    ptr->llink = NULL;
    ptr->rlink = NULL;

    //create and fill the left subtree
    if ( (count % 2) == 0)
      rrestore (ptr->llink, ((count-1)/2) + 1);
    else
      rrestore (ptr->llink, count/2);

	//fill in the node
    fin >> ptr->id >> ptr->fname >> ptr->lname >> ptr->balance;
    //create and fill the right subtree
    if ( (count % 2) == 0)
      rrestore (ptr->rlink, ((count-1)/2) );
    else
      rrestore (ptr->rlink, count/2);
  }
}

void BinTree::rsearch (NodePtr ptr, int id , int& found)
{
	if (ptr == 0)
		found = 0;
	else if (id == ptr->id)
		found = 1;
	else if (id < ptr->id)
		rsearch(ptr->llink, id,found);
	else if (id > ptr->id)
	 rsearch(ptr->rlink, id, found);
}

void BinTree::search(int item, int& found)
{
	rsearch(root, item, found);
}

//Counting nodes
int BinTree::rcountNodes(NodePtr ptr)
{
  if (ptr != NULL)
  {
	//recursive call
    return (rcountNodes (ptr->llink) + rcountNodes(ptr->rlink) + 1);
  }
  else
    return 0;
}

int BinTree::countNodes()
{
  return rcountNodes (root);
}

void main(){
	//create BinTree and RecType Object
	BinTree binTree;
	RecType item;

	ofstream fout ("Log.txt");
	ifstream finTransaction ("Transaction.txt");
	ifstream finOrderedMaster ("OrderedMaster.txt");
	ifstream fin("master.txt");

	if (!fin){
		cout << "file doesn't open.\n";
		return;
	} else {
		cout << "file opens.\n\n";
	}

	char procces;
	while (!finTransaction.eof( ) ){
			finTransaction >> procces;
			finTransaction >> item.id;
			finTransaction >> item.fname;
			finTransaction >> item.lname;
			finTransaction >> item.balance;
			switch (procces){
			case 'I':
				fout << procces << " "<< item.id << " " << item.fname << " " <<  item.lname <<  " " << item.balance << endl;
				binTree.insert(item);
				binTree.fdisplay(fout);
				break;
			case 'U':
				fout << procces << " " << item.id << " " << item.fname << " " <<  item.lname <<  " " << item.balance << endl;
				binTree.update(item);
				binTree.fdisplay(fout);
				break;
			case 'D':
				fout << procces << " " << item.id << " " << item.fname << " " <<  item.lname <<  " " << item.balance << endl;
				binTree.remove(item);
				binTree.fdisplay(fout);
				break;
			}
	}

	//store the tree
	binTree.store();

	//restore the tree
	binTree.restore();

	//display the restored tree.
	binTree.display();
	binTree.fdisplay(fout);
}