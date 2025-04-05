#include <iostream>
using namespace std;

class Time
{
private:
    int hours, minutes;
public:
    Time()
    {
        hours = 0;
        minutes = 0;
    }
    Time(int hr, int mn)
    {
        hours = hr;
        minutes = mn;
    }
    friend ostream &operator<<(ostream &output, Time &t);
    friend istream &operator>>(istream &input,  Time &t);
    // your code
};

// your code
ostream &operator<<(ostream &output, Time &t)
{
    output << "Hours: " << t.hours << " Minutes: " << t.minutes << endl;
    return output;
}
istream &operator>>(istream  &input, Time &t)
{
    cout << "Enter hours and minutes: ";
    input >> t.hours >> t.minutes;
    return input;
}

int main()
{
    Time t1, t2, t3, t4, t5, t6;
    cin >> t1;          // take input for hours and minutes of t1
    cin >> t2;          // take input for hours and minutes of t2
    cout << t1;         // output hours and minutes of t1
    cout << t2;         // output hours and minutes of t2
    t3 = t1 + t2;       // t3 is the sum of t1 and t2
    cout << t3;         // output hours and minutes of t3
    t4 = t1 + t2 + t3;  // t4 is the sum of t1, t2, and t3
    cout << t4;         // output hours and minutes of t4
    t5 = t1 + 3;        // t5 is t1 plus 3 minutes
    cout << t5;         // output hours and minutes of t5
    t6 = 4 + t1;        // t6 is t1 plus 4 hours
    cout << t6;         // output hours and minutes of t6
    if(t1 > t2)         // compare t1 and t2
        cout << "t1 is greater than t2" << endl;
    else
        cout << "t1 is less than (or equal to) t2" << endl;
    t1+=t2;             // t1 is t1 plus t2
    cout << t1;         // output hours and minutes of t1
    return 0;
}

/* sample output
Enter hours and minutes: 2 45
Enter hours and minutes: 3 55
Hours: 2 Minutes: 45
Hours: 3 Minutes: 55
Hours: 6 Minutes: 40
Hours: 13 Minutes: 20
Hours: 2 Minutes: 48
Hours: 6 Minutes: 45
t1 is less than (or equal to) t2
Hours: 6 Minutes: 40
*/