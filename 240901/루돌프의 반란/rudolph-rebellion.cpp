#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<climits>
#include<cmath>
using namespace std;

int N, M, P, C, D;
int dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

struct Santa
{
	int num;
	int y;
	int x;
	int dist;
	int point = 0;
	int fainting = -1;
	int dir;
	bool survive = true;
};

int map[52][52] = { 0, };
vector<Santa> v;

struct Rudolph
{
	int y, x;
	int dir;
};

Rudolph r;

bool cmp(Santa a, Santa b)
{
    if (a.survive == b.survive)
    {
        if (a.dist == b.dist)
        {
            if (a.y == b.y)
            {
                return a.x > b.x;
            }
            return a.y > b.y;
        }
        return a.dist < b.dist;
    }
    return a.survive > b.survive;
}

bool cmp_num(Santa a, Santa b)
{
    return a.num < b.num;
}

// 루돌프-산타 거리 계산
int cal_distance(Rudolph r, Santa s)
{
    return pow((r.y - s.y), 2) + pow((r.x - s.x), 2);
}

void input()
{
    cin >> N >> M >> P >> C >> D;
    cin >> r.y >> r.x;

    for (int i = 1; i <= P; i++)
    {
        Santa s;
        cin >> s.num >> s.y >> s.x;
        s.dist = cal_distance(r, s);
        map[s.y][s.x] = s.num;

        v.push_back(s);
    }
}

// 상호작용. a : 굴러온 돌, b : 박힌 돌
void interaction(Santa& a, Santa& b)
{
    b.dir = a.dir;
    b.y += dy[b.dir];
    b.x += dx[b.dir];

    if (b.y < 1 || b.x < 1 || b.y > N || b.x > N)
    {
        b.survive = false;
    }

    map[a.y][a.x] = a.num;

    // 연쇄 상호작용
    if (map[b.y][b.x] > 0)
    {
        // 해당 산타 찾기
        for (int i = 0; i < P; i++)
        {
            if (v[i].y == b.y && v[i].x == b.x)
            {
                interaction(b, v[i]);
                break;
            }
        }
    }

    map[b.y][b.x] = b.num;
}

// 충돌
// mover : 움직인 주체. 루돌프면 1, 산타면 2
void collision(int mover, Rudolph r, Santa& s)
{
    if (mover == 1)
    {
        s.point += C;
        s.y += dy[r.dir] * C;
        s.x += dx[r.dir] * C;
        s.dir = r.dir;
        s.fainting = 2;

        if (s.y < 1 || s.x < 1 || s.y > N || s.x > N)
        {
            s.survive = false;
            map[s.y][s.x] = 0;
        }
    }
    else if (mover == 2)
    {
        s.point += D;
        map[s.y][s.x] = 0;
        s.y += dy[(s.dir + 4) % 8] * D;
        s.x += dx[(s.dir + 4) % 8] * D;
        s.dir = (s.dir + 4) % 8;
        s.fainting = 2;

        if (s.y < 1 || s.x < 1 || s.y > N || s.x > N)
        {
            s.survive = false;
        }
    }

    // 밀려난 위치에 다른 산타가 있으면
    if (map[s.y][s.x] > 0)
    {
        // 해당 산타 찾기
        for (int i = 0; i < P; i++)
        {
            if (v[i].y == s.y && v[i].x == s.x && v[i].survive == true && v[i].num != s.num)
            {
                interaction(s, v[i]);
                break;
            }
        }
    }

    if (s.survive == true) map[s.y][s.x] = s.num;
}

void move_rudolph(Santa& target)
{
    int ny;
    int nx;
    int dir;
    int new_dist = 10000;

    for (int i = 0; i < 8; i++)
    {
        Rudolph temp;
        temp.y = r.y + dy[i];
        temp.x = r.x + dx[i];
        int temp_dist = cal_distance(temp, target);

        if (temp.y < 1 || temp.x < 1 || temp.y > N || temp.x > N) continue;

        if (new_dist > temp_dist)
        {
            new_dist = temp_dist;
            ny = temp.y;
            nx = temp.x;
            dir = i;
        }
    }

    r.y = ny;
    r.x = nx;
    r.dir = dir;

    if (map[r.y][r.x] > 0)
    {
        map[r.y][r.x] = 0;
        collision(1, r, target);
    }
}

void move_santa(Santa& s)
{
    int ny;
    int nx;
    int dir;
    int new_dist = 10000;
    bool can_move = false;

    for (int i = 0; i < 8; i += 2)
    {
        Santa temp;
        temp.y = s.y + dy[i];
        temp.x = s.x + dx[i];
        int temp_dist = cal_distance(r, temp);

        if (temp.y < 1 || temp.x < 1 || temp.y > N || temp.x > N) continue;
        if (map[temp.y][temp.x] > 0) continue;
        if (s.dist <= temp_dist) continue;

        if (new_dist > temp_dist)
        {
            new_dist = temp_dist;
            ny = temp.y;
            nx = temp.x;
            dir = i;
            can_move = true;
        }
    }

    if (can_move == false) return;

    map[s.y][s.x] = 0;
    s.y = ny;
    s.x = nx;
    s.dist = new_dist;
    s.dir = dir;
    map[s.y][s.x] = s.num;

    if (s.y == r.y && s.x == r.x)
    {
        map[s.y][s.x] = 0;
        collision(2, r, s);
    }
}

// 출력
void print()
{
    sort(v.begin(), v.end(), cmp_num);
    for (int i = 0; i < P; i++)
    {
        cout << v[i].point << " ";
    }
}

void solution()
{
    for (int i = 1; i <= M; i++)
    {
        if (i == 10)
        {
            int de = -1;
        }

        int cnt = 0;
        sort(v.begin(), v.end(), cmp);
        
        move_rudolph(v[0]);

        sort(v.begin(), v.end(), cmp_num);

        for (int i = 0; i < P; i++)
        {
            if (v[i].survive == false) continue;
            if (v[i].fainting > 0) continue;

            v[i].dist = cal_distance(r, v[i]);

            move_santa(v[i]);
        }

        for (int i = 0; i < P; i++)
        {
            v[i].fainting--;
            v[i].dist = cal_distance(r, v[i]);

            if (v[i].survive == false) cnt++;
            else v[i].point++;
        }

        if (cnt == P) break;
    }

    print();
}

int main() {
    //freopen("sample_input.txt", "r", stdin);
    input();

    solution();

    return 0;
}