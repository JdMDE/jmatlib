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
 
#ifndef _MATMETADATA_H
#define _MATMETADATA_H

#include "fullmatrix.h"
#include "sparsematrix.h"
#include "symmetricmatrix.h"
#include <cmath>

/// @file matmetadata.h

#ifndef DOXYGEN_SHOULD_SKIP_THIS
void InternalGetBinNames(std::string fname,unsigned char whichnames,std::vector<std::string> &rnames,std::vector<std::string> &cnames);
#endif

/*!
 Function to get the names of the rows of the JMatrix stored in a binary file\n
 The matrix is not loaded into memory
 @param[in] fname Name of the binary file containing the matrix
 @return A vector of strings with the row names
 */
std::vector<std::string> JGetRowNames(std::string fname);

/*!
 Function to get the names of the columns of the JMatrix stored in a binary file\n
 The matrix is not loaded into memory
 @param[in] fname Name of the binary file containing the matrix
 @return A vector of strings with the column names
 */
std::vector<std::string> JGetColNames(std::string fname);
#endif
