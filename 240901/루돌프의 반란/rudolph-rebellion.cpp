#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<climits>
#include<cmath>
#define MAX INT_MAX
using namespace std;


int n, m, p, c, d;
int ry, rx;
int dead_cnt;
// 상,우,하,좌 대각
int dy[8] = { -1,0,1,0,1,1,-1,-1 };
int dx[8] = { 0,1,0,-1,1,-1,-1,1 };
int map[52][52];
struct Info {
	int y, x, score, stun_day;
	bool dead;
};

struct Pos {
	int y, x, dis;
};

struct Cmp {
	bool operator()(Pos a, Pos b) {
		if (a.dis != b.dis) {
			return a.dis > b.dis;
		}
		else if (a.y != b.y) {
			return a.y < b.y;
		}
		else {
			return a.x < b.x;
		}
	}
};

vector<Info>santa;

bool isIn(int y, int x) {
	return y >= 1 && x >= 1 && y <= n && x <= n;
}

void interaction(int y, int x, int num, int dir, int kind) {
	// 현재 그 지점으로 튕겨져 날아오는 산타 번호 num
	// 기존에 그 지점에 있던 산타 번호 now_num
	// 그 지점 (y,x)
	int now_num = map[y][x];
	map[y][x] = num;
	santa[num].y = y;
	santa[num].x = x;
	int ny = y;
	int nx = x;
	if (kind == 1) {
		ny -= dy[dir];
		nx -= dx[dir];
	}
	else {
		ny += dy[dir];
		nx += dx[dir];
	}
	if (!isIn(ny, nx)) {
		santa[now_num].dead = true;
		return;
	}
	else {
		if (map[ny][nx] > 0) {
			interaction(ny, nx, now_num, dir, kind);
		}
		else {
			map[ny][nx] = now_num;
			santa[now_num].y = ny;
			santa[now_num].x = nx;
			return;
		}
	}
}

