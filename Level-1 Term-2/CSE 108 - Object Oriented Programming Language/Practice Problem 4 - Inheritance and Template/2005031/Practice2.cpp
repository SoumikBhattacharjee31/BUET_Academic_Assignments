# include <iostream >
# include <typeinfo >
# include <cmath >
# include <cstdlib >

#define X double
#define type "double"

using namespace std;

/* Output for            #define X double
                         #define type "double"

         d
         i

         3NumIdE
         6SquareIdE
         8Sqr_rootIdE

         Square <double> object.   Value is: 400

         Generating random objects .

         Sqr_root <double> object.   Value is: 2.64575
         Square <double> object.   Value is: 0
         Sqr_root <double> object.   Value is: 0
         Sqr_root <double> object.   Value is: 2
         Square <double> object.   Value is: 64
         Sqr_root <double> object.   Value is: 64
         Square <double> object.   Value is: 16
         Sqr_root <double> object.   Value is: 16


   Output for            #define X double
                         #define type "double"


         i
         i

         3NumIiE
         6SquareIiE
         8Sqr_rootIiE

         Square <int> object.   Value is: 400

         Generating random objects .

         Sqr_root <int> object.   Value is: 2
         Square <int> object.   Value is: 0
         Sqr_root <int> object.   Value is: 0
         Sqr_root <int> object.   Value is: 2
         Square <int> object.   Value is: 64
         Sqr_root <int> object.   Value is: 64
         Square <int> object.   Value is: 16
         Sqr_root <int> object.   Value is: 16
*/



template <class T>
class Num
{
public :
    T x;
    Num (T i)
    {
        x = i;
    }
    //Write down a virtual function get_val() here
    virtual T get_val(){return x;};
};

//Write a generic "Square" class here that inherits "Num" class
template <class T>
class Square:public Num<T>
{
public :
    Square (T i):Num<T>(i*i){;}
};


//Write a generic "Sqr_root" function here that inherits "Num" class
template <class T>
class Sqr_root:public Num<T>
{
public :
    Sqr_root (T i):Num<T>(sqrt(i)){;}
};


// A factory for objects derived from Num .
template <class T>
Num <T> * generator ()
{
    switch ( rand () % 2)
    {
    case 0:
        return new Square <T> ( rand () % 10) ;
    case 1:
        return new Sqr_root <T> ( rand () % 20) ;
    }
    return NULL ;
}


int main ()
{
    Num <X> ob1 (10), *p1;
    Square <X> ob2 (20);
    Sqr_root <X> ob3 (16) ;
    int i;

    //write necessary codes to generate the appropriate output.
    char c[100]=type;
    cout<<*c<<endl<<'i'<<endl<<endl;
    cout<<"3NumI"<<*c<<'E'<<endl;
    cout<<"6SquareI"<<*c<<'E'<<endl;
    cout<<"8Sqr_rootI"<<*c<<'E'<<endl<<endl;

    p1 = &ob2;
    if( typeid(ob2) == typeid(Square<X>))
        cout << "Square <" << type << "> object.  ";

    if( typeid(*p1) == typeid(ob2))
        cout << " Value is: " << p1 -> get_val() <<endl <<endl;


    cout << "Generating random objects .\n" << endl;
    for (i=0; i <5; i++)
    {
        p1 = generator<X>(); // get next object

        try
        {
            if( typeid (*p1) == typeid (Square <X>)) throw 1;
            if( typeid (*p1) == typeid(Sqr_root <X>)) throw 2;
        }

        //Write down necessary codes under "catch" codes here to generate appropriate output
        catch(int c){
            if(c==1)
                cout<<"Square <"<<type<<"> object.   Value is: " << p1 -> get_val() <<endl;
            if(c==2);
                cout<<"Sqr_root <"<<type<<"> object.   Value is: " << p1 -> get_val() <<endl;
        }
    }
    return 0;
}
