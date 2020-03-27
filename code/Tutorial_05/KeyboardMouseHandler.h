#ifndef KeyboardMouseHandlerH
#define KeyboardMouseHandlerH

#include <QPoint>
#include <vector>

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

/*! An example keyboard/mouse handler implementation.
	When a keyboard key has been pressed, the corresponding's key state is updated, same
	when it is release (in this case the state will be "was pressed" until cleared). This
	"was pressed" state is needed in those cases, where a delay of state requesting on the
	client side causes a keyPress and keyRelease Sequence to happen before the states
	where checked.

	In addition, when a mouse button is pressed, the corresponding position is stored, so
	that a mouse move delta kann be determined.

	The handler monitors only keys that it was told to monitor. Mouse buttons are all monitored.
	Modification keys are used just as any other key.

	State management:
	- the states of pressed keys are remembered in state WasPressed until clearWasPressedKeyStates() is called
	- the mouse position is updated in mouseDelta(), which requires the new mouse cursor position to update
	  the mouse position for the next call cycle

	The time elapsed since last call is retrieved with timeDelta(), which also resets the time again.

	\code
	// usage pattern

	// ... some change of state is registered during the event loop

	// evaluate input handler state

	// process all keys and check if they are either held or where pressed

	if (m_inputHandler.keyDown(Qt::Key_W)) {
		...
	}

	// get and reset mouse delta (pass current mouse cursor position)
	QPoint mouseDelta = m_inputHandler.mouseDelta(QCursor::pos());

	// get and reset time delta
	double timeSinceLastCheck = m_inputHandler.timeDelta(); // in ms

	// compute mouse speed
	static const float rotatationSpeed   = 0.001f; // in space units/ms

	float yRot = rotationSpeed * timeSinceLastCheck * mouseDelta.y();

	...

	// finally, reset "WasPressed" key states
	m_inputHandler.clearWasPressedKeyStates();
	\endcode
*/
class KeyboardMouseHandler {
public:
	KeyboardMouseHandler();
	virtual ~KeyboardMouseHandler();

	// Functions to handle key press and mouse press events
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

	enum KeyStates {
		StateNotPressed,
		StateHeld,
		StateWasPressed
	};

	/*! Call this function for each key we are listening to. */
	void addRecognizedKey(Qt::Key k);
	/*! Clears list of recognized keys. */
	void clearRecognizedKeys();

	/*! This resets all key states currently marked as "WasPressed". */
	void clearWasPressedKeyStates();

	/*! Called when a key was pressed. */
	bool pressKey(Qt::Key k);
	/*! Called when a key was released. */
	bool releaseKey(Qt::Key k);
	/*! Called when a mousebutton was pressed. */
	bool pressButton(Qt::MouseButton btn, QPoint currentPos);
	/*! Called when a mousebutton was released. */
	bool releaseButton(Qt::MouseButton btn);

	/*! Returns the position that was recorded, when a mouse button was pressed. */
	QPoint mouseDownPos() const { return m_mouseDownPos; }

	/*! Returns the difference between last and current mouse position and *updates*
		last mouse position to currentPos.
		The retrieved point (x and y distances) should be used to modify state based transformations.
	*/
	QPoint mouseDelta(const QPoint currentPos);

	/*! Returns, whether the key is pressed. */
	bool keyDown(Qt::Key k) const;
	/*! Returns, whether the mouse button is pressed. */
	bool buttonDown(Qt::MouseButton btn) const;

private:
	std::vector<Qt::Key>	m_keys;
	std::vector<KeyStates>	m_keyStates;

	KeyStates				m_leftButtonDown;
	KeyStates				m_middleButtonDown;
	KeyStates				m_rightButtonDown;

	QPoint					m_mouseDownPos;
};

#endif // KeyboardMouseHandlerH
