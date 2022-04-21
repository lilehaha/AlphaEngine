#pragma once
#include "AInput.h"

class AInputWin32 : public AInput
{
public:
	AInputWin32();
	~AInputWin32() = default;
	virtual void Update() override;
	virtual bool GetKeyHold(KeyCode Key) override;
	virtual bool GetKeyUp(KeyCode Key) override;
	virtual bool GetKeyDown(KeyCode Key) override;
	virtual std::pair<float, float> GetMousePosition() override;
	virtual void OnMouseMove(int x, int y) override;
	virtual void OnMouseDown(KeyCode MouseBtn, int x, int y) override;
	virtual void OnMouseUp(KeyCode MouseBtn) override;
	virtual bool GetMouseDown(KeyCode MouseBtn) override;
	virtual bool GetMouseUp(KeyCode MouseBtn) override;
	    
protected:
	virtual void UpdateKeyState(KeyCode Key, int WindowsKey) override;
	virtual void UpdateMouseState() override;

protected:
	std::array<KeyState, (size_t)KeyCode::NUM> mKeyStates;
	std::pair<float, float> mMousePosition;
};

