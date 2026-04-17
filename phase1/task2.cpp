#include <bits/stdc++.h>

using namespace std;

typedef pair<string, string> ProductPair;

bool comparePairs(const pair<ProductPair, int>& a, const pair<ProductPair, int>& b) {
    if (a.second != b.second) {
        return a.second > b.second;
    }
    if (a.first.first != b.first.first) {
        return a.first.first < b.first.first;
    }
    return a.first.second < b.first.second;
}

int main() {
    
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    map<ProductPair, int> pair_counts;

    for (int i = 0; i < n; ++i) {
        int m;
        cin >> m;
        
        vector<string> products(m);
        for (int j = 0; j < m; ++j) {
            cin >> products[j];
        }

        sort(products.begin(), products.end());

        for (int j = 0; j < m; ++j) {
            for (int k = j + 1; k < m; ++k) {
                // 事前にソートしているため、必ず products[j] < products[k]
                ProductPair p = {products[j], products[k]};
                pair_counts[p]++; 
            }
        }
    }

    vector<pair<ProductPair, int>> sorted_pairs(pair_counts.begin(), pair_counts.end());

    sort(sorted_pairs.begin(), sorted_pairs.end(), comparePairs);

    int q;
    cin >> q;

    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;
        
        for (int j = a - 1; j < b; ++j) {
            cout << sorted_pairs[j].second << " " 
                 << sorted_pairs[j].first.first << " " 
                 << sorted_pairs[j].first.second << "\n";
        }
    }

    return 0;
}
