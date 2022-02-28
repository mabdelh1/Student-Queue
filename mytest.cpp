#include "rqueue.h"
#include "rqueue.cpp"
using namespace std;

class Tester{
public:
  bool mergingExceptionTest(RQueue queueA, RQueue queueB);
  bool dequeueEmptyQueueTest(RQueue queue);
  bool testCopyConstructor(RQueue queue, RQueue copy);
  bool testAssignmentOperator(RQueue queue, RQueue copy);
  void BufListEnqueuePerformance(int numTrials, int N);
};

int priorityYear(const Student &tudent);
int priorityR(const Student &student);

int main() {
  Tester tester;
  RQueue queueA(priorityYear);
  RQueue queueB(priorityR);
  RQueue queueC(priorityYear);
  RQueue queueD(priorityR);
  RQueue queueE(priorityYear);
  
  //Identify Students  
  Student student1("Leonard",1,1,2,1);
  Student student2("Denis",2,1,0,0);
  Student student3("Kachi",3,2,1,1);
  Student student4("Penelope",4,2,2,1);
  Student student5("Ahmed",6,3,2,0);
  Student student6("Ashnell",5,3,2,1);
  
  //Add students to queueA
  queueA.insertStudent(student1);
  queueA.insertStudent(student2);
  queueA.insertStudent(student3);
  queueA.insertStudent(student4);
  //Add students to queueB
  queueB.insertStudent(student5);
  queueB.insertStudent(student6);
  //Add students to queueC
  queueC.insertStudent(student1);
  queueC.insertStudent(student2);
  queueC.insertStudent(student3);
  queueC.insertStudent(student4);
  queueC.insertStudent(student5);
  queueC.insertStudent(student6);
  //Add students to queueD
  queueD.insertStudent(student1);
  queueD.insertStudent(student2);
  queueD.insertStudent(student3);
  queueD.insertStudent(student4);
  queueD.insertStudent(student5);
  queueD.insertStudent(student6);
  
  //Test merging two queues with different priorities, in order to pass an domain_error exception must be caughts
  cout << "\nTest Merging Queues with Different Priorities" << endl;
  if (tester.mergingExceptionTest(queueA, queueB)) cout << "Exception Caught: Test Passed" << endl;
  else cout << "Exception Not Caught: Test Faild" << endl;
  
  queueB.setPriorityFn(priorityYear);
  queueA.mergeWithQueue(queueB);
  
  //Test copy constructor with normal cases, where the queue has a considrable amount of students
  RQueue copyA(queueC);
  cout << "\n Test Copy Constructor: Normal Case" << endl;
  if(tester.testCopyConstructor(queueC, copyA)) cout << "Copy Constructor Test Passed!" << endl;
  else cout << "Copy Constructor Test Faild" << endl;
  
  //Test copy constructor with edge case, no students in the queue
  RQueue copyC(queueE);
  cout << "\n Test Copy Constructor: Edge Case" << endl;
  if(tester.testCopyConstructor(queueE, copyC)) cout << "Copy Constructor Test Passed!" << endl;
  else cout << "Copy Constructor Test Faild" << endl;
  
  //Test assignment operator with normal case,where the queue has a considrable amount of students
  RQueue copyB = queueD;
  cout << "\n Test Assignment Operator: Normal Case" << endl;
  if(tester.testAssignmentOperator(queueD, copyB)) cout << "Assignment Operator Test Passed!" << endl;
  else cout << "Assignment Operator Test Faild" << endl;
  
  //Test assignment operator with edge case, no students in the queue
  RQueue copyD = queueE;
  cout << "\n Test Assignment Operator: Edge Case" << endl;
  if(tester.testAssignmentOperator(queueE, copyD)) cout << "Assignment Operator Test Passed!" << endl;
  else cout << "Assignment Operator Test Faild" << endl;
  
  //Display contents and arrangment of queue before changing the priority
  cout << "\n Connect of QueueA before changing priority" << endl;
  queueA.printStudentQueue();
  cout << "\n Dump QueueA before changing priority" << endl;
  queueA.dump();
  
  //Chnage priority
  queueA.setPriorityFn(priorityR);
  
  //Display contents and arrangment of queue after changing the priority 
  cout << "\n Dump QueueA After changing priority" << endl;
  queueA.dump();
  cout << "\nPrint Ordered QueueA After Changing Priority\n";
  while (queueA.numStudents() > 0) {
    Student student = queueA.getNextStudent();
    cout << "[" << priorityYear(student) << "] " << student << endl;
  }
  
  //Test removing from queue when it is empty, in order to pass, a domain_error exception must be caught
  cout << "\nTest Dequeue Empty Queue" << endl;
  if (tester.dequeueEmptyQueueTest(queueA)) cout << "Exception Caught: Test Passed" << endl;
  else cout << "Exception Not Caught: Test Faild" << endl;
  
  cout << "\nMeasuring the efficiency of insertion functionality:" << endl;
  int M = 8;//number of trials  
  int N = 100;//original input size  
  tester.BufListEnqueuePerformance(M, N);
  return 0;
}

