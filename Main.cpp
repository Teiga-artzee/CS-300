/*
Name        : Teigeler_ABCU
Author      : Alexandrea H. Teigeler
Last Edit   : December 10th, 2021
Class       : CS 300 SNHU
--------------------------------------------------------------------------
Description : Program will read through a text file
			  that has course information from ABCU.
			  ------------------------------------------------------------
			  This data will be saved to a vector and
			  use to create Linked Lists, Hashtables, and BST for ABCU. 
              ------------------------------------------------------------
			  Will deterimne the efficiency of each search.
*/



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ReadCourseInfo.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <vector>



using namespace std;
using std::cout; 
using std::cin;
using std::endl; 
using std::string;
using std::vector; 
using std::istringstream;
using std::stringstream;

// Global definitions

const unsigned int DEFAULT_HASH_SIZE = 180;

// define a structure to hold course info

struct Course // structure 
{
	string courseNum; //The Unique identifer
	string courseName;
	string preRequisitesCourse;
};

struct Node // internal structure for BST
{
	Course course;
	Node* left;
	Node* right;
	Node() // Default constructor
	{
		left = nullptr;
		right = nullptr;
	}
	Node(Course aCourse) : Node() // Initialize course
	{
		this->course = aCourse;
	}
};

class runProgram
{
public:
	runProgram() {}; // default constructor
	int ProgramControl();
	void MenuPrint();
};

class LinkedList
{

private:

	struct Node
	{
		Course course;
		Node* next;

		Node() //default constructor
		{
			next = nullptr;
		}
		Node(Course aCourse) // initilize node
		{
			course;
			next = nullptr;
		}
	};

	Node* head;
	Node* tail;
	int size = 0;

public:
	LinkedList();
	virtual ~LinkedList();
	void Append(Course course);
	void Prepend(Course course);
	void Remove(string courseNum);
	void PrintSampleSchedule();
	Course Search(string courseNum);
	int Size();
	//void readFile(LinkedList* list);
	void displayCourseInfoFULL(Course course);
	void displayPrerequisite(Course course);

};

class HashTable
{
private:
	//define struct
	struct Node
	{
		Course course;
		unsigned int key;
		Node* next;

		Node() // default constructor
		{
			key = UINT_MAX;
			next = nullptr;
		}
		Node(Course aCourse) : Node() // initialize with just a course
		{
			course = aCourse;
		}
		Node(Course aCourse, unsigned int aKey) // initialize with course and unique key
		{
			key = aKey;
		}
	};

	//default constructor for nodes
	vector<Node> nodes;

	unsigned int tableSize = 180; 
	unsigned int hash(int key);
	
public:
	HashTable();
	HashTable(unsigned size);
	virtual ~HashTable();
	void Insert(Course course);
	void PrintSampleSchedule();
	void Remove(string courseNum);
	Course Search(string courseNum);
	void readFile(HashTable* hashTable);
	void displayCourseInfoFULL(Course course);
	void displayPrerequisite(Course course);
};

class BinarySearchTree
{

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	void preOrder(Node* node);
	void postOrder(Node* node);
	Node* removeNode(Node* node, string courseNum);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void PreOrder();
	void PostOrder();
	void Insert(Course course);
	void Remove(string courseNum);
	Course Search(string courseNum);
	//void readFile(BinarySearchTree* bst);
	void displayCourseInfoFULL(Course course);
	void displayPrerequisite(Course course);
};

LinkedList::LinkedList()
{
	/*
		Default constructor
	*/
	head = nullptr;
	tail = nullptr;
}

LinkedList::~LinkedList()
{
	/*
	Deconstructor: garbage cleaner
	*/
	Node* current = head;
	Node* temp;

	while (current != nullptr)
	{
		temp = current;
		current = current->next;
		delete temp; // remove orphan node
	}
}

void LinkedList::Append(Course course)
{
	/*
		Add a new node at end list/tail
		IF the list is empty, this addition becomes both the head and tail
		Otherwise, becomes tail
		Add size 
	*/

	Node* temp = new Node;
	temp->course = course;
	temp->next = NULL;

	if (head == NULL)
	{
		head = temp;
		tail = temp;
	}
	else
	{
		tail->next = temp;
		tail = tail->next;
	}
	size += 1;
}

