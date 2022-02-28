// CMSC 341 - Spring 2021 - Project 3
// RQueue: 

#include "rqueue.h"
using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

RQueue::RQueue(prifn_t priFn)
{
  //Set the priority value, and initialize other variables
  priority = priFn;
  _heap = nullptr;
  _size = 0;
}

RQueue::~RQueue()
{
  //Call clear function to deallocate all memory
  clear();
  _heap = nullptr;
  _size = 0;
}

RQueue::RQueue(const RQueue& rhs)
{
  //Clone variables and call copy function to make copy of the tree
  _size = rhs._size;
  priority = rhs.priority;
  _heap = copy(rhs._heap);
}


RQueue& RQueue::operator=(const RQueue& rhs)
{
  //if statement to guard against self assignment
  if(this != &rhs){
    clear(); //start by deallocating memory
    //The proceed to clone all data
    _size = rhs._size;
    priority = rhs.priority;
    _heap = copy(rhs._heap);
  }
  
  return *this;
}

void RQueue::insertStudent(const Student& input) {
  RQueue newStudent(getPriorityFn()); //create a new RQueue object to merge with running one
  newStudent._size = 1; //Set it's size to 1, since it will only hold one student
  newStudent._heap = new Node(input); //Allocate memory for new student
  mergeWithQueue(newStudent); //Proceed to merge both RQueues
  newStudent._heap = nullptr; //Set RQueue object to null to avoid double deletion
  newStudent._size = 0;
}

Student RQueue::getNextStudent() {
  if(_size == 0) //If user attempts to retrieve a student when the queue is empty, throw a domain_error exception
    throw std::domain_error("Extracting a student from an empty heap is not allowed.");
  else{
    Node* temp = _heap; //Hold root to be deleted later
    Student nextStudent = temp->getStudent(); //extract next student from root
    _heap = merge(temp->_left, temp->_right); //merge left and right children of the root
    _size = _size - 1; //Decrement tree size since one student has been removed
    delete temp; //proceed to delete the former root
    return nextStudent;
  }
}


void RQueue::mergeWithQueue(RQueue& rhs) {
  if(this != &rhs){ //if statement to guard from self merging
    if(priority != rhs.priority){ //if both queues to be merged do not have the same priority throw domain_error exception
      throw std::domain_error("Merging with different priority functions is not allowed.");
    }
    else{
      _heap = merge(_heap, rhs._heap); //call merge to merge two heaps
      _size = _size + rhs._size; //update size accordingly
    }
  }
  rhs._heap = nullptr;;
}

void RQueue::clear() {
  clear(_heap); //Call recursize clear function with the root as parameter
}

int RQueue::numStudents() const
{
  return _size; //return size as the number of students
}

void RQueue::printStudentQueue() const {
  if(_size == 0) //if queue is empty, notify user
    cout << "Empty Skew heap.\n";
  else{
    print(_heap);
  }
}

prifn_t RQueue::getPriorityFn() const {
  return priority; //return current set priority
}

void RQueue::setPriorityFn(prifn_t priFn) {
  priority = priFn; //updtae priority variable
  if(_size > 0){ //reorganize tree if it is not empty
    Student cloneStudent[_size]; //Array to temporerly hold student objects
    int i = 0;
    int end = _size;
    while(i < end){ //extract all student objects into the array
      cloneStudent[i] = getNextStudent();
      i++;
    }
    for(int c = 0; c < end; c++) //Add extracted students back to the queue
      insertStudent(cloneStudent[c]);
  }
}

// for debugging
void RQueue::dump() const
{
  if (_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(_heap);
    cout << endl;
  }
}

// for debugging
void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->_left);
    cout << priority(pos->_student) << ":" << pos->_student.getName();
    dump(pos->_right);
    cout << ")";
  }
}

// overloaded insertion operator for Student class
ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student: " << student.getName() << ", priority: " << student.getPriority()
       << ", year: " << student.getYear() << ", major: " << student.getMajorStr() 
       << ", group: " << student.getGroupStr();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getStudent();
  return sout;
}

void RQueue::clear(Node* heap){
  if(heap == nullptr)
    return;
  else{
    //The code never gets here
    clear(heap->_left);
    clear(heap->_right);
    delete heap;
  }
}

Node* RQueue::copy(const Node* heap){
  if(heap == nullptr)
    return nullptr;
  //recursively call this function and create copy of data
  Node* copyNode = new Node(heap->_student);
  copyNode->_left = copy(heap->_left);
  copyNode->_right = copy(heap->_right);
  return copyNode;
}

Node* RQueue::merge(Node* heap1, Node* heap2){
  Node* temp = nullptr; //to hold node temporarly for swapping
  //if one of the node is nullptr, return the other
  if(heap1 == nullptr)
    return heap2;
  else{
    if(heap2 == nullptr)
      return heap1;
    else{
      if(priority(heap1->_student) <= priority(heap2->_student)){ //evaluate data difference to determine how to swap nodes
	temp = heap1->_right;
	heap1->_right = heap1->_left;
	heap1->_left = merge(heap2, temp);
	return heap1;
      }
      else
	return merge(heap2, heap1);
    }
  }
}

void RQueue::print(Node *heap) const{
  if(heap != nullptr){ //print data by calling this function recursivly
    cout << "[" << priority(heap->getStudent()) << "] " << heap->getStudent() << endl;
    print(heap->_left);
    print(heap->_right);
  }
}
