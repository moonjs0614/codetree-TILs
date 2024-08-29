#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;

int N, M, K;
string map[11][11];
int min_dist = 100;
int square_len;
int runner_cnt;
int sum = 0;

int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };

struct Point
{
	int y, x;
	int move_time = 0;
	int move_dist = 0;
	int exit_dist = 0;
};

bool cmp(Point a, Point b)
{
	if (a.exit_dist == b.exit_dist)
	{
		if (a.x == b.x)
		{
			return a.y < b.y;
		}
		return a.x < b.x;
	}
	return a.exit_dist < b.exit_dist;
};

Point runners[10];
Point Exit;

vector<Point> square;

void input()
{
	cin >> N >> M >> K;
	
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			cin >> map[i][j];
		}
	}

	int y, x;
	for (int i = 0; i < M; i++)
	{
		cin >> y >> x;
		map[y][x] = "A";
		runners[i] = { y, x };
	}
	cin >> y >> x;
	map[y][x] = "E";
	Exit = { y, x };
}

void move(int time)
{
	for (int i = 0; i < M; i++)
	{
		Point runner = runners[i];
		
		// 이번에 이동할 차례가 아니면 패스
		if (runner.move_time != time) continue;

		runner.exit_dist = abs(Exit.y - runner.y) + abs(Exit.x - runner.x);
		bool flag = false;
		bool escape = false;
		Point next = { 0, 0, runner.move_time, runner.move_dist, runner.move_time };

		// 이동 가능한 칸 탐색
		for (int j = 0; j < 4; j++)
		{
			next.y = runner.y + dy[j];
			next.x = runner.x + dx[j];

			// 다음이 출구면
			if (map[next.y][next.x] == "E")
			{
				map[runner.y][runner.x] = "0";
				escape = true;
				runner.move_dist++;
				runner.move_time = 1000;
				runner_cnt--;
				sum++;
				break;
			}

			next.exit_dist = abs(Exit.y - next.y) + abs(Exit.x - next.x);

			if (next.y < 1 || next.y > N || next.x < 1 || next.x > N) continue;
			if (map[next.y][next.x] != "0") continue;
			if (runner.exit_dist <= next.exit_dist) continue;

			flag = true;
			break;
		}

		// 탈출했으면 패스
		if (escape) continue;

		// 이동 가능한 칸이 존재하면
		if (flag)
		{
			map[runner.y][runner.x] = "0";
			if (map[next.y][next.x] == "0")
			{
				map[next.y][next.x] = "A";
			}
			else
			{
				map[next.y][next.x] += "A";
			}
			next.move_dist++;
			next.move_time++;
			runners[i] = next;
			sum++;
			
			min_dist = min(min_dist, next.exit_dist); // 출구와의 최소거리 갱신
		}
		else
		{
			runners[i] = runner;
			runners[i].move_time++;
			
			min_dist = min(min_dist, runner.exit_dist); // 출구와의 최소거리 갱신
		}
	}
}

Point make_square(int time)
{
	for (int i = 0; i < M; i++)
	{
		Point runner = runners[i];

		// 출구와 최소거리만큼 떨어져 있으면 정사각형 후보군
		if (runner.exit_dist == min_dist && runner.move_time == time + 1)
		{
			int y_gap = abs(Exit.y - runner.y);
			int x_gap = abs(Exit.x - runner.x);
			int size = max(y_gap, x_gap);

			int max_y = max(Exit.y, runner.y) - size;
			if (max_y < 1) max_y = 1;

			int max_x = max(Exit.x, runner.x) - size;
			if (max_x < 1) max_x = 1;

			square.push_back({ max_y, max_x, 0, 0, size });
		}
	}

	sort(square.begin(), square.end(), cmp);

	return square[0];
}

void rotate(Point st)
{
	square_len = st.exit_dist;
	Point end = { st.y + square_len, st.x + square_len };
	string temp_map[11][11];

	for (int i = 0; i <= square_len; i++)
	{
		for (int j = 0; j <= square_len; j++)
		{
			temp_map[st.y + j][st.x + square_len - i] = map[st.y + i][st.x + j];
			
			if (temp_map[st.y + j][st.x + square_len - i] >= "1" && temp_map[st.y + j][st.x + square_len - i] <= "9")
			{
				int num = stoi(temp_map[st.y + j][st.x + square_len - i]) - 1;
				temp_map[st.y + j][st.x + square_len - i] = to_string(num);
			}
		}
	}

	for (int i = st.y; i <= st.y + square_len; i++)
	{
		for (int j = st.x; j <= st.x + square_len; j++)
		{
			map[i][j] = temp_map[i][j];
		}
	}

	for (int i = 0; i < M; i++)
	{
		Point runner = runners[i];

		// 회전하는 사각형 범위 내에 있는 참가자 좌표 회전
		if ((runner.y >= st.y && runner.y <= end.y) && (runner.x >= st.x && runner.x <= end.x))
		{
			int ny, nx;
			int y_to_start = runner.y - st.y;
			nx = end.x - y_to_start;

			int x_to_start = runner.x - st.x;
			ny = st.y + x_to_start;

			runner.y = ny;
			runner.x = nx;

			runners[i] = runner;
		}
	}
	// 출구 회전
	int ny, nx;
	int y_to_start = Exit.y - st.y;
	nx = end.x - y_to_start;

	int x_to_start = Exit.x - st.x;
	ny = st.y + x_to_start;

	Exit.y = ny;
	Exit.x = nx;
}

int cal()
{
	int sum = 0;

	for (int i = 0; i < M; i++)
	{
		sum += runners[i].move_dist;
	}


	return sum;
}

void solution()
{
	runner_cnt = M;

	for (int i = 0; i < K; i++)
	{
		move(i);
		Point rot = make_square(i);
		square.clear();
		rotate(rot);

		min_dist = 100;

		if (runner_cnt == 0) break;
	}

	cout << sum << endl;
	cout << Exit.y << " " << Exit.x << endl;
}

int main() {
	//freopen("sample_input.txt", "r", stdin);
	input();

	solution();
}