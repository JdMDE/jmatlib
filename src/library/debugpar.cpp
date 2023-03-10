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

#include "../headers/debugpar.h"

unsigned char DEB=NODEBUG;

//' JMatrixSetDebug
//'
//' Sets debugging in jmatrix package to ON (with TRUE) or OFF (with FALSE).\cr
//' On package load the default status is OFF.\cr
//' Setting debugging to ON shows a message. Setting to OFF does not show anything (since debugging is OFF...)
//'
//' @param deb   boolean, TRUE to generate debug messages and FALSE to turn them off. Default: true
void JMatrixSetDebug(bool deb = true)
{
 if (deb)
 {
  DEB |= DEBJM;
  std::cout << "Debugging for jmatrix package set to ON.\n";
 }
 else
  DEB &= (~DEBJM);
}

void JMatrixStop(std::string errortext)
{
 std::cerr << "Error message from the JMatrix library:\n";
 std::cerr << "   " << errortext;
 exit(1);
}

void JMatrixWarning(std::string warntext)
{
 std::cout << "Warning message from the JMatrix library:\n";
 std::cout << "   " << warntext;
}
