#include <bits/stdc++.h>

using namespace std;

// 比較関数：第一条件を購入回数の降順、第二条件を商品名の昇順（辞書順）とする
bool compareProducts(const pair<string, int>& a, const pair<string, int>& b) {
    if (a.second != b.second) {
        return a.second > b.second; // 購入回数が異なる場合は降順
    }
    return a.first < b.first;       // 購入回数が同じ場合は商品名の辞書順
}

int main() {
    
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    map<string, int> product_counts;

    for (int i = 0; i < n; ++i) {
        int m;
        cin >> m;
        for (int j = 0; j < m; ++j) {
            string product;
            cin >> product;
            product_counts[product]++; 
        }
    }

    // ソートのためにマップのデータをベクターに移す
    vector<pair<string, int>> sorted_products(product_counts.begin(), product_counts.end());

    // ベクターをソート
    sort(sorted_products.begin(), sorted_products.end(), compareProducts);

    int q;
   
    cin >> q;

  
    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;

        // テスト用の区切り線
        cout << "--- Query: " << a << " to " << b << " ---" << "\n";
        
        // 配列のインデックスは0始まりのため、a番目は a-1、b番目は b-1 
        for (int j = a - 1; j < b; ++j) {
            
            cout << sorted_products[j].second << " " << sorted_products[j].first << "\n";
        }
    }

    return 0;
}