void LinkedList::Prepend(Course course)
{
	/*
		Creates a new node ahead of another
	*/

	Node* temp = new Node;
	temp->course = course;

	if (head == tail)
	{
		tail = temp;
	}
	else
	{
		temp->next = head->next;
		head->next = temp;
		size += 1;
	}
}

void LinkedList::Remove(string courseNum)
{
	/*
		Remove logic
		special case if node matches head: make head point to next node in list
		decrease size of list
		return
	*/

	Node* temp = head;
	Node* temp2 = new Node;
	temp2->course.courseName = courseNum;

	while (temp != nullptr)
	{
		if (temp->next == temp2)
		{
			delete temp;
			size -= 1;
			return;
		}
		temp = temp->next;
	}

}

Course LinkedList::Search(string courseNum)
{
	/*
		Will Continue to search until reaches nullptr
		if current node matches, will return node
		else current node now points to next node
	*/

	Node* temp = head;
	Node* temp2 = new Node;
	temp2->course.courseName = courseNum;

	//starts at head of list

	while (temp != nullptr)
	{
		if (temp->next == temp2)
		{
			return temp->course;
		}
		temp = temp->next;
	}
	return temp->course;
}

int LinkedList::Size()
{
	return size;
}

void LinkedList::PrintSampleSchedule()
{
	Node* curNode = head;

	while (curNode != nullptr)
	{
		cout << curNode << endl;
		curNode = curNode->next;
	}
}

//Default constructor for Hash
HashTable::HashTable()
{
	nodes.resize(tableSize);
}

// constructor for specifying size of table
HashTable::HashTable(unsigned int size)
{
	this->tableSize = size;
	nodes.resize(tableSize);
}

// destructor: Garbage collector
HashTable::~HashTable()
{
	nodes.erase(nodes.begin());
}

unsigned int HashTable::hash(int key)
{
	/*
		Calculate hash for unique key
	*/

	key = key % tableSize;
	return key;
}

// insert into table
void HashTable::Insert(Course course)
{
	/*
		Will insert a new node into the hash table
		Will continue to search for a free space
	*/
	
	unsigned key = hash(atoi(course.courseNum.c_str()));
	Node* oldNode = &(nodes.at(key));

	if (oldNode == nullptr) // No Node Found, construct new node, insert into vector
	{
		Node* newNode = new Node(course, key);
		nodes.insert(nodes.begin() + key, (*newNode));
	}
	else // Node was found
	{
		if (oldNode->key == UINT_MAX)
		{
			oldNode->key = key;
			oldNode->course = course;
			oldNode->next = nullptr;
		}
		else // A Node was found, has value, so move to next node for placement check
		{
			while (oldNode->next != nullptr)
			{
				oldNode = oldNode->next;
			}
			oldNode->next = new Node(course, key);
		}
	}
}

void HashTable::PrintSampleSchedule()
{
	/*
		Print out the courses
	*/
	//program code provided by Professoor Jeff Sanford
	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) // Automatically iterates at the begining of the hash table -> goes until iteration reaches the end
	{
		if (iter->key != UINT_MAX) // while the key has been allocated and being used
		{
			cout << "Key " << iter->key << ": " << iter->course.courseNum << " | " << iter->course.courseName << " | " << iter->course.preRequisitesCourse << endl;
			Node* node = iter->next; // gets next data in hashtable
			while (node != nullptr)  // while iterate until this reaches the last key in hash tablede
			{
				cout << "Key " << node->key << ": " << node->course.courseNum << " | " << node->course.courseName << " | " << node->course.preRequisitesCourse << endl;
				node = node->next;
			}
		}
	}
}

void HashTable::Remove(string courseNum)
{
	/*
		Remove a node from hash table
	*/
	unsigned key = hash(atoi(courseNum.c_str()));
	nodes.erase(nodes.begin() + key);
}

Course HashTable::Search(string courseNum)
{
	/*
		Search for a specific course
	*/
	Course course;
	unsigned key = hash(atoi(courseNum.c_str()));
	
	Node* node = &(nodes.at(key));

	if ((node == nullptr) || (node->key == UINT_MAX)) // no entry found
	{
		return course;
	}
	if ((node != nullptr) && (node->key != UINT_MAX) && (node->course.courseNum.compare(courseNum) == 0)) // does node match key
	{
		return node->course;
	}
	while (node != nullptr) // iterate through hash
	{
		if ((node->key != UINT_MAX) && (node->course.courseNum.compare(courseNum) == 0))
		{
			return node->course;
		}
		node = node->next;
	}

	return course; // returns empty course
}

