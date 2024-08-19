#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <list>
using namespace std;

int N;
string cmd;
list<int> l;

void push_front(int num)
{
    l.push_front(num);
}

void push_back(int num)
{
    l.push_back(num);
}

void pop_front()
{
    cout << l.front() << endl;
    l.pop_front();
}

void pop_back()
{
    cout << l.back() << endl;
    l.pop_back();
}

void size()
{
    cout << l.size() << endl;
}

void empty()
{
    cout << l.empty() << endl;
}

void front()
{
    cout << l.front() << endl;
}

void back()
{
    cout << l.back() << endl;
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
            if (cmd[1] == 'u')
            {
                if (cmd[5] == 'f') // push_front
                {
                    cin >> num;
                    push_front(num);
                }
                else if (cmd[5] == 'b') // push_back
                {
                    cin >> num;
                    push_back(num);
                }
            }
            else if (cmd[1] == 'o')
            {
                if (cmd[4] == 'f') // pop_front
                {
                    pop_front();
                }
                else if (cmd[4] == 'b') // pop_back
                {
                    pop_back();
                }
            }
        }
        else if (cmd[0] == 'f') // front
        {
            front();
        }
        else if (cmd[0] == 'b') // back
        {
            back();
        }
        else if (cmd[0] == 'e') // empty
        {
            empty();
        }
        else if (cmd[0] == 's') // size
        {
            size();
        }
    }
}