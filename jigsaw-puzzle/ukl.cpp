// Zadanie zaliczeniowe 6 - rozwiazanie
// Autor: Andrzej Jablonski
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

namespace {
    // Pomocnicza funkcja do sprawdzania, czy [i, j] to poprawne wspolrzedne w vec
    bool in_vector(int i, int j, const vector<string>& vec) {
        return 0 <= i && i < (int)vec.size() && 0 <= j && j < (int)vec[i].size();
    }
}

class Solution {
    // Struktura reprezentujaca klocek
    struct Tile {
        // pierwszy punkt z gory i z lewej - wzgledem niego porownujemy potem klocki:
        int special_i = -1, special_j = -1; 

        int n = 0, m = 0; // wymiary planszy na ktorej zapisany jest ten klocek 
        vector<string> t; // plansza: '.' -> nie ma klocka '[A-Z]' -> klocek i jego literka

        // Funkcja znajdujaca punkt specjalny
        void find_special_point() {
            for (int i = 0; i < (int)t.size(); i++) {
                for (int j = 0; j < (int)t[i].size(); j++) {
                    if (t[i][j] != '.') {
                        special_i = i;
                        special_j = j;
                        return;
                    }
                }
            }
        }
    public:
        int size = 0; // powierzchnia klocka, wedlug niej sortujemy klocki w algorytmie

        // Funkcja wczytujaca klocek z wejscia, narysowany na planszy [height] x [width]
        void read(int height, int width) {
            n = height; m = width;
            t = vector<string>(n);
            size = 0;
            for (int i = 0; i < n; i++) {
                cin >> t[i]; // zakladamy, ze szerokosc t[i] to m
                for (int j = 0; j < m; j++)
                    if (t[i][j] != '.')
                        size += 1;
            }
            find_special_point();
        }

        // Funkcja porownujaca 2 (wczytane) klocki 
        bool operator==(const Tile& other) const {
            // zakladamy, ze *this tile_idx other sa juz wczytanymi klockami

            if (size != other.size) 
                return false;

            int matched = 0; // liczba porownanych czesci klocka
            int di = other.special_i - special_i, dj = other.special_j - special_j;
            for (int i = special_i; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (t[i][j] != '.') {
                        // wtedy other.t[i][j] tez musi byc != '.'
                        if (!in_vector(i + di, j + dj, other.t))
                            return false; 
                        if (other.t[i + di][j + dj] == '.')
                            return false;

                        matched++;
                    }
                }
            }

            return matched == size;
        }

        // Funkcja zwraca kopie danego klocka, obrocona o 90 stopni
        Tile rotated90() const {
            Tile tile;
            tile.n = m; tile.m = n;
            tile.t = vector<string>(m, string(n, '.'));
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    tile.t[j][n-1-i] = t[i][j];
            tile.size = size;
            tile.find_special_point();
            return tile;
        }
    };

    // Struktura utrzymujaca plansze, na ktorej kladziemy i zdejmujemy klocki
    struct Board {
        int n, m; // wymiary planszy: n -> wysokosc, m -> szerokosc
        vector<string> t; // plansza opisana tak jak klocek (pola '.' lub '[A-Z]')
        int filled = 0; // liczba aktualnie zapelnionych pol planszy
        Board(int height = 0, int width = 0): n(height), m(width) {
            t = vector<string>(n, string(m, '.'));
        }

        // Czy plansza jest calkowicie pokryta klockami - znalezlismy rozwiazanie?
        bool is_filled() {
            return filled == n * m;
        }

        // Czy da sie wstawic klocek tile na pole (pos_i, pos_j) w planszy 
        bool can_insert(int pos_i, int pos_j, const Tile& tile) {
            // [da, db] -> przesuniecie wspolrzednych z klocka do wspolrzednych planszy 
            int da = pos_i - tile.special_i, db = pos_j - tile.special_j;
            for (int a = 0; a < tile.n; a++) {
                for (int b = 0; b < tile.m; b++) {
                    if (tile.t[a][b] != '.') {
                        // jesli t[a][b] to czesc klocka
                        // wtedy odpowiadajace pole (a + da, b + db)
                        // musi istniec *na* planszy oraz byc puste ('.')
                        if (!in_vector(a + da, b + db, t))
                            return false;
                        if (t[a + da][b + db] != '.')
                            return false;
                    }
                }
            }
            return true;
        }

        // Wstawienie klocka tile na pozycje (pos_i, pos_j) planszy
        void insert(int pos_i, int pos_j, const Tile& tile) {
            // Zakladamy, ze sprawdzilismy, ze *da sie* wstawic klocek (funkcja can_insert(...))
            int da = pos_i - tile.special_i, db = pos_j - tile.special_j;
            for (int a = 0; a < tile.n; a++) {
                for (int b = 0; b < tile.m; b++) {
                    if (tile.t[a][b] != '.') {
                        // wiemy, ze pole (a + da, b + db) 
                        // istnieje na planszy i jest puste ('.')
                        t[a + da][b + db] = tile.t[a][b];
                        filled++;
                    }
                }
            }
        }

        // Usuniecie z planszy klocka tile, ktory byl wstawiony na pozycji (pos_i, pos_j)
        void remove(int pos_i, int pos_j, const Tile& tile) {
            // (Zakladamy, ze ten klocek rzeczywiscie na planszy jest)
            int da = pos_i - tile.special_i, db = pos_j - tile.special_j;
            for (int a = 0; a < tile.n; a++) {
                for (int b = 0; b < tile.m; b++) {
                    if (tile.t[a][b] != '.') {
                        // wiemy, ze in_vector(a + da, b + db, t)
                        // oraz pole t[a + da][b + db] != '.' (jest zajete przez nasz klocek)
                        t[a + da][b + db] = '.';
                        filled--;
                    }
                }
            }
        }
    };

