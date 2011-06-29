// qtractorCurveCommand.cpp
//
/****************************************************************************
   Copyright (C) 2005-2011, rncbc aka Rui Nuno Capela. All rights reserved.

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

#include "qtractorAbout.h"
#include "qtractorCurveCommand.h"

#include "qtractorSession.h"

#include "qtractorMainForm.h"
#include "qtractorTracks.h"


//----------------------------------------------------------------------
// class qtractorCurveBaseCommand - declaration.
//

// Constructor.
qtractorCurveBaseCommand::qtractorCurveBaseCommand ( const QString& sName )
	: qtractorCommand(sName)
{
}

// Virtual command methods.
bool qtractorCurveBaseCommand::redo (void)
{
	return execute(true);
}

bool qtractorCurveBaseCommand::undo (void)
{
	return execute(false);
}


// Virtual command methods.
bool qtractorCurveBaseCommand::execute ( bool /*bRedo*/ )
{
	qtractorSession *pSession = qtractorSession::getInstance();
	if (pSession && !pSession->isPlaying())
		pSession->process_curve(pSession->playHead());

	qtractorMainForm *pMainForm = qtractorMainForm::getInstance();
	if (pMainForm && pMainForm->tracks())
		pMainForm->tracks()->updateTrackList();

	return true;
}


//----------------------------------------------------------------------
// class qtractorCurveCommand - declaration.
//

// Constructor.
qtractorCurveCommand::qtractorCurveCommand (
	const QString& sName, qtractorCurve *pCurve )
	: qtractorCurveBaseCommand(sName), m_pCurve(pCurve)
{
}


//----------------------------------------------------------------------
// class qtractorCurveListCommand - declaration.
//

// Constructor.
qtractorCurveListCommand::qtractorCurveListCommand (
	const QString& sName, qtractorCurveList *pCurveList )
	: qtractorCurveBaseCommand(sName), m_pCurveList(pCurveList)
{
}


//----------------------------------------------------------------------
// class qtractorCurveSelectCommand - declaration.
//

// Constructor.
qtractorCurveSelectCommand::qtractorCurveSelectCommand (
	qtractorCurveList *pCurveList, qtractorCurve *pCurrentCurve )
	: qtractorCurveListCommand(QObject::tr("automation select"), pCurveList),
		m_pCurrentCurve(pCurrentCurve)
{
}


// Virtual command methods.
bool qtractorCurveSelectCommand::execute ( bool /*bRedo*/ )
{
	if (m_pCurveList == NULL)
		return false;

	qtractorCurve *pCurrentCurve = m_pCurveList->currentCurve();
	m_pCurveList->setCurrentCurve(m_pCurrentCurve);
	m_pCurrentCurve = pCurrentCurve;

	return true;
}


//----------------------------------------------------------------------
// class qtractorCurveModeCommand - declaration.
//

// Constructor.
qtractorCurveModeCommand::qtractorCurveModeCommand (
	qtractorCurve *pCurve, qtractorCurve::Mode mode )
	: qtractorCurveCommand(QObject::tr("automation mode"), pCurve),
		m_mode(mode)
{
}


// Virtual command methods.
bool qtractorCurveModeCommand::execute ( bool bRedo )
{
	if (m_pCurve == NULL)
		return false;

	qtractorCurve::Mode mode = m_pCurve->mode();
	m_pCurve->setMode(m_mode);
	m_pCurve->update();
	m_mode = mode;

	return qtractorCurveBaseCommand::execute(bRedo);
}


//----------------------------------------------------------------------
// class qtractorCurveProcessCommand - declaration.
//

// Constructor.
qtractorCurveProcessCommand::qtractorCurveProcessCommand (
	qtractorCurve *pCurve, bool bProcess )
	: qtractorCurveCommand(QObject::tr("automation play"), pCurve),
		m_bProcess(bProcess)
{
}


// Virtual command methods.
bool qtractorCurveProcessCommand::execute ( bool bRedo )
{
	if (m_pCurve == NULL)
		return false;

	bool bProcess = m_pCurve->isProcess();
	if (!m_bProcess)
		m_pCurve->setCapture(false);
	m_pCurve->setProcess(m_bProcess);
	m_bProcess = bProcess;

	return qtractorCurveBaseCommand::execute(bRedo);
}


