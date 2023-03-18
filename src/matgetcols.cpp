/*
 *
 * Copyright (C) 2022 Juan Domingo (Juan.Domingo@uv.es)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include "headers/fullmatrix.h"
#include "headers/sparsematrix.h"
#include "headers/symmetricmatrix.h"
#include "headers/matmetadata.h"

extern unsigned char DEB;

/***********************************************************
 * 
 * Functions related with getting cols from the jmatrix binary format
 *
 **********************************************************/
 
// Functions to read one or more columns. These are more complex, since jmatrix format is primarily row-oriented.

// Auxiliary functions to read one or many columns of the matrices stored in binary jmatrix format without reading the full matrix in memory

template <typename T>
void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<T> &v)
{
 T *data = new T [nrows]; 
 
 std::ifstream f(fname.c_str());
 // This places us at the nc column of first row (row 0)
 size_t offset=HEADER_SIZE+nc*sizeof(T);
 for (indextype r=0; r<nrows; r++)
 {
  f.seekg(offset,std::ios::beg);
  // We read one element
  f.read((char *)(&data[r]),sizeof(T));
  // This jumps exactly one row, up to just before the nc column of next row.
  // The number of bytes in one row is the number of columns multiplied by the size of one element.
  offset += (ncols*sizeof(T));
 }
 f.close();

 v=std::vector<T>(nrows,T(0));
 for (size_t r=0; r<nrows; r++)
  v[r]=data[r];

 delete[] data;

}

template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned char> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<char> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned short> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<short> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned int> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<int> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned long> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<long> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned long long> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<long long> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<float> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<double> &v);
template void GetJustOneColumnFromFull(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<long double> &v);

template <typename T>
void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<T>> &m)
{
 T data;
 
 std::ifstream f(fname.c_str());
 size_t offset;
 m.clear();
 m=std::vector<std::vector<T>>(nrows);
 for (size_t r=0;r<nrows;r++)
   m[r]=std::vector<T>(ncs.size(),T(0));

 for (size_t t=0; t<ncs.size(); t++)
 {
  // This places us at the requested column of the first row
  offset=HEADER_SIZE+ncs[t]*sizeof(T);
  for (indextype r=0; r<nrows; r++)
  {
   f.seekg(offset,std::ios::beg);
   // We read one element
   f.read((char *)(&data),sizeof(T));

   m[r][t]=data;
   // This jumps exactly one row, up to just before the nc column of next row.
   // The number of bytes in one row is the number of columns multiplied by the size of one element.
   offset += (ncols*sizeof(T));
  }
  
 }
 f.close();
}

template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<unsigned char>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<char>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<unsigned short>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<short>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<unsigned int>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<int>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<unsigned long>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<long>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<unsigned long long>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<long long>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<float>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<double>> &m);
template void GetManyColumnsFromFull(std::string fname,std::vector<indextype> ncs,indextype nrows,indextype ncols,std::vector<std::vector<long double>> &m);

template <typename T>
void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<T> &v)
{
 T *data = new T [nrows];
 indextype *idata = new indextype [ncols];  // This is by excess. Most rows will not have ncols real columns, since the matrix is sparse.
 
 std::ifstream f(fname.c_str());
 
 // Start of row nr is at the end of former rows, each of them having a different number of elements
 // we must go to the start of each row to find out how many...
 size_t offset=HEADER_SIZE;
 indextype c,ncr;
 for (indextype r=0; r<nrows; r++)
 {
  f.seekg(offset,std::ios::beg);
  // At the beginning of row r: read how many element there are in it (ncr):
  f.read((char *)&ncr,sizeof(indextype));
  // Read the indices of this row
  f.read((char *)idata,ncr*sizeof(indextype));
  // See if the index of the column we are looking for is there (a while loop with premature exit is OK, indices are ordered)
  c=0;
  while ( (c<ncr) && (idata[c]<nc) )
   c++;
  if ((c>=ncr) || (idata[c]!=nc))
   data[r]=0;
  else
  {
   // offset is now the beginning of the current row. We jump the indices (and the number of non-null indices, which is the reason of '+1' in ncr+1)
   // and also jump the first c data, too.
   f.seekg(offset+((ncr+1)*sizeof(indextype))+(c*sizeof(T)),std::ios::beg);
   f.read((char *)(&data[r]),sizeof(T));
  }
  // We advance up to the beginning of next row. The size of this row consists on the nc indices, plus the number of non-null indices, plut the number of present values.
  offset += ((ncr+1)*sizeof(indextype)+(ncr*sizeof(T)));
 }
 
 f.close();

 // Clear the vector to be returned
 v=std::vector<T>(ncols,T(0));
 for (size_t r=0; r<nrows; r++)
  v[r]=data[r];
  
 delete[] data;
 delete[] idata;
}

