// qtractorPropertyCommand.h
//
/****************************************************************************
   Copyright (C) 2005, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*****************************************************************************/

#ifndef __qtractorPropertyCommand_h
#define __qtractorPropertyCommand_h

#include "qtractorCommand.h"


//----------------------------------------------------------------------
// class qtractorPropertyCommand - template declaration.
//

template<typename T>
class qtractorPropertyCommand : public qtractorCommand
{
public:

	// Constructor.
	qtractorPropertyCommand(qtractorMainForm *pMainForm,
	    const QString& sName, T& ref, const T& val)
	    : qtractorCommand(pMainForm, sName), m_ref(ref), m_val(val) {}

	// Cannonical command methods.
	bool redo()
	{
		T val = m_ref;
		m_ref = m_val;
		m_val = val;
		return true;
	}

	bool undo() { return redo(); }

private:

	// Instance variables.
	T& m_ref;
	T  m_val;
};


#endif  // __qtractorPropertyCommand_h


// end of qtractorPropertyCommand.h
