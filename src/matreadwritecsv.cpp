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
 
 
#include "headers/fullmatrix.h"
#include "headers/sparsematrix.h"
#include "headers/symmetricmatrix.h"

extern unsigned char DEB;

using namespace std;

void JCsvDump(string ifile, string csvfile, char csep =',',bool withquotes=false)
{
 unsigned char mtype,ctype,endian,mdinf;
 indextype nrows,ncols;

 MatrixType(ifile,mtype,ctype,endian,mdinf,nrows,ncols);

 if (mtype==MTYPEFULL)
 {
     switch (ctype)
     {
        case UCTYPE: { FullMatrix<unsigned char> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SCTYPE: { FullMatrix<char> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case USTYPE: { FullMatrix<unsigned short> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SSTYPE: { FullMatrix<short> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case UITYPE: { FullMatrix<unsigned int> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SITYPE: { FullMatrix<int> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case ULTYPE: { FullMatrix<unsigned long> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SLTYPE: { FullMatrix<long> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case FTYPE:  { FullMatrix<float> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case DTYPE:  { FullMatrix<double> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case LDTYPE: { FullMatrix<long double> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        default: break;
    }
 }
 if (mtype==MTYPESPARSE)
 {
     switch (ctype)
     {
        case UCTYPE: { SparseMatrix<unsigned char> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SCTYPE: { SparseMatrix<char> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case USTYPE: { SparseMatrix<unsigned short> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SSTYPE: { SparseMatrix<short> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case UITYPE: { SparseMatrix<unsigned int> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SITYPE: { SparseMatrix<int> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case ULTYPE: { SparseMatrix<unsigned long> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SLTYPE: { SparseMatrix<long> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case FTYPE:  { SparseMatrix<float> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case DTYPE:  { SparseMatrix<double> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case LDTYPE: { SparseMatrix<long double> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        default: break;
    }
 }
 if (mtype==MTYPESYMMETRIC)
 {
     switch (ctype)
     {
        case UCTYPE: { SymmetricMatrix<unsigned char> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SCTYPE: { SymmetricMatrix<char> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case USTYPE: { SymmetricMatrix<unsigned short> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SSTYPE: { SymmetricMatrix<short> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case UITYPE: { SymmetricMatrix<unsigned int> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SITYPE: { SymmetricMatrix<int> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case ULTYPE: { SymmetricMatrix<unsigned long> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case SLTYPE: { SymmetricMatrix<long> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case FTYPE:  { SymmetricMatrix<float> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case DTYPE:  { SymmetricMatrix<double> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        case LDTYPE: { SymmetricMatrix<long double> M(ifile); M.WriteCsv(csvfile,csep,withquotes); break; };
        default: break;
    }
 }
}

template <typename T>
void CsvDataToBinMat(string ifname,string ofname,unsigned char vtype,char csep,bool isfull)
{
 if (isfull)
 {
  FullMatrix<T> M(ifname,vtype,csep);
  M.WriteBin(ofname);
 }
 else
 {
  SparseMatrix<T> M(ifname,vtype,csep);
  M.WriteBin(ofname);
 }
}

void JCsvToJMat(string iname,string oname,char sep,unsigned char mtype,unsigned char valtype)
{
 switch (valtype)
 {
  case UCTYPE: CsvDataToBinMat<unsigned char>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case SCTYPE: CsvDataToBinMat<char>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case USTYPE: CsvDataToBinMat<unsigned short>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case SSTYPE: CsvDataToBinMat<short>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case UITYPE: CsvDataToBinMat<unsigned int>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case SITYPE: CsvDataToBinMat<int>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case ULTYPE: CsvDataToBinMat<unsigned long>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case SLTYPE: CsvDataToBinMat<long>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case ULLTYPE: CsvDataToBinMat<unsigned long long>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case SLLTYPE: CsvDataToBinMat<long long>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case FTYPE: CsvDataToBinMat<float>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case DTYPE: CsvDataToBinMat<double>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  case LDTYPE: CsvDataToBinMat<long double>(iname,oname,valtype,sep,(mtype==MTYPEFULL)); break;
  default: JMatrixStop("Unexpected error in JCdvToJMat: unknown data type.\n"); break;
 }
}
