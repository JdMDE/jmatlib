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

/**
 * @file jmat.cpp
 * @brief <h2>jmat</h2>
 *        See program use in the documention to main() below\n
 * \n
 *        NOTE: The includes in this source file are for compilation of this program as an example together with the library,\n
 *        before the library itself is installed. Once you have installed the library (assuming headers are in\n
 *        /usr/local/include, lib is in /usr/local/lib or in other place included in your compiler search path)\n
 *        you should substitute this by\n
 *\n
 *        #include <jmatrixlib/debugpar.h>   etc...\n
 *\n
 *        and compile with something like
 *
 *        g++ -Wall jmat.cpp -o jmat -ljmatrix
 *
 *\n
*/
#include "../headers/debugpar.h"
#include "../headers/jmatrix.h"
#include "../headers/apitocommands.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

// Possible commands identified by a constant
// Other can be added later, modifying appropriately the NUM_COMMANDS constant
const unsigned char INFO=0;
const unsigned char ROWNUM=1;
const unsigned char ROWSNUM=2;
const unsigned char ROWNAME=3;
const unsigned char ROWSNAME=4;
const unsigned char COLNUM=5;
const unsigned char COLSNUM=6;
const unsigned char COLNAME=7;
const unsigned char COLSNAME=8;
const unsigned char SUBDIAG=9;
const unsigned char SETRNAMES=10;
const unsigned char SETCNAMES=11;
const unsigned char SETRCNAMES=12;
const unsigned char GETRNAMES=13;
const unsigned char GETCNAMES=14;
const unsigned char CSVDUMP=15;
const unsigned char CSVREAD=16;
const unsigned char SETCOM=17;
const unsigned int NUM_COMMANDS=18;

// Strings associated to each command
const string command_names[NUM_COMMANDS]={"info","rownum","rownums","rowname","rownames","colnum","colnums","colname","colnames","subdiag","setrnames","setcnames","setrcnames","getrnames","getcnames","csvdump","csvread","setcom"};

unsigned short ComFromName(string com)
{
 unsigned short ret=0;
 while ((ret<NUM_COMMANDS) && (command_names[ret]!=com))
  ret++;
 return ret;
}

