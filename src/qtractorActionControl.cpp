// qtractorActionControl.cpp
//
/****************************************************************************
   Copyright (C) 2005-2015, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#include "qtractorActionControl.h"

#include <QAction>


//----------------------------------------------------------------------
// class qtractorActionControl -- (QAction) MIDI observers map.
//

// Kind of singleton reference.
qtractorActionControl *qtractorActionControl::g_pActionControl = NULL;


// ctor.
qtractorActionControl::qtractorActionControl ( QObject *pParent )
	: QObject(pParent)
{
	// Pseudo-singleton reference setup.
	g_pActionControl = this;
}


// dtor.
qtractorActionControl::~qtractorActionControl (void)
{
	// Pseudo-singleton reference shut-down.
	g_pActionControl = NULL;

	clear();
}


// Kind of singleton reference.
qtractorActionControl *qtractorActionControl::getInstance (void)
{
	return g_pActionControl;
}


// MIDI observer map cleaner.
void qtractorActionControl::clear (void)
{
	qDeleteAll(m_midiObservers);
	m_midiObservers.clear();
};


// MIDI observer map methods.
qtractorActionControl::MidiObserver *qtractorActionControl::getMidiObserver (
	QAction *pAction )
{
	return m_midiObservers.value(pAction, NULL);
}


qtractorActionControl::MidiObserver *qtractorActionControl::addMidiObserver (
	QAction *pAction )
{
	MidiObserver *pMidiObserver = getMidiObserver(pAction);
	if (pMidiObserver == NULL) {
		pMidiObserver = new MidiObserver(pAction);
		m_midiObservers.insert(pAction, pMidiObserver);
	}

	QObject::connect(
		pAction, SIGNAL(triggered(bool)),
		this, SLOT(triggeredSlot(bool)));

	return pMidiObserver;
}


void qtractorActionControl::removeMidiObserver ( QAction *pAction )
{
	MidiObserver *pMidiObserver = getMidiObserver(pAction);
	if (pMidiObserver) {
		QObject::disconnect(
			pAction, SIGNAL(triggered(bool)),
			this, SLOT(triggeredSlot(bool)));
		m_midiObservers.remove(pAction);
		delete pMidiObserver;
	}
}


// MIDI observer trigger slot.
void qtractorActionControl::triggeredSlot ( bool bOn )
{
	QAction *pAction = qobject_cast<QAction *> (sender());
	if (pAction) {
		MidiObserver *pMidiObserver = getMidiObserver(pAction);
		if (pMidiObserver) {
		#ifdef CONFIG_DEBUG
			qDebug("qtractorActionControl::MidiObserver[%p]::triggered(%d)", pMidiObserver, int(bOn));
		#endif
			const float v0 = pMidiObserver->value();
			const float vmax = pMidiObserver->maxValue();
			const float vmin = pMidiObserver->minValue();
			if (pAction->isChecked()) {
				pMidiObserver->setValue(bOn ? vmax : vmin);
			} else {
				const float vmid = 0.5f * (vmax + vmin);
				if (bOn)
					pMidiObserver->setValue(v0 > vmid ? vmin : vmax);
				else
					pMidiObserver->setValue(v0 > vmid ? vmax : vmin);
			}
		}
	}
}


// end of qtractorActionControl.cpp