//----------------------------------------------------------------------
// class qtractorCurveCaptureCommand - declaration.
//

// Constructor.
qtractorCurveCaptureCommand::qtractorCurveCaptureCommand (
	qtractorCurve *pCurve, bool bCapture )
	: qtractorCurveCommand(QObject::tr("automation record"), pCurve),
		m_bCapture(bCapture)
{
}


// Virtual command methods.
bool qtractorCurveCaptureCommand::execute ( bool bRedo )
{
	if (m_pCurve == NULL)
		return false;

	bool bCapture = m_pCurve->isCapture();
	if (m_bCapture)
		m_pCurve->setProcess(true);
	m_pCurve->setCapture(m_bCapture);
	m_bCapture = bCapture;

	return qtractorCurveBaseCommand::execute(bRedo);
}


//----------------------------------------------------------------------
// class qtractorCurveLogarithmicCommand - declaration.
//

// Constructor.
qtractorCurveLogarithmicCommand::qtractorCurveLogarithmicCommand (
	qtractorCurve *pCurve, bool bLogarithmic )
	: qtractorCurveCommand(QObject::tr("automation logarithmic"), pCurve),
		m_bLogarithmic(bLogarithmic)
{
}


// Virtual command methods.
bool qtractorCurveLogarithmicCommand::execute ( bool /*bRedo*/ )
{
	if (m_pCurve == NULL)
		return false;

	bool bLogarithmic = m_pCurve->isLogarithmic();
	m_pCurve->setLogarithmic(m_bLogarithmic);
	m_bLogarithmic = bLogarithmic;

	return true;
}


//----------------------------------------------------------------------
// class qtractorCurveCaptureCommand - declaration.
//

// Constructor.
qtractorCurveColorCommand::qtractorCurveColorCommand (
	qtractorCurve *pCurve, const QColor& color )
	: qtractorCurveCommand(QObject::tr("automation color"), pCurve),
		m_color(color)
{
}


// Virtual command methods.
bool qtractorCurveColorCommand::execute ( bool /*bRedo*/ )
{
	if (m_pCurve == NULL)
		return false;

	QColor color = m_pCurve->color();
	m_pCurve->setColor(m_color);
	m_color = color;

	return true;
}


//----------------------------------------------------------------------
// class qtractorCurveProcessAllCommand - declaration.
//

// Constructor.
qtractorCurveProcessAllCommand::qtractorCurveProcessAllCommand (
	qtractorCurveList *pCurveList, bool bProcessAll )
	: qtractorCurveListCommand(QObject::tr("automation play all"), pCurveList),
		m_bProcessAll(bProcessAll)
{
}


// Virtual command methods.
bool qtractorCurveProcessAllCommand::execute ( bool bRedo )
{
	if (m_pCurveList == NULL)
		return false;

	bool bProcessAll = m_pCurveList->isProcessAll();
	if (!m_bProcessAll)
		m_pCurveList->setCaptureAll(false);
	m_pCurveList->setProcessAll(m_bProcessAll);
	m_bProcessAll = bProcessAll;

	return qtractorCurveBaseCommand::execute(bRedo);
}


//----------------------------------------------------------------------
// class qtractorCurveCaptureAllCommand - declaration.
//

// Constructor.
qtractorCurveCaptureAllCommand::qtractorCurveCaptureAllCommand (
	qtractorCurveList *pCurveList, bool bCaptureAll )
	: qtractorCurveListCommand(QObject::tr("automation record all"), pCurveList),
		m_bCaptureAll(bCaptureAll)
{
}


// Virtual command methods.
bool qtractorCurveCaptureAllCommand::execute ( bool bRedo )
{
	if (m_pCurveList == NULL)
		return false;

	bool bCaptureAll = m_pCurveList->isCaptureAll();
	if (m_bCaptureAll)
		m_pCurveList->setProcessAll(true);
	m_pCurveList->setCaptureAll(m_bCaptureAll);
	m_bCaptureAll = bCaptureAll;

	return qtractorCurveBaseCommand::execute(bRedo);
}


