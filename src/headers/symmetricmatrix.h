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

#ifndef SYMMETRICMATRIX_H
#define SYMMETRICMATRIX_H

#include "jmatrix.h"
#include "memhelper.h"

/// @file symmetricmatrix.h

/**
 * @SymmetricMatrix Class to hold arbitrarily big symmetric square matrices. For a matrix of size NxN, only Nx(N+1)/2 elements are stored.
 */
template <typename T>
class SymmetricMatrix: public JMatrix<T>
{
 public:
    /**
     * Default constructor
     */
    SymmetricMatrix();

    /**
     * Constructor with number of rows/columns (the same, it is square)
     * 
     * @param[in] nrows Number of rows
     */
    SymmetricMatrix(indextype nrows);

    /**
     * Constructor with number of rows/columns (the same, it is square) giving memory warnings
     *
     * @param[in] nrows Number of rows
     * @param[in] warn  Boolean value to give memory warnings\n
     * The idea is to use this form of constructor with warn=true, which internally calls MemoryWarnings, if one suspects
     * the matrix to be constructed might provoke memory problems.
     */
    SymmetricMatrix(indextype nrows,bool warn);

    /**
     * Constructor to fill the matrix content from a binary file\n
     * Binary file header as explained in the documetation to WriteBin
     * 
     * PRELIMINARY VERSION. ASSUMES SAME ENDIANESS FOR WRITER AND READER MACHINE
     * 
     * @param[in] fname The name of the file to read
     * 
     */
    SymmetricMatrix(std::string fname); 

    /**
     * Constructor to fill the matrix content from a binary file with warnings\n
     * Binary file header as explained in the documetation to WriteBin
     *
     * PRELIMINARY VERSION. ASSUMES SAME ENDIANESS FOR WRITER AND READER MACHINE
     *
     * @param[in] fname The name of the file to read
     * @param[in] warn  Boolean value to give memory warnings
     *
     */
    SymmetricMatrix(std::string fname,bool warn);

    /** 
     * Function to resize the matrix\n
     * WARNING: previous content, if any, IS LOST (to be reviewed)
     * 
     * @param[in] newnr New number of rows (and columns)
     * 
     */
    void Resize(indextype newnr);
    
    /**
     * Copy constructor
     *
     * @param[in] other Reference to the SymmetricMatrix to be copied
     */
    SymmetricMatrix(const SymmetricMatrix<T>& other);

    /**
     * Destructor
     */
    ~SymmetricMatrix();

    /**
     * Assignment operator
     *
     * @param[in] other Reference to the SymmetricMatrix to be assigned
     * @return Reference to the newly created SymmetricMatrix
     */
    SymmetricMatrix<T>& operator=(const SymmetricMatrix<T>& other);

    /**
     * Test of correctness\n
     * This is meant to test if the symmetric matrix is a distance or dissimilarity matrix.
     * It checks that all elements in the main diagonal are 0 and all outside the main diagonal are stricty possitive
     * 
     * @return true if the matrix can be a distance or dissimilarity matrix. false otherwise.
     * 
     */
    bool TestDistDisMat();
    
    /** 
     * Function to get acess to an element
     * 
     * @param[in] r The row to access
     * @param[in] c The columns to access
     * 
     * @return value at (r,c) of matrix, of type T
     */
    T Get(indextype r,indextype c);
    
    /** 
     * Function to set an element
     * 
     * @param[in] r The row to access
     * @param[in] c The columns to access
     * @param[in] T The value to be set, of type T
     * 
     */
    void Set(indextype r,indextype c,T v); 
    
    /**
     * Function to get the sum of a row (used frequently by PAM)
     * 
     * @param[in] r The row whose sum we want
     * 
     * @return The sum of all columns of row r, of type T
     */
    T GetRowSum(indextype r);
    
    /**
     * Function to write the matrix content to a CSV file
     * 
     *  @param[in] fname      The name of the file to write
     *  @param[in] csep       The separator character between fields (default: , (comma))
     *  @param[in] withquotes Boolean value to indicate if field names in .csv must be written surrounded by quotes.
     * 
     */
    void WriteCsv(std::string fname,char csep=',',bool withquotes=false);
    
    /**
     * Function to write the matrix content to a binary file\n
     * See format at documentation of JMatrix::WriteBin
     * 
     * @param[in] fname The name of the file to write
     */
    void WriteBin(std::string fname);
    
    /**
     * Function to get memory in MB used by this symmetric matrix
     * 
     * @return The amount of memory in MB
     */
    float GetUsedMemoryMB();
    
 private:
     std::vector< std::vector<T> > data;
};

#endif // SYMMETRICMATRIX_H
