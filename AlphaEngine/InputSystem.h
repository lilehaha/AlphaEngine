#pragma once
#include "stdafx.h"
#include "AInput.h"
#include "functional"

class InputSystem : public Singleton<InputSystem>
{
public:
	InputSystem();
	~InputSystem();
	void Register(std::unordered_map<KeyCode, std::function<void()>> KeyEvents);
	void Register(KeyCode Key, std::function<void()> KeyEvent);
	void Update();
private:
	std::unordered_map<KeyCode, std::function<void()>> EditorEventTable;
};

