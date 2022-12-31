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

#ifndef _MATINFO_H
#define _MATINFO_H

#include <iostream>
#include <string>
#include "indextype.h"

/// @file matinfo.h

///@{
/**
 * Auxiliary funcions to check characteristics of matrix stored in binary file looking only at its header.
 * Matrix in file is NOT loaded into memory.
 *
 * @param[in]  fname        Name of the binary file
 * @param[out] mtype        Returns matrix type (full,sparse,symmetric)
 * @param[out] ctype        Returns matrix data type
 * @param[out] endianness   Returns endianness (big,little) of the data stored in the matrix
 * @param[out] nrows        Returns the number of rows
 * @param[out] ncols        Returns the number of columns
 * @param[out] mdinf        Returns the signal for the presence of metadata
 */
void MatrixType(std::string fname,unsigned char &mtype);
void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype);
void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype,unsigned char &endianness);
void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype,unsigned char &endianness,unsigned char &mdinf);
void MatrixType(std::string fname,unsigned char &mtype,unsigned char &ctype,unsigned char &endianness,unsigned char &mdinf,indextype &nrows,indextype &ncols);
///@}

/*!
 Gives information about the JMatrix stored in a file
 @param[in] fname The name of the binary file that contains the matrix
 @param[in] fres  The name of the text file that will contain the information, or the empty string to show the information in the console
 */
void JMatInfo(std::string fname, std::string fres = "");

#endif
