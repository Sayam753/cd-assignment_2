#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<string> a;
    a.push_back("amrit");
    a.push_back("anubhav");
    if (a[0][0] == 'a')
    {
        cout << "yes";
    }
    else
    {
        cout << "no";
    }
}