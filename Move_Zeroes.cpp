#include<bits/stdc++.h>
#define ll long long int
#define vi vector< int >
#define vii vector< pair< int,int > >
#define vc vector< char >
#define vvi vector < vector< int > >
#define vvl vector < vector< ll > >
#define vvc vector < vector< char > >
#define vl vector< ll >
#define vll vector< pair< ll,ll > >
#define pii pair< int, int >
#define pll pair<ll, ll>
#define pf push_front
#define pb push_back
// #define range(c) c.begin(), c.end()
// #define ranger(c) c.rbegin(), c.rend()
#define all(c) c.begin(), c.end()
#define allr(c) c.rbegin(), c.rend()
#define forn(i, n) for(ll i = 0; i < n; ++i)
#define fornr(i, n) for(ll i = n-1; i >= 0; --i)
#define foro(i, n) for(ll i = 1; i <= n; ++i)
#define foror(i, n) for(ll i = n; i >= 1; --i)
#define endl '\n'
#define MOD 998244353
#define FIO ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
#define pr(a) for(auto it :a) cout << (it) << " "; cout << endl;
#define pr2(a) for(int i = 0; i<a.size(); ++i){ for(int j = 0; j<a[i].size(); ++j) cout << a[i][j] << ' '; cout << endl;}
#define test ll T; cin >> (T); while(T--)

#define present(container, element) (container.find(element) != container.end())
// for set and maps
#define cpresent(container, element) (find(all(container),element) != container.end())
// for vectors

using namespace std;

void moveZeroes(vector<int>& arr) {
    int n = arr.size(), i = 0;
    int j = 1;

    // while (i < j) {
    while (j < n && i < n) {
        if (arr[i] != 0) {
            ++i;
            j = max(j, i + 1);
        }
        else if (arr[j] == 0)
            ++j;
        else {
            arr[i] = arr[j];
            arr[j] = 0;
            ++i;
            ++j;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("/home/shivam/input.txt", "r", stdin);
    freopen("/home/shivam/output.txt", "w", stdout);
    freopen("/home/shivam/error.txt", "w", stderr);
#endif

    FIO;

    // vector<int> arr = {0, 1, 0, 3, 12};
    vector<int> arr = {2, 1};
    pr(arr);
    moveZeroes(arr);
    pr(arr);

    return 0;
}
