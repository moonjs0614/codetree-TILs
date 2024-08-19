#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int N;
string cmd;
vector<int> v;

void get_element(int num)
{
    cout << v[num] << endl;
}

int main() {
    //freopen("sample.txt", "r", stdin);
    cin >> N;
    int num;

    for (int i = 0; i < N; i++)
    {
        cin >> cmd;

        if (cmd[0] == 'p')
        {
            if (cmd[1] == 'u') // push
            {
                cin >> num;
                v.push_back(num);
            }
            else if (cmd[1] == 'o') // pop
            {
                v.pop_back();
            }
        }
        else if (cmd[0] == 'g') // get
        {
            cin >> num;
            get_element(num - 1);
        }
        else if (cmd[0] == 's') // size
        {
            cout << v.size() << endl;
        }
    }

    return 0;
}