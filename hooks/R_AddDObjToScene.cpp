#include "R_AddDObjToScene.hpp"

void Hooks::R_AddDObjToScene(const __int64 obj, const __int64 pose, unsigned int unk, unsigned int entityNumber, unsigned int renderFlags, GfxSceneEntityMutableShaderData* entityMutableShaderData, const vec3_t* lightingOrigin, int materialTime) {
	auto SetHudOutlineInfo = [](GfxSceneHudOutlineInfo& info, color_t color) {
		info.color = color.Pack();
		info.drawOccludedPixels = true;
		info.drawNonOccludedPixels = true;
		info.fill = true;
		info.renderMode = 0;
		info.lineWidth = 1;
		};

	auto type = *(EntityType_s*)pose;
	switch (type) {
	case ET_PLAYER:
		if (entityNumber < 152 && renderFlags > 513)
			SetHudOutlineInfo(entityMutableShaderData->hudOutlineInfo, color_t(0.f, 1.f, 1.f, 1.f));
		break;

	case ET_AGENT:
		if (renderFlags != 4608 && renderFlags > 513)
			SetHudOutlineInfo(entityMutableShaderData->hudOutlineInfo, color_t(1.f, 0.f, 0.f, 1.f));
		break;

	case ET_VEHICLE:
		if (renderFlags == 4608)
			SetHudOutlineInfo(entityMutableShaderData->hudOutlineInfo, color_t(1.f, 0.f, 0.f, 1.f));
		break;

	default:
		break;
	}

	return SpoofCall(O_R_AddDObjToScene, obj, pose, unk, entityNumber, renderFlags, entityMutableShaderData, lightingOrigin, materialTime);
}