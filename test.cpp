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
    a.push_back("anjali");
    a.push_back("naman");
    a.push_back("nonu");
    a.push_back("sayam");
    a.push_back("dev");
    a.push_back("#");
    a.push_back("sonam");
    a.push_back("yami");
    sort(a.begin(), a.end());

    int production_size = a.size();
    vector<pair<int, int> > common_section_slices;
    for (int index = 0; index < production_size; index++)
    {
        int go_ahead = index;
        while (go_ahead < production_size)
        {
            if (a[index][0] == a[go_ahead][0])
                go_ahead++;
            else
                break;
        }
        go_ahead = go_ahead - 1;
        common_section_slices.push_back(make_pair(index, go_ahead));
        index += (go_ahead - index);
    }
    for (int index = 0; index < production_size; index++)
    {
        cout << a[index] << " ";
    }
    cout << endl;
    for (vector<pair<int, int> >::iterator it = common_section_slices.begin(); it != common_section_slices.end(); it++)
    {
        cout << (*it).first << " " << (*it).second << endl;
    }
}