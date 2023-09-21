#include <stdlib.h>
#include <stdio.h>

// ========================== Funkcje pomocnicze =============================
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

int* nowa_tablica(int n) {
    return (int*)malloc(sizeof(int) * (size_t)n);
}

void wczytaj(int* x) {
    if (scanf("%d", x)) {}
}


// ======================== Najblizsza trojka moteli =========================
int znajdz_najblizsza_trojke(int n, int* t, int* d) {
    int najblizsza = d[n - 1] - d[0] + 1;// ustawiamy wartosc - wartownika, 
                                         // prawdziwy wynik bedzie mniejszy 
    // -------------------------------------------------
    // Przechodzimy po przedzialach postaci xyyyyyyyz 
    // Sprawdzamy, czy x != z   (x,y,z to numery sieci)
    // I aktualizujemy wynik...
    // -------------------------------------------------
    for (int i = 0; i < n - 2; ) {
        if (t[i] == t[i + 1]) {
            i++;
        }
        else {
            // Wiemy, ze t[i] != t[i+1]
            // Szukamy pierwszego j, takiego ze t[j] != t[i+1]
            int j = i + 2;
            while (j < n && t[j] == t[i + 1])
                j++;

            // Sprawdzamy, czy mamy 3 rozne typy moteli i aktualizujemy wynik
            if (j < n && t[j] != t[i]) {
                for (int k = i + 1; k <= j - 1; k++) {
                    najblizsza = min(najblizsza, 
                                     max(d[k] - d[i], d[j] - d[k]));
                }
            }
            i = j - 1;
        }
    }
    if (najblizsza == d[n - 1] - d[0] + 1) // Jesli nic nie znalezlismy
        najblizsza = 0;
    return najblizsza;
}


// ========================= Najdalsza trojka moteli ========================= 
// Funkcja pomocnicza zwracajaca brzegowe pierwsze 3 motele roznych sieci 
// Przekazujemy jej tablice t, jej przedzial -> [start...koniec) 
// i kierunek poruszania sie po nim          -> delta (+1 lub -1)
int* znajdz_3_brzegowe(int* t, int start, int koniec, int delta) {
    int* brzeg = nowa_tablica(3);
    brzeg[0] = brzeg[1] = brzeg[2] = -1;
    for (int i = start; i != koniec && brzeg[2] == -1; i += delta) {
        for (int j = 0; j < 3; j++) {
            if (brzeg[j] == -1) { 
                brzeg[j] = i; 
                break; 
            }
            else if (t[brzeg[j]] == t[i]) {
                break;
            }
        }
    }
    return brzeg;
}

// Funkcja wlasciwa
int znajdz_najdalsza_trojke(int n, int* t, int* d) {
    // indeksy pierwszych (ostatnich) 3 moteli z roznych sieci
    int* pierwsze = znajdz_3_brzegowe(t, 0, n, +1);
    int* ostatnie = znajdz_3_brzegowe(t, n - 1, -1, -1);

    int najdalsza = 0;
    for (int li = 0; li < 3; li++) {
        for (int ri = 0; ri < 3; ri++) {
            int a = pierwsze[li], b = ostatnie[ri];
            if (a != -1 && b != -1 && a < b && t[a] != t[b]) {
                // -----------------------------------------------------
                // Wiemy, ze pierwszym i trzecim motelem bedzie a i b
                // Przeszukujemy wszystkich kandydatow na srodkowy motel 
                // i maksymalizujemy wynik:
                // -----------------------------------------------------
                for (int c = a + 1; c <= b - 1; c++) {
                    if (t[c] != t[a] && t[c] != t[b]) {
                        najdalsza = max(najdalsza, 
                                        min(d[c] - d[a], d[b] - d[c]));
                    }
                }
            }
        }
    }

    free(pierwsze); 
    free(ostatnie);
    return najdalsza;
}


int main()
{
    int n; wczytaj(&n);
    int* t = nowa_tablica(n);
    int* d = nowa_tablica(n);
    for (int i = 0; i < n; i++) {
        wczytaj(&t[i]); 
        wczytaj(&d[i]);
    }

    int najblizsza = znajdz_najblizsza_trojke(n, t, d);
    int najdalsza = znajdz_najdalsza_trojke(n, t, d);
    printf("%d %d\n", najblizsza, najdalsza);

    free(t);
    free(d);
}