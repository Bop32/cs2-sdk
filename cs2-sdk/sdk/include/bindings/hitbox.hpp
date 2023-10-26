#pragma once

#include <types/utlvector.hpp>

// also available at schema table of "animationsystym.dll"
class CHitBox
{
public:
	const char* szName; // 0x0
	const char* szSurfaceProperty; // 0x8
	const char* szBoneName; // 0x10
	Vector m_vMinBounds; // 0x18
	Vector m_vMaxBounds; // 0x24
	float flShapeRadius; // 0x30
	uint32_t uBoneNameHash; // 0x34
	int nGroupId; // 0x38
	std::uint8_t nShapeType; // 0x3C
	bool bTranslationOnly; // 0x3D	
private:
	std::byte pad0[0x2]; // 0x3E
public:
	uint32_t uCRC; // 0x40	
	uint32_t colRender; // 0x44	
	std::uint16_t nHitBoxIndex; // 0x48	
};
//static_assert(sizeof(CHitBox) == 0x70); // size verify:   @ida: client.dll -> [ABS["E8 ? ? ? ? 0F B6 45 77" + 0x1] + 0x1B]		   //Size: 0x0070

class CHitBoxSet {
   public:
    SCHEMA_EXTENDED(CUtlVector<CHitBox>, m_HitBoxes, CConstants::ANIMATIONSYSTEM_LIB, "CHitBoxSet", "m_HitBoxes", 0);
};
