#pragma once
#include "../core/ClientState.h"


class AntiFlash
{
public:
	AntiFlash() { }

	void run() {
		if (cs.memory.Read<int32_t>(cs.loc.lPlayer + m_flFlashDuration)) {
			cs.memory.Write<int32_t>(cs.loc.lPlayer + m_flFlashDuration, 0);
		}
	}

	void enable() { enabled = true; }
	void disable() { enabled = false; }
	bool isEnabled() { return enabled; }

private:
	bool enabled = false;

};