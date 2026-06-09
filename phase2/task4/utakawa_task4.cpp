#include <bits/stdc++.h>
using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 入力ファイルを開く
    ifstream fin("test.txt");

    // ファイル開けなかった場合
    if (!fin) {
        cout << "Cannot open file\n";
        return 1;
    }

    int N;
    fin >> N;

    unordered_map<string, int> pair_count;

    for (int i = 0; i < N; i++) {

        int M;
        fin >> M;

        vector<string> products(M);

        for (int j = 0; j < M; j++) {
            fin >> products[j];
        }

        // 重複削除
        sort(products.begin(), products.end());

        products.erase(
            unique(products.begin(), products.end()),
            products.end()
        );

        int size = products.size();

        // ペア生成
        for (int j = 0; j < size; j++) {

            for (int k = j + 1; k < size; k++) {

                string a = products[j];
                string b = products[k];

                string key = a + "#" + b;

                pair_count[key]++;
            }
        }
    }

    // vectorへ変換
    vector<tuple<int,string,string>> vec;

    for (auto &kv : pair_count) {

        string key = kv.first;
        int count = kv.second;

        int pos = key.find('#');

        string a = key.substr(0, pos);
        string b = key.substr(pos + 1);

        vec.push_back({count, a, b});
    }

    // ソート
    sort(vec.begin(), vec.end(),
        [](auto &x, auto &y) {

            if (get<0>(x) != get<0>(y))
                return get<0>(x) > get<0>(y);

            if (get<1>(x) != get<1>(y))
                return get<1>(x) < get<1>(y);

            return get<2>(x) < get<2>(y);
        }
    );

    int Q;
    fin >> Q;

    while (Q--) {

        int a, b;
        fin >> a >> b;

        for (int i = a - 1; i < b; i++) {

            cout << get<0>(vec[i]) << " "
                 << get<1>(vec[i]) << " "
                 << get<2>(vec[i]) << "\n";
        }
    }

    fin.close();
    auto end = chrono::high_resolution_clock::now();

    double elapsed =
        chrono::duration<double>(end - start).count();

    cerr << "Time: " << elapsed << " sec\n";

    return 0;
}