int priorityYear(const Student& student) {
  //priority value is determined by student's year
  return student.getYear();
}

int priorityR(const Student& student) {
  //priority is determined by student class                            
  return student.getPriority();
}

bool Tester::mergingExceptionTest(RQueue queueA, RQueue queueB){
  try{
    queueA.mergeWithQueue(queueB);
  }
  catch(domain_error &e){
    //the exception that we expect
    return true;
  }
  catch(...){
    //any other exception, this case is not acceptable
    return false;
  }
  //no exception thrown, this case is not acceptable
  return false;
}

bool Tester::dequeueEmptyQueueTest(RQueue queue){
  while (queue.numStudents() > 0) {
    Student student = queue.getNextStudent();
  }
  try{
    Student student2 = queue.getNextStudent();
  }
  catch(domain_error &e){
    //the exception that we expect
    return true;
  }
  catch(...){
    //any other exception, this case is not acceptable
    return false;
  }
  //no exception thrown, this case is not acceptable
  return false;
}

bool Tester::testCopyConstructor(RQueue queue, RQueue copy){
  bool verdict = false; //The verdict will be set to false, unless changes
  //check if both are nullptr in case this is an edge case test
  if(queue._heap == nullptr && copy._heap == nullptr)
    return true;
  if(queue._heap != copy._heap){ //if heaps are not equal, they so far the test is passing
    if(queue._size == copy._size && queue.priority == copy.priority){
      while(copy.numStudents() > 0){
	Student copyStudents = copy.getNextStudent();
	Student queueStudebts = queue.getNextStudent();
	//All data must be equal
	if(copyStudents.getName() == queueStudebts.getName() &&
	   copyStudents.getPriority() == queueStudebts.getPriority() &&
	   copyStudents.getYear() == queueStudebts.getYear() &&
	   copyStudents.getMajor() == queueStudebts.getMajor())
	  verdict = true; //if code arrives here, this means the test passes, therefore change verdict to true
      }
    }
  }
  return verdict;
}

bool Tester::testAssignmentOperator(RQueue queue, RQueue copy){
  bool verdict = false;//The verdict will be set to false, unless changes
  //check if both are nullptr in case this is an edge case test
  if(queue._heap == nullptr && copy._heap == nullptr)
    return true;
  if(queue._heap != copy._heap){//if heaps are not equal, they so far the test is passing
    if(queue._size == copy._size && queue.priority == copy.priority){
      while(copy.numStudents() > 0){
        Student copyStudents = copy.getNextStudent();
        Student queueStudebts = queue.getNextStudent();
	//All data must be equal
        if(copyStudents.getName() == queueStudebts.getName() &&
           copyStudents.getPriority() == queueStudebts.getPriority() &&
           copyStudents.getYear() == queueStudebts.getYear() &&
           copyStudents.getMajor() == queueStudebts.getMajor())
          verdict = true;//if code arrives here, this means the test passes, therefore change verdict to true 
      }
    }
  }
  return verdict;
}

void Tester::BufListEnqueuePerformance(int M, int N){
  Student student("Mohamed",2,1,0,0);
  const int a = 2;//scaling factor for input size  
  double T = 0.0;//to store running times
  clock_t start, stop;//stores the clock ticks while running the program
  for(int k=0;k<M-1;k++)
    {
      RQueue queueF(priorityR);
      start = clock();
      for(int i=0; i<N; i++)
	queueF.insertStudent(student);//the algorithm to be analyzed for efficiency              
      stop = clock();
      T = stop - start;//number of clock ticks the algorithm took  
      cout << "Inserting " << N * N << " students took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
      N = N * a;//increase the input size by the scaling factor                                                                        
    }
}