public:
    // chance[tile_idx, s] czy jest mozliwe uzyskanie sumy pol s z podzbioru klockow [tile_idx, n)
    vector<vector<bool>> chance; 

    // vector klockow oraz ich mozliwych *roznych* obrotow
    vector<vector<Tile>> tiles; // (tiles[i] -> wszystkie rozne obroty klocka i)

    // Backtracking, w wywolaniu rekurencyjnym rozwazamy, 
    // czy wziac klocek nr tile_idx i w jakie mozliwe miejsca go wstawic 
    bool backtrack(Board& board, int tile_idx) {
        // czy znalezlismy rozwiazanie
        if (board.is_filled()) {
            cout << "TAK\n";
            for (int i = 0; i < board.n; i++)
                cout << board.t[i] << "\n";
            return true;
        }
        // jesli nie, a klocki sie skonczyly
        if (tile_idx == (int)tiles.size()) return false;

        // czy jest sens w ogole ten klocek wstawiac?
        if (chance[tile_idx][board.n * board.m - board.filled]) {
            // mozemy sprobowac uzyc tego klocka
            // sprawdzamy wszystkie mozliwe pozycje i obroty klocka tile_idx
            for (const auto& current_tile : tiles[tile_idx]) {
                for (int i = 0; i < board.n; i++) {
                    for (int j = 0; j < board.m; j++) {
                        if (board.can_insert(i, j, current_tile)) {
                            board.insert(i, j, current_tile);

                            // wyszukujemy rozwiazanie dla kolejnego klocka 
                            // jesli jakies sie znalazlo to nic wiecej nie robimy
                            if (backtrack(board, tile_idx + 1))
                                return true;

                            board.remove(i, j, current_tile);
                        }
                    }
                }
            }
        }
        // jesli nic nie mamy, to sprawdzamy jeszcze pozostale pokrycia, nie uzywajace tego klocka
        return backtrack(board, tile_idx + 1);
    }

    // Funkcja wczytujaca klocki ze standardowego wejscia do vectora tiles[]
    // n, m, k - jak w specyfikacji zadania
    void read_tiles(int n, int m, int k) {
        tiles = vector<vector<Tile>>(k);
        for (int i = 0; i < k; i++) {
            Tile tile; tile.read(n, m);

            auto& tile_rotations = tiles[i];
            tile_rotations.push_back(tile);

            for (int rot = 0; rot < 3; rot++) {
                tile = tile.rotated90(); // generujemy kolejny obrot klocka 
                bool same = false; // sprawdzamy, czy nie wyszedl nam taki sam:
                for (int j = 0; j < (int)tile_rotations.size(); j++) {
                    if (tile_rotations[j] == tile) {
                        same = true;
                        break;
                    }
                } 
                if (!same) tile_rotations.push_back(tile);
            }
        }

        // sortujemy klocki od najwiekszych do najmniejszych
        sort(tiles.begin(), tiles.end(),
            [&](const vector<Tile>& a, const vector<Tile>& b) {
                return a.size() > b.size();
            }
        );
    }

    // Funkcja obliczajaca pomocniczy dynamik chance[k][s] do obcinania galezi rekurencji
    // n, m, k - jak w specyfikacji zadania
    void calculate_dp(int n, int m, int k) {
        chance = vector<vector<bool>>(k + 1, vector<bool>(n * m + 3, false));
        chance[k][0] = true;
        for (int i = k - 1; i >= 0; i--) {
            int tile_size = tiles[i][0].size;
            for (int s = tile_size; s <= n * m; s++)
                for (int j = i + 1; j <= k; j++)
                    if (chance[j][s - tile_size])
                        chance[i][s] = true;
        }
    }

    // Funkcja, ktora wczytuje dane i rozwiazuje cale zadanie
    void solve() {
        int n, m, k;
        cin >> n >> m >> k;

        read_tiles(n, m, k);

        calculate_dp(n, m, k);

        Board board(n, m);
        if (!backtrack(board, 0)) {
            cout << "NIE\n";
        }
    }
};

int main() {
    unique_ptr<Solution> solution(new Solution());
    solution->solve();
}
