#include <bits/stdc++.h> 

using namespace std;

// 文字列を整数IDに変換
unordered_map<string, uint32_t> str_to_id;
vector<string> id_to_str;

// 文字列に対応するID取得
uint32_t get_or_create_id(const string& s) {
    auto it = str_to_id.find(s);
    if (it != str_to_id.end()) {
        return it->second;
    }
    uint32_t new_id = id_to_str.size();
    str_to_id[s] = new_id;
    id_to_str.push_back(s);
    return new_id;
}

struct PairData {
    uint32_t id1;
    uint32_t id2;
    int count;
};

bool comparePairs(const PairData& a, const PairData& b) {
    if (a.count != b.count) {
        return a.count > b.count;
    }
    
    const string& s1_a = id_to_str[a.id1];
    const string& s1_b = id_to_str[b.id1];
    if (s1_a != s1_b) {
        return s1_a < s1_b;
    }
    
    return id_to_str[a.id2] < id_to_str[b.id2];
}

int main() {
    
    auto start_time = chrono::high_resolution_clock::now();

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    unordered_map<uint64_t, int> pair_counts;

    for (int i = 0; i < n; ++i) {
        int m;
        cin >> m;
        
        vector<string> products(m);
        for (int j = 0; j < m; ++j) {
            cin >> products[j];
        }

        sort(products.begin(), products.end());

        // ソート済みの文字列をIDに変換
        vector<uint32_t> p_ids(m);
        for (int j = 0; j < m; ++j) {
            p_ids[j] = get_or_create_id(products[j]);
        }

        // ペアの集計
        for (int j = 0; j < m; ++j) {
            for (int k = j + 1; k < m; ++k) {

                uint32_t id1 = p_ids[j];
                uint32_t id2 = p_ids[k];
                
                uint64_t key = ((uint64_t)id1 << 32) | id2;
                pair_counts[key]++;
            }
        }
    }

    vector<PairData> sorted_pairs;
    sorted_pairs.reserve(pair_counts.size());
    for (const auto& kv : pair_counts) {
        uint64_t key = kv.first;
        uint32_t id1 = key >> 32;                
        uint32_t id2 = key & 0xFFFFFFFF;         
        sorted_pairs.push_back({id1, id2, kv.second});
    }

    sort(sorted_pairs.begin(), sorted_pairs.end(), comparePairs);

    int q;
    cin >> q;

    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;

        int end_idx = min(b, (int)sorted_pairs.size());
        
        for (int j = a - 1; j < end_idx; ++j) {
            cout << sorted_pairs[j].count << " " 
                 << id_to_str[sorted_pairs[j].id1] << " " 
                 << id_to_str[sorted_pairs[j].id2] << "\n";
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "\ntime: " << elapsed.count() << " seconds\n";

    return 0;
}