template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned char> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<char> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned short> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<short> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned int> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<int> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned long> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<long> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<unsigned long long> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<long long> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<float> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<double> &v);
template void GetJustOneColumnFromSparse(std::string fname,indextype nc,indextype nrows,indextype ncols,std::vector<long double> &v);

// Auxiliary function to get columns from a sparse matrix whose indexes are in vector nc. Columns are left consecutively in the passed matrix
// in the same order as in vector nc, so if vector is not ordered, resulting columns will be unordered, too.
template <typename T>
void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<T>> &m)
{
 // Differently to the function to get rows, here we need the offsets of absolutely all rows, since at least one element will be extracted from each of them
 std::vector<size_t> offsets(nrows,HEADER_SIZE);
 
 std::ifstream f(fname.c_str()); 
 size_t offset=HEADER_SIZE;
 
 indextype ncr;
 for (size_t t=0;t<nrows;t++)
 {
  offsets[t]=offset;
  f.seekg(offset,std::ios::beg);
  f.read((char *)&ncr,sizeof(indextype));
  offset += ((ncr+1)*sizeof(indextype)+ncr*sizeof(T));
 }
 
 // These are temporary arrays to store the indices and values of a row.
 // Since each row stores a different number of values, we book sufficient space for the biggest case, the number of columns
 // (this is the case of an absolutely unsparse row, no zeros in it)
 indextype *idata = new indextype [ncols];
 T *data = new T [ncols];
 
 m.clear();
 m=std::vector<std::vector<T>>(nrows);
 for (size_t r=0;r<nrows;r++)
   m[r]=std::vector<T>(nc.size(),T(0));

 // Here we run through all rows, getting from each of them the requested columns
 for (size_t t=0; t<nrows; t++)
 {
  f.seekg(offsets[t],std::ios::beg);
  // At the beginning of row r: read how many element there are in it (ncr):
  f.read((char *)&ncr,sizeof(indextype));
  // Let's read its indices... No problem with size, ncr is always smaller than ncols
  f.read((char *)idata,ncr*sizeof(indextype));
  // ... and let's read the data
  f.read((char *)data,ncr*sizeof(T));
  
  // Fill the appropriate places of the matrix (those dictated by the indices in idata)
  for (size_t c=0; c<nc.size(); c++)
  {
   size_t q=0;
   while ((q<ncr) && (idata[q]!=nc[c]))
    q++;
   if (q<ncr)
    m[t][c]=data[q];
  }
 }
 delete[] data;
 delete[] idata;
  
 f.close();
}

template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<unsigned char>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<char>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<unsigned short>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<short>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<unsigned int>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<int>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<unsigned long>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<long>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<unsigned long long>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<long long>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<float>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<double>> &m);
template void GetManyColumnsFromSparse(std::string fname,std::vector<indextype> nc,indextype nrows,indextype ncols,std::vector<std::vector<long double>> &m);

// GetJustOneColumnFromSymmetric is indeed the same as GetJustOneRowFromSymmetric, but it is easier to have it here by name
template <typename T>
void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<T> &v)
{
 T *data = new T [ncols]; 
 
 std::ifstream f(fname.c_str());
 
 // This is the beginning of row nr in the binary symmetric data
 size_t offset=HEADER_SIZE+((nr*(nr+1))/2)*sizeof(T);
 f.seekg(offset,std::ios::beg);
 
 // Here we read the nr+1 values present in that row, including the (nr,nr) at the main diagonal (which will be normally 0 in a dissimilarity matrix)
 f.read((char *)data,(nr+1)*sizeof(T));
 
 // The rest of this row is not physically after; we must read the rest of the column that starts in the diagonal, down to the end.
 // It would be clearer to write r=nr+1; r<nrows; r++ but we haven't a variable nrows. But we don't need: symmetric matrices are square.
 offset=HEADER_SIZE+(nr+((nr+1)*(nr+2))/2)*sizeof(T);
 for (indextype r=nr+1; r<ncols; r++)
 {
  f.seekg(offset,std::ios::beg);
  // Here we read just one value...
  f.read((char *)&(data[r]),sizeof(T));
  // and advance to the next row, taking into account that each row has a different number of really stored columns, which is r+1
  offset += ((r+1)*sizeof(T));
 }
 f.close();

 // Clear the vector to be returned
 v=std::vector<T>(ncols,T(0));
 for (size_t c=0; c<ncols; c++)
  v[c]=data[c];
  
 delete[] data;
}

