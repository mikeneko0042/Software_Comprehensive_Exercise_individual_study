#include <bits/stdc++.h>

using namespace std;

int W, H, N;

vector<vector<bool>> is_obstacle;

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

vector<vector<int>> get_distances(int start_x, int start_y) {
    // 距離を -1で初期化
    vector<vector<int>> dist(W, vector<int>(H, -1));
    queue<pair<int, int>> q;

    dist[start_x][start_y] = 0;
    q.push({start_x, start_y});

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
                // 障害物がなく、まだ訪れていないマスなら進む
                if (!is_obstacle[nx][ny] && dist[nx][ny] == -1) {
                    dist[nx][ny] = dist[cx][cy] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }
    return dist;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> W >> H >> N)) return 0;

    is_obstacle.assign(W, vector<bool>(H, false));

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            if (y == 0 && x != 1 && x != W - 2) {
                is_obstacle[x][y] = true;
            }
            if (y == H - 1 && (x == 0 || x == W - 1)) {
                is_obstacle[x][y] = true;
            }
        }
    }

    map<string, pair<int, int>> product_targets;

    // 商品と棚の配置を設定
    for (int i = 0; i < N; ++i) {
        int x, y;
        string s;
        char d;
        cin >> x >> y >> s >> d;

        is_obstacle[x][y] = true;

        // 商品を取るための隣接座標を計算
        int tx = x, ty = y;
        if (d == 'E') tx += 1;
        else if (d == 'W') tx -= 1;
        else if (d == 'N') ty += 1;
        else if (d == 'S') ty -= 1;

        product_targets[s] = {tx, ty};
    }

    // 入口と出口からの全マスの最短距離をあらかじめ計算しておく
    auto dist_from_ent = get_distances(1, 0);       // 入口(1,0) からの距離 
    auto dist_from_ext = get_distances(W - 2, 0);   // 出口(W-2,0) からの距離 

    int Q;
    cin >> Q;

    for (int i = 0; i < Q; ++i) {
        int m;
        string p;
        cin >> m >> p; 

        auto [tx, ty] = product_targets[p];

        // 総移動距離 
        int total_dist = dist_from_ent[tx][ty] + dist_from_ext[tx][ty];
        cout << total_dist << "\n";
    }

    return 0;
}