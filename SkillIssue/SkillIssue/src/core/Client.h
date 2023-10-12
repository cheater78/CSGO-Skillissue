#pragma once
#include "ClientState.h"


#include <vector>
#include "../systems/GlowWall.h"
#include "../systems/BunnyHop.h"
#include "../systems/AntiFlash.h"
#include "Entities.h"
#include "../systems/GlowWall.h"
#include "../systems/TriggerBot.h"
#include "../systems/AimBot.h"
#include "../systems/RCS.h"

using namespace std;

class Client {

public:
	Client() : glowWall{}, bhop{}, antiFlash{} {

		glowWall.enable();
		bhop.enable();
		antiFlash.enable();
		trigBot.enable();
		aimBot.enable();
		rcs.enable();

		update();
	}

	bool run() {
		while (running) {
			init();
			while (loop());
		}
		return !running;
	}

	void init() {
		std::cout << "> Currently not in a Match" << std::endl;
		while (!cs.init());
		std::cout << "> Compensating SkillIssue" << std::endl;

		
	}

	bool loop() {
		cs.update();
		update();

		glowWall.run(entities);
		antiFlash.run();
		bhop.run();
		trigBot.run();
		aimBot.run(entities);
		rcs.run();

		return cs.isMatchInProgress() && running;
	}
	

private:
	bool running = true;

	vector<Entity> entities{};

	GlowWall glowWall;
	AntiFlash antiFlash;
	BunnyHop bhop;
	TriggerBot trigBot;
	AimBot aimBot;
	RCS rcs;


	void update() {
		//Clear entitiestack
		if (entities.size() > 0) entities.clear();

		getEntities();
	}

	void getEntities() {
		const auto entityList = cs.memory.Read<uintptr_t>(cs.client + dwEntityList);
		const auto maxEnts = cs.memory.Read<uintptr_t>(cs.client + dwEntityList + 0x24);

		for (uint32_t entID = 0; entID < 700; entID++) {
			Entity entity(entID);

			if (!entity.exits()) continue;

			if (entID < 64) {
				//Player
				if (entity.isPlayer()) {
					Player pl(entity);
					if (pl.isAlive() && !pl.isDormant())
						entities.push_back(pl);
				}
			}
			else {
				//Other Entities
				if (entity.classID == 23 && false) {
					C4 c4 = entity;

					entities.push_back(c4);
				}
			}
		}

	}
};