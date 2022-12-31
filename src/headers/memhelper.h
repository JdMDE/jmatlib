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

#ifndef _MEMHELPER_H
#define _MEMHELPER_H

#include <iostream>

/// @file memhelper.h

/*!
 * Finds how much memory is available and how much swap at call time.
 * @param[out] avmem Available memory in bytes
 * @param[out] avswap Available swap in bytes\n
 * ===============================================================\n
 * WARNING: currently this funcion work only on Linux systems.\n
 * Calling in other systems simply sets the variables to 0\n
 * ===============================================================\n
 */
void GetAvailableMemAndSwap(unsigned long &avmem,unsigned long &avswap);

/*!
 * Shows memory warnings if the memory needed to declare a FullMatrix is
 * above 75\% of available memory or directly shows an error and stops
 * the program is it is avobe the available memory\n
 * The idea is to call this function before going into an scope where
 * a FullMatrix will be declared if you think it might be too large, or
 * if you don't know its size in advance
 * @param[in] nr Number of rows of the prospective FullMatrix
 * @param[in] nc Number of columns of the prospective FullMatrix
 * @param[in] s Size in bytes of each element (normally obtained with sizeof(Type))\n
 * ===============================================================\n
 * WARNING: currently this funcion work only on Linux systems.\n
 * Calling in other systems simply sets the variables to 0\n
 * ===============================================================\n
 */
void MemoryWarnings(unsigned long nr,unsigned long nc,int s);

/*!
 * Shows memory warnings if the memory needed to declare a SymmetricMatrix is
 * above 75\% of available memory or directly shows an error and stops
 * the program is it is avobe the available memory\n
 * The idea is to call this function before going into an scope where
 * a SymmetricMatrix will be declared if you think it might be too large, or
 * if you don't know its size in advance
 * @param[in] nr Number of rows of the prospective SymmetricMatrix (obviously, it will be square)
 * @param[in] s Size in bytes of each element (normally obtained with sizeof(Type))\n
 * ===============================================================\n
 * WARNING: currently this funcion work only on Linux systems.\n
 * Calling in other systems simply sets the variables to 0\n
 * ===============================================================\n
 */
void MemoryWarnings(unsigned long nr,int s);

#endif
