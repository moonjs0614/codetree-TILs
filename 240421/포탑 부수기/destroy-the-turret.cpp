#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

int N, M, K;
int dy[8] = { 0, 1, 0, -1, -1, 1, 1, -1 };
int dx[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
struct Turret
{
    int y, x;
    int power = -1;
    int attack_time = 0;
};
bool cmp_attacker(Turret a, Turret b)
{
    if (a.attack_time > b.attack_time)
    {
        return true;
    }
    else if (a.attack_time == b.attack_time)
    {
        if ((a.y + a.x) > (b.y + b.x))
        {
            return true;
        }
        else if ((a.y + a.x) == (b.y + b.x))
        {
            if (a.y > b.y)
            {
                return true;
            }
        }
    }
    return false;
}
bool cmp_target(Turret a, Turret b)
{
    if (a.attack_time < b.attack_time)
    {
        return true;
    }
    else if (a.attack_time == b.attack_time)
    {
        if ((a.y + a.x) < (b.y + b.x))
        {
            return true;
        }
        else if ((a.y + a.x) == (b.y + b.x))
        {
            if (a.y < b.y)
            {
                return true;
            }
        }
    }
    return false;
}
Turret map[11][11];
int laser_flag = 0;
int visited[11][11];
vector<Turret> path;

void input()
{
    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            Turret t;
            t.y = i;
            t.x = j;
            cin >> t.power;
            map[i][j] = t;
        }
    }
}

Turret select_attacker() // 공격자 탐색
{
    Turret min_t;
    int min_power = 10000;
    vector<Turret> v; // 동일한 최저값을 가진 포탑들을 담는 벡터

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            Turret t = map[i][j];
            if (t.power == 0) continue;
            if (t.power < min_power)
            {
                min_power = t.power; // 최저값 갱신

                // 최저값이 바뀌었으니 벡터도 수정. 
                if (v.size()) // 만약 기존에 기록된 최저값 포탑들이 있다면
                {
                    v.clear(); // 벡터 비우기
                }
                v.push_back(t); // 벡터에 최저값 포탑 담기
            }
            else if (t.power == min_power)
            {
                v.push_back(t); // 벡터에 최저값 포탑 담기
            }
        }
    }

    if (v.size() == 1) // 맵을 다 돌았을 때 최저값 포탑이 하나라면
    {
        min_t = v[0]; // 최저값 포탑 = v[0]
        map[min_t.y][min_t.x].power += (N + M);
        min_t.power += (N + M);
        return min_t; // 최저값 포탑 리턴
    }
    else // 최저값 포탑이 여러개면
    {
        sort(v.begin(), v.end(), cmp_attacker);
        min_t = v[0]; // 최저값 포탑 = v[0]
        map[min_t.y][min_t.x].power += (N + M);
        min_t.power += (N + M);
        return min_t; // 최저값 포탑 리턴
    }
}

Turret select_target(Turret attacker) // 공격 대상 탐색
{
    Turret max_t;
    int max_power = -1;
    vector<Turret> v; // 동일한 최대값을 가진 포탑들을 담는 벡터

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            Turret t = map[i][j];
            if (t.power == 0) continue;
            if (t.y == attacker.y && t.x == attacker.x) continue;
            if (t.power > max_power)
            {
                max_power = t.power; // 최대값 갱신

                // 최대값이 바뀌었으니 벡터도 수정.
                if (v.size()) // 만약 기존에 기록된 최저값 포탑들이 있다면
                {
                    v.clear(); // 벡터 비우기
                }
                v.push_back(t); // 벡터에 최대값 포탑 담기
            }
            else if (t.power == max_power)
            {
                v.push_back(t); // 벡터에 최대값 포탑 담기
            }
        }
    }

    if (v.size() == 1) // 맵을 다 돌았을 때 최대값 포탑이 하나라면
    {
        max_t = v[0];
        return max_t;
    }
    else // 최대값 포탑이 여러개면
    {
        sort(v.begin(), v.end(), cmp_target);
        return v[0];
    }
}

bool laser(Turret attacker, Turret target) // 레이저 공격 BFS
{
    int visited[11][11] = { 0, };
    queue<Turret> q;
    unordered_map<int, Turret> parent;
    int flag = 0;

    q.push(attacker);

    while (!q.empty())
    {
        Turret now = q.front();
        q.pop();
        visited[now.y][now.x] = 1;

        if (now.y == target.y && now.x == target.x)
        {
            flag = 1;
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            int ny = now.y + dy[i];
            int nx = now.x + dx[i];

            if (ny < 1) ny = N;
            if (ny > N) ny = 1;
            if (nx < 1) nx = M;
            if (nx > M) nx = 1;

            if (visited[ny][nx] == 1) continue;
            if (map[ny][nx].power == 0) continue;

            visited[ny][nx] = 1;
            int idx = ny * M - (M - nx);
            parent[idx] = now;
            q.push(map[ny][nx]);
        }
    }

    if (flag)
    {
        // 목적지까지의 최단경로 역추적하여 저장
        Turret now = target;
        while (now.y != attacker.y || now.x != attacker.x)
        {
            path.push_back(now);
            int idx = now.y * M - (M - now.x);
            now = parent[idx];
        }

        //역순으로 저장된 경로를 뒤집어서 출발지 ~ 목적지 최단경로 얻음
        reverse(path.begin(), path.end());

        return true;
    }
    else
    {
        return false;
    }
}

