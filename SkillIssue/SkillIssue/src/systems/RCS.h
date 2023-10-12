#pragma once
#include "../core/ClientState.h"


class RCS
{
public:
	RCS() { }

	void run() {

	}

	void enable() { enabled = true; }
	void disable() { enabled = false; }
	bool isEnabled() { return enabled; }

private:
	bool enabled = false;

};