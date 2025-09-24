///////////////////////////////////////////////////////////////////////////////
// Name               : StdInclude.h
// Purpose            : Takes care of precompiled header issues
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 21.05.2009
// Copyright          : (C) 2009 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDINC_H
#define STDINC_H

#pragma GCC system_header

// check if to use precompiled headers: do it for most Windows compilers unless
// explicitly disabled by defining NOPCH
#if ( defined(__WXMSW__) && \
        ( defined(__BORLANDC__)    || \
          defined(__VISUALC__)     || \
          defined(__DIGITALMARS__) || \
          defined(__WATCOMC__) ) ) || \
       defined(__VISAGECPP__) || \
       defined(__MWERKS__)

     // If user did not request NOPCH and we're not building using configure
     // then assume user wants precompiled headers.
     #if !defined(NOPCH) && !defined(__WX_SETUP_H__)
         #define WX_PRECOMP
     #endif
 #endif

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define __WIN
#elif defined(linux) || defined(__linux)
#define __LINUX
#else
#error "Neither a Linux nor a Windows system was found!"
#endif

#endif /* STDINC_H */

