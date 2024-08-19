#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

int n, m;
char cmd;

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

        if (prevNode != tail)
        {
            temp->prev = prevNode;
            temp->next = prevNode->next;

            prevNode->next = temp;
            temp->next->prev = temp;
        }
        else if (prevNode == tail)
        {
            temp->prev = prevNode;
            prevNode->next = temp;

            tail = temp;
        }
    }

    // 노드 삭제
    void deleteNode(node* prevNode)
    {
        // 만약 삭제되는 노드가 헤드이면
        if (prevNode == head->prev)
        {
            head = prevNode->next;
            head->prev = NULL;

            delete prevNode;

            return;
        }
        node* temp = prevNode->next;

        // 만약 삭제되는 노드가 테일이면
        if (temp == tail)
        {
            prevNode->next = NULL;
            tail->prev = NULL;
            tail = prevNode;
        }
        // 삭제되는 노드가 테일이 아니면
        else
        {
            prevNode->next = temp->next;
            prevNode->next->prev = prevNode;
        }
        
        delete temp;
    }

    // 첫번째 노드 가져오기
    node* getHead()
    {
        return head->next;
    }

    // 마지막 노드 가져오기
    node* getTail()
    {
        return tail;
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
    }
}

int main() {
    //freopen("sample.txt", "r", stdin);
    a.addNode('0');

    input();
    node* cursor = a.getTail();

    for (int i = 0; i < m; i++)
    {
        cin >> cmd;

        if (cmd == 'L')
        {
            node* temp = a.getHead();
            if (cursor == temp->prev) continue;

            cursor = cursor->prev;
        }
        else if (cmd == 'R')
        {
            node* temp = a.getTail();
            if (cursor == temp) continue;

            cursor = cursor->next;
        }
        else if (cmd == 'P')
        {
            char data;
            cin >> data;

            a.insertNode(cursor, data);
            cursor = cursor->next;
        }
        else if (cmd == 'D')
        {
            node* temp = a.getTail();
            if (cursor == temp) continue;

            a.deleteNode(cursor);
        }
    }

    a.display(a.getHead());
}