#pragma once
#include "../utils/memory.h"
#include "../utils/vector.h"
#include "../../lib/hazedumper-v2.4.1/csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;
using namespace std;



struct ClientState {

	Memory memory = Memory{ "csgo.exe" };
	uintptr_t client;
	uintptr_t engine;

	uintptr_t clientState;
	uintptr_t glowManager;
	int playerCount;

	struct LocalPlayer{
		uintptr_t lPlayer;
		int32_t lPlayerId;
		int32_t lTeam;
		int32_t lCrossID;

		struct Postitions {
			Vector3 lPostition;
			Vector3 lEyePosition;
			Vector3 lViewAngles;
			Vector3 lAimPunchAng;

		}pos;
		
		struct ForceInput {

			uint8_t iJumpR;
			uint8_t iJumpP;
			void forceJump(bool jump) { cs.memory.Write<uint8_t>(cs.client + dwForceJump, (jump) ? iJumpP : iJumpR); };

			uint8_t iAttR;
			uint8_t iAttP;
			void forceAtt(bool att) { cs.memory.Write<uint8_t>(cs.client + dwForceAttack, (att) ? iAttP : iAttR); };

		}input;

		struct Flags {
			uint8_t raw;

			bool onGround() { return (raw & 1 << 0); };
			bool isCrouched() { return (raw & 1 << 1); };
			bool isCrouchInProg() { return (raw & 1 << 2); };
		}flags;

	}loc;
	
	//init func called for setup
	bool init() {
		if (memory.isNull()) {
			memory = Memory{ "csgo.exe" };
			if (memory.isNull()) return false;
		}
		client = memory.GetModuleAddress("client.dll");
		engine = memory.GetModuleAddress("engine.dll");

		if (client == NULL || engine == NULL) return false;

		clientState = memory.Read<uintptr_t>(engine + dwClientState);

		if (!isMatchInProgress()) return false;

		loc.input.iJumpR = memory.Read<uint8_t>(cs.client + dwForceJump);
		loc.input.iJumpP = loc.input.iJumpR + 1;

		loc.input.iAttR = memory.Read<uint8_t>(cs.client + dwForceAttack);
		loc.input.iAttP = loc.input.iAttR + 1;


		return true;
	}

	//update func called every loop cycle
	void update() {
		//global
		clientState = memory.Read<uintptr_t>(engine + dwClientState);
		glowManager = memory.Read<uintptr_t>(client + dwGlowObjectManager);

		playerCount = 0;
		for (int p = 0; p < 32; p++)
			if (memory.Read<uintptr_t>(client + dwEntityList + p * 0x10) != NULL) playerCount++;

		//LocalPlayer
		{

			loc.lPlayer = memory.Read<uintptr_t>(client + dwLocalPlayer);
			loc.lPlayerId = memory.Read<int32_t>(clientState + dwClientState_GetLocalPlayer);
			loc.lTeam = memory.Read<int32_t>(loc.lPlayer + m_iTeamNum);
			loc.lCrossID = memory.Read<int32_t>(loc.lPlayer + m_iCrosshairId);

			//Positions
			{
				loc.pos.lPostition = memory.Read<Vector3>(loc.lPlayer + m_vecOrigin);
				loc.pos.lEyePosition = loc.pos.lPostition + memory.Read<Vector3>(loc.lPlayer + m_vecViewOffset);
				loc.pos.lViewAngles = memory.Read<Vector3>(clientState + dwClientState_ViewAngles);
				loc.pos.lAimPunchAng = memory.Read<Vector3>(loc.lPlayer + m_aimPunchAngle) * 2;
			}
			//Flags
			{
				loc.flags.raw = cs.memory.Read<uint8_t>(cs.loc.lPlayer + m_fFlags);
			}
		}		
	}

	bool isMatchInProgress() { return (memory.Read<uint32_t>(clientState + dwClientState_State) == 6); }

}cs;