template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned char> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<char> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned short> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<short> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned int> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<int> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<long> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long long> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<long long> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<float> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<double> &v);
template void GetJustOneColumnFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<long double> &v);

// Again GetManyColumnsFromSymmetric is indeed the same as GetManyRowsFromSymmetric, but it is easier to have it here by name
// This explains the apparent inconsistency in the names of the variables.
// Auxiliary function to get from a symmetric matrix the columns whose indexes are in vector nr. Columns are left consecutively in the passed matrix
// in the same order as in vector r, so if vector is not ordered, resulting columns will be unordered, too.
template <typename T>
void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<T>> &m)
{
 T *data = new T [ncols]; 
 
 std::ifstream f(fname.c_str());
 size_t offset;

 m.clear();
 m=std::vector<std::vector<T>>(ncols);
 for (size_t r=0;r<ncols;r++)
   m[r]=std::vector<T>(nr.size(),T(0));

 for (size_t t=0; t<nr.size(); t++)
 {
  // This is the beginning of row nr in the binary symmetric data
  offset=HEADER_SIZE+((nr[t]*(nr[t]+1))/2)*sizeof(T);
  f.seekg(offset,std::ios::beg);
  // Here we read the nr+1 values present in that row, including the (nr,nr) at the main diagonal (which will be normally 0 in a dissimilarity matrix)
  f.read((char *)data,(nr[t]+1)*sizeof(T));
 
  for (size_t c=0; c<nr[t]+1; c++)
   m[c][t]=data[c];                   // This is the difference w.r.t. the original, m is filled transposed
   
  // The rest of this row is not physically after; we must read the rest of the column that starts in the diagonal, down to the end.
  // It would be clearer to write r=nr+1; r<nrows; r++ but we haven't a variable nrows. But we don't need: symmetric matrices are square.
  offset=HEADER_SIZE+(nr[t]+((nr[t]+1)*(nr[t]+2))/2)*sizeof(T);
  for (indextype r=nr[t]+1; r<ncols; r++)
  {
   f.seekg(offset,std::ios::beg);
   // Here we read just one value...
   f.read((char *)&(data[r]),sizeof(T));
   // and advance to the next row, taking into account that each row has a different number of really stored columns, which is r+1
   offset += ((r+1)*sizeof(T));
  }
  
  for (size_t c=nr[t]+1; c<ncols; c++)
   m[c][t]=data[c];                  // This is the difference w.r.t. the original, m is filled transposed
 }
 
 f.close();
 
 delete[] data;
}

template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<unsigned char>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<char>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<unsigned short>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<short>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<unsigned int>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<int>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<unsigned long>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<long>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<unsigned long long>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<long long>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<float>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<double>> &m);
template void GetManyColumnsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<long double>> &m);

using namespace std;

template <typename T>
void NCEmit(string ofile,vector<T> &v,unsigned char mtype,const vector<string> &rownames,string colname)
{
 if ((mtype==MTYPEFULL) || (mtype==MTYPESYMMETRIC))
 {
   FullMatrix<T> V(v.size(),1);
   for (size_t i=0;i<v.size();i++)
    V.Set(i,0,v[i]);
   if (rownames.size()>0)
    V.SetRowNames(rownames);
   if (colname!="")
   {
    vector<string> dummy;
    dummy.push_back(colname);
    V.SetColNames(dummy);
   }
   V.WriteBin(ofile);
  }
  else
  {
   SparseMatrix<T> V(v.size(),1);
   for (size_t i=0;i<v.size();i++)
    V.Set(i,0,v[i]);
   if (rownames.size()>0)
    V.SetRowNames(rownames);
   if (colname!="")
   {
    vector<string> dummy;
    dummy.push_back(colname);
    V.SetColNames(dummy);
   }
   V.WriteBin(ofile);
  }
}