void Usage(char *pname,unsigned char specific)
{
 if (specific>=NUM_COMMANDS)
 {
  cerr << "Usage:\n\n";
  cerr << "   " << pname << " command matrix_file [other_options] -o out_matrix file\n\n";
  cerr << "where command is one of\n\n ";
  for (unsigned int c=0;c<NUM_COMMANDS;c++)
  {
   cerr << "'" << command_names[c] << "' ";
   if ((c%4)==3)
    cerr << "\n ";
  }
  cerr << "\n\nother_options are options dependent on the command (call '" << pname << " any_command' for specific information)\n\n";
  cerr << "Option -o out_matrix_file will name the file to contain either the binary matrix (or, for the info command,\n";
  cerr << "the ASCII/CSV) output file that results from the command.\n";
  cerr << "Also, remember that if this program is called as jmatd (symbolic link to jmat) you will get debugging messages in the console.\n\n";
 }
 else
 {
  switch (specific)
  {
    case INFO:
        cerr << "\n  " << pname << " info matrix_file [-o info_file.txt]\n\nDumps in ASCII to console or to info_file.txt information about the jmatrix in file matrix_file.\n";
        break;
    case ROWNUM:
        cerr << "\n  " << pname << " rownum matrix_file n -o res_file\n\nCreates a jmatrix file with the n-th row of the jmatrix in file matrix_file (index n from 0).\n";
        break;
    case ROWSNUM:
        cerr << "\n  " << pname << " rownums matrix_file n1,n2,... -o res_file\n\nCreates a jmatrix file with the rows in the comma-sepparated list of the jmatrix in file matrix_file (indices n1,n2,... from 0).\n";
        break;
    case ROWNAME:
        cerr << "\n  " << pname << " rowname matrix_file rname -o res_file\n\nCreates a jmatrix file with the row named 'rname' of the jmatrix in file matrix_file (index n from 0).\n";
        break;
    case ROWSNAME:
        cerr << "\n  " << pname << " rownames matrix_file rn1,rn2,... -o res_file\n\nCreates a jmatrix file, with the rows named 'rn1','rn2'... in the comma-sepparated list of the jmatrix n file matrix_file.\n";
        break;
    case COLNUM:
        cerr << "\n  " << pname << " colnum matrix_file n -o res_file\n\nCreates a jmatrix file, with the n-th column of the jmatrix in file matrix_file (index n from 0).\n";
        break;
    case COLSNUM:
        cerr << "\n  " << pname << " colnums matrix_file n1,n2,... -o res_file\n\nCreates a jmatrix file, with the columns in the comma-sepparated list of the jmatrix in file matrix_file (indices form 0).\n";
        break;
    case COLNAME:
        cerr << "\n  " << pname << " colname matrix_file cname -o res_file\n\nCreates a jmatrix file, with the column named 'cname' of the jmatrix in file matrix_file\n";
        break;
    case COLSNAME:
        cerr << "\n  " << pname << " colnames matrix_file cn1,cn2,... -o res_file\n\nCreates a jmatrix file, with the columns named 'cn1','cn2'... in the comma-sepparated list of the jmatrix in file matrix_file.\n";
        break;
    case SUBDIAG:
        cerr << "\n  " << pname << " subdiag symmetric_matrix_file -o res_file\n\nCreates a jmatrix file with the lower-triangular part (not including the diagonal) of the symmetric matrix in the input.\n";
        break;
    case GETRNAMES:
        cerr << "\n  " << pname << " getrnames matrix_file -o res_file.txt\n\nDumps to ASCII file res_file.txt the names of the rows in the input matrix.\n";
        break;
    case GETCNAMES:
        cerr << "\n  " << pname << " getcnames matrix_file -o res_file.txt\n\nDumps to ASCII file res_file.txt the names of the columns in the input matrix.\n";
        break;
    case SETRNAMES:
        cerr << "\n  " << pname << " setrnames matrix_file rownames_file.txt -o res_file\n\nCreates a copy of the original jmatrix setting or changing the row names to those given in file rownames_file.txt, which must have as many lines as rows in input matrix.\n";
        break;
    case SETCNAMES:
        cerr << "\n  " << pname << " setcnames matrix_file colnames_file.txt -o res_file\n\nCreates a copy of the original jmatrix setting or changing the column names to those given in file colnames_file.txt, which must have as many lines as columns in input matrix.\n";
        break;
    case SETRCNAMES:
        cerr << "\n  " << pname << " setrcnames matrix_file rownames_file.txt colnames_file.txt -o res_file\n\nCreates a copy of the original jmatrix setting or changing the row and column names to those given in files rownames_file.txt and colnames_file.txt respectively, which must have as many lines as rows/columns in input matrix.\n";
        break;
    case CSVDUMP:
        cerr << "\n  " << pname << " csvdump matrix_file format -o res_file.csv\n\nDumps the content of the matrix in the input file to the .csv output file. Format must be one of these strings:\n";
        cerr << "  cn  Comma as separator, row/column names not surrounded by double quotes.\n";
        cerr << "  cq  Comma as separator, row/column names surrounded by double quotes.\n";
        cerr << "  tn  Tab as separator, row/column names not surrounded by double quotes.\n";
        cerr << "  tq  Tab as separator, row/column names surrounded by double quotes.\n";
        break;
    case CSVREAD:
        cerr << "\n  " << pname << " csvread input_file.csv sepchar mtype valtype -o res_file\n\nReads the input file, which must be a csv file, and creates a binary jmatrix file with its content.\n";
        cerr << "  sepchar must be c or t to indicate that the expected field separator will be a comma or a tab, respectively.\n";
        cerr << "  mtype must be one of the strings 'full' or 'sparse'. Symmetric matrices cannot be read as such from a CSV file.\n";
        cerr << "  valtype must be one of the strings 'u8','s8','u16','s16','u32','s32','u64','s64','f','d' or 'ld'.\n";
        cerr << "  These stands for unsigned/signed integers of 8,16,32 or 64 bits, float, double, or long double datatypes, respectively.\n";
        break;
    case SETCOM:
        cerr << "\n" << pname << " setcom matrix_file 'new comment' -o res_file\n\nCopy the input matrix with the given comment.\n";
        cerr << "  The comment is set to the new one. Previous comment, if any, is discarded.\n";
        cerr << "  Setting the new comment to the empty string, '', gets rid of the old comment.\n";
    default: break;
  }
 }
 exit(1);
}

