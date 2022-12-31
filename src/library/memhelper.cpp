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

#ifdef __linux__
#include <sys/sysinfo.h>
#endif

#include "../headers/debugpar.h"
#include "../headers/memhelper.h"

extern unsigned char DEB;

/*******************************************************
 * Function to get available memory
 * Just now, valid only for Linux
********************************************************/

// After call, avmem and avswap will return the available memory and available swap, in KiB

void GetAvailableMemAndSwap(unsigned long &avmem,unsigned long &avswap)
{
#ifdef __linux__
 struct sysinfo sf;
 if (sysinfo(&sf)==0)
 {
  avmem=(unsigned long)((unsigned long long)sf.freeram*(unsigned long long)sf.mem_unit/1024);
  avswap=(unsigned long)((unsigned long long)sf.freeswap*(unsigned long long)sf.mem_unit/1024);
 }
 else
 {
  avmem=0;
  avswap=0;
 }
#else
// It seems that sysctlbyname in OSX could help. Look at https://stackoverflow.com/questions/12523704/mac-os-x-equivalent-header-file-for-sysinfo-h-in-linux
// In W..s GlobalMemoryStatus() could help...
 avmem=0;
 avswap=0;
#endif
}

void MemoryWarnings(unsigned long nr,int s)
{
 if (nr%2)
  MemoryWarnings(nr/2,nr+1,s);
 else
  MemoryWarnings(nr,(nr+1)/2,s);
}

void MemoryWarnings(unsigned long nr,unsigned long nc,int s)
{
 unsigned long long estimated_size=s*nr*nc/1024;
 unsigned long mem=0,swap=0;
 
 GetAvailableMemAndSwap(mem,swap);
 
 if (mem==0)
  return;
  
 if (DEB)                        // Messages on memory does not depend on the package purpose. They will be shown as long as any debug flag is active
 {
  double percent=double(estimated_size)/double(mem);
  percent = int(10000.0*percent)/100.0;
  std::cout << "  Memory used by the matrix: " << estimated_size << " KiB, which is " << percent << "% of the available memory, which is " << mem << " Kib.\n";
  if (percent < 50.0)
   std::cout << "  That seems OK.\n";
  else
   if (percent < 75.0)
    std::cout << "  This is quite tight. Consider closing some application you don't need just now.\n";
   else
    std::cout << "  You are exhausting your memory. You should close some application you don't need just now.\n";
 }
 
 if (double(estimated_size)>double(mem)+double(swap))
  JMatrixStop("Sorry, your computer has not enough memory to hold the matrix, not even using swap. Unfortunately, nothing can be done about that except getting more RAM.\n");
 
 if (double(estimated_size)>double(mem))
  JMatrixWarning("Your computer has not enough memory to hold the matrix so swap will be used. This means that calculation can be terribly slow. Use Ctrl-C to interrupt the program if you want.\n");
 
 if (double(estimated_size)>0.75*double(mem))
  JMatrixWarning("The matrix needs more than 75% of your computer's memory. This might provoke use of swap which will make calculation terribly slow. Close other applications, if possible, or interrupt the program with Ctrl-C.\n");
    
}