void shell(Turret attacker, Turret target) // 포탄 공격
{
    for (int i = 0; i < 8; i++) // target 주변 포탑
    {
        int ny = target.y + dy[i];
        int nx = target.x + dx[i];

        if (ny > N) ny = 1; // 레이저가 맵 최하단을 넘어가면 최상단으로 이동
        if (ny < 1) ny = N; // 레이저가 맵 최상단을 넘어가면 최하단으로 이동
        if (nx > M) nx = 1; // 좌우도 마찬가지
        if (nx < 1) nx = M;

        if (ny == attacker.y && nx == attacker.x) continue; // 공격자면 제외
        path.push_back(map[ny][nx]);
        if (map[ny][nx].power == 0) continue; // 이미 부서진 포탑이면 제외
        map[ny][nx].power -= attacker.power / 2;
    }

    // target 포탑
    map[target.y][target.x].power -= attacker.power;
}

void fracture() // 부서짐 처리
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            Turret now = map[i][j];
            if (now.power <= 0)
            {
                map[i][j].power = 0;
            }
        }
    }
}

void maintenance_laser(Turret attacker) // 레이저 공격 성공시 포탑 정비
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            int flag = 0;
            if (map[i][j].power == 0) continue; // 부서진 포탑이면 continue
            if (i == attacker.y && j == attacker.x) continue; // 공격자이면 continue

            for (int k = 0; k < path.size(); k++)
            {
                Turret now = path[k];
                if (i == path[k].y && j == path[k].x) // 경로상의 포탑이면
                {
                    flag = 1;
                    break;
                }
            }

            if (flag == 0)
            {
                map[i][j].power++;
            }
        }
    }
}

void maintenance_shell(Turret attacker, Turret target) // 포탄 공격 후 포탑 정비
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            int flag = 1;
            Turret now = map[i][j];
            if (map[now.y][now.x].power == 0) continue;
            if (now.y == attacker.y && now.x == attacker.x) continue;
            if (now.y == target.y && now.x == target.x)  continue;
            
            for (int k = 0; k < path.size(); k++)
            {
                if (now.y == path[k].y && now.x == path[k].x)
                {
                    flag = 0;
                    break;
                }
            }

            if (flag)
            {
                map[now.y][now.x].power++;
            }
        }
    }
}

int check_remain()
{
    int cnt = 0;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            if (map[i][j].power > 0) cnt++;
        }
    }
    return cnt;
}

void solution()
{
    for (int action = 1; action <= K; action++) // K만큼 반복
    {

        path.clear(); // 공격 경로 초기화
        Turret attacker = select_attacker(); // 공격자 탐색
        Turret target = select_target(attacker);// 공격 대상 탐색

        if (action == 2)
        {
            int de = -1;
        }

        if (!laser(attacker, target)) // 레이저 공격이 실패했다면
        {
            shell(attacker, target); // 포탄 공격
            map[attacker.y][attacker.x].attack_time = action; // 공격자의 공격 시간을 현재로 갱신
            fracture(); // 부서짐 처리. 0보다 작거나 같으면 전부 0으로 맞춰줌
            maintenance_shell(attacker, target); // 포탑 정비
        }
        else // 레이저 공격이 성공했다면
        {
            // 공격 처리
            for (int i = 0; i < path.size(); i++)
            {
                Turret now = path[i];
                if (now.y == target.y && now.x == target.x)
                {
                    map[target.y][target.x].power -= attacker.power;
                    continue;
                }
                map[now.y][now.x].power -= (attacker.power / 2);
            }
            map[attacker.y][attacker.x].attack_time = action; // 공격자의 공격 시간을 현재로 갱신

            fracture(); // 부서짐 처리. 0보다 작거나 같으면 전부 0으로 맞춰줌
            maintenance_laser(attacker); // 포탑 정비
        }

        int remain = check_remain();
        if (remain == 1)
        {
            break;
        }
    }
}

Turret find_max()
{
    int max_power = 0;
    Turret strongest;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            if (map[i][j].power > max_power)
            {
                max_power = map[i][j].power;
                strongest = map[i][j];
            }
        }
    }

    return strongest;
}

int main() {
    //freopen("sample_input.txt", "r", stdin);
    input();

    solution();

    Turret answer = find_max();

    cout << answer.power << endl;
}