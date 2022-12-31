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

#ifndef _INDEXTYPE_H
#define _INDEXTYPE_H

/// @file indextype.h

/*!
 \typedef unsigned int indextype
 This is the type of the indexes. It is left fixed, but using a typedef allows easy recompilation of the library if
 we decide otherwise in the future.\n
 It is quite unlikely we need to change indextype, unless, to save some memory and if you know for sure that there will
 be no more than 65536 individuals, can be defined as unsigned short. But this might provoke hidden problems in other places
 so, please, think twice before...\n
 Increasing it to 128 bits (unsigned long long) would be possible, but if you need such size of matrices, better don't use this
 library...
*/
typedef unsigned int indextype;

#endif
