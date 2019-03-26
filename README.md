# What's in here?
Solutions to various problems that I came across.
## Usage
Requirements:  
 * git
 * meson build system (for UTs)
 * ninja (for UTs)
 * c++ compiler (tested on gcc 7 and clang 7)

To use the header only lib:

    cd ~
    git clone https://github.com/paler123/algorithms_and_riddles
 
And copy the headers wherever you need them.  

To run UTs, after cloning:

    cd algorithm_and_riddles
    meson build
    cd build
    ninja test
# Problems
## max_matrix_sums
This project consists of a solution to a simple dynamic programming problem and some compile time tests for it.

### Problem statement
Given a NxM matrix, find maximum sum of elements, with following constraints:
 * for each row exactly one element of that row is included in the sum
 * if element at (i, j) (in i-th row, j-th column) is included in the sum, then one at (i + 1, j) is not.

### Solution
We can consider a solution to the problem, to be a sequence of column indices, such that consecutive elements are different in the sequence,
that is the solution can be identified as one of the elements of the set `P[N] = { (a[1], a[2],..., a[N]) | a[i] != a[i+1] }`.
Let now `R[N]` be a relation on `P[N]` defined in the following way: `aR[N]b <=> a[N] = b[N]`. Of course `R[N]` is an equivalence relation.
Now we can consider elements with maximum sum of matrix elements (path A = (a[1], a[2], ..., a[N]), has sum S(A) := M(1, a[1]) + M(2, a[2]) +... M(N, a[N]) ) 
among the classes abstraction classes of `R[N]` (informally: "best" paths through given
element of last row of matrix).  
Let's now consider the set `P[N - 1]`, with corresponding relation `R[N - 1]`, and consider elements with highest sum in each class of abstraction
of `R[N - 1]`, and let's take maximizing elements from two classes of abstraction with highest sums. That is we have two paths given by sequences
`A = (a[1],..., a[N-1]), B = (b[1],..., b[N -1])`, such that:
 1) `a[N - 1] != b[N - 1]` (since they come from different classes of abstraction
 2) for any valid path `C = (c[1], c[2],..., c[N - 2], a[N - 1]), S(C) <= S(A)`, symilarly for B
 3) for any valid path `C = (c[1], c[2],..., c[N - 1])`, such that `b[N - 1] != c[N - 1] != a[N - 1], S(B) >= S(C) <= S(A)`.

Now we can formulate following lemma

#### Lemma
Given two best paths `A = (a1, a2, ..., a[N-1]), B = (b1,..., B[N - 1])` as above and column index `j`, either `(A, j)` or `(B, j)` is the best path
going through j-th column in N-th row.

#### Proof of the lemma
Let `(C, j) = (c[1], ..., c[N - 1]), j)` be any valid path going through `(N, j)`.
Now if `c[N - 1] = a[N - 1]`, then `(A, j)` is valid and from 2) `S(C) <= S(A, j`), same if `c[N -1] = b[N - 1]`, then `S(C) <= S(B, j)`, and `(B, j)` is valid.
Now, let's assume that `a[N - 1] != c[N - 1] != b[N -1]`. Then at least one of paths `(A, j)` or `(B, j)` is valid (indeed `(A, j) is invalid <=> a[N - 1] = j`, but then `(B, j)` is valid, 
similarly we can show that if `(B, j)` is invalid then `(A, j)` is valid). Suppose that `(A, j)` is valid `=> S(A, j) >= S(C, j)`, same if `(B, j)` is valid.

### Algorithm
With the lemma in mind it's easy to now observe that given two best paths (with differnt endpoints) A and B for (N-1 x M) submatrix of initial problem we can easily
construct two best paths (with different endpoints) for the N x M matrix, and one of them (with higher S) realizes the max in question.  
So we can construct the solution for the whole matrix iteratively, algorithm can be written in pseudo-code in the following manner:
    
     input : N x M matrix A
     best_paths_in_row = two_highest_elements_of_first_row(A)
     for row 2, 3..., N
         best_paths_in_row = top_two_paths(row, best_paths_in_row)
     return better_path(best_paths_in_row)

Algorithm underneath `top_two_paths` is trivially derived from the discussed solution, we just need to go through each element of the row,
and find best path through the element (which based on lemma we derive from 2 best paths in previous row), updating two best paths as we go.

### Complexity
Since each element of the matrix is inspected once, and there is O(1) operations done per element (index comparisons and summations of sum values),
the complexity is O(NM)- optimal, since each element has to inspected at least once. Required auxilarry space is O(1), since only values and indices of last elements of top 2 paths need to be kept during
the calculations. 

### Notes on the code
I've aimed to provide a "generic" solution, that is working with any matrix-like container, that's why the main function used for solving the problem (and subsequent functions called by it) `MaxSum::solve(Matrix const & input)` is templated on the matrix type. SFINAE has been used to assure matrixness of the type used to call `solve` with (traits defined in `matrix_type_traits.hpp`. The existence of 2-d indexing operator is checked and arithmeticity of the return type, having `cols()` and `rows()` member functions returning `std::size_t` is also required (easy to relax that constraint and allow of integer types, but I did not bother).  
`constepxr` specifier was used on appropriate methods, so that the tests can (and in fact do) run during compilation.

## Regexes
A simplified regex implementation, covering: only exact matching, character matching, '.' wildcard, '\*' for matching 0 or more occurences, [] character groups (all chars between the brackets are taking literally, so no ranges). That being said, the implementation is easily extensible to cover more features.
