#pragma once
#include <cstdint>

class Entity {
public:

	uint32_t id;
	uintptr_t base;
	int classID = 0;
	
	Entity() : id{ 0 }, base{ 0 }, classID{ -1 } {};
	Entity(uint32_t id) : id{ id }, base{ cs.memory.Read<uintptr_t>(cs.client + dwEntityList + id * 0x10) }, classID{ getClassIDfromBase(base) } {};

	bool exits() { return base != NULL; }


	int getClassID() { return classID; }
	bool isPlayer() { return (classID == 40); }
	bool isC4() { return (classID == 23 || classID == 9999); }



	int getGlowIndex() { return cs.memory.Read<int>(base + m_iGlowIndex); }
	uintptr_t getGlowObj() { return cs.glowManager + (getGlowIndex() * 0x38) + 0x8; }

	static int getClassIDfromBase(uintptr_t entity)
	{
		uintptr_t vtable = cs.memory.Read<uintptr_t>(entity + 0x8); // IClientNetworkable
		uintptr_t fn = cs.memory.Read<uintptr_t>(vtable + 0x8); // 4 bytes per fn, 2nd index
		uintptr_t ptr = cs.memory.Read<uintptr_t>(fn + 0x1); // mov eax, ptr_to_client_class
		return cs.memory.Read<int>(ptr + 0x14);
	}
private:
	uintptr_t glowIndexOff = 0;
};

class Player :public Entity {

public:
	Player(uint32_t id) : Entity(id) { }
	Player(Entity entity) : Entity(entity) { }

	int getEntity() { return base; }
	int getHealth() { return cs.memory.Read<int>(base + m_iHealth); }
	int getTeam() { return cs.memory.Read<int>(base + m_iTeamNum); }

	int isDormant() { return cs.memory.Read<int>(base + m_bDormant); }
	int isDefusing() { return cs.memory.Read<int>(base + m_bIsDefusing); }
	//int hasDefuser() { return cs.memory.Read<int>(base + m_bHasDefuser); }
	//int isBombCarrier() { return cs.memory.Read<int>(base + is_c4_owner); }

	Vector3 getBasePos() { return cs.memory.Read<Vector3>(base + m_vecOrigin); }
	Vector3 getHeadPos() { return getBonePos(8); }
	Vector3 getBonePos(uint32_t b_id) {
		const auto boneMatrix = cs.memory.Read<std::uintptr_t>(base + m_dwBoneMatrix);
		return Vector3{
			cs.memory.Read<float>(boneMatrix + 0x30 * b_id + 0x0C),
			cs.memory.Read<float>(boneMatrix + 0x30 * b_id + 0x1C),
			cs.memory.Read<float>(boneMatrix + 0x30 * b_id + 0x2C)
		}; 
	}

	bool isAlive() { return (getHealth() > 0 && getHealth() <= 100); }
	bool isEnemy() { return (getTeam() != cs.loc.lTeam); }

private:
	uintptr_t glowIndexOff = 0;


};



class C4 : public Entity {

public:


	C4(uintptr_t base) : Entity(base) { }
	C4(Entity entity) : Entity{ entity } { }


private:
	uintptr_t glowIndexOff = 0;
};