BinarySearchTree::BinarySearchTree()
{
	/*
		Default Constructor
	*/
	root = nullptr;
}

BinarySearchTree::~BinarySearchTree()
{
	/*
		Destructor
	*/
}

//Traverse Tree in 3 different ways: In order, Post Order, PreOrder
void BinarySearchTree::InOrder()
{
	this->inOrder(root);
}

void BinarySearchTree::PostOrder()
{
	this->postOrder(root);
}

void BinarySearchTree::PreOrder()
{
	this->preOrder(root);
}

void BinarySearchTree::Insert(Course course)
{
	if (root == nullptr) // if the tree is empty
	{
		root = new Node(course); // create root
	}
	else
	{
		this->addNode(root, course); // add node to tree
	}
}

void BinarySearchTree::Remove(string courseNum)
{
	this->removeNode(root, courseNum); // find node to remove
}

Course BinarySearchTree::Search(string courseNum)
{
	/*
		Search through tree, going either left or right till found
		if not found, send empty course
	*/
	Course course;
	Node* current = root; // We will start search at root

	while (current != nullptr) // Search down the tree
	{
		if (current->course.courseNum.compare(courseNum) == 0) // is it a match?
		{
			return current->course;
		}
		if (courseNum.compare(current->course.courseNum) < 0) // Go to left of current node
		{
			current = current->left;
		}
		else // Go to right of current node
		{
			current = current->right;
		}
	}
	
	return course; // not found
}

void BinarySearchTree::addNode(Node* node, Course course)
{
	/*
		Add a node to the tree
		Will compare each node to find where to place
		Will traverse down tree till placed
	*/

	if (node->course.courseNum.compare(course.courseNum) > 0) // if new node larger than this node, make node to left
	{
		if (node->left == nullptr) // Check for child, if none, add child to LEFT
		{
			node->left = new Node(course);
		}
		else // traverse down tree till a spot is found: GO left
		{
			this->addNode(node->left, course);
		}
	}
	else // if node smaller, go right
	{
		if (node->right == nullptr) //Check for child, if none, make a child to RIGHT
		{
			node->right = new Node(course);
		}
		else // traverse down tree till a spot is found: GO right
		{
			this->addNode(node->right, course);
		}
	}
}

