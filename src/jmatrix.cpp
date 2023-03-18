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

#ifndef JMATRIX_CPP
#define JMATRIX_CPP

#include "headers/jmatrix.h"
#include "internal_headers/templatemacros.h"

extern unsigned char DEB;

// Auxiliary functions to check type of matrix in binary file. These are not templated, since they are equal for all types
void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype,unsigned char &endianness,unsigned char &mdinf,indextype &nrows,indextype &ncols)
{
 std::ifstream ifile;
 ifile.open(fname.c_str(),std::ios::binary);
 if (!ifile.is_open())
    {
    	std::string err="Error: cannot open file "+fname+" to verify matrix type.\n";
        JMatrixStop(err);
        
    }
       
 unsigned char header[HEADER_SIZE];
 ifile.read((char *)header,HEADER_SIZE);
 ifile.close();
 
 mtype=header[0];
 ctype=header[1] & 0x0F;
 endianness=header[1] & 0xF0;
 indextype *t;
 t=(indextype *)(header+2);
 nrows=*t;
 t=(indextype *)(header+2+sizeof(indextype));
 ncols=*t;
 mdinf=*(header+2+2*sizeof(indextype));
}

void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype,unsigned char &endianness,unsigned char &mdinf)
{
 indextype nr,nc;
 MatrixType(fname,mtype,ctype,endianness,mdinf,nr,nc);
}

void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype,unsigned char &endianness)
{
 indextype nr,nc;
 unsigned char mdinf;
 MatrixType(fname,mtype,ctype,endianness,mdinf,nr,nc);
}

void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype)
{
 indextype nr,nc;
 unsigned char mdinf,endianness;
 MatrixType(fname,mtype,ctype,endianness,mdinf,nr,nc);
}

void MatrixType(std::string fname,unsigned char &mtype)
{
 indextype nr,nc;
 unsigned char mdinf,endianness,ctype;
 MatrixType(fname,mtype,ctype,endianness,mdinf,nr,nc);
}

// Auxiliary function to fix a string as needed
std::string FixQuotes(std::string s,bool withquotes)
{
 std::string ret;
 if (withquotes)
 {
  if (s.front()=='"')
   ret = (s.back()=='"') ? s : s+"\"";
  else
   ret = (s.back()=='"') ? "\""+s : "\""+s+"\"";
 }
 else
 {
  if (s.front()!='"')
   ret = (s.back()!='"') ? s : s.substr(0,s.size()-1);
  else
   ret = (s.back()=='"') ? s.substr(1,s.size()-2) : s.substr(1,s.size()-1);
 }
 return ret;   
}

/****************************************
  TEMPLATED CONSTRUCTORS AND FUNCTIONS
*****************************************/

// Default constructor (empty matrix)
template <typename T>
JMatrix<T>::JMatrix(unsigned char mtype)
{
 jmtype=mtype;
 jctype=NOTYPE;
 nr=nc=0;
 mdinfo=NO_METADATA;
 for (size_t i=0;i<COMMENT_SIZE;i++)
  comment[i]='\0';
}

TEMPLATES_CONST(JMatrix,unsigned char mtype)

//////////////////////

// Constructor with numrows,numcolums

template <typename T>
JMatrix<T>::JMatrix(unsigned char mtype,indextype nrows,indextype ncols)
{
 jmtype=mtype;
 jctype=TypeNameToId();                 // TODO: WARNING. See if this call may provoke problems, as it does in constructor from csv file.
 nr=nrows;
 nc=ncols;
 mdinfo=NO_METADATA;
 for (size_t i=0;i<COMMENT_SIZE;i++)
  comment[i]='\0';
}

TEMPLATES_CONST(JMatrix,SINGLE_ARG(unsigned char mtype,indextype nrows,indextype ncols))

//////////////////////

