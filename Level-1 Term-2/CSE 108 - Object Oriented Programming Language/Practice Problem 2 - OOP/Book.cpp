/************Topics of Practice*************************/
/* => Dynamic Allocation of objects         		   */
/* => Constructor overloading		        		   */
/* => Copy constructor				        		   */
/* => Passing and returning objects from functions     */
/*******************************************************/

#include<iostream>
#include<cstring>
#include<cmath>

using namespace std;

class Book{
	int isbn;
	char* title;
	double price;
	public:
		//Constructor functions
		Book(){//default constructor
		    cout<<"In the default constructor of Book"<<endl;
			//Write your code here
		}

		Book(int isbn, char* title, int price){//constructor with arguments
			cout<<"In the parameterized constructor of Book "<<isbn<<endl;
			//Write your code here

		}
		Book(const Book& book){//Copy constructor
		    cout<<"In the copy constructor of Book "<<isbn<<endl;
			//Write your code here
		}

		//Destructor function
		~Book(){
		    cout<<"In the destructor of Book "<<isbn<<endl;
			//Write your code here
		}

		//Write the setter functions here


		//Write the getter functions here

		//Service functions
		void print(){
			//Write your code here
		}

		int isTheSameBook(Book b){
			//Write your code here
		}

		Book generateNewEdition(int isbn, int price){
			//Write your code here
		}
};

int main(){
	//To do: Create a book object named b1 with the default constructor

	//To do: Set the properties of b1 as follows: isbn=101, title="Teach yourself C++", price=230

	//To do: Print the details of b1

	//To do: Create a book object named b2 with values initialized as- isbn=102, title="Teach Yourself Java", price=235

	//To do: Crate a book object b3 and initialize with b2

	//To do: Check if b2 and b3 represent the same book. Print "Yes" or "No" based on the check result.

	//To do: Change the title of b3 to "Teach Yourself Java in Thirty Days"

	//To do: Change the isbn of b3 to 103

	//To do: Print the details of book b2 and b3

	//To do: Create a book b4 as a new edition of b3. Set the isbn and price of the new edition to 104 and 600, respectively

	//To do: Print the details of b4

	//To do: Create a Book pointer named bp1

	//To do: Allocate a Book object to bp1 initialized as- isbn=104, title="C++ For Dummies", price=400

	//To do: Print the details of the book pointed by bp1

	//To do: Free the memory allocated to bp1

	//To do: Create a Book pointer named bl1 and allocate three book objects to it without any initialization

	//To do: Set the properties of the three book objects pointed to by bl1 as follows:
	// <isbn=201, title=Compilers, price=340>,
	// <isbn=202, title=Operating Systems, price=450>,
	// <isbn=203, title=Data Communications, price=600>

	//To do: Print the details of all the three books pointed to by bl1

	//To do: Free the memory allocated to bl1
}