bool IsNum(string n,indextype &vn)
{
 string::const_iterator it = n.begin();
 while (it != n.end() && isdigit(*it))
  ++it;
 if (!n.empty() && it == n.end())
 {
  vn=indextype(atol(n.c_str()));
  return true;
 }
 return false;
}

bool IsNumList(string nl,vector<indextype> &vnl)
{
 string delimiter = ",";

 size_t pos = 0;
 string token;
 vnl.clear();
 indextype n;
 while ((pos = nl.find(delimiter)) != string::npos)
 {
    token = nl.substr(0, pos);
    if (!IsNum(token,n))
     return false;
    vnl.push_back(n);
    nl.erase(0, pos + delimiter.length());
 }
 if (!IsNum(nl,n))
  return false;
 vnl.push_back(n);
 return true;
}

bool IsNameList(string ml,vector<string> &vml)
{
 string delimiter = ",";

 size_t pos = 0;
 string token;
 vml.clear();
 while ((pos = ml.find(delimiter)) != string::npos)
 {
    token = ml.substr(0, pos);
    vml.push_back(token);
    ml.erase(0, pos + delimiter.length());
 }
 vml.push_back(ml);
 return true;
}

bool NameListInFile(string ml,vector<string> &vml)
{
 ifstream f(ml.c_str());
 if (!f.is_open())
 {
  cerr << "Cannot open file of names " << ml << " to read from it.\n";
  return false;
 }
 vml.clear();
 string s;
 while (getline(f,s))
  vml.push_back(s);
 f.close();
 return true;
}

bool CorrectSeparator(string s,char &sep,bool &q)
{
 if ( (s[0]!='c') && (s[0]!='t') )
 {
  JMatrixStop("Incorrect format specifier in csvdump subcommand.\n");
  return false;
 }
 sep=(s[0]=='c') ? ',' : '\t';

 if ( (s[1]!='q') && (s[1]!='n') )
 {
  JMatrixStop("Incorrect format specifier in csvdump subcommand.\n");
  return false;
 }
 q=(s[1]=='q');

 return true;
}

bool CorrectWriteSpecs(vector<string> spec,char &sep,unsigned char &mtype,unsigned char &valtype)
{
 if ( (spec[0][0]!='c') && (spec[0][0]!='t') )
 {
   JMatrixStop("Incorrect format specifier for separator character in csvread subcommand.\n");
   return false;
 }
 sep=(spec[0][0]=='c') ? ',' : '\t';

 if (spec[1]=="full")
  mtype=MTYPEFULL;
 else
  if (spec[1]=="sparse")
   mtype=MTYPESPARSE;
  else
  {
   JMatrixStop("Incorrect format specifier for matrix type in csvread subcommand.\n");
   return false;
  }

 if (spec[2]=="u8") { valtype=UCTYPE; return true; }
 if (spec[2]=="s8") { valtype=SCTYPE; return true; }
 if (spec[2]=="u16") { valtype=USTYPE; return true; }
 if (spec[2]=="s16") { valtype=SSTYPE; return true; }

 if (spec[2]=="u32")
 {
   // Depending on the machine, there is still discrepancy on the size of int/long. long is always 32 bits, as far as I know.
   // int is usally 32 bits to nowadays, but one never knows....
   if (sizeof(unsigned int)==32)
     valtype=UITYPE;
   else
     valtype=ULTYPE;
   return true;
 }
 if (spec[2]=="s32")
 {
   // Depending on the machine, there is still discrepancy on the size of int/long. long is always 32 bits, as far as I know.
   // int is usally 32 bits to nowadays, but one never knows....
   if (sizeof(int)==32)
     valtype=SITYPE;
   else
     valtype=SLTYPE;
   return true;
 }

 if (spec[2]=="u64") { valtype=ULLTYPE; return true; }
 if (spec[2]=="s64") { valtype=SLLTYPE; return true; }
 if (spec[2]=="f") { valtype=FTYPE; return true; }
 if (spec[2]=="d") { valtype=DTYPE; return true; }
 if (spec[2]=="ld") { valtype=LDTYPE; return true; }

 JMatrixStop("Incorrect format specifier for matrix data type in csvread subcommand.\n");
 return false;
}

