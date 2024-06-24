#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>

using namespace std;

pair<vector<long long>, vector<int>> dijkstra(int w, int h, int start, vector<vector<pair<int, int>>> &G) {
    const long long INF = 1e18;
    vector<long long> dist(w * h, INF);
    vector<int> pred(w * h);
    dist[start] = 0;
    pred[start] = -1;
    set<pair<long long, int>> pq;
    pq.insert({0, start});
    while (!pq.empty()) {
        int u = pq.begin()->second;
        pq.erase(pq.begin());
        for (auto [v, wt] : G[u]) {
            if (dist[u] + wt < dist[v]) {
                pq.erase({dist[v], v});
                dist[v] = dist[u] + wt;
                pred[v] = u;
                pq.insert({dist[v], v});
            }
        }
    }
    return {dist, pred};
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
    int start = st_x * h + st_y;
    pair<vector<long long>, vector<int>> answer = dijkstra(w, h, start, G);
    vector<long long> dist = answer.first;
    vector<int> pred = answer.second;
    int pos = f_x * h + f_y;
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