void BinarySearchTree::inOrder(Node* node)
{
	if (node != nullptr)
	{
		inOrder(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << ", " << node->course.preRequisitesCourse << endl;
		inOrder(node->right);
	}
	if (node == nullptr)
	{
		return;
	}
}

Node* BinarySearchTree::removeNode(Node* node, string courseNum)
{
	/*
		Will search through tree to remove node
		This is done recursively by calling function again till node found
		If node had children: will need to make the children to the next available node above
	*/
	if (node == nullptr) // the tree is empty: This avoids a crash-- hopefully
	{
		return node;
	}
	if (courseNum.compare(node->course.courseNum) < 0) //check to go left recursively down tree
	{
		node->left = removeNode(node->left, courseNum);
	}
	else if (courseNum.compare(node->course.courseNum) > 0) // check to go right recursively down tree
	{
		node->right = removeNode(node->right, courseNum);
	}
	else // no children-- leaf node
	{
		if ((node->left == nullptr) && (node->right == nullptr))
		{
			delete node;
			node = nullptr;
		}
		else if ((node->left != nullptr) && (node->right == nullptr)) // 1 child to left
		{
			Node* temp = node;
			node = node->left;
			delete temp;
		}
		else if ((node->left == nullptr) && (node->right != nullptr)) // 1 child to right
		{
			Node* temp = node;
			node = node->right;
			delete temp;
		}
		else // two children
		{
			Node* temp = node->right;
			while (temp->left != nullptr)
			{
				temp = temp->left;
			}
			node->course = temp->course;
			node->right = removeNode(node->right, temp->course.courseNum);
		}
	}
	
	return node;
}

void BinarySearchTree::postOrder(Node* node)
{
	if (node != nullptr)
	{
		postOrder(node->left);
		postOrder(node->right);
		cout << node->course.courseNum << ", " << node->course.courseName << ", " << node->course.preRequisitesCourse << endl;
	}
}

void BinarySearchTree::preOrder(Node* node)
{
	if (node != nullptr)
	{
		cout << node->course.courseNum << ", " << node->course.courseName << ", " << node->course.preRequisitesCourse << endl;
		preOrder(node->left);
		preOrder(node->right);
	}
}

void runProgram::MenuPrint()
{
	cout << "                  *ABC University Computer Science Course Path Program*                     " << endl;
	cout << "                           Choose from the following actions:                               " << endl;
	cout << "********************************************************************************************" << endl;
	cout << "********************************************************************************************" << endl;
	cout << "1. Load Course Information from file."                                                        << endl;
	cout << "============================================================================================" << endl;
	cout << "2. Print a Sample Schedule Using a Linked List." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "3. Print a Sample Schedule Using a Hash Table." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "4. Print a Sample Schedule Using a Binary Search Tree." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "5. Search for a Specific Course Using a Linked List." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "6. Search for a Specific Course Using a Hash Table." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "7. Search for a Specific Course using a Binary Search Tree." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "8. Search for a Specific Course and any required prerequisites Using a Linked List." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "9. Search for a Specific Course and any required prerequisites Using a Hash Table." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "10. Search for a Specific Course and any required prerequisites using a Binary Search Tree." << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "11. End Program." << endl;
	cout << "============================================================================================" << endl;

	cout << endl;
	cout << endl;
}

void readFile(LinkedList *list) // Linked list read
	{
	fstream myFile; // object to open & read from file
	string tempLine; // used to help hold each line till they are seperated
	string courseName;  // name of course
	string courseNUM;  // course number associated with course
	string prerequistes = ""; // name of courses needed
	int prerequisiteNUM; // num of courses needed prior to taking current course

	//Following used to iterate through each line to then add info into struct
	string word;
	vector<string> courseInfo{};
	char myCharSpace = ',';
	int tempSize = 0;
	//

	myFile.open("ABCUCourses.txt"); // open the file

	if (!myFile.is_open())
	{
		cout << "The file was unable to open." << endl;  // check to make sure file opens correctly
	}
	else
	{
		while (!myFile.fail())
		{
			myFile >> tempLine;
			
			//std::replace(tempLine.begin(), tempLine.end(), ",", " "); // gets rid of comma
			stringstream sstream(tempLine);
			
			while (std::getline(sstream, word, myCharSpace))
			{
				tempSize += 1;
			}
			while (std::getline(sstream, word, myCharSpace))
			{
				for (int i = 0; i < tempSize; i++)
				{
					if (i == 0)
					{
						courseNUM = word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());

					}
					if (i == 1)
					{
						courseName = word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());

						if (tempSize == 2)
						{
							prerequistes = "No prerequistes to take.";
							prerequisiteNUM = 0;
						}
					}
					if (i > 1)
					{
						prerequistes += word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
						prerequisiteNUM += 1;
						if (tempSize > 3)
						{
							prerequistes += ", "; //if there are more than one prerequistes
						}
					}

				}
			}

			Course course; // create data struct

			course.courseNum = courseNUM;
			course.courseName = courseName;
			course.preRequisitesCourse = prerequistes;

			list->Append(course);
			prerequisiteNUM = 0;
			tempSize = 0;

		}
		myFile.close(); // close file
	}
}

void HashTable::readFile(HashTable* hashTable) // HashTable read
{
	fstream myFile; // object to open & read from file
	string tempLine; // used to help hold each line till they are seperated
	string courseName;  // name of course
	string courseNUM;  // course number associated with course
	string prerequistes = ""; // name of courses needed
	int prerequisiteNUM; // num of courses needed prior to taking current course

	//Following used to iterate through each line to then add info into struct
	string word;
	vector<string> courseInfo{};
	char myCharSpace = ',';
	int tempSize = 0;
	//

	myFile.open("ABCUCourses.txt"); // open the file

	if (!myFile.is_open())
	{
		cout << "The file was unable to open." << endl;  // check to make sure file opens correctly
	}
	else
	{
		while (!myFile.fail())
		{
			myFile >> tempLine;
			//std::replace(tempLine.begin(), tempLine.end(), ",", " "); // gets rid of comma
			stringstream sstream(tempLine);

			while (std::getline(sstream, word, myCharSpace))
			{
				tempSize += 1;
			}
			while (std::getline(sstream, word, myCharSpace))
			{
				for (int i = 0; i < tempSize; i++)
				{
					if (i == 0)
					{
						courseNUM = word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());

					}
					if (i == 1)
					{
						courseName = word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());

						if (tempSize == 2)
						{
							prerequistes = "No prerequistes to take.";
							prerequisiteNUM = 0;
						}
					}
					if (i > 1)
					{
						prerequistes += word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
						prerequisiteNUM += 1;
						if (tempSize > 3)
						{
							prerequistes += ", "; //if there are more than one prerequistes
						}
					}

				}
			}

			Course course; // create data struct

			course.courseNum = courseNUM;
			course.courseName = courseName;
			course.preRequisitesCourse = prerequistes;

			hashTable->Insert(course);
			prerequisiteNUM = 0;
			tempSize = 0;

		}

		myFile.close(); // close file
	}
}

