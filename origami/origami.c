// Rozwiazanie zadania: Origami 
// Autor: Andrzej Jablonski 
// Reviewer: Mateusz Winiarek
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

// Struktura danych pozwalajaca trzymac punkt, wektor, itp.
typedef struct Pair {
    double x, y;
} Pair;

// ------------------------ Pomocnicze funkcje ------------------------------
// Funkcja sprawdzajaca, czy double jest zerem
bool is_zero(double x) { 
    return fabs(x) < 1e-10; 
}

// Funkcje do wczytywania danych ze standardowego wejscia: 
// - koncza program z kodem bledu w przypadku problemow z odczytem
void read_int(int* x) {
    if (!scanf("%d", x))
        exit(42);
}

void read_double(double* x) {
    if (!scanf("%lf", x))
        exit(43);
}

void read_char(char* c) {
    if (!scanf(" %c", c))
        exit(44);
}

void read_pair(Pair* p) {
    read_double(&p->x); read_double(&p->y);
}
// --------------------------------------------------------------------------



// ----------------------- Geometryczne funkcje -----------------------------
// Funkcja zwracajaca wynik dodawania wektorow p1 + p2
Pair add(Pair p1, Pair p2) {
    Pair result = { p1.x + p2.x, p1.y + p2.y };
    return result;
}

// Funkcja zwracajaca wynik odejmowania wektorow p1 - p2
Pair subtract(Pair p1, Pair p2) {
    Pair result = { p1.x - p2.x, p1.y - p2.y };
    return result;
}

// Funkcja zwracajaca wektor p pomnozony przez skalar k
Pair multiply(Pair p, double k) {
    Pair result = { p.x * k, p.y * k };
    return result;
}

