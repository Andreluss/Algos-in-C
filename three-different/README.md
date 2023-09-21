Three different
---------------------------

Along the highway, which we can imagine as a straight line, there are `n` motels. We number the motels from 1 to `n` consecutively along the highway. Each motel belongs to some motel chain, which we describe by a number from the range from 1 to `n`.

Bajtek told Bitek that during his drive on the highway he stopped three times at a motel for the night, and each time he stayed at a motel of a different chain. Bitek wonders how far apart these three motels had to be. He is interested in the _closest_ and _farthest_ trio of motels.

Formally, Bitek would like to determine three motels A, B, C located in this order along the highway and belonging to different chains, for which:

1.  the maximum of the distances from A to B and from B to C is as small as possible (closest trio),
    
2.  the minimum of the distances from A to B and from B to C is as large as possible (farthest trio).

Write a program that will read from `stdin` the number `n` of motels, and then descriptions of motels in order along the highway - for each motel its chain number and distance from the beginning of the highway - and write to `stdout` two numbers - as above, the maximum of minimum distances and the minimum of maximum distances of three motels belonging to different chains. If Bajtek was wrong and no trio of motels of different chains occurs along the highway, both numbers should be equal to zero. The exact format of input and output is described by the example below (at the ends there are line breaks).

You can assume that the number of motels will be positive and will not exceed one million, and that all numbers on input will be non-negative and will fit in type `int`. It may happen that several motels are located at the same point on the highway, and even that Bajtek stayed at two or three such motels.

Only solutions with linear time complexity can count on maximum points. A solution that only writes one of the two required numbers correctly can get half of the points for the task.

**Input**

    9   
    1 2   
    2 6   
    2 9   
    1 13   
    1 17   
    3 20   
    1 26   
    3 27
    
    1 30


**Output**

    7 10

Explanation: Motels 3, 4 and 6 (those at distances 9, 13 and 20 from the beginning of the highway) all belong to different chains (respectively 2, 1 and 3) and minimize the maximum of distances. Motels 2, 6 and 9 (those at distances 6, 20 and 30 from the beginning of the highway) all belong to different chains (respectively 2, 3 and 1) and maximize the minimum of distances.
