Next in range
-----------------

**Note:** The complexity of the `init` function should be O(|X|log^2|X|) in the worst case.

Your task is to implement a data structure that allows efficient answering of specific queries about a sequence of numbers X.

For a fixed sequence X=x_0, x_1, ..., x_{n-1}, we are interested in the function:

_nextInRange(i, [a, b])_

which determines the smallest index _j_ such that j >= i and x_j \in [a, b]. If such an index does not exist, the result of the function should be -1.

That is:

nextInRange(i, [a, b]) = min{j >= i : x_j \in [a, b]} or -1 (if such an index does not exist)

Your module should provide the following functions:

`void init(const vector<int> &x)` - initialization of the sequence X by the values of the vector `x` (note! the sequence can contain any values that fit in type `int`)

`int nextInRange(int i, int a, int b)` - calculate the value of _nextInRange(i, [a, b])._ You can assume that 0 <= i < |X| and INT_MIN <= a <= b <= INT_MAX.

`void done()` - release all memory used to handle the sequence X.

The declarations of the given functions are in the file [next.h](https://github.com/Andreluss/Algos-in-C/blob/main/next-in-range/attachments/next.h). Your task is to complete in the file [next.h](https://github.com/Andreluss/Algos-in-C/blob/main/next-in-range/attachments/next.h) the definition of type `struct interesant` (without changing anything else in this file) and implement the given functions in the file `next.cpp`.

Compilation command:

`g++ @opcjeCpp main.cpp next.cpp -o main.e`

For your solution to get maximum points, the time cost of the _nextInRange_ function must be O(log|X|). If the time cost of this function will be O(log^2|X|), you can get 4 points.

Your solution will also be run using the `valgrind` tool, which allows you to check, among other things, whether the program had any memory leaks. In case of detection of memory leaks using command:

`valgrind --tool=memcheck --leak-check=yes ./main.e`

you may lose from 1 to 2 points for task.

**Attachment:**
[opcjeCpp](https://github.com/Andreluss/Algos-in-C/blob/main/next-in-range/attachments/opcjeCpp)
[next.h](https://github.com/Andreluss/Algos-in-C/blob/main/next-in-range/attachments/next.h)