bool func() {

	// 루돌프 이동
	priority_queue<Pos, vector<Pos>, Cmp>rudol_dis;
	for (int i = 1; i <= p; i++) {
		Info now_santa = santa[i];
		if (now_santa.dead)continue;
		int dis = pow(ry - now_santa.y, 2) + pow(rx - now_santa.x, 2);
		rudol_dis.push({ now_santa.y,now_santa.x,dis });
	}

	// 8방향에 대해 이동 시 가장 거리가 가까워지는 지점 찾기
	if (rudol_dis.empty())return false;
	Pos selectedSanta = rudol_dis.top();
	int dis = MAX;
	int rdir = 0;
	int final_ry = 0;
	int final_rx = 0;
	for (int i = 0; i < 8; i++) {
		int temp_y = ry + dy[i];
		int temp_x = rx + dx[i];
		if (!isIn(temp_y, temp_x))continue;
		int temp_dis = pow(temp_y - selectedSanta.y, 2) + pow(temp_x - selectedSanta.x, 2);
		if (dis > temp_dis) {
			final_ry = temp_y;
			final_rx = temp_x;
			dis = temp_dis;
			rdir = i;
		}
	}

	// 루돌프 이동 후 산타가 있는지 확인
	map[ry][rx] = 0;
	// 있으면
	if (map[final_ry][final_rx] > 0) {
		// 튕겨나간 산타의 정보
		int santa_y = final_ry + c * dy[rdir];
		int santa_x = final_rx + c * dx[rdir];
		int santa_num = map[final_ry][final_rx];
		// C 획득및 기절
		santa[santa_num].score += c;
		santa[santa_num].stun_day = 2;
		// 경기장 밖이면 아웃
		if (!isIn(santa_y, santa_x)) {
			santa[santa_num].dead = true;
			dead_cnt++;
			if (dead_cnt == p) {
				return false;
			}
		}

		// 경기장 안이면
		else {
			// 뒤에 산타 상호작용 확인
			if (map[santa_y][santa_x] > 0) {
				// 그 산타 번호 저장
				int temp_santa_num = map[santa_y][santa_x];
				// 현재 산타 착지
				map[santa_y][santa_x] = santa_num;
				santa[santa_num].y = santa_y;
				santa[santa_num].x = santa_x;
				// 그 다음 지점도 확인하여 연쇄작용
				int nny = santa_y + dy[rdir];
				int nnx = santa_x + dx[rdir];
				// 다음 지점이 장외라면 사망 처리 후 진행 x
				if (!isIn(nny, nnx)) {
					santa[temp_santa_num].dead = true;
					dead_cnt++;
					if (dead_cnt == p) {
						return false;
					}
				}
				// 산타가 뒤에 또 있었다면?
				if (map[nny][nnx] > 0) {
					// 상호작용 실시
					interaction(nny, nnx, temp_santa_num, rdir, 2);
				}
				// 없으면 한칸 뒤로 저장 
				else {
					map[nny][nnx] = temp_santa_num;
					santa[temp_santa_num].y = nny;
					santa[temp_santa_num].x = nnx;
				}
			}
			// 산타의 다음 지점에 다른 산타 없으면 그냥 저장
			else {
				map[santa_y][santa_x] = santa_num;
				santa[santa_num].y = santa_y;
				santa[santa_num].x = santa_x;
			}
		}

	}
	// 해당 지점 루돌프 이동
	ry = final_ry;
	rx = final_rx;
	map[ry][rx] = -1;


	// 산타 이동
	for (int i = 1; i <= p; i++) {
		Info now_santa = santa[i];
		// 죽었거나 기절한 산타는 패스
		if (now_santa.dead || now_santa.stun_day > 0)continue;
		int sy = now_santa.y;
		int sx = now_santa.x;
		int sdis = pow(sy - ry, 2) + pow(sx - rx, 2);
		int tempdis = MAX;
		int to_y = 0, to_x = 0;
		int sdir = 0;
		bool can_move = false;
		for (int j = 0; j < 4; j++) {
			int ny = sy + dy[j];
			int nx = sx + dx[j];
			// 게임판 밖 또는 산타가 이미 있는 경우
			if (!isIn(ny, nx) || map[ny][nx] > 0)continue;
			int ndis = pow(ny - ry, 2) + pow(nx - rx, 2);
			// 움직여도 가까워지지 않는 경우
			if (sdis <= ndis)continue;
			if (tempdis > ndis) {
				// 상우하좌 순서로 방향 배열 설정
				// 만약 동일한 값이 저장된 경우
				// 갱신 안하므로 상우좌하 순서로 방향 고정되어있음
				to_y = ny;
				to_x = nx;
				sdir = j;
				tempdis = ndis;
				can_move = true;
			}
		}
		// 움직일 수 없거나, 가까워질 수 없다면 이동 x
		if (!can_move)continue;

		// 이동
		map[sy][sx] = 0;
		// 루돌프가 있다면?
		if (map[to_y][to_x] == -1) {
			// D 획득 및 기절
			santa[i].score += d;
			santa[i].stun_day = 2;
			// (to_y,to_x) 기준으로 D만큼 밀려남
			int ny = to_y - d * dy[sdir];
			int nx = to_x - d * dx[sdir];
			// 사망했나?
			if (!isIn(ny, nx)) {

				santa[i].dead = true;
				dead_cnt++;
				if (dead_cnt == p) {
					return false;
				}
				map[sy][sx] = 0;
				continue;
			}
			// 살았다면?
			// 만약 산타가 그 곳에 있으면?
			if (map[ny][nx] > 0) {

				// 그 산타 번호 저장
				int temp_santa_num = map[ny][nx];
				// 현재 산타 착지
				map[sy][sx] = 0;
				map[ny][nx] = i;
				santa[i].y = ny;
				santa[i].x = nx;
				// 그 다음 지점도 확인하여 연쇄작용
				int nny = ny - dy[sdir];
				int nnx = nx - dx[sdir];
				// 다음 지점이 장외라면 사망 처리 후 진행 x
				if (!isIn(nny, nnx)) {
					santa[temp_santa_num].dead = true;
					dead_cnt++;
					if (dead_cnt == p) {
						return false;
					}
					continue;
				}
				// 산타가 뒤에 또 있었다면?
				if (map[nny][nnx] > 0) {
					// 상호작용 실시
					interaction(nny, nnx, temp_santa_num, sdir, 1);
				}
				// 없으면 한칸 뒤로 저장 
				else {
					map[nny][nnx] = temp_santa_num;
					santa[temp_santa_num].y = nny;
					santa[temp_santa_num].x = nnx;
				}

			}
			// 산타가 없으면 그냥 착지
			else {
				map[sy][sx] = 0;
				map[ny][nx] = i;
				santa[i].y = ny;
				santa[i].x = nx;
			}
		}
		// 루돌프가 없다면 그냥 이동
		else {
			map[sy][sx] = 0;
			map[to_y][to_x] = i;
			santa[i].y = to_y;
			santa[i].x = to_x;
		}

	}

	return true;
}

int main() {

	cin >> n >> m >> p >> c >> d;
	santa.resize(p + 1);
	cin >> ry >> rx;
	map[ry][rx] = -1;
	for (int i = 0; i < p; i++) {
		int num, py, px;
		cin >> num >> py >> px;
		santa[num] = { py,px,0,0,false };
		map[py][px] = num;
	}

	for (int i = 0; i < m; i++) {
		for (int j = 1; j <= p; j++) {
			if (santa[j].stun_day > 0) {
				santa[j].stun_day--;
			}
		}
		if (!func()) {
			break;
		}
		for (int j = 1; j <= p; j++) {
			if (!santa[j].dead) {
				santa[j].score++;
			}
		}
	}
	for (int i = 1; i <= p; i++) {
		cout << santa[i].score << " ";
	}

	return 0;
}