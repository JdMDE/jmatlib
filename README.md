jmatrixlib: a matrix library to manipulate big 2D matrices.
Matrix indexes are a 32-bit unsigned integers so matrix size is in practice limited by the amount of memory.
This library manages full, sparse and symmetric matrices using as less memory as possible.
Matrix elements can be of any data type.
Matrices can be read from/written to disk in a compact binary format.
Rows and columns may have names (any C++ string).
The characteristics of a matrix can be known without loading it into memory.
Rows and columns can be read from disk, either by their names or numbers, without loading the complete matrix in memory.

One example program is provided, jmat, which is a command-line interface that allows
creation from/writing to jmatrices from CSV files, as long as any kind of matrix
manipulation.

This library is used by ppamlib, a library to implement in parallel the Partiioning Around Medoids (PAM) clustering method
that can be found in https://github.com/JdMDE/ppamlib

Its code with interface modifications is also used inside the jmatrix R package (https://CRAN.R-project.org/package=jmatrix)

See documentation at subdirectory html, file index.html

