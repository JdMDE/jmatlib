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

#include "../headers/matmetadata.h"

extern unsigned char DEB;

/***********************************************************
 * 
 * Functions related with jmatrix metadata manipulation
 *
 **********************************************************/
 
// Internal functions to get metadata: names of rows and columns

// Auxiliary functions to skip the mark of binary block separation
unsigned char ChSep(std::ifstream &ifile)
{
 unsigned char dummy[BLOCKSEP_LEN];
 
 ifile.read((char *)dummy,BLOCKSEP_LEN);
 
 size_t i=0;
 while (i<BLOCKSEP_LEN && dummy[i]==BLOCKSEP[i])
  i++;
 
 return ((i==BLOCKSEP_LEN) ? READ_OK : ERROR_READING_SEP_MARK);
}

// Auxiliary functions to read metadata: row or column names (one after the other)
unsigned char RNames(std::ifstream &ifile,std::vector<std::string> &names)
{
 char dummy[MAX_LEN_NAME];
 char b;
 
 indextype j=0;
 do
 {
  b=char(ifile.get());
  
  if (ifile.eof())    // We have reached the end-of-file
   return ((j==0) ? READ_OK : ERROR_READING_STRINGS);
   
  if ((unsigned char)b==BLOCK_MARK)     // We have at the start between block of metadata
  {
   ifile.unget();
   return READ_OK;
  }
  
  if (b==0x00)   // We have reached end of this name
  {
   dummy[j]=b;   // Set it as end of string
   names.push_back(std::string(dummy));     // and take note of the string
   j=0;                             // Reinitialize the position in dummy
  }
  else
  { 
   dummy[j]=b;         // Usual case: still reading characters
   j++;
   if (j>=MAX_LEN_NAME)
    return ERROR_READING_STRINGS;
  }
 } 
 while (!ifile.eof());
 
 return ERROR_READING_STRINGS;
}

// Internal function to get simultaneously the row and column names
void InternalGetBinNames(std::string fname,unsigned char whichnames,std::vector<std::string> &rnames,std::vector<std::string> &cnames)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 
 MatrixType(fname,mtype,ctype,endian,mdinfo,nrows,ncols);
 
 if (
     ( ((mdinfo & ROW_NAMES)==0) && ((whichnames & ROW_NAMES)!=0) ) ||
     ( ((mdinfo & COL_NAMES)==0) && ((whichnames & COL_NAMES)!=0) )
    )
 {
  if (DEB & DEBJM)
  {
   std::string w;
   if ((whichnames & ROW_NAMES) && (whichnames & COL_NAMES))
    w="Asking for row and colum names in file "+fname+", which did not store at least one of such data (even if there is one, the returned value will be empty).\n";
   else
   {
    if (whichnames & ROW_NAMES)
     w="Asking for row names in file "+fname+", which did not store such data.\n";
    else
     w="Asking for column names in file "+fname+", which did not store such data.\n";
   } 
   JMatrixWarning(w);
  }
  return;
 }
 
 unsigned long long start_metadata,start_comment;
 PositionsInFile(fname,&start_metadata,&start_comment);

 // Then, position the stream at the beginning of the metadata
 std::ifstream f(fname.c_str());
 f.seekg(start_metadata,std::ios::beg);
 if (whichnames & ROW_NAMES)
 {
  if (RNames(f,rnames) == ERROR_READING_STRINGS)
  {
   f.close();
   JMatrixStop("Cannot read row names from binary file (even they are supposed to be there...).\n");
  }
  if (ChSep(f)==ERROR_READING_SEP_MARK)
   JMatrixStop("Cannot read separation mark from binary file (even it should be supposed to be there...).\n");
 }
 else
 {
  // Row names are before column names in binary file. This obligues us to read them, if they are there, even if the caller did not ask for them.
  // But in this case parametr rnames will not be altered
  if (mdinfo & ROW_NAMES)
  {
   std::vector<std::string> dummy;
   if (RNames(f,dummy) == ERROR_READING_STRINGS)
   {
    f.close();
    JMatrixStop("Cannot read row names from binary file (even they are supposed to be there...).\n");
   }
   if (ChSep(f)==ERROR_READING_SEP_MARK)
    JMatrixStop("Cannot read separation mark from binary file (even it should be supposed to be there...).\n");
  }
 }
 
 if (whichnames & COL_NAMES)
 {
  if (RNames(f,cnames) == ERROR_READING_STRINGS)
  {
   f.close();
   JMatrixStop("Cannot read column names from binary file (even they are supposed to be there...).\n");
  }
 }
 // On the contrary, if no column names have been asked, we don't need to read them.

 f.close();
}

using namespace std;

