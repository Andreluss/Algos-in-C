#include "kol.h"
#include <cassert>
#include <iostream>

namespace {
    // ------------------------- Funkcje pomocnicze --------------------------

    /* Funkcja do bezpiecznego i wygodnego alokowania pamieci */
    template<class T>
    T* alloc(size_t size = 1) {
        T* result = nullptr;
        result = (T*)malloc(sizeof(T) * size);
        assert(result);
        return result;
    }

    /**
     * Funkcja do zwalniania pamieci:
     * - zwalnia pamiec jesli sie da 
     * - ustawia wskaznik na nullptr
    **/
    template<class T>
    void free_pointer(T*& pointer) {
        if (!pointer) return;
        free(pointer);
        pointer = nullptr;
    }



    // -------- Funkcje do operacji na elementach kolejek (list) -------------

    // Czy interesanci a i b stoja obok siebie (i oboje o tym wiedza)?
    bool are_connected(interesant* a, interesant* b) {
        return (a->next == b || a->prev == b) 
            && (b->prev == a || b->next == a);
    }

    // Rozlacz interesantow a i b (jesli sie da)
    void unlink(interesant* a, interesant* b) {
        if (a->next == b) a->next = nullptr;
        else if (a->prev == b) a->prev = nullptr;

        if (b->next == a) b->next = nullptr;
        else if (b->prev == a) b->prev = nullptr;
    }

    // Polacz interesantow a i b (jesli sie da)
    void link(interesant* a, interesant* b) {
        if (a->next == nullptr) a->next = b;
        else if (a->prev == nullptr) a->prev = b;

        if (b->next == nullptr) b->next = a;
        else if (b->prev == nullptr) b->prev = a;
    }

    // Wyciagnij interesanta i z jego kolejki
    void pull_out(interesant* i) {
        // a, b - sasiedzi interesanta i
        auto a = i->next;
        auto b = i->prev;

        unlink(a, i);
        unlink(i, b);
        link(a, b);
    }

    // Zwraca drugiego sasiada interesanta curr (pierwszy sasiad to prev)
    interesant* get_next(interesant* curr, interesant* prev = nullptr) {
        if (curr == nullptr) return nullptr; // przydatne w fast_track'u
        if (curr->next != prev) {
            return curr->next;
        }
        else {
            assert(curr->prev != prev);
            return curr->prev;
        }
    }


    // -------------- Implementacja kolejki do jednego okienka ---------------

    struct Queue {
        interesant* head, *tail; // Atrapy na poczatek i koniec

        Queue(): head(nullptr), tail(nullptr) {
            // alokujemy z checkiem na null
            head = alloc<interesant>();
            tail = alloc<interesant>();

            // malloc nie wywoluje konstruktorow...
            head->prev = nullptr;
            head->next = tail;
            tail->prev = head;
            tail->next = nullptr;
            head->num = tail->num = -42;
        }

        Queue(const Queue& q) = delete;
    
        ~Queue() {
            free_pointer(head);
            free_pointer(tail);
        }

        // Czy kolejka jest pusta?
        bool empty() {
            return are_connected(head, tail);
        }
    
        // Wstaw interesanta i na koniec kolejki
        interesant* push_back(interesant* i) {
            //Debug: Sprawdzamy, czy i jest 'czysty'
            assert(i->num != -42);
            assert(i->next == i->prev && i->prev == nullptr);

            // -----------------------------------------
            // Dotychczas ostatni interesant w kolejce k
            interesant* last = get_next(tail, nullptr);
            assert(are_connected(last, tail));

            // Przed: [tail] <-> [last]    [it]
            unlink(tail, last);
            link(tail, i);
            link(i, last);
            // Po:    [tail] <-> [it] <-> [last]

            return i;
        }

        // Wyciagnij interesanta z poczatku kolejki
        interesant* pop_front() {
            assert(!empty());
            auto last = get_next(head, nullptr);
            auto new_last = get_next(last, head); 
        
            unlink(head, last);
            unlink(last, new_last);
            link(head, new_last);

            return last;
        }

        // Zlacz kolejki (dodaj cala kolejke other na koniec)
        void merge(Queue& other) {
            if (other.empty()) return;

            auto other_first = get_next(other.head);
            auto other_last = get_next(other.tail);
            unlink(other.head, other_first);
            unlink(other_last, other.tail);
            link(other.head, other.tail);

            auto last = get_next(tail);
            unlink(last, tail);

            link(last, other_first);
            link(other_last, tail);
        }

        // Odwroc kolejke
        void reverse() {
            std::swap(head, tail);
        }
    };

    // Wektor przechowujacy wszystkie kolejki
    std::vector<Queue> queues;
}



// ------------------------------ Interfejs ----------------------------------

void otwarcie_urzedu(int m) {
    queues = std::vector<Queue>(m);
}

interesant* nowy_interesant(int k) {
    // Aktualny numerek do przydzielenia nastepnemu interesantowi
    static int curr_num = 0; 

    // Tworzymy nowego interesanta
    interesant* it = alloc<interesant>();
    it->num = curr_num++;
    it->next = it->prev = nullptr;

    return queues[k].push_back(it);
}

int numerek(interesant* i) {
    return i->num;
}

interesant* obsluz(int k) {
    if (queues[k].empty()) 
        return nullptr;

    return queues[k].pop_front();
    
}

void zmiana_okienka(interesant* i, int k) {
    pull_out(i); // wyciagniecie i z jego kolejki
    queues[k].push_back(i); // wstawienie go na koniec kolejki k
}

void zamkniecie_okienka(int k1, int k2) {
    if (k1 == k2) return;
    queues[k2].merge(queues[k1]);
}

std::vector<interesant*> fast_track(interesant* i1, interesant* i2) {
    if (i1 == i2) {
        pull_out(i1);
        return std::vector<interesant*>{ i1 };
    }

    interesant* a = i1->next, * b = i1->prev, * a_old = i1, * b_old = i1;
    std::vector<interesant*> dir_a{ i1, a }, dir_b{ i1, b };

    while (a != i2 && b != i2) {
        assert(!!a || !!b); // przesuwamy wskazniki dalej w przeciwne strony
        interesant* a_new = get_next(a, a_old);
        a_old = a; a = a_new;
        dir_a.push_back(a);

        interesant* b_new = get_next(b, b_old);
        b_old = b; b = b_new;
        dir_b.push_back(b);
    }

    if (a == i2) {
        interesant* a_next = get_next(a, a_old), *a_prev = i1->prev;
        unlink(a_next, a);
        unlink(i1, a_prev);
        link(a_next, a_prev);
        return dir_a;
    }
    else {
        interesant* b_next = get_next(b, b_old), *b_prev = i1->next;
        unlink(b_next, b);
        unlink(i1, b_prev);
        link(b_next, b_prev);
        return dir_b;
    }
}

void naczelnik(int k) {
    queues[k].reverse();
}

std::vector<interesant*> zamkniecie_urzedu() {
    std::vector<interesant*> all;
    for (auto& queue : queues) {
        while (!queue.empty()) {
            all.push_back(queue.pop_front());
        }
    }
    return all;
}
