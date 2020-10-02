/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "KeyboardMouseHandler.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>


KeyboardMouseHandler::KeyboardMouseHandler() :
	m_leftButtonDown(StateNotPressed),
	m_middleButtonDown(StateNotPressed),
	m_rightButtonDown(StateNotPressed),
	m_wheelDelta(0)
{
}


KeyboardMouseHandler::~KeyboardMouseHandler() {
}



void KeyboardMouseHandler::keyPressEvent(QKeyEvent *event) {
	if (event->isAutoRepeat()) {
		event->ignore();
	}
	else {
		pressKey(static_cast<Qt::Key>((event->key())));
	}
}


void KeyboardMouseHandler::keyReleaseEvent(QKeyEvent *event) {
	if (event->isAutoRepeat())	{
		event->ignore();
	}
	else {
		releaseKey(static_cast<Qt::Key>((event->key())));
	}
}


void KeyboardMouseHandler::mousePressEvent(QMouseEvent *event) {
	pressButton(static_cast<Qt::MouseButton>(event->button()), event->globalPos());
}


void KeyboardMouseHandler::mouseReleaseEvent(QMouseEvent *event) {
	releaseButton(static_cast<Qt::MouseButton>(event->button()), event->globalPos());
}


void KeyboardMouseHandler::wheelEvent(QWheelEvent *event) {
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;

	if (!numPixels.isNull()) {
		m_wheelDelta += numPixels.y();
	} else if (!numDegrees.isNull()) {
		QPoint numSteps = numDegrees / 15;
		m_wheelDelta += numSteps.y();
	}

	event->accept();
}


void KeyboardMouseHandler::addRecognizedKey(Qt::Key k) {
	if (std::find(m_keys.begin(), m_keys.end(), k) != m_keys.end())
		return; // already known
	// remember key to be known and expected
	m_keys.push_back(k);
	m_keyStates.push_back(StateNotPressed);
}


void KeyboardMouseHandler::clearRecognizedKeys() {
	m_keys.clear();
	m_keyStates.clear();
}


void KeyboardMouseHandler::clearWasPressedKeyStates() {
	m_leftButtonDown = (m_leftButtonDown == StateWasPressed) ? StateNotPressed  : m_leftButtonDown;
	m_middleButtonDown = (m_middleButtonDown == StateWasPressed) ? StateNotPressed  : m_middleButtonDown;
	m_rightButtonDown = (m_rightButtonDown == StateWasPressed) ? StateNotPressed  : m_rightButtonDown;

	for (unsigned int i=0; i<m_keyStates.size(); ++i)
		m_keyStates[i] = static_cast<KeyStates>(m_keyStates[i] & 1); // toggle "WasPressed" bit -> NotPressed
}



bool KeyboardMouseHandler::pressKey(Qt::Key k) {
	for (unsigned int i=0; i<m_keys.size(); ++i) {
		if (m_keys[i] == k) {
			m_keyStates[i] = StateHeld;
			return true;
		}
	}
	return false;
}


bool KeyboardMouseHandler::releaseKey(Qt::Key k) {
	for (unsigned int i=0; i<m_keys.size(); ++i) {
		if (m_keys[i] == k) {
			m_keyStates[i] = StateWasPressed;
			return true;
		}
	}
	return false;
}


bool KeyboardMouseHandler::pressButton(Qt::MouseButton btn, QPoint currentPos) {
	switch (btn) {
		case Qt::LeftButton		: m_leftButtonDown = StateHeld; break;
		case Qt::MiddleButton	: m_middleButtonDown = StateHeld; break;
		case Qt::RightButton	: m_rightButtonDown = StateHeld; break;
		default: return false;
	}
	m_mouseDownPos = currentPos;
	return true;
}


bool KeyboardMouseHandler::releaseButton(Qt::MouseButton btn, QPoint currentPos) {
	switch (btn) {
		case Qt::LeftButton		: m_leftButtonDown = StateWasPressed; break;
		case Qt::MiddleButton	: m_middleButtonDown = StateWasPressed; break;
		case Qt::RightButton	: m_rightButtonDown = StateWasPressed; break;
		default: return false;
	}
	m_mouseReleasePos = currentPos;
	return true;
}


QPoint KeyboardMouseHandler::resetMouseDelta(const QPoint currentPos) {
	QPoint dist = currentPos - m_mouseDownPos;
	m_mouseDownPos = currentPos;
	return dist;
}


int KeyboardMouseHandler::wheelDelta() const {
	return m_wheelDelta;
}


int KeyboardMouseHandler::resetWheelDelta() {
	int wd = m_wheelDelta;
	m_wheelDelta = 0;
	return wd;
}


bool KeyboardMouseHandler::keyDown(Qt::Key k) const {
	for (unsigned int i=0; i<m_keys.size(); ++i) {
		if (m_keys[i] == k)
			return m_keyStates[i] == StateHeld;
	}
	return false;
}


bool KeyboardMouseHandler::buttonDown(Qt::MouseButton btn) const {
	switch (btn) {
		case Qt::LeftButton		: return m_leftButtonDown == StateHeld;
		case Qt::MiddleButton	: return m_middleButtonDown == StateHeld;
		case Qt::RightButton	: return m_rightButtonDown == StateHeld;
		default: return false;
	}
}


bool KeyboardMouseHandler::buttonReleased(Qt::MouseButton btn) const {
	switch (btn) {
		case Qt::LeftButton		: return m_leftButtonDown == StateWasPressed;
		case Qt::MiddleButton	: return m_middleButtonDown == StateWasPressed;
		case Qt::RightButton	: return m_rightButtonDown == StateWasPressed;
		default: return false;
	}
}