void readFile(BinarySearchTree* bst) // BST read
{
	fstream myFile; // object to open & read from file
	string tempLine; // used to help hold each line till they are seperated
	string courseName;  // name of course
	string courseNUM;  // course number associated with course
	string prerequistes = ""; // name of courses needed
	int prerequisiteNUM; // num of courses needed prior to taking current course

	//Following used to iterate through each line to then add info into struct
	string word;
	vector<string> courseInfo{};
	char myCharSpace = ',';
	int tempSize = 0;
	//
	
	myFile.open("ABCUCourses.txt"); // open the file

	if (!myFile.is_open())
	{
		cout << "The file was unable to open." << endl;  // check to make sure file opens correctly
	}
	else
	{
		while (!myFile.fail())
		{
			
			myFile >> tempLine;
			
			//std::replace(tempLine.begin(), tempLine.end(), ",", " "); // gets rid of comma
			stringstream sstream(tempLine);
			
			
			while (std::getline(sstream, word, myCharSpace))
			{
				tempSize += 1;
			}
			while (std::getline(sstream, word, myCharSpace))
			{
				for (int i = 0; i < tempSize; i++)
				{
					if (i == 0)
					{
						courseNUM = word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
						
					}
					if (i == 1)
					{
						courseName = word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
						
						if (tempSize == 2)
						{
							prerequistes = "No prerequistes to take.";
							prerequisiteNUM = 0;
						}
					}
					if (i > 1)
					{
						prerequistes += word;
						word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
						prerequisiteNUM += 1;
						
						if (tempSize > 3)
						{
							prerequistes += ", "; //if there are more than one prerequistes
						}
					}
					
				}
			}

			Course course; // create data struct

			course.courseNum = courseNUM;
			course.courseName = courseName;
			course.preRequisitesCourse = prerequistes;

			bst->Insert(course);
			prerequisiteNUM = 0;
			tempSize = 0;

		}

		myFile.close(); // close file
	}
}

void LinkedList::displayCourseInfoFULL(Course course)
{
	cout << course.courseNum << ", " << course.courseName << ", " << course.preRequisitesCourse << endl;
	return;
}

void LinkedList::displayPrerequisite(Course course)
{
	cout << course.preRequisitesCourse << endl;
	return;
}

void HashTable::displayCourseInfoFULL(Course course)
{
	cout << course.courseNum << ", " << course.courseName << ", " << course.preRequisitesCourse << endl;
	return;
}

void HashTable::displayPrerequisite(Course course)
{
	cout << course.preRequisitesCourse << endl;
	return;
}

void BinarySearchTree::displayCourseInfoFULL(Course course)
{
	cout << course.courseNum << ", " << course.courseName << ", " << course.preRequisitesCourse << endl;
	return;
}

void BinarySearchTree::displayPrerequisite(Course course)
{
	cout << course.preRequisitesCourse << endl;
	return;
}


int runProgram::ProgramControl()
{
	/*
		Used to keep Main clean
		Only allows user to enter integers
		Prevents program from shutting down if an incorrect item is entered
	*/
	bool correct_Menu_Select = false;
	int user_Choice = 0;

	// do-try-catch-while blocks to catch wrong input from user
	do
	{
		try
		{
			std::cin >> user_Choice;

			// Catch error if user_Choice gets input that is NOT an int
			while (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(256, '\n');
				cout << "Unknown Entry. Please Try Again." << endl;
				std::cin >> user_Choice;
			}
			if ((user_Choice >= 1) && (user_Choice <= 11))
			{
				// pass the try test
				//exit do-while
				correct_Menu_Select = true;
			}
			else
			{
				throw (user_Choice);
			}
		}
		catch (...)
		{
			correct_Menu_Select = false;
			cout << "Please Enter a correct Menu Option." << endl;
		}
	} while (correct_Menu_Select == false);
	return user_Choice;
}

