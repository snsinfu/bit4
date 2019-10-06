Motivated by the recent HN post:

- https://news.ycombinator.com/item?id=21095766
- https://codingnest.com/modern-sat-solvers-fast-neat-and-underused-part-1-5-of-n/

## To run

First install the master-keying fork of the minisat:

```
git clone https://github.com/master-keying/minisat.git
cd minisat
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=(path-to-bit4)/test878-minisat/packages
cd build
make install
```

Then build test878:

```
cmake -H. -Bbuild -DCMAKE_FIND_ROOT_PATH=packages
cd build
make
```

and run:

```
$ ./main
A = 0
B = 1
C = 1
```
