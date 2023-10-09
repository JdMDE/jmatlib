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
#include "../headers/fullmatrix.h"
#include "../headers/sparsematrix.h"
#include "../headers/symmetricmatrix.h"
#include "..//headers/matmetadata.h"

extern unsigned char DEB;

/***********************************************************
 * 
 * Functions related with getting rows from the jmatrix binary format 
 *
 **********************************************************/
 
// Functions to read one or more columns. These are simpler, since jmatrix format is primarily row-oriented.

// Auxiliary functions to read one or many rows of the matrices stored in binary jmatrix format without reading the full matrix in memory

template <typename T>
void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<T> &v)
{
 std::streampos nrl=(std::streampos)nr;
 T *data = new T [ncols]; 
 
 std::ifstream f(fname.c_str());
 // Start of row nr is at the end of former rows, each of them having ncols elements
 f.seekg(HEADER_SIZE+nrl*ncols*sizeof(T),std::ios::beg);
 // Here we simply read ncols elements
 f.read((char *)data,(std::streamsize)ncols*sizeof(T));
 f.close();
 
 v=std::vector<T>(ncols,T(0));
 for (size_t c=0; c<ncols; c++)
  v[c]=data[c];
  
 delete[] data;
}

template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<unsigned char> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<char> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<unsigned short> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<short> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<unsigned int> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<int> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<long> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long long> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<long long> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<float> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<double> &v);
template void GetJustOneRowFromFull(std::string fname,indextype nr,indextype ncols,std::vector<long double> &v);

// Auxiliary function to get from a full matrix the rows whose indexes are in vector nr. Rows are left consecutively in the passed matrix
// in the same order as in vector r, so if vector  is not ordered, resulting rows will be unordered, too.
template <typename T>
void GetManyRowsFromFull(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<T>> &m)
{
 T *data = new T [ncols]; 
 
 m.clear();
 std::vector<T> vdata;
 std::ifstream f(fname.c_str());
 unsigned long long nrl;
 for (size_t t=0; t<nr.size(); t++)
 {
  nrl=(unsigned long long)nr[t];
  // Start of row nr is at the end of former rows, each of them having ncols elements
  f.seekg((std::streampos)(HEADER_SIZE+nrl*ncols*sizeof(T)),std::ios::beg);
  // Here we simply read ncols elements
  f.read((char *)data,(std::streamsize)ncols*sizeof(T));
  // and put them in the matrix
  for (indextype c=0; c<ncols; c++)
   vdata.push_back(data[c]);

  m.push_back(vdata);
 }
 f.close();
 delete[] data;
}

template <typename T>
void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<T> &v)
{
 indextype ncr;
 
 std::ifstream f(fname.c_str());
 // Start of row nr is at the end of former rows, each of them having a different number of elements
 // we must go to the start of each row to find out how many...
 std::streampos offset=HEADER_SIZE;
 f.seekg(offset,std::ios::beg);
 f.read((char *)&ncr,sizeof(indextype));
 
 unsigned long long ncrl;
 for (indextype r=0; r<nr; r++)
 {
  ncrl=(unsigned long long)ncr;
  // Advance to jump over the ncr indextype indices, plus the own ncr value, plus the ncr values of data
  offset += ((ncrl+1)*sizeof(indextype)+ncrl*sizeof(T));
  f.seekg(offset,std::ios::beg);
  // At the beginning of row r: read how many element there are in it (ncr):
  f.read((char *)&ncr,(std::streamsize)sizeof(indextype));
 }

 // Clear the vector to be returned
 v=std::vector<T>(ncols,T(0));

 if (ncr!=0)
 {
  indextype *idata;
  T *data;
  // We are just at the beginning of the row we want to read, and we have already read its number of non-null entries (ncr)
  // Let's read its indices...
  idata = new indextype [ncr];
  f.read((char *)idata,(std::streamsize)ncr*sizeof(indextype));
  // ... and let's read the data
  data = new T [ncr];
  f.read((char *)data,(std::streamsize)ncr*sizeof(T));
 
  // Fill the appropriate places of the vector (those dictated by the indices in idata)
  for (size_t c=0; c<ncr; c++)
   v[data[c]]=data[c];
  
  delete[] data;
  delete[] idata;
 }
 
 f.close();
}