void NameChanged(vector<string> ends)
{
 cerr << "You have changed the name of this program. Don't do that. Its name must be (or at least, must end in) ";
 for (size_t j=0;j<ends.size();j++)
  cerr << "'" << ends[j] << "' ";
 cerr << "\n";
 exit(1);
}

int CheckProgName(string pname,vector<string> possible_endings)
{
 sort(possible_endings.begin(),possible_endings.end(),[](string a, string b) { return a.size()<b.size(); });
 size_t i=0;
 while (i<possible_endings.size())
 {
  if (pname.size()<possible_endings[i].size())
   NameChanged(possible_endings);
  if (pname.substr(pname.size()-possible_endings[i].size())==possible_endings[i])
   return i;
  i++;
 }
 NameChanged(possible_endings);
 return -1;  // Just to avoid a warning
}

#endif

/**
 * <h2>jmat</h2>
 * A program to manipulate matrices created with the jmatrix library and written to the disk as binary files.
 *
 * The program must be called as
 *
 *     jmat command matrix_file other_options -o out_matrix file
 *
 * where command is one of a predefined list (see below) which is followed by the matrix to be manipulated, other relevant options
 * for the particular command and (optionally) the -o option with the result of the command.\n
 * If -o option is not given, the result is dumped to the console in ASCII\n
 * <b>other_options</b> are options dependent on the command (call 'jmat any_command' for specific information)\n
 * Also, remember that if this program is called as <b>jmatd</b> (symbolic link to jmat) you will get debugging messages in the console.\n
 * \n
 * Possible commands are:
 *
 *     jmat info matrix_file [-o info_file.txt]
 *
 *   Dumps in ASCII to console or to info_file.txt information about the jmatrix in file matrix_file.
 *
 *
 *     jmat rownum matrix_file n -o res_file
 *
 *   Creates a jmatrix file with the n-th row of the jmatrix in file matrix_file (index n from 0).
 *
 *
 *     jmat rownums matrix_file n1,n2,... -o res_file
 *
 *   Creates a jmatrix file with the rows in the comma-sepparated list of the jmatrix in file matrix_file (indices n1,n2,... from 0).
 *
 *
 *     jmat rowname matrix_file rname -o res_file
 *
 *   Creates a jmatrix file with the row named 'rname' of the jmatrix in file matrix_file (index n from 0).
 *
 *
 *     jmat rownames matrix_file rn1,rn2,... -o res_file
 *
 *   Creates a jmatrix file, with the rows named 'rn1','rn2'... in the comma-sepparated list of the jmatrix n file matrix_file.
 *
 *
 *     jmat colnum matrix_file n -o res_file
 *
 *   Creates a jmatrix file, with the n-th column of the jmatrix in file matrix_file (index n from 0).
 *
 *
 *     jmat colnums matrix_file n1,n2,... -o res_file
 *
 *   Creates a jmatrix file, with the columns in the comma-sepparated list of the jmatrix in file matrix_file (indices form 0).
 *
 *
 *     jmat colname matrix_file cname -o res_file
 *
 *   Creates a jmatrix file, with the column named 'cname' of the jmatrix in file matrix_file
 *
 *
 *     jmat colnames matrix_file cn1,cn2,... -o res_file
 *
 *   Creates a jmatrix file, with the columns named 'cn1','cn2'... in the comma-sepparated list of the jmatrix in file matrix_file.
 *
 *
 *     jmat subdiag symmetric_matrix_file -o res_file
 *
 *   Creates a jmatrix file with the lower-triangular part (not including the diagonal) of the symmetric matrix in the input.
 *
 *
 *     jmat getrnames matrix_file -o res_file.txt
 *
 *   Dumps to ASCII file res_file.txt the names of the rows in the input matrix.
 *
 *
 *     jmat getcnames matrix_file -o res_file.txt
 *
 *   Dumps to ASCII file res_file.txt the names of the columns in the input matrix.
 *
 *
 *     jmat setrnames matrix_file rownames_file.txt -o res_file
 *
 *   Creates a copy of the original jmatrix setting or changing the row names to those given in file rownames_file.txt, which must have as many lines as rows in input matrix.
 *
 *
 *     jmat setcnames matrix_file colnames_file.txt -o res_file
 *
 *   Creates a copy of the original jmatrix setting or changing the column names to those given in file colnames_file.txt, which must have as many lines as columns in input matrix.
 *
 *
 *     jmat setrcnames matrix_file rownames_file.txt colnames_file.txt -o res_file
 *
 *   Creates a copy of the original jmatrix setting or changing the row and column names to those given in files rownames_file.txt and colnames_file.txt respectively, which must have as many lines as rows/columns in input matrix.
 *
 *
 *     jmat csvdump matrix_file format -o res_file.csv
 *
 *   Dumps the content of the matrix in the input file to the .csv output file. Format must be one of these strings:\n
 *   cn  Comma as separator, row/column names not surrounded by double quotes.\n
 *   cq  Comma as separator, row/column names surrounded by double quotes.\n
 *   tn  Tab as separator, row/column names not surrounded by double quotes.\n
 *   tq  Tab as separator, row/column names surrounded by double quotes.
 *
 *
 *     jmat csvread input_file.csv sepchar mtype valtype -o res_file
 *
 *   Reads the input file, which must be a csv file, and creates a binary jmatrix file with its content.\n
 *   sepchar must be c or t to indicate that the expected field separator will be a comma or a tab, respectively.\n
 *   mtype must be one of the strings 'full' or 'sparse'. Symmetric matrices cannot be read as such from a CSV file.\n
 *   valtype must be one of the strings 'u8','s8','u16','s16','u32','s32','u64','s64','f','d' or 'ld'.\n
 *   These stands for unsigned/signed integers of 8,16,32 or 64 bits, float, double, or long double datatypes, respectively.
 *
 *     jmat setcom matrix_file 'new_comment' -o out_file
 *
 *   Copy the input matrix in the output file, setting the comment to the one specified (which can be the empty string, '')
 *
 */
