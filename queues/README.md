Queues
-------------
**Note:** From this task, programs should be written in C++ (C++17 standard). In practice, to solve this task, a set of instructions from the C language and `vector` from C++ are enough (but you can use more).

Your task is to efficiently simulate the events occurring in the day of work of the office. In the office there are m windows, numbered from 0 to m-1. Customers queue up at each window. When the clerk is free, he serves the first customer in the queue, then he leaves the office and does not return that day. In addition, there are various other situations in the office, described by the following functions. Your task is to implement these functions.

`void otwarcie_urzedu(int m)` - This function will be called only once, as the first one. Calling this function informs your library that there are `m` windows in the office (and one special window - more on that later). There are no customers in the office yet.

`interesant *nowy_interesant(int k)` - A new customer enters the office and immediately queues up at the end of the queue at window number `k`. The function should create a new element of type interesant and give a pointer to that element as a result.

`int numerek(interesant* i)` - returns the number of customer `i`. Each customer gets a number as soon as they enter the office. The numbers start from 0 and are consecutive integers.

`interesant *obsluz(int k)` - The clerk serving window number `k` tries to serve the next customer. If the queue to the window is not empty, the first customer in the queue is served and leaves the office; then the result of the function should be a pointer to that customer. Otherwise, no one is served, and the result of the function should be `NULL`.

`void zmiana_okienka(interesant *i, int k)` - Customer `i` realizes that he is standing in line for the wrong window. He leaves his queue and queues up at the end of the queue for window `k`, which he currently considers to be correct. You can assume that just before calling this function, customer `i` was queued up in some queue and it was not a queue for window number `k`.

`void zamkniecie_okienka(int k1, int k2)` - The clerk serving window number `k1` goes on a break. All customers standing in line for this window are directed to window number `k2` and queue up at the end of the queue for window `k2` in the same order as they stood in line for window `k1`. We do not know how long the clerk's break will last; it may happen that on the same day customers will queue up for window `k1` and that they will be served.

`std::vector<interesant*> fast_track(interesant *i1, interesant *i2)` - The clerk opens a special window for a moment, where you can quickly handle all matters. A group of customers who were standing consecutively in one queue realizes what is happening and immediately moves to the special window, where they are served right away in the same order as they stood in their previous queue, then they leave the office and the special window closes. The group of customers is given by means of the first (`i1`) and last (`i2`) customer in the group; if `i1=i2`, only customer `i1` is served. The result of the function should be a `vector` of customers served consecutively at the special window, in order of service.

`void naczelnik(int k)` - From time to time, from his office, looks out the head of the office and out of boredom rearranges people standing in line. More precisely, each time he chooses one window number `k` and orders all customers standing in line for that window to reverse their order of queuing. Although such an operation is essentially incomprehensible, customers follow orders from head immediately for fear that he will come up with an even stranger idea. If there was at most one customer in line for that window, head's operation has no effect.

`std::vector<interesant *> zamkniecie_urzedu()` - This function will be called once, at the end of the interaction with the library. It means the end of the day of work of the office. All remaining customers are quickly served and the office closes for the day. The result of the function should be a `vector` of all customers who remained in the office until then, in order: first all those standing in line for window 0 (in order of service), then all those standing in line for window 1 (in order of service) etc.

Your library does not free memory after any customer. This is the responsibility of the user of the library. The user frees memory only when the customer leaves the office. **Note:** The user will free memory for customers using the `free` function, so your library must allocate memory using the `malloc` function.

The declarations of the given functions are in the file [kol.h](https://github.com/Andreluss/Algos-in-C/blob/main/queues/attachments/kol.h). Your task is to complete in the file [kol.h](https://github.com/Andreluss/Algos-in-C/blob/main/queues/attachments/kol.h) the definition of type `struct interesant` (without changing anything else in this file) and implement the given functions in the file `kol.cpp`.

Compilation command:

`g++ @opcjeCpp main.cpp kol.cpp -o main.e`

The differences in the file [opcjeCpp](https://github.com/Andreluss/Algos-in-C/blob/main/queues/attachments/opcjeCpp) compared to the file `opcje` from C are as follows: removed were compilation options: `-Wvla` (which made using variable-length arrays considered a defect), `-Wjump-misses-init` (option specific to C) and `-std=c17`, and instead added was compilation option `-std=c++17`.

For your solution to get maximum points, the time cost of each function must be proportional to the size of parameters and output. The exception is the function `otwarcie_urzedu`, which can run in O(m) time.

Your solution will also be run using the `valgrind` tool, which allows you to check, among other things, whether the program had any memory leaks (you will learn a little more about this tool in next week's scenario). We assume that memory for customers who left the office will be freed by the user. In case of detection of memory leaks using command:

`valgrind --tool=memcheck --leak-check=yes ./main.e`

you may lose from 1 to 2 points for task.

**Example in attachments:**  
[```main.cpp```](https://github.com/Andreluss/Algos-in-C/blob/main/queues/attachments/main.cpp)
[```kol.h```](https://github.com/Andreluss/Algos-in-C/blob/main/queues/attachments/kol.h)
[```opcjeCpp```](https://github.com/Andreluss/Algos-in-C/blob/main/queues/attachments/opcjeCpp)