template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<unsigned char> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<char> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<unsigned short> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<short> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<unsigned int> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<int> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<long> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long long> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<long long> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<float> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<double> &v);
template void GetJustOneRowFromSparse(std::string fname,indextype nr,indextype ncols,std::vector<long double> &v);

// WARNING: see if there is any problem. It seems to be corrected.
// Auxiliary function to get rows from a sparse matrix whose row indexes are in vector nr. Rows are left consecutively in the passed matrix
// in the same order as in vector nr, so if vector is not ordered, resulting rows will be unordered, too.
template <typename T>
void GetManyRowsFromSparse(std::string fname,std::vector<indextype> nr,indextype nrows,indextype ncols,std::vector<std::vector<T>> &m)
{
 std::vector<std::streampos> offsets(nrows);
 
 std::ifstream f(fname.c_str());

 indextype ncr;
 offsets[0]=HEADER_SIZE;
 unsigned long long ncrl,to_add;
 for (size_t t=0;t<nrows;t++)
 {
  f.seekg(offsets[t],std::ios::beg);
  f.read((char *)&ncr,(std::streamsize)sizeof(indextype));
  ncrl=(std::streampos)ncr;
  // Advance to jump over the ncr indextype indices, plus the own ncr value, plus the ncr values of data
  if (t<nrows-1)
  {
   to_add = (ncrl+1)*sizeof(indextype)+ncrl*sizeof(T);
   offsets[t+1] = offsets[t]+(std::streampos)to_add;
  }
 }
 
 // These are temporary arrays to store the indices and values of a row.
 // Since each row stores a different number of values, we book sufficient space for the biggest case, the number of columns
 // (this is the case of an absolutely unsparse row, no zeros in it)
 indextype *idata = new indextype [ncols];
 T *data = new T [ncols];
 
 m.clear();
 for (size_t t=0; t<nr.size(); t++)
 {
  // Clear the corresponding row of the matrix to be returned
  m.push_back(std::vector<T>(ncols,T(0)));
  
  f.seekg(offsets[nr[t]],std::ios::beg);
  // At the beginning of row r: read how many element there are in it (ncr):
  f.read((char *)&ncr,(std::streamsize)sizeof(indextype));
  if (ncr!=0)
  {
   // Let's read its indices... No problem with size, ncr is always smaller than ncols
   f.read((char *)idata,(std::streamsize)ncr*sizeof(indextype));
   // ... and let's read the data
   f.read((char *)data,(std::streamsize)ncr*sizeof(T));
  }
  
  // Fill the appropriate places of the  (those dictated by the indices in idata)
  for (size_t c=0; c<ncr; c++)
   m[t][idata[c]]=data[c];
 }
  
 delete[] data;
 delete[] idata;
 
 f.close();
}


