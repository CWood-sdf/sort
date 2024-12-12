template <int l, int r>
concept Smaller = l < r;

/// array class. stores elements as parameter pack
template <int... v> class Array {};

template <int n, int... v> class Array<n, v...> {
public:
  // having a size value is helpful
  static constexpr int size = 1 + Array<v...>::size;
};

// 1 and 0 element arrays
template <int n> class Array<n> {
public:
  static constexpr int size = 1;
};
template <> class Array<> {
public:
  static constexpr int size = 0;
};

// just a helper class to merge two arrays
// NOTE: MergeArray is only defined for Array<n>, Array<...>
template <class L, class R> class MergeArray {};
template <int L, int... R> class MergeArray<Array<L>, Array<R...>> {
public:
  typedef Array<L, R...> Output;
};

// a helper class that merges two sorted arrays together
template <class L, class R> class MergeSorted {};

// if we are merging sorted arrays and one is empty, just return the other
template <class L> class MergeSorted<L, Array<>> {
public:
  typedef L Output;
};
template <class R> class MergeSorted<Array<>, R> {
public:
  typedef R Output;
};

// merge sort arrays by taking the first element of two sorted arrays
template <int nl, int... l, int nr, int... r>
  requires Smaller<nl, nr>
class MergeSorted<Array<nl, l...>, Array<nr, r...>> {
  // merge subarrays
  typedef MergeSorted<Array<l...>, Array<nr, r...>> FinishMerge;

public:
  // combine for final output
  typedef typename MergeArray<Array<nl>, typename FinishMerge::Output>::Output
      Output;
};

template <int nl, int... l, int nr, int... r>
  requires(!Smaller<nl, nr>)
class MergeSorted<Array<nl, l...>, Array<nr, r...>> {
  typedef MergeSorted<Array<nl, l...>, Array<r...>> FinishMerge;

public:
  typedef typename MergeArray<Array<nr>, typename FinishMerge::Output>::Output
      Output;
};

// a class that takes array A and splits it into left side of size n, and right
// side of size A.size - n
template <typename A, int n> class SplitArr {};

// basic case if n > 1, keep splitting
template <int base, int... v, int n> class SplitArr<Array<base, v...>, n> {
  // remove first element, split more
  typedef SplitArr<Array<v...>, n - 1> Split;

public:
  // merge output left
  typedef
      typename MergeArray<Array<base>, typename Split::OutLeft>::Output OutLeft;
  // merge out right
  typedef typename Split::OutRight OutRight;
};

// base case of n=1: just forward first element, then take off rest as right
template <int base, int... v> class SplitArr<Array<base, v...>, 1> {
public:
  typedef Array<base> OutLeft;
  typedef Array<v...> OutRight;
};

/// MergeSorts Array<...>
template <class N> class MergeSort {};

// base case size=0
template <> class MergeSort<Array<>> {
public:
  typedef Array<> Output;
};

// base case size=1
template <int n> class MergeSort<Array<n>> {
public:
  typedef Array<n> Output;
};

// recursive case
template <int... n> class MergeSort<Array<n...>> {
  // split in half
  typedef SplitArr<Array<n...>, Array<n...>::size / 2> Split;
  // merge left and right
  typedef typename MergeSort<typename Split::OutLeft>::Output LeftSorted;
  typedef typename MergeSort<typename Split::OutRight>::Output RightSorted;

public:
  // combine
  typedef typename MergeSorted<LeftSorted, RightSorted>::Output Output;
};

int main() {
  typedef typename MergeSort<Array<7, 8, 3, 4, 67, 9>>::Output S;
  // note that S is Array<3, 4, 7, 8, 9, 67>
  S arr;
}