// Constructor reading from binary file
template <typename T>
JMatrix<T>::JMatrix(std::string fname,unsigned char mtype)
{
 ifile.open(fname.c_str(),std::ios::binary);
 if (!ifile.is_open())
    {
    	std::string err="Error: cannot open file "+fname+" to read the matrix.\n";
        JMatrixStop(err);
        
    }
    
 unsigned char mt;
 ifile.read((char *)(&mt),1);

 if (mt!=mtype)
 {
 	std::string err="Error: matrix stored in file "+fname+" is of type "+MatrixTypeName(mt)+" and you are trying to store it as a "+ MatrixTypeName(mtype)+". If it is not of type "+MatrixTypeName(MTYPENOTYPE)+" you must use the right class.\n";
        JMatrixStop(err);
 }
 unsigned char td;
 ifile.read((char *)(&td),1);
 
 size_t tds=SizeOfType(td);
 if (tds != sizeof(T))
 {
    std::ostringstream errst;
    
    errst << "Error: matrix stored in file " << fname << " has data of different size than those of the matrix supposed to hold it.\n";
    errst << "The stored matrix says to have elements of size " << tds << " whereas this matrix is declared to hold elements of size " << sizeof(T) << std::endl;
    JMatrixStop(errst.str());
 }
 
 // TODO: check this is OK, but.. how??
 jctype = td & 0x0F;
 
 if ( (td & 0xF0) != ThisMachineEndianness() )
 {
    std::string err;
    err = "Error: matrix stored in file " +fname+" has different endianness to that of this machine, which is ";
    err = err + ((ThisMachineEndianness() == BIGEND) ? "big endian.\n" : "little endian.\n");
    err = err + "Changing endianness when reading is not yet implemented. Sorry.\n";
    JMatrixStop(err);
    // TODO: implement change of endianness in reading
 }

 ifile.read((char *)&nr,sizeof(indextype));
 ifile.read((char *)&nc,sizeof(indextype));
 ifile.read((char *)&mdinfo,sizeof(unsigned char));
 
 // We read the rest of the header, which should be empty...
 unsigned char zero;
 bool empty=true;
 for (size_t i=0;i<HEADER_SIZE-3-2*sizeof(indextype);i++)
 {
  ifile.read((char *)&zero,1);
  empty=(zero==0x00);
 }
 if (!empty)
  JMatrixWarning("At least one byte in the (supposingly) empty part of the header is not 0.\n");
}

TEMPLATES_CONST(JMatrix,SINGLE_ARG(std::string fname,unsigned char mtype))

//////////////////////

template <typename T>
void JMatrix<T>::SetDataType(unsigned char dtype)
{
 jctype=dtype;
}

TEMPLATES_FUNC(void,JMatrix,SetDataType,unsigned char dtype)

//////////////////////

template <typename T>
void JMatrix<T>::Resize(indextype newnr,indextype newnc)
{
 if (newnr<nr)
  rownames.erase(rownames.end()-(nr-newnr),rownames.end());
 if (newnr>nr)
  for (unsigned i=nr; i<newnr; i++)
   rownames.push_back("NA");
 nr=newnr;
   
 if (newnc<nc)
  colnames.erase(colnames.end()-(nc-newnc),colnames.end());
 if (newnc>nc)
  for (unsigned i=nc; i<newnc; i++)
   colnames.push_back("NA");
 nc=newnc;
}

TEMPLATES_FUNC(void,JMatrix,Resize,SINGLE_ARG(indextype newnr,indextype newnc))

//////////////////////

template <typename T>
std::vector<std::string> JMatrix<T>::GetColNames()
{
 return colnames;
}

TEMPLATES_FUNC(std::vector<std::string>,JMatrix,GetColNames,)


//////////////////////////////////////////////////////////////////////////////

template <typename T>
std::vector<std::string> JMatrix<T>::GetRowNames()
{
 return rownames;
}

TEMPLATES_FUNC(std::vector<std::string>,JMatrix,GetRowNames,)


////////////////////////////////////////////////////////////////////////
template <typename T>
bool JMatrix<T>::ProcessFirstLineCsv(std::string line,char csep)
{
 std::string delim=" ";
 delim[0]=csep;
 
 size_t pos=0;
 std::string token,tt;
 int p=0;
 while ((pos=line.find(delim)) != std::string::npos)
 {
     token=line.substr(0,pos);
     line.erase(0,pos+1);
     tt="";
     std::remove_copy(token.begin(),token.end(),std::back_inserter(tt),'\"');
     if ( ( p==0 && tt!="" ) || ( p!=0 && tt=="" ) )
         return false;
     // Each token (except the first one) is stored as a column name
     if (p>0)
      colnames.push_back(token);
     p++;
 }
 colnames.push_back(line);
 nc=colnames.size();
 return true;
}