template <typename T>
void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<T> &v)
{
 unsigned long long nrl=(std::streampos)nr;
 T *data = new T [ncols]; 
 
 std::ifstream f(fname.c_str());
 
 // This is the beginning of row nr in the binary symmetric data
 std::streampos offset=HEADER_SIZE + sizeof(T)*(nrl*(nrl+1))/2;
 
 f.seekg(offset,std::ios::beg);
 
 // Here we read the nr+1 values present in that row, including the (nr,nr) at the main diagonal (which will be normally 0 in a dissimilarity matrix)
 f.read((char *)data,(std::streamsize)(sizeof(T)*(nrl+1)));
 
 // The rest of this row is not physically after; we must read the rest of the column that starts in the diagonal, down to the end.
 // It would be clearer to write r=nr+1; r<nrows; r++ but we haven't a variable nrows. But we don't need: symmetric matrices are square.
 offset=HEADER_SIZE+sizeof(T)*(nrl+((nrl+1)*(nrl+2))/2);
 for (indextype r=nr+1; r<ncols; r++)
 {
  f.seekg(offset,std::ios::beg);
  // Here we read just one value...
  f.read((char *)&(data[r]),(std::streamsize)sizeof(T));
  // and advance to the next row, taking into account that each row has a different number of really stored columns, which is r+1
  offset += ((std::streampos)(r+1)*sizeof(T));
 }
 f.close();

 // Clear the vector to be returned
 v=std::vector<T>(ncols,T(0));
 for (size_t c=0; c<ncols; c++)
  v[c]=data[c];
  
 delete[] data;
}

template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned char> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<char> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned short> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<short> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned int> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<int> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<long> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<unsigned long long> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<long long> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<float> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<double> &v);
template void GetJustOneRowFromSymmetric(std::string fname,indextype nr,indextype ncols,std::vector<long double> &v);

// Auxiliary function to get from a symmetric matrix the rows whose indexes are in vector nr. Rows are left consecutively in the passed matrix
// in the same order as in vector r, so if vector  is not ordered, resulting rows will be unordered, too.
template <typename T>
void GetManyRowsFromSymmetric(std::string fname,std::vector<indextype> nr,indextype ncols,std::vector<std::vector<T>> &m)
{
 T *data = new T [ncols]; 
 
 std::ifstream f(fname.c_str());
 std::streampos offset;
 unsigned long long nrl;
 m.clear();
 for (size_t t=0; t<nr.size(); t++)
 {
  // This is the beginning of row nr in the binary symmetric data
  nrl=(unsigned long long)nr[t];
  offset=HEADER_SIZE+sizeof(T)*((nrl*(nrl+1))/2);
  f.seekg(offset,std::ios::beg);
  // Here we read the nr+1 values present in that row, including the (nr,nr) at the main diagonal (which will be normally 0 in a dissimilarity matrix)
  f.read((char *)data,(std::streamsize)(sizeof(T)*(nrl+1)));
 
  m.push_back(std::vector<T>(ncols,T(0)));
  for (size_t c=0; c<nr[t]+1; c++)
   m[t][c]=data[c];
   
  // The rest of this row is not physically after; we must read the rest of the column that starts in the diagonal, down to the end.
  // It would be clearer to write r=nr+1; r<nrows; r++ but we haven't a variable nrows. But we don't need: symmetric matrices are square.
  offset=HEADER_SIZE+sizeof(T)*(nrl+((nrl+1)*(nrl+2))/2);
  for (indextype r=nr[t]+1; r<ncols; r++)
  {
   f.seekg(offset,std::ios::beg);
   // Here we read just one value...
   f.read((char *)&(data[r]),(std::streamsize)sizeof(T));
   // and advance to the next row, taking into account that each row has a different number of really stored columns, which is r+1
   offset += ((std::streampos)(r+1)*sizeof(T));
  }
  
  for (size_t c=nr[t]+1; c<ncols; c++)
   m[t][c]=data[c];
  
 }
 
 f.close();
 delete[] data;
}

using namespace std;

template <typename T>
void NREmit(string ofile,vector<T> &v,unsigned char mtype,const string rowname,const vector<string> &colnames)
{
 if ((mtype==MTYPEFULL) || (mtype==MTYPESYMMETRIC))
 {
   FullMatrix<T> V(1,v.size());
   for (size_t i=0;i<v.size();i++)
    V.Set(0,i,v[i]);
   if (colnames.size()>0)
    V.SetColNames(colnames);
   if (rowname!="")
   {
    vector<string> dummy;
    dummy.push_back(rowname);
    V.SetRowNames(dummy);
   }
   V.WriteBin(ofile);
  }
  else
  {
   SparseMatrix<T> V(1,v.size());
   for (size_t i=0;i<v.size();i++)
    V.Set(0,i,v[i]);
   if (colnames.size()>0)
    V.SetColNames(colnames);
   if (rowname!="")
   {
    vector<string> dummy;
    dummy.push_back(rowname);
    V.SetRowNames(dummy);
   }
   V.WriteBin(ofile);
  }
}

