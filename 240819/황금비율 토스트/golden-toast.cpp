#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

int n, m;
char cmd;
int idx = 0;
int Size = 0;

struct node
{
    node* prev;
    char data;
    node* next;
};

class LinkedList
{
private:
    node* head;
    node* tail;
public:
    LinkedList()
    {
        head = NULL;
        tail = NULL;
    }

    // 맨 앞에 노드 추가
    void addFrontNode(char c)
    {
        node* temp = new node;
        temp->data = c;

        if (head == NULL)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            temp->next = head;
            head->prev = temp;
            head = temp;
        }
    }

    // 맨 뒤에 노드 추가
    void addNode(char c)
    {
        node* temp = new node;

        temp->data = c;
        temp->next = NULL;

        if (head == NULL)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
    }

    // 노드 삽입
    void insertNode(node* prevNode, char c)
    {
        node* temp = new node;

        temp->data = c;

        temp->prev = prevNode;
        temp->next = prevNode->next;

        prevNode->next = temp;
        temp->next->prev = temp;
    }

    // 헤드 삭제
    void deleteHead()
    {
        node* temp = head;
        head->next->prev = NULL;
        head = head->next;

        delete temp;
    }

    // 중간 노드 삭제
    void deleteNode(node* prevNode)
    {
        node* temp = prevNode->next;

        prevNode->next = temp->next;
        prevNode->next->prev = prevNode;
        
        delete temp;
    }

    // 테일 삭제
    void deleteTail()
    {
        node* temp = tail;
        tail->prev->next = NULL;
        tail = tail->prev;

        delete temp;
    }

    // 첫번째 노드 가져오기
    node* getHead()
    {
        return head;
    }

    // LinkedList 출력
    void display(node* head)
    {
        if (head == NULL)
        {
            return;
        }
        else
        {
            cout << head->data;
            display(head->next);
        }
    }
};

LinkedList a;

void input()
{
    cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        char data;
        cin >> data;

        a.addNode(data);
        idx++;
        Size++;
    }
}

int main() {
    //freopen("sample.txt", "r", stdin);
    input();

    for (int i = 0; i < m; i++)
    {
        cin >> cmd;

        if (cmd == 'L')
        {
            idx--;
        }
        else if (cmd == 'P')
        {
            char data;
            cin >> data;

            if (idx == 0)
            {
                a.addFrontNode(data);
            }
            else if (idx == Size)
            {
                a.addNode(data);
            }
            else
            {
                node* t = a.getHead();
                for (int i = 1; i < idx; i++)
                {
                    t = t->next;
                }

                a.insertNode(t, data);
                idx++;
            }
        }
        else if (cmd == 'R')
        {
            idx++;
        }
        else if (cmd == 'D')
        {
            if (idx == 0)
            {
                a.deleteHead();
            }
            else if (idx == Size)
            {
                a.deleteTail();
            }
            else
            {
                node* t = a.getHead();
                for (int i = 1; i < idx; i++)
                {
                    t = t->next;
                }
                a.deleteNode(t);
            }
        }
    }

    a.display(a.getHead());
}