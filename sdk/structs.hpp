#pragma once
#include <map>
#include <set>
#include <iostream>
#include <corecrt_math_defines.h>
#include "../Spoofer/Spoofer.hpp"

// defines
class IWEngine;

// classes
class vector3_t
{
public:
	float x;
	float y;
	float z;
};

union vec3_t
{
	float v[3];
	vector3_t s1;
};

class vector4_t
{
public:
	float x;
	float y;
	float z;
	float w;
};

class color_t
{
public:
	float r;
	float g;
	float b;
	float a;

	uint32_t Pack() const { return (static_cast<uint8_t>(a * 255.f) << 24) | (static_cast<uint8_t>(b * 255.f) << 16) | (static_cast<uint8_t>(g * 255.f) << 8) | static_cast<uint8_t>(r * 255.f); }
};

union vec4_t
{
	float v[4];
	vector4_t s1;
	color_t s2;
	vec3_t xyz;
};

class GfxSceneHudOutlineInfo
{
public:
	char pad_0001[0x8];
	unsigned int color;
	char pad_0002[0x8];
	bool drawOccludedPixels;
	bool drawNonOccludedPixels;
	char pad_0003[0x1];
	bool fill;
	char pad_0004[0x7];
	unsigned __int8 renderMode;
	unsigned __int8 lineWidth;
};

class GfxSceneEntityMutableShaderData
{
public:
	char pad_0001[0x48];
	GfxSceneHudOutlineInfo hudOutlineInfo;
};

// enums
enum EntityType_s : __int16 {
	ET_FIRST = 0x0,
	ET_GENERAL = 0x0,
	ET_PLAYER = 0x1,
	ET_PLAYER_CORPSE = 0x2,
	ET_ITEM = 0x3,
	ET_MISSILE = 0x4,
	ET_INVISIBLE = 0x5,
	ET_SCRIPTMOVER = 0x6,
	ET_SOUND = 0x7,
	ET_FX = 0x8,
	ET_LOOP_FX = 0x9,
	ET_PRIMARY_LIGHT = 0xA,
	ET_TURRET = 0xB,
	ET_HELICOPTER = 0xC,
	ET_PLANE = 0xD,
	ET_VEHICLE = 0xE,
	ET_VEHICLE_CORPSE = 0xF,
	ET_VEHICLE_SPAWNER = 0x10,
	ET_AGENT = 0x11,
	ET_AGENT_CORPSE = 0x12,
	ET_ACTOR = 0x13,
	ET_ACTOR_SPAWNER = 0x14,
	ET_ACTOR_CORPSE = 0x15,
	ET_PHYSICS_CHILD = 0x16,
	ET_BEAM = 0x17,
	ET_CLIENT_CHARACTER = 0x18,
	ET_RAGDOLL_CONSTRAINT = 0x19,
	ET_PHYSICS_VOLUME = 0x1A,
	ET_COVERWALL = 0x1B,
	ET_INFO_VOLUME_GRAPPLE = 0x1C,
	ET_EVENTS = 0x1D,
	ET_COUNT = 0x1E,
	ET_MAX = 0x100
};