TEMPLATES_FUNC(bool,JMatrix,ProcessFirstLineCsv,SINGLE_ARG(std::string line,char csep))

//////////////////////

template<typename T>
bool JMatrix<T>::ProcessDataLineCsv(std::string line, char csep,T *rowofdata)
{
    std::string delim=" ";
    delim[0]=csep;
 
    std::string token,tt;
    
    // This reads the first token, which is the row name
    size_t pos=line.find(delim);
    token=line.substr(0,pos);
    rownames.push_back(token);
    line.erase(0,pos+1);
    
    size_t p=0;
    while ((pos=line.find(delim)) != std::string::npos)
    {
     token=line.substr(0,pos);
     rowofdata[p]=atof(token.c_str());
     p++;
     line.erase(0,pos+1);
    }
    if (p!=nc-1)
        return false;
    rowofdata[p]=atof(line.c_str());
    p++;
    
    return true;
}

template bool JMatrix<unsigned char>::ProcessDataLineCsv(std::string line, char csep,unsigned char *rowofdata);
template bool JMatrix<char>::ProcessDataLineCsv(std::string line, char csep,char *rowofdata);
template bool JMatrix<unsigned short>::ProcessDataLineCsv(std::string line, char csep,unsigned short *rowofdata);
template bool JMatrix<short>::ProcessDataLineCsv(std::string line, char csep,short *rowofdata);
template bool JMatrix<unsigned int>::ProcessDataLineCsv(std::string line, char csep,unsigned int *rowofdata);
template bool JMatrix<int>::ProcessDataLineCsv(std::string line, char csep,int *rowofdata);
template bool JMatrix<unsigned long>::ProcessDataLineCsv(std::string line, char csep,unsigned long *rowofdata);
template bool JMatrix<long>::ProcessDataLineCsv(std::string line, char csep,long *rowofdata);
template bool JMatrix<unsigned long long>::ProcessDataLineCsv(std::string line, char csep,unsigned long long *rowofdata);
template bool JMatrix<long long>::ProcessDataLineCsv(std::string line, char csep,long long *rowofdata);
template bool JMatrix<float>::ProcessDataLineCsv(std::string line, char csep,float *rowofdata);
template bool JMatrix<double>::ProcessDataLineCsv(std::string line, char csep,double *rowofdata);
template bool JMatrix<long double>::ProcessDataLineCsv(std::string line, char csep,long double *rowofdata);

////////////////////////////////////////////

// Constructor reading from csv file
template <typename T>
JMatrix<T>::JMatrix(std::string fname,unsigned char mtype,unsigned char valuetype,char csep)
{
 jmtype=mtype;
 jctype=valuetype;
  
 // In principle, csv files contain names for rows and columns, even row names could be empty srings...
 mdinfo = ROW_NAMES | COL_NAMES;
 for (size_t i=0;i<COMMENT_SIZE;i++)
  comment[i]='\0';
  
 ifile.open(fname.c_str());
 if (!ifile.is_open())
    {
        std::string err = "Error: cannot open file "+fname+" to read the matrix.\n";
        JMatrixStop(err);
    }   
 std::string first_line;
 
 getline(ifile,first_line);
 if (!ProcessFirstLineCsv(first_line,csep))
 {
     std::string err = "Error: incorrect format of first line of file "+fname+".\n";
     JMatrixStop(err);
 }
 if (DEB & DEBJM)
     std::cout << nc+1 << " columns (excluding column of names) in file " << fname << ".\n";
}

TEMPLATES_CONST(JMatrix,SINGLE_ARG(std::string fname,unsigned char mtype,unsigned char valuetype,char csep))

///////////////////////////////////////////////////////////

// Copy constructor
template <typename T>
JMatrix<T>::JMatrix( const JMatrix<T>& other )
{
 if (jmtype != other.jmtype)
  JMatrixStop("Error from assigment operator: trying to assign between different matrix types (full/sparse/symmetric).\n");
 
 jctype = other.jctype;
 
 if (jctype != other.jctype)
  JMatrixStop("Error from assigment operator: trying to assign between different matrix _data_ types. Type promotion/conversion is not yet implemented.\n");
  
 nr=other.nr;
 nc=other.nc;
 mdinfo=other.mdinfo;
 rownames=other.rownames;
 colnames=other.colnames;
 for (size_t i=0;i<COMMENT_SIZE;i++)
  comment[i]=other.comment[i];
}