int main()
{
	//runProgram class
	runProgram program;

	int user_Choice = 0;
	string courseSearchPick = "";
	clock_t ticks;

	//List requirements
	LinkedList courseList;
	Course course; // struct object

	//Hash requirements
	HashTable* courseTable;
	courseTable = new HashTable(); // error occurs when only initialized in cases

	//BST requirements
	BinarySearchTree* bst;
	bst = new BinarySearchTree();

	do 
	{
		program.MenuPrint();
		user_Choice = program.ProgramControl();

		switch (user_Choice)
		{
		case 1:
			// load file
			ticks = clock();
			
			// For linked list
			readFile(&courseList); 

			cout << "Time for Linked List: " << endl;
			cout << courseList.Size() << " Courses read using Linked list" << endl;
			ticks = clock() - ticks; 
			cout << "time: " << ticks << " milliseconds" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			
			// For Hash Table--Currently breaking program :(
			//courseTable->readFile(courseTable);

			//cout << "Time for Hash Table: " << endl;
			//ticks = clock() - ticks; 
			//cout << "time: " << ticks << " clock ticks" << endl;
			//cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

			// For BST
			readFile(bst);

			cout << "Time for Binary Search Tree: " << endl;
			ticks = clock() - ticks; 
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			
			break;
		case 2:
			// display sample schedule Linked List
			courseList.PrintSampleSchedule();
			break;
		case 3:
			// display sample schedule Hash Table
			courseTable->PrintSampleSchedule();
			break;
		case 4:
			// display sample schedule BST
			bst->InOrder();
			break;
		case 5:
			// need user input
			// Get Course info Linked List
			cout << "Please Enter your Course to Search: " << endl;
			std::cin >> courseSearchPick;
			course = courseList.Search(courseSearchPick);

			if (!course.courseNum.empty())
			{
				courseList.displayCourseInfoFULL(course);
			}
			else
			{
				cout << "Course Not Found" << endl;
			}
			break;
		case 6:
			// need user input
			// Get Course Info Hash
			cout << "Please Enter your Course to Search: " << endl;
			std::cin >> courseSearchPick;

			if (!course.courseNum.empty())
			{
				courseTable->displayCourseInfoFULL(course);
			}
			else
			{
				cout << "Course Not Found" << endl;
			}
			break;
		case 7:
			// need user input
			// Get Course info BST
			cout << "Please Enter your Course to Search: " << endl;
			std::cin >> courseSearchPick;
			course = bst->Search(courseSearchPick);
			if (!course.courseNum.empty())
			{
				bst->displayCourseInfoFULL(course);
			}
			else
			{
				cout << "Course Not Found" << endl;
			}
			break;
		case 8:
			// need user input
			// Course and prerequ info linked list
			cout << "Please Enter your Course to Search: " << endl;
			std::cin >> courseSearchPick;
			course = courseList.Search(courseSearchPick);

			if (!course.courseNum.empty())
			{
				courseList.displayPrerequisite(course);
			}
			else
			{
				cout << "Course Not Found" << endl;
			}
			break;
		case 9:
			// need user input
			// course and prerequ info hash
			cout << "Please Enter your Course to Search: " << endl;
			std::cin >> courseSearchPick;
			if (!course.courseNum.empty())
			{
				courseTable->displayPrerequisite(course);
			}
			else
			{
				cout << "Course Not Found" << endl;
			}
			break;
		case 10:
			// need user input
			// course and prerequ info bst
			cout << "Please Enter your Course to Search: " << endl;
			std::cin >> courseSearchPick;
			course = bst->Search(courseSearchPick);
			if (!course.courseNum.empty())
			{
				bst->displayPrerequisite(course);
			}
			else
			{
				cout << "Course Not Found" << endl;
			}
			break;
		case 11:
			//Exit program
			break;
		}
		
	} 	while (user_Choice != 11);

	// Print End User Program
	cout << endl;
	cout << "********************************************************************************************" << endl;
	cout << endl;
	cout << "        Thank you for using the ABC University Computer Science Course Path Program.        " << endl;
	cout << endl;
	cout << "********************************************************************************************" << endl;
}
