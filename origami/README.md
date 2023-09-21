Origami
----------

**Note:** The compilation options are the same as usual. We allow `-lm`.

Write a program for origami fans to check how many layers a cleverly folded sheet of paper has at a given point. The program should read from `stdin` the description of creating subsequent origami sheets and questions about the number of layers at given points of given sheets. In the first line of input there are two numbers `n` and `q` denoting respectively the number of sheets and the number of questions. Then on the input there are _n_ lines describing the next sheets and _q_ questions about the number of layers of the given sheets at the given points.

The description of the _i_-th sheet (1 ≤ _i_ ≤ _n_) is in line _i_ + 1. This description has one of three forms:

1.  `P x1 y1 x2 y2`
2.  `K x y r`
3.  `Z k x1 y1 x2 y2`

The first form represents a closed rectangle with sides parallel to the axes of the coordinate system with the lower left corner at point _P_1 = (_x_1, _y_1) and the upper right at point _P_2 = (_x_2, _y_2). Point _P_1 must therefore be strictly to the left and down from point _P_2. When you pin this sheet inside (or on the edges) of the rectangle, the sheet will be pierced 1 time, in other cases 0 times.

The second form represents a closed circle with center at point (_x_, _y_) and radius _r_.

The third form represents a sheet obtained by folding the _k_-th sheet (1 ≤ _k_ < i) along a line passing through points _P_1 = (_x_1, _y_1) and _P_1 = (_x_2, _y_2) - these must be different points. The paper is folded in such a way that from the right side of the line (looking in the direction from _P_1 to _P_2) it is transferred to the left. For such a folded sheet, piercing on the right side of the line gives 0 layers; piercing exactly on the line should return as much as piercing the sheet before folding; on the left side - as much as before folding plus piercing the unfolded sheet at the point that overlapped with the piercing point.

The _j_-th query (1 ≤ _j_ ≤ _q_) is in line _n_ + _j_ + 1 and has the form `k x y`, where _k_ is the number of the sheet (1 ≤ _k_ ≤ _n_), and (_x_, _y_) is the point where we pin to determine the number of layers of the _k_-th sheet at this point.

The program should write to `stdout` _q_ lines - answers to queries about the number of layers.

**Remarks:** Numbers denoting coordinates of points or radius of circle are floating-point numbers. There are no restrictions on time complexity of solution. In particular, exponential solutions will be allowed. Our tests are not malicious with respect to precision of calculations.

**Sample input**

    4 5
    P 0 0 2.5 1
    Z 1 0.5 0 0.5 1
    K 0 1 5
    Z 3 0 1 -1 0
    1 1.5 0.5
    2 1.5 0.5
    2 0 0.5
    3 4 4
    4 4 4

**Sample output**
    
    1
    0
    2
    1
    2
