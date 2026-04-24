#include <bits/stdc++.h>

using namespace std;

int main() {
    random_device rd;
    mt19937 gen(rd());
    
    vector<string> pool = {"apple", "banana", "bread", "butter", "carrot", "onion", "potato", "rice", "tea", "water"};
    
    uniform_int_distribution<> dist_N(1, 100);
    int N = dist_N(gen);
    cout << N << "\n";
    
    set<string> unique_products;
    
    for (int i = 0; i < N; ++i) {
        uniform_int_distribution<> dist_M(1, pool.size());
        int M = dist_M(gen);
        
        // 重複を避けるためシャッフルして先頭からM個を取得
        vector<string> temp_pool = pool;
        shuffle(temp_pool.begin(), temp_pool.end(), gen);
        
        cout << M;
        for (int j = 0; j < M; ++j) {
            cout << " " << temp_pool[j];
            unique_products.insert(temp_pool[j]);
        }
        cout << "\n";
    }
    
    int total_unique = unique_products.size();
    uniform_int_distribution<> dist_Q(1, 10);
    int Q = dist_Q(gen);
    cout << Q << "\n";
    
    for (int i = 0; i < Q; ++i) {
        uniform_int_distribution<> dist_a(1, total_unique);
        int a = dist_a(gen);
        uniform_int_distribution<> dist_b(a, total_unique); 
        int b = dist_b(gen);
        cout << a << " " << b << "\n";
    }
    
    return 0;
}
