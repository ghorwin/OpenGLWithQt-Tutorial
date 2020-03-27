#include "KeyboardMouseHandler.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>


KeyboardMouseHandler::KeyboardMouseHandler() :
	m_leftButtonDown(StateNotPressed),
	m_middleButtonDown(StateNotPressed),
	m_rightButtonDown(StateNotPressed)
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
	pressButton(static_cast<Qt::MouseButton>(event->button()), event->pos());
}


void KeyboardMouseHandler::mouseReleaseEvent(QMouseEvent *event) {
	releaseButton(static_cast<Qt::MouseButton>(event->button()));
}


void KeyboardMouseHandler::mouseMoveEvent(QMouseEvent *) {
	// if needed, implement some logic here
}


void KeyboardMouseHandler::wheelEvent(QWheelEvent *event) {

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


bool KeyboardMouseHandler::releaseButton(Qt::MouseButton btn) {
	switch (btn) {
		case Qt::LeftButton		: m_leftButtonDown = StateWasPressed; break;
		case Qt::MiddleButton	: m_middleButtonDown = StateWasPressed; break;
		case Qt::RightButton	: m_rightButtonDown = StateWasPressed; break;
		default: return false;
	}
	return true;
}


QPoint KeyboardMouseHandler::mouseDelta(const QPoint currentPos) {
	QPoint dist = currentPos - m_mouseDownPos;
	m_mouseDownPos = currentPos;
	return dist;
}


bool KeyboardMouseHandler::keyDown(Qt::Key k) const {
	for (unsigned int i=0; i<m_keys.size(); ++i) {
		if (m_keys[i] == k)
			return (bool)m_keyStates[i];
	}
	return false;
}


bool KeyboardMouseHandler::buttonDown(Qt::MouseButton btn) const {
	switch (btn) {
		case Qt::LeftButton		: return m_leftButtonDown;
		case Qt::MiddleButton	: return m_middleButtonDown;
		case Qt::RightButton	: return m_rightButtonDown;
		default: return false;
	}
}
