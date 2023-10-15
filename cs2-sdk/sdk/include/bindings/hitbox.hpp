#pragma once

#include <types/utlvector.hpp>

class CHitBox {
   public:
    //SCHEMA_EXTENDED(Vector, m_vMinBounds, CConstants::ANIMATIONSYSTEM_LIB, "CHitBox", "m_vMinBounds", 0);
    //SCHEMA_EXTENDED(Vector, m_vMaxBounds, CConstants::ANIMATIONSYSTEM_LIB, "CHitBox", "m_vMaxBounds", 0);
	char* m_name; //0x0000
	char* m_sSurfaceProperty; //0x0008
	char* m_sBoneName; //0x0010
	Vector m_vMinBounds; //0x0018
	Vector m_vMaxBounds; //0x0024
	float m_flShapeRadius; //0x0030
	uint32_t m_nBoneNameHash; //0x0034
	int32_t m_nGroupId; //0x0038
	int8_t m_nShapeType; //0x003C
	bool m_bTranslationOnly; //0x003D
	char pad_003E[2]; //0x003E
	uint32_t m_CRC; //0x0040
	uint32_t m_cRenderColor; //0x0044
	int16_t m_nHitBoxIndex; //0x0048
	char pad_004A[38]; //0x004A
};			   //Size: 0x0070

class CHitBoxSet {
   public:
    SCHEMA_EXTENDED(CUtlVector<CHitBox>, m_HitBoxes, CConstants::ANIMATIONSYSTEM_LIB, "CHitBoxSet", "m_HitBoxes", 0);
};
