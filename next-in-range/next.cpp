#include "next.h"
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

namespace {
    using namespace std;
    const int Inf = int(1e9 + 42 * 17); // stala reprezentujaca nieskonczonosc
    // Trwale drzewo przedzialowe 
    // - z operacja ustawiania wartosci w punkcie (Set)
    // - i brania minimum na przedziale (Get)
    struct PersistentMinTree {
    private:
        // Wezel drzewa
        struct Node {
            shared_ptr<Node> l, r;
            int value;

            Node(int val = Inf) : l(nullptr), r(nullptr), value(val) {}
            Node(shared_ptr<Node> left, shared_ptr<Node> right) : l(left), r(right), value(Inf) {
                if (l) value = min(l->value, value);
                if (r) value = min(r->value, value);
            }
        };

        // [Disclaimer] parametry w funkcjach prywatnych oznaczaja: 
        // v - aktualny wierzcholek, w ktorym jestesmy
        // tl, tr - lewy i prawy koniec przedzialu drzewowego 
        //          [tl, tr] wyznaczanego przez v
        //          w szczegolnosci, jesli tl == tr, to jestesmy w lisciu
        // tm - srodek przedzialu [tl, tr], dzielacy go na [tl, tm] u [tm+1, tr]
        // a, b - szukany w zapytaniu przedzial (jesli pokrywa sie z [tl, tr], to mamy odpowiedz)       

        // Buduje rekurencyjnie drzewo, wypelniajac je wartosciami +Inf
        shared_ptr<Node> build(int tl, int tr) { 
            if (tl == tr) return make_shared<Node>();
            int tm = (tl + tr) / 2;
            return make_shared<Node>(build(tl, tm), build(tm + 1, tr));
        }

        // Prywatna implementacja funkcji Get(time, a, b)
        int Get(shared_ptr<Node> v, int tl, int tr, int a, int b) {
            if (!(a <= b)) return Inf;
            if (tl == tr) return v->value;
            int tm = (tl + tr) / 2;
            int result_left = Get(v->l, tl, tm, a, min(b, tm));
            int result_right = Get(v->r, tm + 1, tr, max(a, tm + 1), b);
            return min(result_left, result_right);
        }

        // Prywatna implementacja funkcji Set(pos, value)
        shared_ptr<Node> Set(int pos, int val, shared_ptr<Node> v, int tl, int tr) {
            if (tl == tr) {
                int better_new_value = min(v->value, val);
                return make_shared<Node>(better_new_value);
            }
            int tm = (tl + tr) / 2;
            if (pos <= tm)
                return make_shared<Node>(Set(pos, val, v->l, tl, tm), v->r);
            else
                return make_shared<Node>(v->l, Set(pos, val, v->r, tm + 1, tr));
        }

        // Ilosc lisci w drzewie, o numerach w przedziale [0, leaf_count)
        int leaf_count = 4;
        // Vector z korzeniami w kolejnych momentach czasu 
        std::vector<shared_ptr<Node>> root;
        

    public:
        // Tworzy drzewo na przedziale [0, size), wypelnione wartosciami +Inf
        PersistentMinTree(int size) {
            while (leaf_count < size) leaf_count *= 2;
            root.emplace_back(build(0, leaf_count - 1));
        }

        // Ustawia nowa, mniejsza wartosc val na pozycji pos \in [0, size)
        void Set(int pos, int val) {
            root.emplace_back(Set(pos, val, root.back(), 0, leaf_count - 1));
        }

        // Zwraca minimum na przedziale [a, b] w time-tym drzewie w historii
        int Get(int time, int a, int b) {
            return Get(root[time], 0, leaf_count - 1, a, b);
        }
    };

    // Zwraca przeskalowany vector t o wartosciach \in [0, ile_roznych(t))
    vector<int> scaled(vector<int> t) {
        map<int, int> mp;
        for (auto& ti : t) mp[ti];
        int cnt = 0;
        for (auto& [k, v] : mp) v = cnt++;
        for (auto& ti : t) ti = mp[ti];
        return t;
    }

    // Drzewo, ktorego bedziemy uzywac w zadaniu
    shared_ptr<PersistentMinTree> tree;
    // Posortowany vector par (x[indeks], indeks)
    vector<pair<int, int>> X;
    // Y[indeks] - przeskalowana wartosc x[indeks] z oryginalnego ciagu
    vector<int> Y;
}

void init(const std::vector<int>& x) {
    int n = (int)x.size();
    X = vector<pair<int, int>>(n);
    for (int i = 0; i < n; i++)
        X[i] = { x[i], i };
    sort(X.begin(), X.end());

    Y = scaled(x); // Y[i] - przeskalowane x[i]

    // jak dlugi przedzial potrzebujemy w drzewie:
    int M = *max_element(Y.begin(), Y.end()) + 1; 

    tree = make_shared<PersistentMinTree>(M);
    for (int i = n - 1; i >= 0; i--) {
        tree->Set(Y[i], i);
    }
}

int nextInRange(int i, int a, int b) { // min{ j : j >= i && x[j] \in [a..b] } or -1
    // Moment, kiedy w drzewie byly tylko indeksy [i, i+1, ..., n-1]
    int tree_time = int(X.size() - i);

    auto it = lower_bound(X.begin(), X.end(), pair{ a, -1 });
    if (it == X.end()) 
        return -1; // jesli X[] < a
    int a_new = Y[it->second]; // przeskalowana wartosc a

    it = upper_bound(X.begin(), X.end(), pair{ b, Inf });
    if (it == X.begin())
        return -1; // jesli b < X[]
    int b_new = Y[(--it)->second]; // przeskalowana wartosc b 
                                   // (--it, bo chcemy znalezc ostatni x[i] <= b)
    if (a_new > b_new) 
        return -1; // jesli z jakiegos powodu przedzial [a_new, b_new] jest zly

    int result = tree->Get(tree_time, a_new, b_new);
    if (result == Inf)
        return -1;
    else
        return result;
}

void done() {
}