template <typename T>
void NCSEmit(string ofile,vector<vector<T>> &v,unsigned char mtype,const vector<string> &rownames,vector<string> &colnames)
{
 if ((mtype==MTYPEFULL) || (mtype==MTYPESYMMETRIC))
 {
   FullMatrix<T> V(v.size(),v[0].size());
   for (size_t i=0;i<v.size();i++)
    for (size_t j=0;j<v[0].size();j++)
    V.Set(i,j,v[i][j]);
   if (rownames.size()>0)
    V.SetRowNames(rownames);
   if (colnames.size()!=0)
    V.SetColNames(colnames);
   V.WriteBin(ofile);
 }
 else
 {
   SparseMatrix<T> V(v.size(),v[0].size());
   for (size_t i=0;i<v.size();i++)
    for (size_t j=0;j<v[0].size();j++)
    V.Set(i,j,v[i][j]);
   if (rownames.size()>0)
    V.SetRowNames(rownames);
   if (colnames.size()!=0)
    V.SetColNames(colnames);
   V.WriteBin(ofile);
 }
}

template <typename T>
void NCSelect(string inmat,indextype ncol,indextype nrows,indextype ncols,string ofile,unsigned char mtype)
{
 vector<T> v;
 vector<string> rownames,colnames;
 InternalGetBinNames(inmat,ROW_NAMES | COL_NAMES,rownames,colnames);
 string colname="";
 if (colnames.size()>0)
   colname=colnames[ncol];
 switch (mtype)
 {
  case MTYPEFULL:
        GetJustOneColumnFromFull<T>(inmat,ncol,nrows,ncols,v);
        NCEmit<T>(ofile,v,MTYPEFULL,rownames,colname);
        break;
  case MTYPESPARSE:
        GetJustOneColumnFromSparse<T>(inmat,ncol,nrows,ncols,v);
        NCEmit<T>(ofile,v,MTYPESPARSE,rownames,colname);
        break;
  case MTYPESYMMETRIC:
        GetJustOneColumnFromSymmetric<T>(inmat,ncol,ncols,v);
        NCEmit<T>(ofile,v,MTYPESYMMETRIC,rownames,colname);
        break;
  default: cerr << "Unexpected error: unknown matrix type.\n"; exit(1); break;
 }
}

template <typename T>
void NCSSelect(string inmat,vector<indextype> lcols,indextype nrows,indextype ncols,string ofile,unsigned char mtype)
{
 vector<vector<T>> v;
 vector<string> rownames,colnames;
 InternalGetBinNames(inmat,ROW_NAMES | COL_NAMES,rownames,colnames);
 vector<string> sel_colnames;
 if (colnames.size()>0)
  for (size_t c=0;c<lcols.size();c++)
   sel_colnames.push_back(colnames[lcols[c]]);
 switch (mtype)
 {
  case MTYPEFULL:
        GetManyColumnsFromFull<T>(inmat,lcols,nrows,ncols,v);
        NCSEmit<T>(ofile,v,MTYPEFULL,rownames,sel_colnames);
        break;
  case MTYPESPARSE:
        GetManyColumnsFromSparse<T>(inmat,lcols,nrows,ncols,v);
        NCSEmit<T>(ofile,v,MTYPESPARSE,rownames,sel_colnames);
        break;
  case MTYPESYMMETRIC:
        GetManyColumnsFromSymmetric<T>(inmat,lcols,ncols,v);
        NCSEmit<T>(ofile,v,MTYPESYMMETRIC,rownames,sel_colnames);
        break;
  default: cerr << "Unexpected error: unknown matrix type.\n"; exit(1); break;
 }
}

