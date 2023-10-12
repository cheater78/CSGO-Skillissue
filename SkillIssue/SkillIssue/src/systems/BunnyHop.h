#pragma once
#include "../core/ClientState.h"
#include "../core/KeyBinds.h"


class BunnyHop
{
public:
	BunnyHop() { }

	void run() {
		if(enabled)
			if (GetAsyncKeyState(keyBinds.K_BHOP))
				if (cs.loc.flags.onGround()) {
					cs.loc.input.forceJump(true);
				}
				else {
					cs.loc.input.forceJump(false);
				}
	}

	void enable() { enabled = true; }
	void disable() { enabled = false; }
	bool isEnabled() { return enabled; }

private:
	bool enabled = false;

};