void JGetRowNames(string iname,string oname)
{
 vector<string> rn,cn;
 InternalGetBinNames(iname,ROW_NAMES,rn,cn);
 ofstream f(oname.c_str());
 if (!f.is_open())
 {
  cerr << "Error: file " << oname << " cannot be opened to write row names.\n";
  exit(1);
 }
 for (size_t r=0;r<rn.size();r++)
   f << rn[r] << endl;
 f.close();
}

void JGetColNames(string iname,string oname)
{
 vector<string> rn,cn;
 InternalGetBinNames(iname,COL_NAMES,rn,cn);
 ofstream f(oname.c_str());
 if (!f.is_open())
 {
  cerr << "Error: file " << oname << " cannot be opened to write column names.\n";
  exit(1);
 }
 for (size_t c=0;c<cn.size();c++)
   f << cn[c] << endl;
 f.close();
}

template <typename T>
void SRCSelect(string inmat,string ofile,unsigned char mtype,vector<string> rnames,vector<string> cnames)
{
 switch (mtype)
 {
  case MTYPEFULL:
  {
    FullMatrix<T> M(inmat);
    if (rnames.size()>0)
      M.SetRowNames(rnames);
    if (cnames.size()>0)
      M.SetColNames(cnames);
    M.WriteBin(ofile);
  }
  break;
  case MTYPESPARSE:
  {
    SparseMatrix<T> M(inmat);
    if (rnames.size()>0)
      M.SetRowNames(rnames);
    if (cnames.size()>0)
      M.SetColNames(cnames);
    M.WriteBin(ofile);
  }
  break;
  case MTYPESYMMETRIC:
  {
    SymmetricMatrix<T> M(inmat);
    if (rnames.size()>0)
      M.SetRowNames(rnames);
    if (cnames.size()>0)
      M.SetColNames(cnames);
    M.WriteBin(ofile);
  }
  default: cerr << "Unexpected error: unknown matrix type.\n"; exit(1); break;
 }
}

template <typename T>
void SetComment(string inmat,string ofile,unsigned char mtype,string comment)
{
 switch (mtype)
 {
  case MTYPEFULL:
  {
    FullMatrix<T> M(inmat);
    M.SetComment(comment);
    M.WriteBin(ofile);
  }
  break;
  case MTYPESPARSE:
  {
    SparseMatrix<T> M(inmat);
    M.SetComment(comment);
    M.WriteBin(ofile);
  }
  break;
  case MTYPESYMMETRIC:
  {
    SymmetricMatrix<T> M(inmat);
    M.SetComment(comment);
    M.WriteBin(ofile);
  }
  break;
  default: cerr << "Unexpected error: unknown matrix type.\n"; exit(1); break;
 }
}

void JSetRowNames(string iname,string oname,vector<string> rnames)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 if (rnames.size()!=nrows)
  JMatrixStop("Matrix row size is different from the number of strings found in the file of new row names.\n");
 vector<string> cnames;
 cnames.clear();
 switch (ctype)
 {
  case UCTYPE: SRCSelect<unsigned char>(iname,oname,mtype,rnames,cnames); break;
  case SCTYPE: SRCSelect<char>(iname,oname,mtype,rnames,cnames); break;
  case USTYPE: SRCSelect<unsigned short>(iname,oname,mtype,rnames,cnames); break;
  case SSTYPE: SRCSelect<short>(iname,oname,mtype,rnames,cnames); break;
  case UITYPE: SRCSelect<unsigned int>(iname,oname,mtype,rnames,cnames); break;
  case SITYPE: SRCSelect<int>(iname,oname,mtype,rnames,cnames); break;
  case ULTYPE: SRCSelect<unsigned long>(iname,oname,mtype,rnames,cnames); break;
  case SLTYPE: SRCSelect<long>(iname,oname,mtype,rnames,cnames); break;
  case ULLTYPE: SRCSelect<unsigned long long>(iname,oname,mtype,rnames,cnames); break;
  case SLLTYPE: SRCSelect<long long>(iname,oname,mtype,rnames,cnames); break;
  case FTYPE: SRCSelect<float>(iname,oname,mtype,rnames,cnames); break;
  case DTYPE: SRCSelect<double>(iname,oname,mtype,rnames,cnames); break;
  case LDTYPE: SRCSelect<long double>(iname,oname,mtype,rnames,cnames); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}

