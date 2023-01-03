/// @file intropage.h

/*! \mainpage jmatlib: a matrix library to manipulate big 2D matrices
*
* \section expl General explanation
* Matrix indexes are 32-bit unsigned integers so matrix size is in practice limited by the amount of memory.\n
* This library manages full, sparse and symmetric matrices using as less memory as possible.\n
* Matrix elements can be of any data type.\n
* Matrices can be read from/written to disk in a compact binary format.\n
* Rows and columns may have names (any C++ string).\n
* The characteristics of a matrix can be known without loading it into memory.\n
* Rows and columns can be read from disk, either by their names or numbers, without loading the complete matrix in memory.\n
* \n
* One example program is provided, jmat, which is a command-line interface that allows
* creation from/writing to jmatrices from CSV files, as long as any kind of matrix
* manipulation. See section Files.\n
* \n
* This library is used by parpamlib, a library to implement in parallel the Partiioning Around Medoids (PAM) clustering method
* that can be found in https://github.com/JdMDE/ppamlib\n
*\n
* Its code with interface modifications is also used inside the jmatrix R package (https://cran.r-project.org/web/packages/jmatrix/index.html)
*/

