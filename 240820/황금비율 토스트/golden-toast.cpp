#include <iostream>
#include <string>
#include <list>
 
using namespace std;

int n, m;

string s;

int main(){
    // 입력:
    cin >> n >> m;
    cin >> s;
    
    // 연결리스트 정의
    list<char> l;
    for(int i = 0; i < s.length(); i++)
        l.push_back(s[i]);  
    // iterator 정의
    list<char>::iterator it = l.end();
    
    while(m--) { 
        char command;
        cin >> command;
        
        if(command == 'L') {
            if(it != l.begin())   // 빵들의 맨 앞이 아니라면
                it--;             // 앞으로 이동합니다.
        } 
        else if(command == 'R') {
            if(it != l.end())     // 빵들의 맨 뒤가 아니라면
                it++;             // 뒤로 이동합니다.
        }
        else if(command == 'D') {
            if(it != l.end())     // 빵들의 맨 뒤가 아니라면
                it = l.erase(it); // 바로 뒤에 있는 빵을 제거합니다.
        }
        else if(command == 'P') {
            char c;
            cin >> c;
            l.insert(it, c);      // 가리키는 위치에 문자 c를 추가합니다.
        }    
    }
    
    // 출력:
    for (list<char>::iterator it = l.begin(); it != l.end(); it++) {
        cout << *it;
    }
    return 0;     
}