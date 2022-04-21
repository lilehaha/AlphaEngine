#pragma once
#include "stdafx.h"

enum class KeyCode
{
	None,
	Backspace,
	Tab,
	Clear,
	Return,
	Pause,
	Escape,
	Space,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Keypad0,
	Keypad1,
	Keypad2,
	Keypad3,
	Keypad4,
	Keypad5,
	Keypad6,
	Keypad7,
	Keypad8,
	Keypad9,
	UpArrow,
	DownArrow,
	RightArrow,
	LeftArrow,
	Mouse0,		// Left Mouse Button
	Mouse1,		// Middle Mouse Button
	Mouse2,		// Right Mouse Button
	Add,
	Minus,
	NUM
};

enum class KeyState
{
	None = 0,
	KeyDown = 1,
	KeyHold = 2,
	KeyUp = 4
};

class AInput
{
public:
	virtual void Update() = 0;
	virtual bool GetKeyHold(KeyCode Key) = 0;
	virtual bool GetKeyUp(KeyCode Key) = 0;
	virtual bool GetKeyDown(KeyCode Key) = 0;
	virtual std::pair<float, float> GetMousePosition() = 0;
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMouseDown(KeyCode MouseBtn, int x, int y) = 0;
	virtual void OnMouseUp(KeyCode MouseBtn) = 0;
	virtual bool GetMouseDown(KeyCode MouseBtn) = 0;
	virtual bool GetMouseUp(KeyCode MouseBtn) = 0;

protected:
	virtual void UpdateKeyState(KeyCode keyCode, int WindowsKey) = 0;
	virtual void UpdateMouseState() = 0;
};


