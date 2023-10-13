# jmatlib
## A matrix library to manipulate big 2D matrices.
> This library manages full, sparse and symmetric matrices using as less memory as possible.  
> Matrix elements can be of any data type.  
> Matrix indexes are a 32-bit unsigned integers so matrix size is in practice limited by the amount of memory.  
> Matrices can be read from/written to disk in a compact binary format.  
> Rows and columns may have names (any C++ string).  
> The characteristics of a matrix can be known without loading it into memory.  
> Rows and columns can be read from disk, either by their names or numbers, without loading the complete matrix in memory.  

One example program is provided, jmat, which is a command-line interface that allows
creation from/writing to jmatrices from CSV files, as long as any kind of matrix
manipulation.

This library is used by ppamlib, a library to implement in parallel the Partiioning Around Medoids (PAM) clustering method
that can be found in https://github.com/JdMDE/ppamlib

Its code with interface modifications is also used inside the jmatrix R package (https://CRAN.R-project.org/package=jmatrix)

See documentation at subdirectory html, file index.html

## Compilation
cmake in version >=3.5 is needed (even former versions might work if you modifiy the cmake_minimum_required
in the CMakeLists.txt files)

As a general rule, create a folder to build inside the jmatlib main folder:

mkdir build  
cd build  
ccmake ../  
make

an then, as root or with sudo,

make install

The ccmake menu shows the option CHECK_MATRIX_BOUNDS to OFF by default. Changing it to ON adds the compilation
flag -DWITH_CHECKS_MATRIX with compiles the Get and Set of the matrices with bound checkings and shows a compilation
message warning you about the safer but slightly slower code. If you access the matrix elements billions of times
this may be significant.