int main(int argc,char *argv[])
{
 if (CheckProgName(string(argv[0]),{"jmat","jmatd"})==1)
  JMatrixSetDebug(true);

 if (argc==1)
  Usage(argv[0],NUM_COMMANDS);
 if (argc==2)
  Usage(argv[0],ComFromName(string(argv[1])));

 unsigned short com=ComFromName(string(argv[1]));
 string iname=string(argv[2]);
 if ( string(argv[argc-2])!="-o" && com!=INFO)
  Usage(argv[0],com);
 string oname;
 if ( string(argv[argc-2])=="-o" )
  oname=string(argv[argc-1]);
 else
  oname="cout";

 vector<string> args;
 for (unsigned short i=3;i<argc-2;i++)
  args.push_back(string(argv[i]));

 indextype n;
 vector<indextype> nl;
 vector<string> sl,slc;
 char sep;
 bool quotes;
 unsigned char mtype,valtype;
 switch (com)
 {
  case INFO:
    if (args.size()!=0)
     Usage(argv[0],INFO);
    else
     JMatInfo(iname,oname);
    break;
  case ROWNUM:
    if ( (args.size()!=1) || (!IsNum(args[0],n)) )
     Usage(argv[0],ROWNUM);
    else
     JGetNumRow(iname,oname,n);
    break;
  case COLNUM:
    if ( (args.size()!=1) || (!IsNum(args[0],n)) )
     Usage(argv[0],COLNUM);
    else
     JGetNumCol(iname,oname,n);
    break;
  case ROWNAME:
    if (args.size()!=1)
     Usage(argv[0],ROWNUM);
    else
     JGetNameRow(iname,oname,args[0]);
    break;
  case COLNAME:
    if (args.size()!=1)
     Usage(argv[0],COLNUM);
    else
     JGetNameCol(iname,oname,args[0]);
    break;
  case ROWSNUM:
    if ( (args.size()!=1) || (!IsNumList(args[0],nl)) )
     Usage(argv[0],ROWSNUM);
    else
     JGetNumsRow(iname,oname,nl);
    break;
  case COLSNUM:
    if ( (args.size()!=1) || (!IsNumList(args[0],nl)) )
     Usage(argv[0],COLSNUM);
    else
     JGetNumsCol(iname,oname,nl);
    break;
  case ROWSNAME:
    if ( (args.size()!=1) || (!IsNameList(args[0],sl)) )
     Usage(argv[0],ROWSNAME);
    else
     JGetNamesRow(iname,oname,sl);
    break;
  case COLSNAME:
    if ( (args.size()!=1) || (!IsNameList(args[0],sl)) )
     Usage(argv[0],COLSNAME);
    else
     JGetNamesCol(iname,oname,sl);
    break;
  case SUBDIAG:
    if ( args.size()!=0 )
     Usage(argv[0],SUBDIAG);
    else
     JGetSubDiag(iname,oname);
    break;
  case GETRNAMES:
    if ( args.size()!=0 )
     Usage(argv[0],GETRNAMES);
    else
     JGetRowNames(iname,oname);
    break;
  case GETCNAMES:
    if ( args.size()!=0 )
     Usage(argv[0],GETCNAMES);
    else
     JGetColNames(iname,oname);
    break;
  case SETRNAMES:
    if ( ( args.size()!=1 ) || (!NameListInFile(args[0],sl)) )
     Usage(argv[0],SETRNAMES);
    else
     JSetRowNames(iname,oname,sl);
    break;
  case SETCNAMES:
    if ( ( args.size()!=1 ) || (!NameListInFile(args[0],sl)) )
     Usage(argv[0],SETCNAMES);
    else
     JSetColNames(iname,oname,sl);
    break;
  case SETRCNAMES:
    if ( ( args.size()!=2 ) || (!NameListInFile(args[0],sl)) || (!NameListInFile(args[1],slc)) )
      Usage(argv[0],SETRCNAMES);
    else
     JSetRowColNames(iname,oname,sl,slc);
  case CSVDUMP:
    if ( ( args.size()!=1 ) || (!CorrectSeparator(args[0],sep,quotes)) )
     Usage(argv[0],CSVDUMP);
    else
     JCsvDump(iname,oname,sep,quotes);
    break;
  case CSVREAD:
    if ( (args.size()!=3) || (!CorrectWriteSpecs(args,sep,mtype,valtype)) )
      Usage(argv[0],CSVREAD);
    else
     JCsvToJMat(iname,oname,sep,mtype,valtype);
  case SETCOM:
    if ( (args.size()!=1) )
     Usage(argv[0],SETCOM);
    else
     JSetComment(iname,oname,args[0]);
  default: break;
 }

 return 0;
}