TEMPLATES_COPY_CONST(JMatrix)

///////////////////////////////////////////////////////////////

// Assignment operator
template <typename T>
JMatrix<T>& JMatrix<T>::operator= ( const JMatrix<T>& other )
{
 if (jmtype != other.jmtype)
 {
  std::string err = "Error from assigment operator: trying to assign between different matrix types.\n";
  JMatrixStop(err);
 }
 
 jctype = other.jctype;
 
 if (jctype != other.jctype)
  JMatrixStop("Error from assigment operator: trying to assign between different matrix _data_ types. Type promotion/conversion is not yet implemented.\n");
 
 nr=other.nr;
 nc=other.nc;
 mdinfo=other.mdinfo;
 rownames=other.rownames;
 colnames=other.colnames;
 for (size_t i=0;i<COMMENT_SIZE;i++)
  comment[i]=other.comment[i];
 
 return *this;
}

TEMPLATES_OPERATOR(JMatrix,=)

//////////////////////////////////////////////////////////////////

// Transposition operator
template <typename T>
JMatrix<T>& JMatrix<T>::operator!= ( const JMatrix<T>& other )
{
 jctype = other.jctype;
 
 if (jctype != other.jctype)
  JMatrixStop("Error from transposition operator: trying to transpose between different matrix _data_ types. Type promotion/conversion is not yet implemented.\n");

 // Number of rows and columns is swapped
 nr=other.nc;
 nc=other.nr;
 
 mdinfo=NO_METADATA;
 
 if (other.mdinfo==NO_METADATA)
  return *this;
  
 if (other.mdinfo & COMMENT)
  mdinfo |= COMMENT;
  
 if ( (other.mdinfo & ROW_NAMES) && (!(other.mdinfo & COL_NAMES)) )
 {
  mdinfo |= COL_NAMES;
  colnames=other.rownames;
 }
 else
 {
  if ( (!(other.mdinfo & ROW_NAMES)) && (other.mdinfo & COL_NAMES) )
  {
   mdinfo |= ROW_NAMES;
   rownames=other.colnames;
  }
  else
  {
   if ((other.mdinfo & ROW_NAMES) && (other.mdinfo & COL_NAMES))
   {
    mdinfo |= (ROW_NAMES | COL_NAMES);
    rownames=other.colnames;
    colnames=other.rownames;
   }
  }
 }
 
 for (size_t i=0;i<COMMENT_SIZE;i++)
  comment[i]=other.comment[i];
  
 return *this;
}

TEMPLATES_OPERATOR(JMatrix,!=)

/////////////////////////////////////////////////////////////////////

// Function to write the matrix as CSV file. It just opens the file
template <typename T>
void JMatrix<T>::WriteCsv(std::string fname,char csep,bool withquotes)
{
 ofile.open(fname.c_str());
 if (!ofile.is_open())
 {
    std::string err = "Error: cannot open file "+fname+" to write the matrix.\n";
    JMatrixStop(err);
 }
 if (mdinfo & COL_NAMES)
 {
  if (withquotes)
   ofile << "\"\"" << csep;
  else
   ofile << csep;   // Blank empty field at the beginning of first line
   
  for (unsigned int i=0; i<colnames.size()-1; i++)
   ofile << FixQuotes(colnames[i],withquotes) << csep;
  ofile << FixQuotes(colnames[colnames.size()-1],withquotes) << std::endl;
 }
 else
  if (mdinfo & ROW_NAMES)
  {
   if (withquotes)
    ofile << "\"\"" << csep;
   else
    ofile << csep;   // Blank empty field at the beginning of first line
   
   std::string dummy;
   std::ostringstream odummy;
   for (unsigned int i=0; i<this->nc-1; i++)
   {
    odummy << "C" << i+1;
    dummy=odummy.str();
    ofile << FixQuotes(dummy,withquotes) << csep;
   }
   odummy << "C" << this->nc;
   dummy=odummy.str();
   ofile << FixQuotes(dummy,withquotes) << std::endl;
  }
}

