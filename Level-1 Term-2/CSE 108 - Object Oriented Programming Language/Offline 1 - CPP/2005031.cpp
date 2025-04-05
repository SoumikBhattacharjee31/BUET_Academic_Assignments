#include<iostream>
#include<cstring>
#include<cstdlib>
using namespace std;

class Student{
    char *name;
    int id, credit;
    double cgpa;
public:
    Student(char*,int,int,double);
    ~Student();
    void showInfo();
    void change(char*);
    void change(int);
    void addTermResult(int,double);
};

class Batch{
    Student* s;
    int index=0;
public:
    Batch();
    ~Batch();
    void addStudent(Student&);
    void showAllStudents();
};

int main()
{
 // Part 1
 /*A student class holds the information of the
 student name, id, credit earned so far, and
 cumulative gpa (cgpa)*/
 /*The name of a student is stored using malloc,
 which will be freed during destruction*/
 Student s1("Tamim Iqbal", 1905131, 39, 3.56);
 Student s2("Liton Das", 1905150, 39, 3.52);
 s1.showInfo();
 cout<<"Changing the name of s1"<<endl;
 /*During changing the name, you should reallocate
 memory of the new name and free the previously
 allocated memory*/
 s1.change("Tamim Iqbal Khan");
 s1.showInfo();
 cout<<"Changing the id of s2"<<endl;
 s2.change(1905149);
 s2.showInfo();
 cout<<"Adding a term result of s1"<<endl;
 /*The first argument of addTermResult is the credit
 earned in a term and second one is the gpa obtained
 in that term*/
 s1.addTermResult(19, 3.85);
 s1.showInfo();
 // Part 2
 /*Batch contains a list of students*/
 /*During construction, a Batch object allocates dynamic
 memory for 1200 students using malloc. The memory will
 be freed during destruction*/
 Batch b;
 b.addStudent(s1);
 b.addStudent(s2);
 cout<<"Printing the list of students of the batch b"<<endl;
 b.showAllStudents();
}

Student::Student(char* _name, int _id, int _credit, double _cgpa){
    name=(char*)malloc(strlen(_name)*sizeof(char));
    if(!name){
        cout<<"Error";
        exit(1);
    }
    strcpy(name,_name);
    id=_id;
    credit=_credit;
    cgpa=_cgpa;
}
Student::~Student(){
    free(name);
}
void Student::showInfo(){
    cout<<"Name: "<<name<<", Id: "<<id<<", Credit Completed: "<<credit<<", Cgpa:"<<cgpa<<endl;
}
void Student::change(char* _name){
    free(name);
    name=(char*)malloc(strlen(_name)*sizeof(char));
    if(!name){
        cout<<"Error";
        exit(1);
    }
    strcpy(name,_name);
}
void Student::change(int _id){
    id=_id;
}
void Student::addTermResult(int _credit, double _cgpa){
    cgpa=(cgpa*credit+_cgpa*_credit)/(credit+_credit);
    credit+=_credit;
}

Batch::Batch(){
    s=(Student*)malloc(1200*sizeof(Student));
    if(!s){
        cout<<"Error";
        exit(1);
    }
}
Batch::~Batch(){
    free(s);
}
void Batch::addStudent(Student &_s){
    s[index]=_s;
    index++;
}
void Batch::showAllStudents(){
    for(int i=0;i<index;i++)
        s[i].showInfo();
}