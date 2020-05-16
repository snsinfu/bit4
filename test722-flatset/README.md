C++ random insertion to sorted sequence in different algorithms.

The benchmarks below ran on Fedora 31 / Ryzen 3600 with
`EXTRA_CXXFLAGS=-march=native`. The characteristics differ among environments.
On MBP 13 inch (2016) heap is the fastest. I can't conclude which is the best
algorithm.


### g++ 9.3.1 (libstdc++)

`boost::flatset` is the fastest.

```
raw: 68.9842 ns/op
set: 189.939 ns/op
flatset: 131.53 ns/op
sortuniq: 171.207 ns/op
heap: 222.47 ns/op
```


### clang++ 9.0.1 (libstd++)

`sort-uniq` is the fastest.

```
raw: 24.1745 ns/op
set: 155.472 ns/op
flatset: 194.176 ns/op
sortuniq: 121.59 ns/op
heap: 166.036 ns/op
```


### clang++ 9.0.1 (libc++)

`std::set` and `sort-uniq` are the fastest.

```
raw: 24.0818 ns/op
set: 136.52 ns/op
flatset: 188.012 ns/op
sortuniq: 132.385 ns/op
heap: 192.141 ns/op
```
