#include "KeyboardMouseHandler.h"


KeyboardMouseHandler::KeyboardMouseHandler() :
	m_leftButtonDown(false),
	m_middleButtonDown(false),
	m_rightButtonDown(false),
	m_stateHasChanged(false)
{
}


KeyboardMouseHandler::~KeyboardMouseHandler() {
}


void KeyboardMouseHandler::keyPressEvent(QKeyEvent *event) {

}

void KeyboardMouseHandler::keyReleaseEvent(QKeyEvent *event) {

}

void KeyboardMouseHandler::mousePressEvent(QMouseEvent *event) {

}

void KeyboardMouseHandler::mouseReleaseEvent(QMouseEvent *event) {

}

void KeyboardMouseHandler::mouseMoveEvent(QMouseEvent *event) {

}

void KeyboardMouseHandler::wheelEvent(QWheelEvent *event) {

}


void KeyboardMouseHandler::addRecognizedKey(Qt::Key k) {
	if (std::find(m_keys.begin(), m_keys.end(), k) != m_keys.end())
		return; // already known
	// remember key to be known and expected
	m_keys.push_back(k);
	m_keyStates.push_back(0);
}


void KeyboardMouseHandler::clearRecognizedKeys() {
	m_keys.clear();
	m_keyStates.clear();
}


bool KeyboardMouseHandler::keyPressed(Qt::Key k) {
	for (unsigned int i=0; i<m_keys.size(); ++i) {
		if (m_keys[i] == k) {
			m_keyStates[i] = 1;
			return true;
		}
	}
	return false;
}


bool KeyboardMouseHandler::keyReleased(Qt::Key k) {
	for (unsigned int i=0; i<m_keys.size(); ++i) {
		if (m_keys[i] == k) {
			m_keyStates[i] = 0;
			return true;
		}
	}
	return false;
}


bool KeyboardMouseHandler::buttonPressed(Qt::MouseButton btn, QPoint currentPos) {
	switch (btn) {
		case Qt::LeftButton		: m_leftButtonDown = true; break;
		case Qt::MiddleButton	: m_middleButtonDown = true; break;
		case Qt::RightButton	: m_rightButtonDown = true; break;
		default: return false;
	}
	m_mouseDownPos = currentPos;
	return true;
}


bool KeyboardMouseHandler::buttonReleased(Qt::MouseButton btn) {
	switch (btn) {
		case Qt::LeftButton		: m_leftButtonDown = false; break;
		case Qt::MiddleButton	: m_middleButtonDown = false; break;
		case Qt::RightButton	: m_rightButtonDown = false; break;
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
