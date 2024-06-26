#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <cmath>

using namespace std;

long double fun_h(int a, int b, int h) {
    return sqrt((a / h - b / h) * (a / h - b / h) + (a % h - b % h) * (a % h - b % h));
}

pair<vector<long double>, vector<int>> a_star(int w, int h, int start, int finish, vector<vector<pair<int, int>>> &G) {
    const long double INF = 1e18;
    vector<long double> f(w * h, INF), g(w * h, INF);
    vector<int> pred(w * h);
    g[start] = 0;
    f[start] = g[start] + fun_h(start, start, h);
    pred[start] = -1;
    set<pair<long double, int>> pq;
    pq.insert({f[start], start});
    while (!pq.empty()) {
        int u = pq.begin()->second;
        pq.erase(pq.begin());
        if (u == finish) {
            break;
        }
        for (auto [v, wt] : G[u]) {
            if (g[u] + wt < g[v]) {
                pq.erase({f[v], v});
                g[v] = g[u] + wt;
                f[v] = g[v] + fun_h(start, v, h);
                pred[v] = u;
                pq.insert({f[v], v});
            }
        }
    }
    return {g, pred};
}

int main(int argc, char* argv[]) {
    ifstream file;
    file.open(argv[1]);
    if (!file) {
        cout << "Не удалось открыть файл\n";
        return 1;
    }
    int w, h;
    file >> w >> h;
    vector<vector<int>> a(w, vector<int>(h));
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            char c;
            file >> c;
            if (c == '.') {
                a[i][j] = 1;
            }
        }
    }
    vector<vector<pair<int, int>>> G(w * h);
    vector<pair<int, int>> sosedi = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (!a[i][j]) {
                continue;
            }
            for (auto e : sosedi) {
                int x = i + e.first, y = j + e.second;
                if (x >= 0 && x < w && y >= 0 && y < h && a[x][y]) {
                    G[i * h + j].push_back({x * h + y, 1});
                    G[x * h + y].push_back({i * h + j, 1});
                }
            }
        }
    }
    int st_x, st_y, f_x, f_y;
    file >> st_x >> st_y >> f_x >> f_y;
    file.close();
    int start = st_x * h + st_y, finish = f_x * h + f_y;
    pair<vector<long double>, vector<int>> answer = a_star(w, h, start, finish, G);
    vector<long double> dist = answer.first;
    vector<int> pred = answer.second;
    int pos = finish;
    vector<int> ans;
    while (pos != -1) {
        ans.push_back(pos);
        pos = pred[pos];
    }
    reverse(ans.begin(), ans.end());
    cout << "Расстояние: " << dist[f_x * h + f_y] << "\n";
    cout << "Путь: \n";
    for (auto x : ans) {
        cout << x / h << " " << x % h << "\n";
    }
    return 0;
}
