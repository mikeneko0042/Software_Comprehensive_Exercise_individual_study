#include <bits/stdc++.h>
using namespace std;

int main() {
    
    random_device rd;
    mt19937 gen(rd());

    const int MIN_N = 1, MAX_N = 100;
    const int MIN_Q = 1, MAX_Q = 10;

    vector<string> product_pool = {
        "tea", "butter", "bread", "onion", 
        "carrot", "potato", "rice", "water"
    };
    int pool_size = product_pool.size();

    uniform_int_distribution<> dist_N(MIN_N, MAX_N);
    int n = dist_N(gen);

    cout << n << "\n";

    set<pair<string, string>> all_pairs;

    for (int i = 0; i < n; ++i) {

        uniform_int_distribution<> dist_M(1, min(10, pool_size));
        int m = dist_M(gen);
        cout << m;

        vector<string> chosen;
        sample(product_pool.begin(), product_pool.end(), back_inserter(chosen), m, gen);

        for (const string& p : chosen) {
            cout << " " << p;
        }
        cout << "\n";

        sort(chosen.begin(), chosen.end());
        for (int j = 0; j < m; ++j) {
            for (int k = j + 1; k < m; ++k) {
                all_pairs.insert({chosen[j], chosen[k]});
            }
        }
    }

    int total_pairs = all_pairs.size();
    
    if (total_pairs == 0) {
        cout << 0 << "\n";
        return 0;
    }

    uniform_int_distribution<> dist_Q(MIN_Q, MAX_Q);
    int q = dist_Q(gen);
    cout << q << "\n";

    for (int i = 0; i < q; ++i) {
        uniform_int_distribution<> dist_b(1, total_pairs);
        int b = dist_b(gen);
        
        uniform_int_distribution<> dist_a(1, b);
        int a = dist_a(gen);
        
        cout << a << " " << b << "\n";
    }

    return 0;
}