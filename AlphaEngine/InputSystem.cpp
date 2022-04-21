#include "stdafx.h"
#include "InputSystem.h"
#include "AEngine.h"

InputSystem::InputSystem()
{
	
}

InputSystem::~InputSystem()
{

}

void InputSystem::Register(std::unordered_map<KeyCode, std::function<void()>> KeyEvents)
{
	for (auto KeyEvent : KeyEvents)
	{
		Register(KeyEvent.first, KeyEvent.second);
	}
}

void InputSystem::Register(KeyCode Key, std::function<void()> KeyEvent)
{
	EditorEventTable.insert({ Key, KeyEvent });
}

void InputSystem::Update()
{
	for (auto Event : EditorEventTable)
	{
		if (Alpha::AEngine::GetSingleton().GetWindow()->GetInput()->GetKeyDown(Event.first))
		{
			Event.second();
			return;
		}
	}
}