template <typename T>
void NRSEmit(string ofile,vector<vector<T>> &v,unsigned char mtype,const vector<string> &rownames,vector<string> &colnames)
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
void NRSelect(string inmat,indextype numrow,indextype ncols,string ofile,unsigned char mtype)
{
 vector<T> v;
 string rowname="";
 vector<string> rownames,colnames;
 InternalGetBinNames(inmat,ROW_NAMES,rownames,colnames);
 if (rownames.size()>0)
  rowname=rownames[numrow];
 switch (mtype)
 {
  case MTYPEFULL:
      GetJustOneRowFromFull<T>(inmat,numrow,ncols,v);
      NREmit<T>(ofile,v,MTYPEFULL,rowname,colnames);
      break;
  case MTYPESPARSE:
      GetJustOneRowFromSparse<T>(inmat,numrow,ncols,v);
      NREmit<T>(ofile,v,MTYPESPARSE,rowname,colnames);
      break;
  case MTYPESYMMETRIC:
      GetJustOneRowFromSymmetric<T>(inmat,numrow,ncols,v);
      NREmit<T>(ofile,v,MTYPESYMMETRIC,rowname,colnames);
      break;
  default: cerr << "Unexpected error: unknown matrix type.\n"; exit(1); break;
 }
}

template <typename T>
void NRSSelect(string inmat,vector<indextype> lrows,indextype nrows,indextype ncols,string ofile,unsigned char mtype)
{
 vector<vector<T>> v;
 vector<string> sel_rownames;
 vector<string> rownames,colnames;
 InternalGetBinNames(inmat,ROW_NAMES | COL_NAMES,rownames,colnames);
 if (colnames.size()>0)
  for (size_t r=0;r<lrows.size();r++)
   sel_rownames.push_back(rownames[lrows[r]]);
 switch (mtype)
 {
  case MTYPEFULL:
        GetManyRowsFromFull<T>(inmat,lrows,ncols,v);
        NRSEmit<T>(ofile,v,MTYPEFULL,sel_rownames,colnames);
        break;
  case MTYPESPARSE:
        GetManyRowsFromSparse<T>(inmat,lrows,nrows,ncols,v);
        NRSEmit<T>(ofile,v,MTYPESPARSE,sel_rownames,colnames);
        break;
  case MTYPESYMMETRIC:
        GetManyRowsFromSymmetric<T>(inmat,lrows,ncols,v);
        NRSEmit<T>(ofile,v,MTYPESYMMETRIC,sel_rownames,colnames);
        break;
  default: cerr << "Unexpected error: unknown matrix type.\n"; exit(1); break;
 }
}

void JGetNumRow(string iname,string oname,indextype numrow)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 if (numrow>nrows-1)
  JMatrixStop("Requested row is beyond the limit of the matrix.\n");

 switch (ctype)
 {
  case UCTYPE:
        NRSelect<unsigned char>(iname,numrow,ncols,oname,mtype); break;
  case SCTYPE:
        NRSelect<char>(iname,numrow,ncols,oname,mtype); break;
  case USTYPE:
        NRSelect<unsigned short>(iname,numrow,ncols,oname,mtype); break;
  case SSTYPE:
        NRSelect<short>(iname,numrow,ncols,oname,mtype); break;
  case UITYPE:
        NRSelect<unsigned int>(iname,numrow,ncols,oname,mtype); break;
  case SITYPE:
        NRSelect<int>(iname,numrow,ncols,oname,mtype); break;
  case ULTYPE:
        NRSelect<unsigned long>(iname,numrow,ncols,oname,mtype); break;
  case SLTYPE:
        NRSelect<long>(iname,numrow,ncols,oname,mtype); break;
  case ULLTYPE:
        NRSelect<unsigned long long>(iname,numrow,ncols,oname,mtype); break;
  case SLLTYPE:
        NRSelect<long long>(iname,numrow,ncols,oname,mtype); break;
  case FTYPE:
        NRSelect<float>(iname,numrow,ncols,oname,mtype); break;
  case DTYPE:
        NRSelect<double>(iname,numrow,ncols,oname,mtype); break;
  case LDTYPE:
        NRSelect<long double>(iname,numrow,ncols,oname,mtype); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}

