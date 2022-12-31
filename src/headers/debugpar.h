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

#ifndef _DEBUGPAR_H
#define _DEBUGPAR_H

#include <iostream>
#include <string>

/// @file debugpar.h

///@{
/**
   These are constants to allow selective debug by library.
   Each library will print messages or not using a test with logical AND
   between its particular constant and the DEB global variable. This
   allows the use of the system either in each separate package or
   in the global one
 */
const unsigned char NODEBUG=0x0;
const unsigned char DEBJM=0x01;
///@}

/*!
 * Sets the debug state to get messages or not
 * @param[in] deb true to get messages, false to suppress them. Default state is false.
 */
void JMatrixSetDebug(bool deb);

/*!
 * Sends an error message to the console and stops the program that is using the library
 * @param[in] errortext The text of the message to be shown. It will appear after a standard message saying that is comes from this library.
 */
void JMatrixStop(std::string errortext);

/*!
 * Sends a warning message to the console and goes on with the program that is using the library
 * @param[in] errortext The text of the message to be shown. It will appear after a standard message saying that is comes from this library.
 */
void JMatrixWarning(std::string warntext);

#endif
