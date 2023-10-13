#pragma once
#include <math/types/vector.hpp>
#include <input/cusercmd.hpp>

class CViewSetup
{
public:
	char pad01[0x490];
	float flOrthoLeft; // 0x0494
	float flOrthoTop; // 0x0498
	float flOrthoRight; // 0x049C
	float flOrthoBottom; // 0x04A0
	char pad02[0x38];
	float flFov; // 0x04D8
	float flFovViewmodel; // 0x04DC
	Vector vecOrigin; // 0x04E0
	char pad03[0xC]; // 0x04EC
	CmdQAngle angView; // 0x04F8
	char pad04[0x14]; // 0x0504
	float flAspectRatio; // 0x0518
};
