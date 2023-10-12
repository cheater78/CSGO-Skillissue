#pragma once

#include "../utils/memory.h"
#include "../utils/vector.h"
#include "../../lib/hazedumper-v2.4.1/csgo.hpp"
#include "../core/ClientState.h"
#include "../core/Entities.h"


#include <vector>


class GlowWall
{
public:
	GlowWall() { }
	
	void run(vector<Entity> entities) {
		if(enabled)
			for (auto entity : entities) {
				if (!entity.exits()) continue;
				cs.memory.Write<GlowStruct>(entity.getGlowObj(), GlowStruct::byEntity(entity));
			}
	}

	void enable() { enabled = true; }
	void disable() { enabled = false; }
	bool isEnabled() { return enabled; }

private:
	bool enabled = false;

public:
	struct GlowStruct {
		float red = 0.f;
		float green = 0.f;
		float blue = 0.f;
		float alpha = 1.f;
		uint8_t pad1[8];
		float unknown = 1.f;
		uint8_t pad2[4];
		BYTE renderOccluded = true;
		BYTE renderUnoccluded = false;
		BYTE fullBloom = false;

		void debug() {
			std::cout << red << " " << green << " " << blue << " " << alpha << " " << (uint32_t)renderOccluded << " " << (uint32_t)renderUnoccluded << " " << (uint32_t)fullBloom << std::endl;
		}

		static GlowStruct byEntity(Entity entity) {
			GlowStruct glow{};
			if (entity.isPlayer()) {
				Player player = entity;
				if (player.isEnemy()) {
					glow.red = 1.f;
					glow.green = 1.f * player.getHealth() / 200;

					if (player.isDefusing()) {
						glow.red = 1.f;
						glow.green = 1.f;
						glow.blue = 1.f;
					}

					glow.alpha = 1.0f;
				}
				else {
					glow.green = 1.f;
					glow.red = 1.f * player.getHealth() / 200;


					if (player.isDefusing()) {
						glow.red = 1.f;
						glow.green = 1.f;
						glow.blue = 1.f;
					}

					glow.alpha = 0.7f;
				}
			}
			else if (entity.isC4()) {
				C4 c4 = entity;

				glow.red = 1.f;
				glow.green = 1.f;
				glow.blue = 1.f;

				glow.alpha = 1.0f;
			}
			

			
			glow.renderOccluded = true;
			glow.fullBloom = false; //ÓPT
			return glow;
		}

	};

	static uintptr_t offset(uintptr_t glowManager, uintptr_t glowIndex) {
		return glowManager + (glowIndex * 0x38) + 0x8;
	}

};