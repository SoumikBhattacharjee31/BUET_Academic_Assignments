/*******************************Offline 2****************************************/
/* Implement the functions of the class BookShop below as per the instructions  */
/* given inline the class definition. Make sure the output of the code in the   */
/* main function matches the given sample output. You may add/delete codes in   */
/* the main function during your testing. But before submission remove all your */
/* modifications from the main function and submit it as provided initially.    */
/********************************************************************************/

#include<iostream>
#include<cstring>
#include<cmath>
#include "Book.cpp"

using namespace std;

class BookShop{
	char* name;
	Book* books; //List of books in the shop
	int size;  //maximum number of books the shop can keep
	int count; //tracks currently how many numbers in the shop
	public:
    void setName(char* name){
		/*Set the name of the bookshop to the given name*/
		delete[] (this->name);
        this->name=new char[strlen(name)+1];
        if(!(this->name)){
            cout<<"Error!";
            exit(-1);
        }
        strcpy(this->name,name);
    }

    BookShop(){//Default constructor
		/*Initialize with size 5 and name to empty string ""*/
        size=5;
        count=0;
        books=new Book[size];
        if(!books){
            cout<<"Error!";
            exit(-1);
        }
        name=new char[strlen("")+1];
        if(!name){
            cout<<"Error!";
            exit(-1);
        }
        strcpy(name,"");
    }

    BookShop(char* name, int size){//Parameterized constructor
		/*Initialize with the given name and size*/
        this->size=size;
        count=0;
        books=new Book[size];
        if(!books){
            cout<<"Error!";
            exit(-1);
        }
        this->name=new char[strlen(name)+1];
        if(!(this->name)){
            cout<<"Error!";
            exit(-1);
        }
        strcpy(this->name,name);
    }

    BookShop(const BookShop& bs){//Copy constructor
		/*Write necessary code for the copy constructor*/
        size=bs.size;
        count=bs.count;
        books=new Book[bs.size];
        if(!books){
            cout<<"Error!";
            exit(-1);
        }
        for(int i=0;i<bs.count;i++){
            books[i].set_isbn(bs.books[i].get_isbn());
            books[i].set_title(bs.books[i].get_title());
            books[i].set_price(bs.books[i].get_price());
        }
        name=new char[strlen(bs.name)+1];
        if(!name){
            cout<<"Error!";
            exit(-1);
        }
        strcpy(name,bs.name);
    }

	~BookShop(){//Destructor
		/*Free memory as applicable*/
        delete[] name;
        delete[] books;
    }

    void addBook(Book b){
		/*Add book b to the end of the list*/
		books[count].set_isbn(b.get_isbn());
        books[count].set_title(b.get_title());
        books[count].set_price(b.get_price());
        count++;
    }

	void addBook(Book* ba, int count){
		/*Add the given array of books ba to the end of the list*/
		for(int i=0;i<count;i++)
            addBook(ba[i]);
    }

    Book getBookInfo(char* title){
		/*Return the **first** book that has the given title. Assume there is atleast one book in the list with the given title*/
        for(int i=0;i,count;i++)
            if(!strcmp(title,books[i].get_title()))
                return books[i];
    }

    void updateBookPrice(int isbn, int price){
		/*Update the price of the book with given isbn to the given price*/
        for(int i=0;i<count;i++)
            if(isbn==books[i].get_isbn()){
                books[i].set_price(price);
                break;
            }
    }

    void removeBook(int isbn){
		/*Remove the book with the given isbn from the list. After removing a book all the books below the removed book will be shifted up, i.e, there will be no hole in the list*/
        for(int i=0;i<count;i++)
            if(isbn==books[i].get_isbn()){
                count--;
                for(int j=0;i+j<count;j++){
                    books[i+j].set_isbn(books[i+j+1].get_isbn());
                    books[i+j].set_price(books[i+j+1].get_price());
                    books[i+j].set_title(books[i+j+1].get_title());
                }
                books[count].set_isbn(-1);
                books[count].set_price(0);
                books[count].set_title("No Title");
            }
    }

	int totalPrice(){
		/*Return the sum of the prices of all the books in the list*/
        double sum=0;
        for(int i=0;i<count;i++)
            sum+=books[i].get_price();
        return sum;
    }

    void print(){
		/*Print the bookshop's name followed by information of all the books in the bookshop*/
        cout<<"Bookshop Name: "<<name<<endl;
        for(int i=0;i<count;i++)
            books[i].print();
    }

    BookShop mergeShop(BookShop b){
		/* Return a new bookshop object that will contain all the books of this bookshop and the given bookshop b*/ 			/* Use the title **MergedShop** of the new bookshop														*/
        BookShop MergedShop("MergedShop",(b.count)+count);
        MergedShop.addBook(books,count);
        MergedShop.addBook(b.books,b.count);
        return MergedShop;
    }
};

int main(){

	BookShop bs1;
	bs1.setName("Boimela");
	Book b1(101,"Teach Yourself C++",100);
	Book b2(102,"Teach Yourself C",200);
	Book b3(103,"Java For Dummies",300);
	bs1.addBook(b1);
	bs1.addBook(b2);
	bs1.addBook(b3);
	bs1.print(); /*Output:
					Bookshop Name: Boimela
					ISBN: 101, Title: Teach Yourself C++, Price: 100
					ISBN: 102, Title: Teach Yourself C, Price: 200
					ISBN: 103, Title: Java For Dummies, Price: 300
				 */
	cout<<endl;
	cout<<"Total price of books: "<<bs1.totalPrice()<<endl;	/*Output:
															   Total price of books: 600
															*/

	cout<<endl;
	bs1.removeBook(102);
	bs1.print(); /*Output:
					Bookshop Name: Boimela
					ISBN: 101, Title: Teach Yourself C++, Price: 100
					ISBN: 103, Title: Java For Dummies, Price: 300
				 */

	cout<<endl;
	bs1.updateBookPrice(101,500);
	bs1.print(); /*Output:
					Bookshop Name: Boimela
					ISBN: 101, Title: Teach Yourself C++, Price: 500
					ISBN: 103, Title: Java For Dummies, Price: 300
				 */

	cout<<endl;
	Book jfd=bs1.getBookInfo("Java For Dummies");
	jfd.print();  /*Output:
					 ISBN: 103, Title: Java For Dummies, Price: 300
				 */

	cout<<endl;
	Book ba[3]={Book(201,"Operating Systems",1000),Book(202,"Compilers",2000),Book(203,"Computer Networks",1500)};
	BookShop bs2("Puthighor",5);
	bs2.addBook(ba,3);
	bs2.print();  /*Output:
					 Bookshop Name: Puthighor
					 ISBN: 201, Title: Operating Systems, Price: 1000
					 ISBN: 202, Title: Compilers, Price: 2000
					 ISBN: 203, Title: Computer Networks, Price: 1500
				 */

	cout<<endl;
	BookShop bs3=bs1.mergeShop(bs2);
	bs3.setName("Mullick Brothers");
	bs3.print(); /*Output:
					Bookshop Name: Mullick Brothers
					ISBN: 101, Title: Teach Yourself C++, Price: 500
					ISBN: 103, Title: Java For Dummies, Price: 300
					ISBN: 201, Title: Operating Systems, Price: 1000
					ISBN: 202, Title: Compilers, Price: 2000
					ISBN: 203, Title: Computer Networks, Price: 1500
				 */
}