void JGetNumCol(string iname,string oname,indextype numcol)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 if (numcol>ncols-1)
  JMatrixStop("Requested column is beyond the limit of the matrix.\n");

 switch (ctype)
 {
  case UCTYPE:
        NCSelect<unsigned char>(iname,numcol,nrows,ncols,oname,mtype); break;
  case SCTYPE:
        NCSelect<char>(iname,numcol,nrows,ncols,oname,mtype); break;
  case USTYPE:
        NCSelect<unsigned short>(iname,numcol,nrows,ncols,oname,mtype); break;
  case SSTYPE:
        NCSelect<short>(iname,numcol,nrows,ncols,oname,mtype); break;
  case UITYPE:
        NCSelect<unsigned int>(iname,numcol,nrows,ncols,oname,mtype); break;
  case SITYPE:
        NCSelect<int>(iname,numcol,nrows,ncols,oname,mtype); break;
  case ULTYPE:
        NCSelect<unsigned long>(iname,numcol,nrows,ncols,oname,mtype); break;
  case SLTYPE:
        NCSelect<long>(iname,numcol,nrows,ncols,oname,mtype); break;
  case ULLTYPE:
        NCSelect<unsigned long long>(iname,numcol,nrows,ncols,oname,mtype); break;
  case SLLTYPE:
        NCSelect<long long>(iname,numcol,nrows,ncols,oname,mtype); break;
  case FTYPE:
        NCSelect<float>(iname,numcol,nrows,ncols,oname,mtype); break;
  case DTYPE:
        NCSelect<double>(iname,numcol,nrows,ncols,oname,mtype); break;
  case LDTYPE:
        NCSelect<long double>(iname,numcol,nrows,ncols,oname,mtype); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}

void JGetNameCol(string iname,string oname,string namecol)
{
 vector<string> rnames,cnames;
 InternalGetBinNames(iname,COL_NAMES,rnames,cnames);
 size_t i=0;
 while ( (i<cnames.size()) && (cnames[i]!=namecol))
  i++;
 if (i>=cnames.size())
 {
  cerr << "Error: no column with name '" << namecol << "' found in matrix contained in file " << iname << ".\n";
  exit(1);
 }
 JGetNumCol(iname,oname,indextype(i));
}

void JGetNumsCol(std::string iname,std::string oname,std::vector<indextype> lcols)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 for (size_t c=0;c<lcols.size();c++)
  if (lcols[c]>=ncols)
  JMatrixStop("At least one of the requested columns is beyond the limit of the matrix.\n");

 switch (ctype)
 {
  case UCTYPE:
        NCSSelect<unsigned char>(iname,lcols,nrows,ncols,oname,mtype); break;
  case SCTYPE:
        NCSSelect<char>(iname,lcols,nrows,ncols,oname,mtype); break;
  case USTYPE:
        NCSSelect<unsigned short>(iname,lcols,nrows,ncols,oname,mtype); break;
  case SSTYPE:
        NCSSelect<short>(iname,lcols,nrows,ncols,oname,mtype); break;
  case UITYPE:
        NCSSelect<unsigned int>(iname,lcols,nrows,ncols,oname,mtype); break;
  case SITYPE:
        NCSSelect<int>(iname,lcols,nrows,ncols,oname,mtype); break;
  case ULTYPE:
        NCSSelect<unsigned long>(iname,lcols,nrows,ncols,oname,mtype); break;
  case SLTYPE:
        NCSSelect<long>(iname,lcols,nrows,ncols,oname,mtype); break;
  case ULLTYPE:
        NCSSelect<unsigned long long>(iname,lcols,nrows,ncols,oname,mtype); break;
  case SLLTYPE:
        NCSSelect<long long>(iname,lcols,nrows,ncols,oname,mtype); break;
  case FTYPE:
        NCSSelect<float>(iname,lcols,nrows,ncols,oname,mtype); break;
  case DTYPE:
        NCSSelect<double>(iname,lcols,nrows,ncols,oname,mtype); break;
  case LDTYPE:
        NCSSelect<long double>(iname,lcols,nrows,ncols,oname,mtype); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}

void JGetNamesCol(std::string iname,std::string oname,std::vector<string> lcols)
{
 vector<indextype> lncols;
 vector<string> rnames,cnames;
 InternalGetBinNames(iname,COL_NAMES,rnames,cnames);

 for (size_t sel=0;sel<lcols.size();sel++)
 {
  size_t all=0;
  while ( (all<cnames.size()) && (lcols[sel]!=cnames[all]) )
   all++;
  if (all>=cnames.size())
  {
   cerr << "Error: no column with name '" << lcols[sel] << "' found in matrix contained in file " << iname << ".\n";
   exit(1);
  }
  lncols.push_back(indextype(all));
 }
 JGetNumsCol(iname,oname,lncols);
}

