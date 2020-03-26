#ifndef KeyboardMouseHandlerH
#define KeyboardMouseHandlerH

#include <QPoint>
#include <vector>

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

/*! Base class for windows that manage and maintaine key pressed/release states.
	When a keyboard key has been pressed, the corresponding's key state is updated, same
	when it is release.

	In addition, when a mouse button is pressed, the corresponding position is stored, so
	that a mouse move delta kann be determined.

	State management:
	- the state of the handler changes discretely between "frames", that is during updateState() calls.
	- The difference between two states can be used to identify which buttens have been just pressed
	  (before release, now pressed), held (before and now pressed), not pressed (before and now released)
	  and the mouse delta (i.e. movement between position when mouse button was pressed and current mouse position).

	State change:
	- when any of the key methods is clicked,


	The computation of the mouse delta needs to be synchronized with view/camera updates.
	When the delta is computed between
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



	/*! Call this function for each key we are listening to. */
	void addRecognizedKey(Qt::Key k);
	/*! Clears list of recognized keys. */
	void clearRecognizedKeys();

	/*! Called when a key was pressed. */
	bool keyPressed(Qt::Key k);
	/*! Called when a key was released. */
	bool keyReleased(Qt::Key k);
	/*! Called when a mousebutton was pressed. */
	bool buttonPressed(Qt::MouseButton btn, QPoint currentPos);
	/*! Called when a mousebutton was released. */
	bool buttonReleased(Qt::MouseButton btn);

	/*! Returns the position that was recorded, when a mouse button was pressed. */
	QPoint mouseDownPos() const { return m_mouseDownPos; }

	/*! Returns the difference between last and current mouse position and updates
		last mouse position to currentPos.
		The retrieved point (x and y distances) should be used to modify state based transformations.
	*/
	QPoint mouseDelta(const QPoint currentPos);

	/*! Returns, whether the key is pressed. */
	bool keyDown(Qt::Key k) const;
	/*! Returns, whether the mouse button is pressed. */
	bool buttonDown(Qt::MouseButton btn) const;

	/*! Call this function with the current cursor position as argument, and
		the function will return true, if the state has changed since it was last stored.
	*/
	bool stateHasChanged() const { return m_stateHasChanged; }

private:
	std::vector<Qt::Key>	m_keys;
	std::vector<int>		m_keyStates;

	bool					m_leftButtonDown;
	bool					m_middleButtonDown;
	bool					m_rightButtonDown;

	QPoint					m_mouseDownPos;

	bool					m_stateHasChanged;
};

#endif // KeyboardMouseHandlerH
