# CppLinAlg

*Small library for doing popular linear algebra functions for machine learning.*

## Installation

To build the test application:

```
$ mkdir build
$ cd build
$ cmake ../
$ cmake --build .
```

## Usage:

To run the test application:

```
$ ./CppLinAlg
```

## Documentation

- Matrix
    - Matrix(int r, int c)
    - Matrix(int r, int c, double* d)
    - Matrix* add(Matrix* m)
    - Matrix* dot(Matrix* m)
    - Matrix* transpose()
    - void set(int r, int c, double v)
    - double get(int r, int c)
    - void print()