void JGetNameRow(string iname,string oname,string namerow)
{
 vector<string> rnames,cnames;
 InternalGetBinNames(iname,ROW_NAMES,rnames,cnames);
 size_t i=0;
 while ( (i<rnames.size()) && (rnames[i]!=namerow))
  i++;
 if (i>=rnames.size())
 {
  cerr << "Error: no row with name '" << namerow << "' found in matrix contained in file " << iname << ".\n";
  exit(1);
 }
 JGetNumRow(iname,oname,indextype(i));
}

void JGetNumsRow(std::string iname,std::string oname,std::vector<indextype> lrows)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 for (size_t r=0;r<lrows.size();r++)
  if (lrows[r]>=nrows)
  JMatrixStop("At least one of the requested columns is beyond the limit of the matrix.\n");

 switch (ctype)
 {
  case UCTYPE:
        NRSSelect<unsigned char>(iname,lrows,nrows,ncols,oname,mtype); break;
  case SCTYPE:
        NRSSelect<char>(iname,lrows,nrows,ncols,oname,mtype); break;
  case USTYPE:
        NRSSelect<unsigned short>(iname,lrows,nrows,ncols,oname,mtype); break;
  case SSTYPE:
        NRSSelect<short>(iname,lrows,nrows,ncols,oname,mtype); break;
  case UITYPE:
        NRSSelect<unsigned int>(iname,lrows,nrows,ncols,oname,mtype); break;
  case SITYPE:
        NRSSelect<int>(iname,lrows,nrows,ncols,oname,mtype); break;
  case ULTYPE:
        NRSSelect<unsigned long>(iname,lrows,nrows,ncols,oname,mtype); break;
  case SLTYPE:
        NRSSelect<long>(iname,lrows,nrows,ncols,oname,mtype); break;
  case ULLTYPE:
        NRSSelect<unsigned long long>(iname,lrows,nrows,ncols,oname,mtype); break;
  case SLLTYPE:
        NRSSelect<long long>(iname,lrows,nrows,ncols,oname,mtype); break;
  case FTYPE:
        NRSSelect<float>(iname,lrows,nrows,ncols,oname,mtype); break;
  case DTYPE:
        NRSSelect<double>(iname,lrows,nrows,ncols,oname,mtype); break;
  case LDTYPE:
        NRSSelect<long double>(iname,lrows,nrows,ncols,oname,mtype); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}

void JGetNamesRow(std::string iname,std::string oname,std::vector<string> lrows)
{
 vector<indextype> lnrows;
 vector<string> rnames,cnames;
 InternalGetBinNames(iname,ROW_NAMES,rnames,cnames);

 for (size_t sel=0;sel<lrows.size();sel++)
 {
  size_t all=0;
  while ( (all<rnames.size()) && (lrows[sel]!=rnames[all]) )
   all++;
  if (all>=rnames.size())
  {
   cerr << "Error: no column with name '" << lrows[sel] << "' found in matrix contained in file " << iname << ".\n";
   exit(1);
  }
  lnrows.push_back(indextype(all));
 }
 JGetNumsRow(iname,oname,lnrows);
}
