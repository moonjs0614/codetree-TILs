#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

int N, M, K;
int Ex, Ey;
int Map[11][11];

int Ans = 0;

int dx[4] = { 1, -1, 0, 0 };
int dy[4] = { 0, 0, 1, -1 };

struct Person {
    int x;
    int y;
};

struct Position {
    int x;
    int y;
    int Size;
};

vector<Person> Vector;
vector<Position> Data;

void Input() {
    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cin >> Map[i][j];
        }
    }

    int X;
    int Y;
    for (int i = 1; i <= M; i++)
    {
        cin >> X >> Y;
        Vector.push_back({ X, Y });
    }

    cin >> Ex >> Ey;
}


// 해당 좌표에서 출구까지의 거리
int Dist_Exit(int x, int y) {
    return abs(x - Ex) + abs(y - Ey);
}

//최선의 정사각형 기준 정렬
bool Cmp(Position& A, Position& B) {
    if (A.Size == B.Size)
    {
        if (A.x == B.x)
        {
            return A.y < B.y;
        }
        return A.x < B.x;
    }
    return A.Size < B.Size;
}


// 각 참가자의 이동
void Exit(Person& P) {

    int Curr = Dist_Exit(P.x, P.y);

    //자동 상, 하 우선 탐색
    for (int i = 0; i < 4; i++)
    {
        int nx = P.x + dx[i];
        int ny = P.y + dy[i];
        int Next = Dist_Exit(nx, ny);

        if (nx < 1 || ny < 1 || nx > N || ny > N) continue; //범위 초과
        if (Curr <= Next) continue;                         //이전보다 가까워져야한다.
        if (Map[nx][ny] > 0) continue;                      //빈칸이 아닌 경우 이동 불가.

        Ans++;
        P.x = nx;   //x 좌표 변경
        P.y = ny;   //y 좌표 변경
        break;
    }

}

//정사각형 정보 생성
void Square_Data(int x, int y) {
    int x_Gap = abs(x - Ex);
    int y_Gap = abs(y - Ey);

    int Size = max(x_Gap, y_Gap);

    int mx = max(Ex, x) - Size;
    if (mx < 1) mx = 1;

    int my = max(Ey, y) - Size;
    if (my < 1) my = 1;

    Data.push_back({ mx, my, Size });
}


//사각형을 시계 방향으로 90도 회전한다.
void Rotation(int x, int y, int Size) {

    vector<pair<pair<int, int>, int>> Pair;
    bool Flag = true; //출구 이동 여부

    for (int i = 0; i <= Size; i++)
    {
        for (int j = 0; j <= Size; j++)
        {
            //기존 좌표
            int px = x + i;
            int py = y + j;

            //전환 좌표
            int nx = x + j;
            int ny = y + Size - i;

            //출구가 회전하는 경우
            if (px == Ex && py == Ey && Flag == true)
            {
                Flag = false;
                Ex = nx;
                Ey = ny;
            }

            Pair.push_back({ {nx, ny}, Map[px][py] });
        }
    }

    //기존 값을 새로운 좌표에 도입 + 벽 허물기
    for (auto E : Pair)
    {
        int x = E.first.first;
        int y = E.first.second;
        int m = E.second;
        if (m == 0)
        {
            Map[x][y] = 0;
        }
        else if (m > 0)
        {
            Map[x][y] = m - 1;
        }
    }

    //참가자 회전
    for (auto& E : Vector)
    {
  
        if (E.x >= x && E.x <= x + Size)
        {

            if (E.y >= y && E.y <= y + Size)
            {

                int i = E.x - x;
                int j = E.y - y;

                E.x = x + j;
                E.y = y + Size - i;

            }
        }
    }


}

int main() {
    Input();
    while (K--)
    {

        for (int i = 0; i < Vector.size(); i++)
        {
            Exit(Vector[i]);

            if (Vector[i].x == Ex && Vector[i].y == Ey)
            {
                Vector.erase(Vector.begin() + i);
                i--;
                continue;
            }

            Square_Data(Vector[i].x, Vector[i].y);
        }


        if (Vector.size() == 0) break;


        sort(Data.begin(), Data.end(), Cmp);
        Rotation(Data[0].x, Data[0].y, Data[0].Size);
        Data.clear();

    }

    cout << Ans << '\n';
    cout << Ex << ' ' << Ey << '\n';
}