//----------------------------------------------------------------------
// class qtractorCurveEditCommand - declaration.
//

// Constructors.
qtractorCurveEditCommand::qtractorCurveEditCommand (
	const QString& sName, qtractorCurve *pCurve )
	: qtractorCurveCommand(sName, pCurve)
{
}

qtractorCurveEditCommand::qtractorCurveEditCommand ( qtractorCurve *pCurve )
	: qtractorCurveCommand(QObject::tr("automation edit"), pCurve)
{
}


// Destructor.
qtractorCurveEditCommand::~qtractorCurveEditCommand (void)
{
	QListIterator<Item *> iter(m_items);
	while (iter.hasNext()) {
		Item *pItem = iter.next();
		if (pItem->autoDelete)
			delete pItem->node;
	}

	qDeleteAll(m_items);
	m_items.clear();
}


// Primitive command methods.
void qtractorCurveEditCommand::addNode ( qtractorCurve::Node *pNode )
{
	m_items.append(new Item(AddNode, pNode));
}


void qtractorCurveEditCommand::removeNode ( qtractorCurve::Node *pNode )
{
	m_items.append(new Item(RemoveNode, pNode));
}


// Composite predicate.
bool qtractorCurveEditCommand::isEmpty (void) const
{
	return m_items.isEmpty();
}


// Common executive method.
bool qtractorCurveEditCommand::execute ( bool bRedo )
{
	if (m_pCurve == NULL)
		return false;

	QListIterator<Item *> iter(m_items);
	if (!bRedo)
		iter.toBack();
	while (bRedo ? iter.hasNext() : iter.hasPrevious()) {
		Item *pItem = (bRedo ? iter.next() : iter.previous());
		// Execute the command item...
		switch (pItem->command) {
		case AddNode:
			if (bRedo)
				m_pCurve->insertNode(pItem->node);
			else
				m_pCurve->unlinkNode(pItem->node);
			break;
			pItem->autoDelete = !bRedo;
		case RemoveNode:
			if (bRedo)
				m_pCurve->unlinkNode(pItem->node);
			else
				m_pCurve->insertNode(pItem->node);
			pItem->autoDelete = bRedo;
			break;
		default:
			break;
		}
	}

	return qtractorCurveBaseCommand::execute(bRedo);
}


//----------------------------------------------------------------------
// class qtractorCurveClearCommand - declaration.
//

// Constructor.
qtractorCurveClearCommand::qtractorCurveClearCommand ( qtractorCurve *pCurve )
	: qtractorCurveEditCommand(QObject::tr("automation clear"), pCurve)
{
	qtractorCurve::Node *pNode = m_pCurve->nodes().first();
	while (pNode) {
		qtractorCurveEditCommand::removeNode(pNode);
		pNode = pNode->next();
	}
}


//----------------------------------------------------------------------
// class qtractorCurveClearAllCommand - declaration.
//

// Constructor.
qtractorCurveClearAllCommand::qtractorCurveClearAllCommand (
	qtractorCurveList *pCurveList )
	: qtractorCurveListCommand(QObject::tr("automation clear all"), pCurveList)
{
	if (m_pCurveList) {
		qtractorCurve *pCurve = m_pCurveList->first();
		while (pCurve) {
			m_commands.append(new qtractorCurveClearCommand(pCurve));
			pCurve = pCurve->next();
		}
	}
}


// Destructor.
qtractorCurveClearAllCommand::~qtractorCurveClearAllCommand (void)
{
	qDeleteAll(m_commands);
	m_commands.clear();
}


// Composite predicate.
bool qtractorCurveClearAllCommand::isEmpty (void) const
{
	return m_commands.isEmpty();
}


// Virtual executive method.
bool qtractorCurveClearAllCommand::execute ( bool bRedo )
{
	if (m_pCurveList == NULL)
		return false;

	QListIterator<qtractorCurveClearCommand *> iter(m_commands);
	while (iter.hasNext()) {
		qtractorCurveClearCommand *pCommand = iter.next();
		if (bRedo)
			pCommand->redo();
		else
			pCommand->undo();
	}

	return true;
}


// end of qtractorCurveCommand.cpp
