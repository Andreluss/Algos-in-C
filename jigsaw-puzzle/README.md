Jigsaw puzzle
------------

Note 2: If you want to use the type `__int128` and have a program working with the option `--pedantic`, you should write:
`__extension__ typedef __int128 int128;`
After that, use the type `int128`.

Note 3: The official tests for the task are not bad, although not necessarily the hardest possible :)

Note 4: The blocks are non-empty.

There are initially an empty board of size n x m composed of n*m fields and k blocks. Each block has an identifier assigned by a capital letter of the English alphabet and is a connected set of fields, where two fields are adjacent if they have a common side. Write a program that will check if you can place a certain number of blocks on the board so that each field of the board is covered by exactly one block. No block can protrude beyond the board. Each block can be used at most once. Blocks can be rotated by multiples of 90 degrees. If the board can be covered, the result should be the cover of the board.

You can assume that 1 <= n,m,k <= 10. In tests worth a total of 4 points, additionally k <= 8. Among them, in tests worth a total of 3 points, additionally k <= 5.

**Input**

In the first line there are numbers n, m, k separated by spaces. Then follows the description of k blocks. The i-th block is described as n lines of m characters each, without spaces; a dot means a field not belonging to the block, and the i-th capital letter of the alphabet means a field belonging to the block. Between descriptions of successive blocks there is a line break.

**Output**

In the first line of output there should be one word YES or NO. The word YES means that the requested cover of the board exists, and the word NO means that it does not exist. If the answer is YES, then next should follow the description of the covered board, in the form of n lines of m characters each. Each character should be a capital letter indicating the identifier of the block that covers that field. If there are multiple correct ways to cover, you should print one, any of them.

**Sample input**

    4 5 4  
    A....  
    A....  
    AAAAA  
    .....  
      
    ..B..  
    .BBB.  
    .BBB.  
    .....  
      
    .....  
    .CC..  
    .CC..  
    .....  
      
    .....  
    .DD..  
    ..D..  
    .DDD.

**Sample output**

    YES  
    AAAAA  
    BBDDA  
    BBBDA  
    BBDDD