TEMPLATES_FUNC(void,JMatrix,WriteCsv,SINGLE_ARG(std::string fname,char csep,bool withquotes))

//////////////////////////////////////////////////////////////////////////////////

// Function to write the matrix as a binary file. It opens the file and writes the header.
template <typename T>
void JMatrix<T>::WriteBin(std::string fname,unsigned char mtype)
{
 ofile.open(fname.c_str(),std::ios::binary);
 if (!ofile.is_open())
 {
        std::string err = "Error: cannot open file "+fname+" to write the matrix.\n";
        JMatrixStop(err);
 }
 unsigned char td=TypeNameToId();
 if (td==NOTYPE)
 {
   std::ostringstream errst;
   errst << "Error: " << int(td) << " is not a valid data type identifier.\n";
   JMatrixStop(errst.str());
 }

 // We always write in the endianness of this machine, so we mark it.
 td |= ThisMachineEndianness();
 
 ofile.write((const char *)(&mtype),1);
 ofile.write((const char *)(&td),1);
 ofile.write((const char *)(&nr),sizeof(indextype));
 ofile.write((const char *)(&nc),sizeof(indextype));
 ofile.write((const char *)(&mdinfo),1);
 
 // We fill the header with 0 up to the predetermined header size, which is 128 bytes.
 // This is to have room to change the header if some time in the future we decide we need other information
 unsigned char zero=0x00;
 for (size_t i=0;i<HEADER_SIZE-3-2*sizeof(indextype);i++)
  ofile.write((const char *)(&zero),1);
}

TEMPLATES_FUNC(void,JMatrix,WriteBin,SINGLE_ARG(std::string fname,unsigned char mtype))

//////////////////////////////////////////////////////////////////////////

template <typename T>
indextype JMatrix<T>::ReadNames(std::vector<std::string> &names)
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

TEMPLATES_FUNC(indextype,JMatrix,ReadNames,SINGLE_ARG(std::vector<std::string> &names))

//////////////////////////////////////////////////////////////////////////////////

template <typename T>
indextype JMatrix<T>::CheckSep()
{
 unsigned char dummy[BLOCKSEP_LEN];
 
 ifile.read((char *)dummy,BLOCKSEP_LEN);
 
 size_t i=0;
 while (i<BLOCKSEP_LEN && dummy[i]==BLOCKSEP[i])
  i++;
 
 return ((i==BLOCKSEP_LEN) ? READ_OK : ERROR_READING_SEP_MARK);
}

TEMPLATES_FUNC(indextype,JMatrix,CheckSep,)

//////////////////////////////////////////////////////////////////////////////////

template <typename T>
void JMatrix<T>::WriteNames(std::vector<std::string> &names)
{
 char dummy[MAX_LEN_NAME+1];
 char *dummy2;
 
 for (size_t i=0; i<names.size(); i++)
 {
  strncpy(dummy,names[i].c_str(),MAX_LEN_NAME);
  if (dummy[0]=='"' && dummy[strlen(dummy)-1]=='"')
  {
   dummy[strlen(dummy)-1]='\0';
   dummy2=dummy+1;
  }
  else
   dummy2=dummy;
  ofile.write((const char *)dummy2,strlen(dummy2)+1);   // +1 is because we want the final null character be copied, too.
 }
}

TEMPLATES_FUNC(void,JMatrix,WriteNames,SINGLE_ARG(std::vector<std::string> &names))

/////////////////////////////////////////////////////////////////////////////////////

template <typename T>
int JMatrix<T>::ReadMetadata()
{
 if (mdinfo == NO_METADATA)
  return READ_OK;
  
 if (mdinfo & ROW_NAMES)
 {
  if (ReadNames(rownames) == ERROR_READING_STRINGS)
    return ERROR_READING_ROW_NAMES;
  if (CheckSep() == ERROR_READING_SEP_MARK)
   return ERROR_READING_SEP_MARK;
 }
 
 if (mdinfo & COL_NAMES)
 {
  if (ReadNames(colnames) == ERROR_READING_STRINGS)
   return ERROR_READING_COL_NAMES;
  if (CheckSep() == ERROR_READING_SEP_MARK)
   return ERROR_READING_SEP_MARK;
 }
 
 if (mdinfo & COMMENT)
 {
  ifile.read((char *)comment,COMMENT_SIZE);
  if (CheckSep() == ERROR_READING_SEP_MARK)
   return ERROR_READING_SEP_MARK;
 }
 
 return READ_OK;
}

