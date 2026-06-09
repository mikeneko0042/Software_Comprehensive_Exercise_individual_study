#include <bits/stdc++.h>
using namespace std;

int main() {

    ofstream fout("test.txt");

    int N = 10000;

    fout << N << "\n";

    vector<string> products = {
        "bread",
        "butter",
        "onion",
        "carrot",
        "potato",
        "rice",
        "tea",
        "water"
    };

    // 乱数生成器
    mt19937 rng(random_device{}());

    for (int i = 0; i < N; i++) {

        // shuffle に変更
        shuffle(products.begin(), products.end(), rng);

        int M = 2 + rand() % 7;

        fout << M;

        for (int j = 0; j < M; j++) {
            fout << " " << products[j];
        }

        fout << "\n";
    }

    int Q = 3;

    fout << Q << "\n";

    fout << "1 5\n";
    fout << "6 10\n";
    fout << "11 15\n";

    fout.close();

    cout << "test.txt created!\n";

    return 0;
}