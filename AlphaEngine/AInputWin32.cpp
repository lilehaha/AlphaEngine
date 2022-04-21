#include "stdafx.h"
#include "AInputWin32.h"

AInputWin32::AInputWin32() : mKeyStates{}, mMousePosition(std::pair<float, float>(0, 0))
{

}

void AInputWin32::Update()
{
	UpdateKeyState(KeyCode::W, 'W');
	UpdateKeyState(KeyCode::A, 'A');
	UpdateKeyState(KeyCode::S, 'S');
	UpdateKeyState(KeyCode::D, 'D');
	UpdateKeyState(KeyCode::Q, 'Q');
	UpdateKeyState(KeyCode::E, 'E');

	UpdateMouseState();
}



bool AInputWin32::GetKeyHold(KeyCode Key)
{
	return ((int)mKeyStates[(int)Key] & (int)KeyState::KeyHold) != 0;
}

bool AInputWin32::GetKeyUp(KeyCode Key)
{
	return ((int)mKeyStates[(int)Key] & (int)KeyState::KeyUp) != 0;
}

bool AInputWin32::GetKeyDown(KeyCode Key)
{
	return ((int)mKeyStates[(int)Key] & (int)KeyState::KeyUp) != 0;
}

std::pair<float, float> AInputWin32::GetMousePosition()
{
	return mMousePosition;
}

void AInputWin32::UpdateKeyState(KeyCode Key, int WindowsKey)
{
	bool isKeyDown = GetAsyncKeyState(WindowsKey) & 0x0001;
	bool isKeyHold = GetAsyncKeyState(WindowsKey) & 0x8000;

	if (isKeyDown)
	{
		mKeyStates[(int)Key] = KeyState::KeyDown;
	}
	else if (isKeyHold)
	{
		mKeyStates[(int)Key] = KeyState::KeyHold;
	}
	else
	{
		mKeyStates[(int)Key] = KeyState::KeyUp;
	}
}

void AInputWin32::OnMouseDown(KeyCode MouseBtn, int x, int y)
{
	mMousePosition = std::pair<float, float>(float(x), float(y));
	mKeyStates[(int)MouseBtn] = KeyState::KeyDown;
}

void AInputWin32::OnMouseUp(KeyCode MouseBtn)
{
	mKeyStates[(int)MouseBtn] = KeyState::KeyUp;
}

bool AInputWin32::GetMouseDown(KeyCode MouseBtn)
{
	return ((int)mKeyStates[(int)MouseBtn] & (int)KeyState::KeyDown) != 0;
}

bool AInputWin32::GetMouseUp(KeyCode MouseBtn)
{
	return ((int)mKeyStates[(int)MouseBtn] & (int)KeyState::KeyUp) != 0;
}

void AInputWin32::UpdateMouseState()
{
	if (mKeyStates[(int)KeyCode::Mouse0] == KeyState::KeyDown)
		mKeyStates[(int)KeyCode::Mouse0] = KeyState::KeyHold;

	if (mKeyStates[(int)KeyCode::Mouse2] == KeyState::KeyDown)
		mKeyStates[(int)KeyCode::Mouse2] = KeyState::KeyHold;
}

void AInputWin32::OnMouseMove(int x, int y)
{
	mMousePosition = std::pair<float, float>(float(x), float(y));
}