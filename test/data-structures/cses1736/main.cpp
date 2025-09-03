#include <bits/stdc++.h>
#include "data-structures/fenwick_tree.hpp"
using namespace std;

void solve() {
    int n, q;
    cin >> n >> q;

    vector <int> a(n);
    for (auto &x: a)
        cin >> x;
    
    FenwickRangeAP fen(n);
    for (int i = 0; i < n; i++)
        fen.add_range_constant(i, i, a[i]);

    for (int i = 0; i < q; i++) {
        int t;
        cin >> t;

        if (t == 1) {
            int l, r; cin >> l >> r; l--; r--;
            fen.add_range_increasing_by_one(l, r);
        }
        else {
            int l, r; cin >> l >> r; l--; r--;
            cout << fen.range_sum(l, r) << "\n";
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int q = 1;
    // cin >> q;

    while (q--) {
        solve();
    }
}