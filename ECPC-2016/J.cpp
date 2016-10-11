#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> edge;

int const N = 500 * 1000 + 8;

template<class t>
int u_bound(t const& a, int l, int r, int x) {
  int idx = -1;
  while(l < r) {
    int m = (l+r)/2;
    if(a[m] > x)
      l = m + 1, idx = m;
    else
      r = m;
  }
  return idx;
}

int n;
int a[N];
vector<edge> g[N];

// HLD
int fa[N];
int sz[N];
int idx[N];
int pref[N];
int chain[N];
int chain_sz[N];
vector<int> part[N];
vector<long long> arr[N];

void dfs(int u = 1, int p = 0) {
  fa[u] = p;
  sz[u] = 1;
  pref[u] = 0;
  chain_sz[u] = 0;

  for(auto const& e : g[u]) {
    int v = e.first;

    if(v == p) continue;

    dfs(v, u);
    sz[u] += sz[v];

    if(sz[ pref[u] ] < sz[v])
      pref[u] = v;
  }
}

void hld(int u = 1, int p = 0, int ch = 1) {
  chain[u] = ch;
  chain_sz[u]++;

  for(auto const& e : g[u]) {
    int v = e.first;

    if(v == p) continue;

    if(v == pref[u]) {
      idx[v] = idx[u] + 1;
      hld(v, u, ch);
    } else {
      idx[v] = 0;
      hld(v, u, v);
    }
  }
}

void hld2(int u = 1, int p = 0, int c = 0) {
  if(arr[ chain[u] ].empty()) {
    arr[ chain[u] ].reserve(chain_sz[u]);
    part[ chain[u] ].reserve(chain_sz[u]);
  }

  arr[ chain[u] ].emplace_back(c);
  part[ chain[u] ].emplace_back(0);

  for(auto const& e : g[u]) {
    int v = e.first;
    int c = e.second;

    if(v == p) continue;

    hld2(v, u, c);
  }
}

void query(int city) {
  int ch = chain[city];
  int index = idx[city];

  part[ch][index]--;
  long long oil = a[city] + (index + 1 < arr[ch].size() ? arr[ch][index+1] : 0);

  while(ch && oil > 0) {
    int index2 = u_bound(arr[ch], 0, index+1, oil);

    if(index2 == -1) {
      part[ch][0]++;
      oil -= arr[ch][0];
      ch = chain[ fa[ch] ];
      index = arr[ch].size()-1;
    } else {
      part[ch][index2]++;
      break;
    }
  }
}

void clr() {
  idx[0] = -1;
  for(int i = 1; i <= n; ++i) {
    g[i].clear();
    arr[i].clear();
    part[i].clear();
  }
}

int main() {
  freopen("car.in", "r", stdin);

  int T;
  scanf("%d",&T);

  while(T--) {
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
      scanf("%d", a+i);

    for(int i = 1; i < n; ++i) {
      int u, v, c;
      scanf("%d%d%d", &u, &v, &c);
      g[u].emplace_back( (edge) {v, c} );
      g[v].emplace_back( (edge) {u, c} );      
    }

    sz[0] = -1;
    idx[0] = -1;
    dfs();
    hld();
    hld2();

    // reverse partial sum
    for(int i = 1; i <= n; ++i)
      for(int j = arr[i].size()-2; j >= 0; --j)
        arr[i][j] += arr[i][j+1];

    /* queries */
    for(int i = 2; i <= n; ++i)
      query(i);

    // partial sum
    for(int i = 1; i <= n; ++i)
      partial_sum(part[i].begin(), part[i].end(), part[i].begin());

    for(int i = 1; i <= n; ++i)
      printf("%d%c", part[chain[i]][idx[i]], i == n ? '\n' : ' ');

    if(T) clr();
  }
}
