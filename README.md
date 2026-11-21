# Assignment 3 — Empirical Analysis of Sorting Algorithms

Course: Analysis of Algorithms

## Algorithms
- Selection Sort
- Bubble Sort (with early-stop optimization)
- Quick Sort (last-element pivot)
- Merge Sort

## Required input cases
Each algorithm is tested with four inputs:
1. N=5, sorted ascending
2. N=5, reverse sorted
3. N=100, sorted ascending
4. N=100, reverse sorted

Total: 4 algorithms × 4 input cases = 16 algorithm/input combinations.

## Measurements
- Three measured runs per combination.
- Each measured run repeats the sort 10,000 times to make microsecond measurements more stable.
- Input copying is outside the timed section.
- The reported time is the average time for one sort, in microseconds.
- The program checks every output using `std::is_sorted`.
- Comparisons and swaps are also recorded as additional analysis.

## Auxiliary space
| Algorithm | Auxiliary space |
|---|---|
| Selection Sort | O(1) |
| Bubble Sort | O(1) |
| Quick Sort | O(log N) average recursion stack; O(N) worst case with this pivot strategy |
| Merge Sort | O(N) temporary array, plus O(log N) recursion stack |

## Compile and run
With g++:
```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o assignment3
./assignment3
```
On Windows:
```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o assignment3.exe
assignment3.exe
```

The program creates `assignment3_results.csv`.

## Important implementation note
Quick Sort uses the last array element as its pivot. This is intentionally simple and makes the effect of sorted/reverse-sorted input visible: for these input patterns, this pivot choice can produce highly unbalanced partitions and O(N²) time. Different pivot strategies may behave differently.

## GitHub
Create a new public GitHub repository for Assignment 3 and paste its URL into the report before submission.
