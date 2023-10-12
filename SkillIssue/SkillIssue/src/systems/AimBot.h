#pragma once
#include "../core/ClientState.h"


class AimBot
{
public:
	AimBot() { }

	void run(vector<Entity> entities) {
		if (!GetAsyncKeyState(keyBinds.K_AIMBOT)) return;
		
		float shortest2Ddist = 40.f;
		Vector3 closestAng{};
		for (auto entity : entities) {
			if (!entity.isPlayer()) continue;
			Player enemy = entity;
			if (!enemy.isEnemy()) continue;
			if (!enemy.isAlive()) continue;
			if (enemy.isDormant()) continue;

			auto angle = Vector3::CalculateAngle(
				cs.loc.pos.lEyePosition,
				enemy.getHeadPos(),
				cs.loc.pos.lViewAngles + cs.loc.pos.lAimPunchAng
			);

			if (rageMode) {
				if (!angle.IsZero()) {
					cs.memory.Write<Vector3>(cs.clientState + dwClientState_ViewAngles, angle);

					int timeout = 0;
					while ((cs.loc.lCrossID - 1) == -1 && timeout < 5) {
						cs.update();
						timeout++;
						Sleep(1);
					}

					if ((cs.loc.lCrossID - 1) == -1) continue;
					else return;
				}
			}
			else {
				
				float c2Ddist = std::hypot(angle.x, angle.y);
				//cout << c2Ddist << endl;

				if (c2Ddist < shortest2Ddist) {
					shortest2Ddist = c2Ddist;
					closestAng = angle;
				}
			}

			
		}

		if (!closestAng.IsZero() && !rageMode) {
			cs.memory.Write<Vector3>(cs.clientState + dwClientState_ViewAngles,  closestAng);
			return;
		}

	}

	void enable() { enabled = true; }
	void disable() { enabled = false; }
	bool isEnabled() { return enabled; }

private:
	bool enabled = false;
	bool rageMode = true;

};