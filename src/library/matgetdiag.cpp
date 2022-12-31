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
 
#include <cmath>
#include "../headers/fullmatrix.h"
#include "../headers/sparsematrix.h"
#include "../headers/symmetricmatrix.h"

extern unsigned char DEB;

/************ Special functions to get the lower-diagonal part of a symmetric matrix as a vector *************/

// Auxiliary function to get the lower-diagonal part of a symmetric matrix

template <typename T>
void GSDiag(std::string fname,indextype nrows,std::vector<T> &v)
{
 T *data = new T [nrows];
  
 std::ifstream f(fname.c_str());
 
 // This is the beginning of row 1 in the binary symmetric data
 size_t offset=HEADER_SIZE+sizeof(T);
 f.seekg(offset,std::ios::beg);
 
 for (indextype r=1; r<nrows; r++)
 {
  // Here we read the r+1 values present in that row, including the (nr,nr) at the main diagonal (which will be normally 0 in a dissimilarity matrix)
  f.read((char *)data,(r+1)*sizeof(T));
  // but we copy all of them, except the last one, at the appropriate place of return array so that theay are ordered by column.
  for (indextype c=0; c<r; c++)
   v[c*(nrows-1)-((c*(c-1))/2)+r-c-1]=data[c];
 }
 
 f.close();
 delete[] data;
}

using namespace std;

template <typename T>
void DEmit(string ofile,vector<T> &v)
{
 FullMatrix<T> V(v.size(),1);
 for (size_t i=0;i<v.size();i++)
  V.Set(i,0,v[i]);
 V.WriteBin(ofile);
}

void JGetSubDiag(string iname,string oname)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 if (mtype!=MTYPESYMMETRIC)
  JMatrixStop("The input matrix is not a diagonal-type matrix (it might be a full or sparse matrix that happens to be diagonal, but it is stored as diagonal).\n");

 switch (ctype)
 {
  case UCTYPE: { vector<unsigned char> v;
        GSDiag<unsigned char>(iname,nrows,v); DEmit(oname,v); } break;
  case SCTYPE: { vector<char> v;
        GSDiag<char>(iname,nrows,v); DEmit(oname,v); } break;
  case USTYPE: { vector<unsigned short> v;
        GSDiag<unsigned short>(iname,nrows,v); DEmit(oname,v); } break;
  case SSTYPE: { vector<short> v;
        GSDiag<short>(iname,nrows,v); DEmit(oname,v); } break;
  case UITYPE: { vector<unsigned int> v;
        GSDiag<unsigned int>(iname,nrows,v); DEmit(oname,v); } break;
  case SITYPE: { vector<int> v;
        GSDiag<int>(iname,nrows,v); DEmit(oname,v); } break;
  case ULTYPE: { vector<unsigned long> v;
        GSDiag<unsigned long>(iname,nrows,v); DEmit(oname,v); } break;
  case SLTYPE: { vector<long> v;
        GSDiag<long>(iname,nrows,v); DEmit(oname,v); } break;
  case ULLTYPE: { vector<unsigned long long> v;
        GSDiag<unsigned long long>(iname,nrows,v); DEmit(oname,v); } break;
  case SLLTYPE: { vector<long long> v;
        GSDiag<long long>(iname,nrows,v); DEmit(oname,v); } break;
  case FTYPE: { vector<float> v;
        GSDiag<float>(iname,nrows,v); DEmit(oname,v); } break;
  case DTYPE: { vector<double> v;
        GSDiag<double>(iname,nrows,v); DEmit(oname,v); } break;
  case LDTYPE: { vector<long double> v;
        GSDiag<long double>(iname,nrows,v); DEmit(oname,v); } break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}
