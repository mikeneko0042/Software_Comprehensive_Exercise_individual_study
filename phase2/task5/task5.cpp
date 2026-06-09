#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9;

int W, H, N;
vector<vector<bool>> is_obstacle;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

int main() {
     auto start_time = chrono::high_resolution_clock::now();

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> W >> H >> N)) return 0;

    is_obstacle.assign(W, vector<bool>(H, false));

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            if (y == 0 && x != 1 && x != W - 2) is_obstacle[x][y] = true;
            if (y == H - 1 && (x == 0 || x == W - 1)) is_obstacle[x][y] = true;
        }
    }

    
    vector<pair<int, int>> nodes(N + 2);
    nodes[0] = {1, 0};
    nodes[1] = {W - 2, 0};

    map<string, int> product_to_node;

    for (int i = 0; i < N; ++i) {
        int x, y;
        string s;
        char d;
        cin >> x >> y >> s >> d;

        is_obstacle[x][y] = true;

     
        int tx = x, ty = y;
        if (d == 'E') tx += 1;
        else if (d == 'W') tx -= 1;
        else if (d == 'N') ty += 1;
        else if (d == 'S') ty -= 1;

        nodes[i + 2] = {tx, ty};
        product_to_node[s] = i + 2;
    }

    // dist_matrix[u][v]:ノードuからノードvへの最短距離
    vector<vector<int>> dist_matrix(N + 2, vector<int>(N + 2, INF));

    for (int i = 0; i < N + 2; ++i) {
        int sx = nodes[i].first;
        int sy = nodes[i].second;

        if (sx < 0 || sx >= W || sy < 0 || sy >= H || is_obstacle[sx][sy]) {
            continue;
        }

        vector<vector<int>> d(W, vector<int>(H, INF));
        queue<pair<int, int>> q;

        d[sx][sy] = 0;
        q.push({sx, sy});

        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();

            for (int dir = 0; dir < 4; ++dir) {
                int nx = cx + dx[dir];
                int ny = cy + dy[dir];

                if (nx >= 0 && nx < W && ny >= 0 && ny < H && !is_obstacle[nx][ny] && d[nx][ny] == INF) {
                    d[nx][ny] = d[cx][cy] + 1;
                    q.push({nx, ny});
                }
            }
        }

        // 計算した距離を他のノードのターゲット座標から抽出し、距離行列に格納
        for (int j = 0; j < N + 2; ++j) {
            int tx = nodes[j].first;
            int ty = nodes[j].second;
            if (tx >= 0 && tx < W && ty >= 0 && ty < H) {
                dist_matrix[i][j] = d[tx][ty];
            }
        }
    }

    // bitDP
    int Q;
    cin >> Q;

    while (Q--) {
        int M;
        cin >> M;
        vector<int> req(M);
        for (int i = 0; i < M; ++i) {
            string p;
            cin >> p;
            req[i] = product_to_node[p]; 
        }

        // dp[S][i]:訪問済みの商品の集合がS、最後に訪問した商品が req[i] であるときの最短距離
        vector<vector<int>> dp(1 << M, vector<int>(M, INF));

        for (int i = 0; i < M; ++i) {
            if (dist_matrix[0][req[i]] != INF) {
                dp[1 << i][i] = dist_matrix[0][req[i]];
            }
        }

        for (int mask = 1; mask < (1 << M); ++mask) {
            for (int i = 0; i < M; ++i) {
                
                if (!(mask & (1 << i)) || dp[mask][i] == INF) continue;

                for (int j = 0; j < M; ++j) {
                    
                    if (mask & (1 << j)) continue;

                    int cost = dist_matrix[req[i]][req[j]];
                    if (cost != INF && dp[mask][i] + cost < dp[mask | (1 << j)][j]) {
                        dp[mask | (1 << j)][j] = dp[mask][i] + cost;
                    }
                }
            }
        }

        int min_total_dist = INF;
        int full_mask = (1 << M) - 1;

        for (int i = 0; i < M; ++i) {
            if (dp[full_mask][i] != INF && dist_matrix[req[i]][1] != INF) {
                min_total_dist = min(min_total_dist, dp[full_mask][i] + dist_matrix[req[i]][1]);
            }
        }

        cout << min_total_dist << "\n";
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "\ntime: " << elapsed.count() << " seconds\n";

    return 0;
}