// Iloczyn skalarny wektorow v1, v2 
double dot(Pair v1, Pair v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Iloczyn wektorowy wektorow v1, v2
double cross(Pair v1, Pair v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

// Po ktorej stronie prostej wyznaczanej przez punkty a, b jest punkt p? 
typedef enum Orientation { LEFT, ON, RIGHT } Orientation;
// LEFT -> (z lewej), 
// ON  -> (na prostej), 
// RIGHT  -> (z prawej)
Orientation orientation(Pair p, Pair a, Pair b) {
    double cross_value = cross(subtract(p, a), subtract(b, a));
    if (is_zero(cross_value)) return ON;
    if (cross_value < 0) return LEFT;
    return RIGHT;
}

// Zwraca odbicie punktu p wzgledem prostej wyznaczanej przez punkty a, b
Pair reflection(Pair p, Pair a, Pair b) {
    Pair u = subtract(b, a);// wektor ab (nasza prosta)
    Pair v = subtract(p, a);// wektor ap 
    Pair n = { .x = -u.y, .y = u.x };// wektor u obrocony o 90 stopni ccw
                                     // czyli wektor normalny do prostej a->b
    double n_mag_2 = dot(n, n);// |n|^2, czyli modul wektora n do kwadratu
    assert(!is_zero(n_mag_2));// powinno zachodzic a != b, wiec n_mag_2 != 0

    // p_proj = wektor [(a) -> (rzut punktu p na wektor n)] lub inaczej:
    // - wektor prostopadly od prostej a->b 
    // - skierowany w strone punktu p, 
    // - o dlugosci rownej odlegosci punktu p od prostej a->b
    Pair p_proj = multiply(n, (dot(v, n) / n_mag_2));

    // p_refl = wynik, czyli przesuniecie punktu p o wektor 2 * (-p_proj)
    Pair p_refl = subtract(p, multiply(p_proj, 2));

    return p_refl;
}
// --------------------------------------------------------------------------



// --------------- Struktury zwiazane z reprezentacja kartek ----------------
struct Circle {
    Pair center;
    double radius;
};

// Czy punkt p lezy w kole c?
bool in_circle(Pair p, struct Circle c) {
    Pair v = subtract(p, c.center);// wektor ze srodka okregu c do punktu p
    double x2y2 = dot(v, v);// kwadrat odleglosci p od c 
    return x2y2 <= c.radius * c.radius;
}

struct Rectangle {
    Pair p1, p2;
};

// Czy punkt p lezy w prostokacie r?
bool in_rectangle(Pair p, struct Rectangle r) {
    bool x_ok = (r.p1.x <= p.x && p.x <= r.p2.x);
    bool y_ok = (r.p1.y <= p.y && p.y <= r.p2.y);
    return x_ok && y_ok;
}

// Operacja skladania
struct Fold {
    Pair p1, p2;
    int prev_sheet_index;
};

enum State {
    RECTANGLE, CIRCLE, FOLD
};

struct Sheet {
    union {
        struct Circle circle;
        struct Rectangle rectangle;
        struct Fold fold;
    };
    enum State type;
};
// --------------------------------------------------------------------------



// ---------------- Przetwarzanie danych wejsciowych ------------------------
typedef struct Data {
    int n, q;
    struct Sheet* sheets;
} Data;

Data* read_data() {
    Data* data = (Data*)malloc(sizeof(Data));
    if (!data) 
        exit(1);

    read_int(&data->n);
    read_int(&data->q);

    data->sheets = (struct Sheet*)malloc((size_t)data->n * sizeof(struct Sheet));
    if (!data->sheets) 
        exit(1);

    for (int sheet_index = 0; sheet_index < data->n; sheet_index++)
    {
        struct Sheet* current_sheet = &data->sheets[sheet_index];
        char type; 
        read_char(&type);
        if (type == 'P') {
            current_sheet->type = RECTANGLE;
            read_pair(&current_sheet->rectangle.p1);
            read_pair(&current_sheet->rectangle.p2);
        }
        else if (type == 'K') {
            current_sheet->type = CIRCLE;
            read_pair(&current_sheet->circle.center);
            read_double(&current_sheet->circle.radius);
        }
        else if (type == 'Z') {
            current_sheet->type = FOLD;
            read_int(&current_sheet->fold.prev_sheet_index);
            current_sheet->fold.prev_sheet_index--;// bo indeksujemy od zera
            read_pair(&current_sheet->fold.p1);
            read_pair(&current_sheet->fold.p2);
        }
    }
    return data;
}

void destroy_data(Data* data) {
    if (data) {
        if (data->sheets) {
            free(data->sheets);
        }
        free(data);
    }
}
// --------------------------------------------------------------------------



// ---------------------- Odpowiadanie na zapytania -------------------------
// Wlasciwa funkcja odpowiadajaca na konkretne zapytanie
long long answer_query(Data* data, int sheet_index, Pair point) {
    // Aktualna wersja kartki:
    struct Sheet* sheet = &data->sheets[sheet_index];

    if (sheet->type == FOLD) {
        // Sprawdzamy, po jakiej stronie prostej 
        // aktualnie zginajacej kartke byl nasz punkt:
        Orientation o = orientation(point, sheet->fold.p1, sheet->fold.p2);

        if (o == ON) {
            // Jesli na, to po tym zgieciu nic sie nie zmienilo
            return answer_query(data, sheet->fold.prev_sheet_index, point);
        }
        else if (o == RIGHT) {
            // Jesli z prawej, to odpowiedz to 0, 
            // poniewaz po wykonaniu aktualnego zgiecia 
            // z prawej strony zginajacej prostej nie ma zadnej czesci kartki
            return 0;
        }
        else if (o == LEFT) {
            // Jesli z lewej, to odpowiedz to suma przecietych warstw 
            // 1) w tym samym miejscu (tych, ktore byly juz przed zgieciem)
            // 2) w odbiciu tego punktu wzgl. aktualnej zginajacej prostej 
            //    (tych, ktore "polozyly" sie na tym punkcie po zgieciu)
            int prev_sheet_index = sheet->fold.prev_sheet_index;
            long long w1 = answer_query(data, prev_sheet_index, point);
            Pair r_point = reflection(point, sheet->fold.p1, sheet->fold.p2);
            long long w2 = answer_query(data, prev_sheet_index, r_point);
            return w1 + w2;
        }
    }
    if (sheet->type == RECTANGLE) {
        return in_rectangle(point, sheet->rectangle);
    }
    if (sheet->type == CIRCLE) {
        return in_circle(point, sheet->circle);
    }
    return 0;
}

// Funkcja wczytujaca i odpowiadajaca na wszystkie zapytania
void answer_queries(Data* data) {
    for (int query_num = 0; query_num < data->q; query_num++) {
        // 1. Wczytanie numeru kartki 
        int sheet_index = 0; 
        read_int(&sheet_index);
        sheet_index--; // odejmujemy 1, bo indeksujemy kartki od zera

        // 2. Wczytanie wspolrzednych punktu, w ktory wbijamy szpilke
        Pair point = { 0, 0 };
        read_pair(&point);
        
        // 3. Wywolanie wlasciwej funkcji
        long long result = answer_query(data, sheet_index, point);
        printf("%lld\n", result);
    }
}
// --------------------------------------------------------------------------


// ---------------------------- Funkcja main --------------------------------

int main()
{
    Data* data = read_data();
    answer_queries(data);
    destroy_data(data);
}