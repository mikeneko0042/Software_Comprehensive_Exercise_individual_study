#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;

const int INF = 1e9;

struct Product {
    int x, y;
    string name;
    char dir;
};

struct POI {
    int x, y;
    int mask;
};

struct Edge {
    int to;
    int weight;
};

int main() {
    auto start_time = chrono::high_resolution_clock::now();

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int W, H, N;
    if (!(cin >> W >> H >> N)) return 0;

    vector<Product> prods(N);
    bool obstacle[105][105] = {false};

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            if ((x == 0 && y == 0) || (x == W - 1 && y == 0) ||
                (x == 0 && y == H - 1) || (x == W - 1 && y == H - 1)) {
                obstacle[x][y] = true;
            }
            if (y == 0 && x != 1 && x != W - 2) {
                obstacle[x][y] = true;
            }
        }
    }

    int dx[256] = {0}, dy[256] = {0};
    dx['E'] = 1; dy['E'] = 0;
    dx['W'] = -1; dy['W'] = 0;
    dx['N'] = 0; dy['N'] = 1;
    dx['S'] = 0; dy['S'] = -1;

    map<string, int> name_to_id;
    for (int i = 0; i < N; ++i) {
        cin >> prods[i].x >> prods[i].y >> prods[i].name >> prods[i].dir;
        obstacle[prods[i].x][prods[i].y] = true; 
        name_to_id[prods[i].name] = i;
    }

    map<pair<int, int>, int> poi_map;
    poi_map[{1, 0}] |= 0;       // Entrance
    poi_map[{W - 2, 0}] |= 0;   // Exit

    for (int i = 0; i < N; ++i) {
        int ax = prods[i].x + dx[(int)prods[i].dir];
        int ay = prods[i].y + dy[(int)prods[i].dir];
        if (ax >= 0 && ax < W && ay >= 0 && ay < H && !obstacle[ax][ay]) {
            poi_map[{ax, ay}] |= (1 << i);
        }
    }

    vector<POI> P;
    int idx_EN = -1, idx_EX = -1;
    for (auto const& [coord, mask] : poi_map) {
        if (coord.first == 1 && coord.second == 0) idx_EN = P.size();
        if (coord.first == W - 2 && coord.second == 0) idx_EX = P.size();
        P.push_back({coord.first, coord.second, mask});
    }

    int K = P.size();
    vector<vector<int>> dist(K, vector<int>(K, INF));
    int move_x[] = {1, -1, 0, 0};
    int move_y[] = {0, 0, 1, -1};

    for (int i = 0; i < K; ++i) {
        vector<vector<int>> d(W, vector<int>(H, INF));
        queue<pair<int, int>> q;
        d[P[i].x][P[i].y] = 0;
        q.push({P[i].x, P[i].y});

        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();

            for (int dir = 0; dir < 4; ++dir) {
                int nx = cx + move_x[dir];
                int ny = cy + move_y[dir];
                if (nx >= 0 && nx < W && ny >= 0 && ny < H && !obstacle[nx][ny]) {
                    if (d[nx][ny] == INF) {
                        d[nx][ny] = d[cx][cy] + 1;
                        q.push({nx, ny});
                    }
                }
            }
        }
        for (int j = 0; j < K; ++j) {
            dist[i][j] = d[P[j].x][P[j].y];
        }
    }

    vector<vector<Edge>> adj(K);
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            if (i != j && dist[i][j] != INF) {
                adj[i].push_back({j, dist[i][j]});
            }
        }
    }

    int num_states = (1 << N) * K;
    vector<int> dp(num_states, INF);
    dp[P[idx_EN].mask * K + idx_EN] = 0;

    for (int mask = 0; mask < (1 << N); ++mask) {
        for (int u = 0; u < K; ++u) {
            int current_dist = dp[mask * K + u];
            if (current_dist == INF) continue;

            for (const auto& edge : adj[u]) {
                int v = edge.to;
                int w = edge.weight;
                int next_mask = mask | P[v].mask;
                if (next_mask == mask) continue;

                int& next_dist = dp[next_mask * K + v];
                if (current_dist + w < next_dist) {
                    next_dist = current_dist + w;
                }
            }
        }
    }

    int Q;
    if (!(cin >> Q)) return 0;
    while (Q--) {
        int M;
        cin >> M;
        int req_mask = 0;
        for (int i = 0; i < M; ++i) {
            string s;
            cin >> s;
            if (name_to_id.count(s)) {
                req_mask |= (1 << name_to_id[s]);
            }
        }

        int min_dist = INF;
        int max_bits = -1;
        string best_str = "";
        string best_output = "";

        for (int mask = 0; mask < (1 << N); ++mask) {
            for (int u = 0; u < K; ++u) {
                if (dp[mask * K + u] == INF) continue;
                if (dist[u][idx_EX] == INF) continue;

                int cur_mask = mask | P[idx_EX].mask;
                if ((cur_mask & req_mask) != req_mask) continue; 

                int cur_dist = dp[mask * K + u] + dist[u][idx_EX];
                int bits = __builtin_popcount(cur_mask);

                if (cur_dist < min_dist || (cur_dist == min_dist && bits > max_bits)) {
                    min_dist = cur_dist;
                    max_bits = bits;

                    string cont_str = "", out_str = "";
                    vector<string> items;
                    for (int i = 0; i < N; ++i) {
                        if (cur_mask & (1 << i)) items.push_back(prods[i].name);
                    }
                    sort(items.begin(), items.end());
                    for (size_t i = 0; i < items.size(); ++i) {
                        cont_str += items[i];
                        if (i > 0) out_str += " ";
                        out_str += items[i];
                    }
                    best_str = cont_str;
                    best_output = out_str;

                } else if (cur_dist == min_dist && bits == max_bits) {
                    string cont_str = "", out_str = "";
                    vector<string> items;
                    for (int i = 0; i < N; ++i) {
                        if (cur_mask & (1 << i)) items.push_back(prods[i].name);
                    }
                    sort(items.begin(), items.end());
                    for (size_t i = 0; i < items.size(); ++i) {
                        cont_str += items[i];
                        if (i > 0) out_str += " ";
                        out_str += items[i];
                    }
                    if (cont_str < best_str) {
                        best_str = cont_str;
                        best_output = out_str;
                    }
                }
            }
        }

        cout << min_dist;
        if (!best_output.empty()) {
            cout << " " << best_output;
        }
        cout << "\n";
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "\ntime: " << elapsed.count() << " seconds\n";

    return 0;
}