TEMPLATES_FUNC(int,JMatrix,ReadMetadata,)

//////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void JMatrix<T>::WriteMetadata()
{
 if (mdinfo == NO_METADATA)
  return;
  
 if (mdinfo & ROW_NAMES)
 {
  if (DEB & DEBJM)
   std::cout << "   Writing row names (" << rownames.size() << " strings written, from " << rownames[0] << " to " << rownames[rownames.size()-1] << ").\n";
  WriteNames(rownames);
  ofile.write((const char *)BLOCKSEP,BLOCKSEP_LEN);
 }
 
 if (mdinfo & COL_NAMES)
 {
  if (DEB & DEBJM)
   std::cout << "   Writing column names (" << colnames.size() << " strings written, from " << colnames[0] << " to " << colnames[colnames.size()-1] << ").\n";
  WriteNames(colnames);
  ofile.write((const char *)BLOCKSEP,BLOCKSEP_LEN);
 }
 
 if (mdinfo & COMMENT)
 {
  if (DEB & DEBJM)
   std::cout << "   Writing comment: " << comment << "\n";
  ofile.write((const char *)comment,COMMENT_SIZE);
  ofile.write((const char *)BLOCKSEP,BLOCKSEP_LEN);
 }
 
}

TEMPLATES_FUNC(void,JMatrix,WriteMetadata,)

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void JMatrix<T>::SetColNames(std::vector<std::string> cnames)
{
 if (cnames.size() != nc)
  JMatrixStop("Trying to set column names with a vector of length different to the current number of columns.\n");
  
 colnames.clear(); 
 colnames=cnames;
 mdinfo |= COL_NAMES;
}

TEMPLATES_FUNC(void,JMatrix,SetColNames,std::vector<std::string> cnames)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void JMatrix<T>::SetRowNames(std::vector<std::string> rnames)
{
 if (rnames.size() != nr)
  JMatrixStop("Trying to set row names with a vector of length different to the current number of rows.\n");
  
 rownames.clear(); 
 rownames=rnames;
 mdinfo |= ROW_NAMES;
}

TEMPLATES_FUNC(void,JMatrix,SetRowNames,std::vector<std::string> rnames)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
unsigned char JMatrix<T>::TypeNameToId()
{ 
 if (std::is_same<T,unsigned char>::value)
  return UCTYPE;
 if (std::is_same<T,char>::value)
  return SCTYPE; 
 if (std::is_same<T,unsigned short>::value)
  return USTYPE;
 if (std::is_same<T,short>::value)
  return SSTYPE; 
 if (std::is_same<T,unsigned int>::value)
  return UITYPE;
 if (std::is_same<T,int>::value)
  return SITYPE; 
 if (std::is_same<T,unsigned long>::value)
  return ULTYPE;  
 if (std::is_same<T,long>::value)
  return SLTYPE;  
 if (std::is_same<T,float>::value)
  return FTYPE;
 if (std::is_same<T,double>::value)
  return DTYPE;
 if (std::is_same<T,long double>::value)
  return LDTYPE;

 return NOTYPE;
}

TEMPLATES_FUNC(unsigned char,JMatrix,TypeNameToId,)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::string JMatrix<T>::GetComment()
{
 std::string ret(comment);
 return ret;
}

TEMPLATES_FUNC(std::string,JMatrix,GetComment,)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void JMatrix<T>::SetComment(std::string cm)
{
 mdinfo |= COMMENT;
 if (cm.size()>COMMENT_SIZE)
 {
  JMatrixWarning("Too long comment. Final characters will be ignored.\n");
  for (size_t i=0; i<COMMENT_SIZE-1; i++)
   comment[i]=cm[i];
  comment[COMMENT_SIZE-1]='\0';
 }
 else
 {
  for (size_t i=0; i<cm.size(); i++)
   comment[i]=cm[i];
  for (size_t i=cm.size(); i<COMMENT_SIZE; i++)
   comment[i]='\0';
 }
}

TEMPLATES_FUNC(void,JMatrix,SetComment,std::string cm)

#endif


