#pragma once
#include "../core/ClientState.h"
#include "../core/Entities.h"

#include <windows.h>




class TriggerBot
{
public:
	TriggerBot() { }

	void run() {
		Player aimAt((cs.loc.lCrossID - 1));

		if (aimAt.isAlive() && aimAt.isEnemy()) {
			LeftClick();
		}
	}

	void enable() { enabled = true; }
	void disable() { enabled = false; }
	bool isEnabled() { return enabled; }

private:
	bool enabled = false;

	void LeftClick()
	{
		INPUT    Input = { 0 };
		// left down 
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		::SendInput(1, &Input, sizeof(INPUT));
		Sleep(1);
		// left up
		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}
};