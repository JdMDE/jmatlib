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

#ifndef _APITOCOMMANDS_H
#define _APITOCOMMANDS_H

#include <string>
#include <vector>
#include "indextype.h"

/// @file apitocommands.h

/*!
 * Function to get information about the JMatrix stored in a binary file and store such information in a text file
 *
 * @param[in] iname Name of the JMatrix binary file
 * @param[in] oname Name of the text file to write the information
 */
void JMatInfo(std::string iname,std::string oname);

/*!
 * Function to get a row by number and write it as a JMatrix in a binary file
 *
 * @param[in] iname  Name of the JMatrix binary file
 * @param[in] oname  Name of the binary file to write the row contents
 * @param[in] numrow The number of the row (0-based index) we want to extract
 */
void JGetNumRow(std::string iname,std::string oname,indextype numrow);

/*!
 * Function to get a column by number and write it as a JMatrix in a binary file
 *
 * @param[in] iname  Name of the JMatrix binary file
 * @param[in] oname  Name of the binary file to write the column contents
 * @param[in] numrow The number of the column (0-based index) we want to extract
 */
void JGetNumCol(std::string iname,std::string oname,indextype numcol);

/*!
 * Function to get a row by name and write it as a JMatrix in a binary file, assuming the input matrix has row names and such name exists
 *
 * @param[in] iname   Name of the JMatrix binary file
 * @param[in] oname   Name of the binary file to write the row contents
 * @param[in] namerow The name of the row we want to extract
 */
void JGetNameRow(std::string iname,std::string oname,std::string namerow);

/*!
 * Function to get a column by name and write it as a JMatrix in a binary file, assuming the input matrix has column names and such name exists
 *
 * @param[in] iname   Name of the JMatrix binary file
 * @param[in] oname   Name of the binary file to write the row contents
 * @param[in] namecol The name of the column we want to extract
 */
void JGetNameCol(std::string iname,std::string oname,std::string namecol);

/*!
 * Function to get several rows by number and write them as a JMatrix in a binary file
 *
 * @param[in] iname Name of the JMatrix binary file
 * @param[in] oname Name of the binary file to write the rows as a matrix
 * @param[in] lrows A vector with the numbers of the rows (0-based index) we want to extract
 */
void JGetNumsRow(std::string iname,std::string oname,std::vector<indextype> lrows);

/*!
 * Function to get several columns by number and write them as a JMatrix in a binary file
 *
 * @param[in] iname Name of the JMatrix binary file
 * @param[in] oname Name of the binary file to write the columns as a matrix
 * @param[in] lcols A vector with the numbers of the columns (0-based index) we want to extract
 */
void JGetNumsCol(std::string iname,std::string oname,std::vector<indextype> lcols);

/*!
 * Function to get several rows by their names and write them as a JMatrix in a binary file
 *
 * @param[in] iname Name of the JMatrix binary file
 * @param[in] oname Name of the binary file to write the rows as a matrix
 * @param[in] lrows A vector of strings with the names of the rows we want to extract
 */
void JGetNamesRow(std::string iname,std::string oname,std::vector<std::string> lrows);

/*!
 * Function to get several columns by their names and write them as a JMatrix in a binary file
 *
 * @param[in] iname Name of the JMatrix binary file
 * @param[in] oname Name of the binary file to write the rows as a matrix
 * @param[in] lcols A vector of strings with the names of the columns we want to extract
 */
void JGetNamesCol(std::string iname,std::string oname,std::vector<std::string> lcols);

/*!
 * Function to get the subdiagonal of a SymmetricMatrix of size (n x n) stored in a binary file and write them as
 * a vector of one row and n x (n-1)/2 columns (rows under the main diagonal, without the diagonal itself) stored
 * in row-major order
 *
 * @param[in] iname Name of the SymmatricMatrix binary file
 * @param[in] oname Name of the binary file to write the subdiagonal elements as a 1-row matrix
 */
void JGetSubDiag(std::string iname,std::string oname);

/*!
 * Function to write to an ASCII text file the names of the rows of the JMatrix stored in a binary file\n
 * The matrix is not loaded into memory
 *
 * @param[in] iname Name of the JMatrix binary file
 * @param[in] oname Name of the ASCII file to contain the row names, one at each line
 */
void JGetRowNames(std::string iname,std::string oname);

/*!
 * Function to write to an ASCII text file the names of the columns of the JMatrix stored in a binary file\n
 * The matrix is not loaded into memory
 *
 * @param[in] iname Name of the JMatrix binary file
 * @param[in] oname Name of the ASCII file to contain the column names, one at each line
 */
void JGetColNames(std::string iname,std::string oname);

/*!
 * Function to create a copy of the original jmatrix setting or changing the row names to those given in a\n
 * vector of strings, which must have as many elements as rows in the input matrix
 *
 * @param[in] iname  Name of the JMatrix binary file with the original matrix
 * @param[in] oname  Name of the JMatrix binary file with the copy matrix with new row names
 * @param[in] rnames Vector of strings with the new row names
 */
void JSetRowNames(std::string iname,std::string oname,std::vector<std::string> rnames);

/*!
 * Function to create a copy of the original jmatrix setting or changing the column names to those given in a\n
 * vector of strings, which must have as many elements as columns in the input matrix
 *
 * @param[in] iname  Name of the JMatrix binary file with the original matrix
 * @param[in] oname  Name of the JMatrix binary file with the copy matrix with new column names
 * @param[in] cnames Vector of strings with the new column names
 */
void JSetColNames(std::string iname,std::string oname,std::vector<std::string> cnames);

/*!
 * Function to create a copy of the original jmatrix setting or changing the column and row names to those given in\n
 * vectors of strings, which must have as many elements as rows and columns respectively in the input matrix
 *
 * @param[in] iname  Name of the JMatrix binary file with the original matrix
 * @param[in] oname  Name of the JMatrix binary file with the copy matrix with new column names
 * @param[in] rnames Vector of strings with the new row names
 * @param[in] cnames Vector of strings with the new column names
 */
void JSetRowColNames(std::string iname,std::string oname,std::vector<std::string> rnames,std::vector<std::string> cnames);

/*!
 * Function to dump the JMatrix contained in a binary file to as ASCII csv file
 *
 * @param[in] iname         Name of the JMatrix binary file with the matrix
 * @param[in] oname         Name of the csv file to generat
 * @param[in] char          The character to be used as sepparator
 * @param[in] withquotes    Boolean value to indicate if field names in .csv must be written surrounded by quotes.
 */
void JCsvDump(std::string iname,std::string oname,char sep,bool with_quotes);

/*!
 * Function to generate a binary JMatrix file from an ASCII csv file\n
 * First line of csv is supposed to contain the column names, starting with an empty field\n
 * First field of each line from the second line and on is supposed to contain the name of such row
 *
 * @param[in] iname CSV file with the data
 * @param[in] oname Name of the binary file to contain the created JMatrix
 * @param[in] sep   The character that csv file uses as field sepparator
 * @param[in] mtype The type of the JMatrix. Possible values: 'full', 'sparse' or 'symmetric'
 * @param[in] ctype The data type to store the read values (value type of the JMatrix). Possible values: 'u8','s8','u16','s16','u32','s32','u64','s64','f','d' or 'ld'
 */
void JCsvToJMat(std::string iname,std::string oname,char sep,unsigned char mtype,unsigned char ctype);
#endif
