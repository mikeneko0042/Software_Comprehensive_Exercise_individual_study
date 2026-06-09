#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>

using namespace std;

const int INF = 1e9;
const int MAX_M = 20;

const int SHIFT = 5; 

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

vector<int> dp_table(1 << (MAX_M + SHIFT));

int main() {
    auto start_time = chrono::high_resolution_clock::now();

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int W, H, N;
    if (!(cin >> W >> H >> N)) return 0;

    vector<bool> is_obstacle(W * H, false);
    auto idx = [&](int x, int y) { return x * H + y; };

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            if (y == 0 && x != 1 && x != W - 2) is_obstacle[idx(x, y)] = true;
            if (y == H - 1 && (x == 0 || x == W - 1)) is_obstacle[idx(x, y)] = true;
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

        is_obstacle[idx(x, y)] = true;

        int tx = x, ty = y;
        if (d == 'E') tx += 1;
        else if (d == 'W') tx -= 1;
        else if (d == 'N') ty += 1;
        else if (d == 'S') ty -= 1;

        nodes[i + 2] = {tx, ty};
        product_to_node[s] = i + 2;
    }

    vector<vector<int>> dist_matrix(N + 2, vector<int>(N + 2, INF));
    vector<int> d(W * H); 

    for (int i = 0; i < N + 2; ++i) {
        int sx = nodes[i].first;
        int sy = nodes[i].second;

        if (sx < 0 || sx >= W || sy < 0 || sy >= H || is_obstacle[idx(sx, sy)]) continue;

        fill(d.begin(), d.end(), INF);
        queue<pair<int, int>> q;

        d[idx(sx, sy)] = 0;
        q.push({sx, sy});

        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();
            
            int current_d = d[idx(cx, cy)];

            for (int dir = 0; dir < 4; ++dir) {
                int nx = cx + dx[dir];
                int ny = cy + dy[dir];

                if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
                    int nidx = idx(nx, ny);
                    if (!is_obstacle[nidx] && d[nidx] == INF) {
                        d[nidx] = current_d + 1;
                        q.push({nx, ny});
                    }
                }
            }
        }

        for (int j = 0; j < N + 2; ++j) {
            int tx = nodes[j].first;
            int ty = nodes[j].second;
            if (tx >= 0 && tx < W && ty >= 0 && ty < H) {
                dist_matrix[i][j] = d[idx(tx, ty)];
            }
        }
    }

    int Q;
    cin >> Q;

    // クエリ結果のキャッシュ
    map<vector<int>, int> query_cache;

    while (Q--) {
        int M;
        cin >> M;
        vector<int> req(M);
        for (int i = 0; i < M; ++i) {
            string p;
            cin >> p;
            req[i] = product_to_node[p]; 
        }

        vector<int> sorted_req = req;
        sort(sorted_req.begin(), sorted_req.end());

        if (query_cache.count(sorted_req)) {
            cout << query_cache[sorted_req] << "\n";
            continue;
        }

        req = sorted_req;

        int limit_mask = 1 << M;
        for (int mask = 0; mask < limit_mask; ++mask) {
            for (int i = 0; i < M; ++i) {
                dp_table[(mask << SHIFT) | i] = INF;
            }
        }

        for (int i = 0; i < M; ++i) {
            int dist_start = dist_matrix[0][req[i]];
            if (dist_start != INF) {
                dp_table[(1 << SHIFT << i) | i] = dist_start; 
            }
        }

        for (int mask = 1; mask < limit_mask; ++mask) {
            int current_mask = mask;
            while (current_mask > 0) {
                int i = __builtin_ctz(current_mask);
                current_mask &= current_mask - 1;

                int current_cost = dp_table[(mask << SHIFT) | i];
                if (current_cost == INF) continue;

                int not_visited = (limit_mask - 1) & ~mask;
                while (not_visited > 0) {
                    int j = __builtin_ctz(not_visited);
                    not_visited &= not_visited - 1;

                    int cost = dist_matrix[req[i]][req[j]];
                    if (cost != INF) {
                        int next_mask_shifted = (mask | (1 << j)) << SHIFT;
                        if (current_cost + cost < dp_table[next_mask_shifted | j]) {
                            dp_table[next_mask_shifted | j] = current_cost + cost;
                        }
                    }
                }
            }
        }

        int min_total_dist = INF;
        int full_mask_shifted = (limit_mask - 1) << SHIFT;

        for (int i = 0; i < M; ++i) {
            int cost = dp_table[full_mask_shifted | i];
            if (cost != INF && dist_matrix[req[i]][1] != INF) {
                min_total_dist = min(min_total_dist, cost + dist_matrix[req[i]][1]);
            }
        }

        // 結果をキャッシュに保存して出力
        query_cache[req] = min_total_dist;
        cout << min_total_dist << "\n";
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "\ntime: " << elapsed.count() << " seconds\n";

    return 0;
}