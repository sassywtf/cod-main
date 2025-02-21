#pragma once
#include "../SDK/Engine.hpp"

namespace Hooks {
	inline void (*O_R_AddDObjToScene)(const __int64, const __int64, unsigned int, unsigned int, unsigned int, GfxSceneEntityMutableShaderData*, const vec3_t*, int);
	void R_AddDObjToScene(const __int64 obj, const __int64 pose, unsigned int unk, unsigned int entityNumber, unsigned int renderFlags, GfxSceneEntityMutableShaderData* entityMutableShaderData, const vec3_t* lightingOrigin, int materialTime);
}