void JSetColNames(string iname,string oname,vector<string> cnames)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 if (cnames.size()!=ncols)
  JMatrixStop("Matrix column size is different from the number of strings found in the file of new column names.\n");
 vector<string> rnames;
 rnames.clear();
 switch (ctype)
 {
  case UCTYPE: SRCSelect<unsigned char>(iname,oname,mtype,rnames,cnames); break;
  case SCTYPE: SRCSelect<char>(iname,oname,mtype,rnames,cnames); break;
  case USTYPE: SRCSelect<unsigned short>(iname,oname,mtype,rnames,cnames); break;
  case SSTYPE: SRCSelect<short>(iname,oname,mtype,rnames,cnames); break;
  case UITYPE: SRCSelect<unsigned int>(iname,oname,mtype,rnames,cnames); break;
  case SITYPE: SRCSelect<int>(iname,oname,mtype,rnames,cnames); break;
  case ULTYPE: SRCSelect<unsigned long>(iname,oname,mtype,rnames,cnames); break;
  case SLTYPE: SRCSelect<long>(iname,oname,mtype,rnames,cnames); break;
  case ULLTYPE: SRCSelect<unsigned long long>(iname,oname,mtype,rnames,cnames); break;
  case SLLTYPE: SRCSelect<long long>(iname,oname,mtype,rnames,cnames); break;
  case FTYPE: SRCSelect<float>(iname,oname,mtype,rnames,cnames); break;
  case DTYPE: SRCSelect<double>(iname,oname,mtype,rnames,cnames); break;
  case LDTYPE: SRCSelect<long double>(iname,oname,mtype,rnames,cnames); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}

void JSetRowColNames(string iname,string oname,vector<string> rnames,vector<string> cnames)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 if (cnames.size()!=ncols)
  JMatrixStop("Matrix column size is different from the number of strings found in the file of new column names.\n");
 if (rnames.size()!=nrows)
  JMatrixStop("Matrix row size is different from the number of strings found in the file of new row names.\n");
 switch (ctype)
 {
  case UCTYPE: SRCSelect<unsigned char>(iname,oname,mtype,rnames,cnames); break;
  case SCTYPE: SRCSelect<char>(iname,oname,mtype,rnames,cnames); break;
  case USTYPE: SRCSelect<unsigned short>(iname,oname,mtype,rnames,cnames); break;
  case SSTYPE: SRCSelect<short>(iname,oname,mtype,rnames,cnames); break;
  case UITYPE: SRCSelect<unsigned int>(iname,oname,mtype,rnames,cnames); break;
  case SITYPE: SRCSelect<int>(iname,oname,mtype,rnames,cnames); break;
  case ULTYPE: SRCSelect<unsigned long>(iname,oname,mtype,rnames,cnames); break;
  case SLTYPE: SRCSelect<long>(iname,oname,mtype,rnames,cnames); break;
  case ULLTYPE: SRCSelect<unsigned long long>(iname,oname,mtype,rnames,cnames); break;
  case SLLTYPE: SRCSelect<long long>(iname,oname,mtype,rnames,cnames); break;
  case FTYPE: SRCSelect<float>(iname,oname,mtype,rnames,cnames); break;
  case DTYPE: SRCSelect<double>(iname,oname,mtype,rnames,cnames); break;
  case LDTYPE: SRCSelect<long double>(iname,oname,mtype,rnames,cnames); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}

void JSetComment(string iname,string oname,string new_comment)
{
 unsigned char mtype,ctype,endian,mdinfo;
 indextype nrows,ncols;
 MatrixType(iname,mtype,ctype,endian,mdinfo,nrows,ncols);
 switch (ctype)
 {
  case UCTYPE: SetComment<unsigned char>(iname,oname,mtype,new_comment); break;
  case SCTYPE: SetComment<char>(iname,oname,mtype,new_comment); break;
  case USTYPE: SetComment<unsigned short>(iname,oname,mtype,new_comment); break;
  case SSTYPE: SetComment<short>(iname,oname,mtype,new_comment); break;
  case UITYPE: SetComment<unsigned int>(iname,oname,mtype,new_comment); break;
  case SITYPE: SetComment<int>(iname,oname,mtype,new_comment); break;
  case ULTYPE: SetComment<unsigned long>(iname,oname,mtype,new_comment); break;
  case SLTYPE: SetComment<long>(iname,oname,mtype,new_comment); break;
  case ULLTYPE: SetComment<unsigned long long>(iname,oname,mtype,new_comment); break;
  case SLLTYPE: SetComment<long long>(iname,oname,mtype,new_comment); break;
  case FTYPE: SetComment<float>(iname,oname,mtype,new_comment); break;
  case DTYPE: SetComment<double>(iname,oname,mtype,new_comment); break;
  case LDTYPE: SetComment<long double>(iname,oname,mtype,new_comment); break;
  default: cerr << "Error:\n   Unknown data type in input matrix.\n"; break;
 }
}
