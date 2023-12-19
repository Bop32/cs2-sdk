#pragma once
#include <imgui/imgui.h>
#include <math/types/vector.hpp>
#include <virtual/virtual.hpp>

template <typename T>
struct RepeatedPtrField_t
{
	struct Rep_t
	{
		int nAllocatedSize;
		T* tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* pArena;
	int nCurrentSize;
	int nTotalSize;
	Rep_t* pRep;
};

class CBasePB
{
	void* pVTable; // 0x0
	std::uint32_t nHasBits; // 0x8
	std::uint64_t nCachedBits; // 0xC
};
static_assert(sizeof(CBasePB) == 0x18);

class CMsgQAngle : public CBasePB
{
public:
	Vector angles; // 0x18
};

class CMsgVector : public CBasePB
{
public:
	Vector vector; // 0x18
};

class CCSGOInterpolationInfo : public CBasePB
{
public:
	float flFraction; // 0x18
	int nSrcTick; // 0x1C
	int nDstTick; // 0x20
};

// credits: @patoke [uc:3872928-post1311]
class CCSGOInputHistoryEntryPB : public CBasePB
{
public:
	CMsgQAngle* pViewCmd; // 0x18
	CMsgVector* pShootOriginCmd; // 0x20
	CMsgVector* pTargetHeadOriginCmd; // 0x28
	CMsgVector* pTargetAbsOriginCmd; // 0x30
	CMsgQAngle* pTargetViewCmd; // 0x38
	CCSGOInterpolationInfo* cl_interp; // 0x40
	CCSGOInterpolationInfo* sv_interp0; // 0x48
	CCSGOInterpolationInfo* sv_interp1; // 0x50	
	CCSGOInterpolationInfo* player_interp; // 0x58
	int nRenderTickCount; // 0x60
	float flRenderTickFraction; // 0x64
	int nPlayerTickCount; // 0x68
	float flPlayerTickFraction; // 0x6C
	int nFrameNumber; // 0x70
	int nTargetEntIndex; // 0x74
};

struct CSubtickMoveStep : CBasePB
{
	uint64_t nButton;
	bool bPressed;
	float flWhen;
};

struct CInButtonStatePB : CBasePB
{
	std::uint64_t State1;
	std::uint64_t State2;
	std::uint64_t State3;
};

class CBaseUserCmdPB : public CBasePB
{
public:
	RepeatedPtrField_t<CSubtickMoveStep> subtickMovesField;
	const char* szMoveCrc;
	CInButtonStatePB* pInButtonState;
	CMsgQAngle* pViewAngles;
	int32_t nCommandNumber;
	int32_t nTickCount;
	float flForwardMove;
	float flSideMove;
	float flUpMove;
	int32_t nImpulse;
	int32_t nWeaponSelect;
	int32_t nRandomSeed;
	int32_t nMousedX;
	int32_t nMousedY;
	//bool bHasBeenPredicted;
	uint32_t nConsumedServerAngleChanges;
	int32_t nCmdFlags;
	uint32_t nPawnEntityHandle;
};

class CCSGOUserCmdPB
{
public:
	std::uint32_t nHasBits;
	std::uint64_t nCachedSize;
	RepeatedPtrField_t<CCSGOInputHistoryEntryPB> inputHistoryField;
	CBaseUserCmdPB* pBase;
	int32_t nWeaponDecision;
	int32_t nWeaponDecisionWeapon;
	int32_t nAttack3StartHhistoryIndex;
	int32_t nAttack1StartHhistoryIndex;
	int32_t nAttack2StartHhistoryIndex;
};

// not protobufs
struct CInButtonState
{
	void* pVTable;
	uint64_t nButtonState1;
	uint64_t nButtonState2;
	uint64_t nButtonState3;
};

class CUserCmd
{
public:
    enum : unsigned __int64
    {
        IN_ATTACK = (1 << 0),
        IN_JUMP = (1 << 1),
        IN_DUCK = (1 << 2),
        IN_FORWARD = (1 << 3),
        IN_BACK = (1 << 4),
        IN_USE = (1 << 5),
        IN_CANCEL = (1 << 6),
        IN_LEFT = (1 << 7),
        IN_RIGHT = (1 << 8),
        IN_MOVELEFT = (1 << 9),
        IN_MOVERIGHT = (1 << 10),
        IN_SECOND_ATTACK = (1 << 11),
        IN_RUN = (1 << 12),
        IN_RELOAD = (1 << 13),
        IN_LEFT_ALT = (1 << 14),
        IN_RIGHT_ALT = (1 << 15),
        IN_SCORE = (1 << 16),
        IN_SPEED = (1 << 17),
        IN_WALK = (1 << 18),
        IN_ZOOM = (1 << 19),
        IN_FIRST_WEAPON = (1 << 20),
        IN_SECOND_WEAPON = (1 << 21),
        IN_BULLRUSH = (1 << 22),
        IN_FIRST_GRENADE = (1 << 23),
        IN_SECOND_GRENADE = (1 << 24),
        IN_MIDDLE_ATTACK = (1 << 25),
        IN_USE_OR_RELOAD = (1 << 26)
    };


    char pad1[0x20];
    CCSGOUserCmdPB csgoUserCmd;
	CBaseUserCmdPB* base;
    int startHistoryIndexAttack1;
    int startHistoryIndexAttack2;
    int startHistoryIndexAttack3;
    char pad2[0x8];
    uint64_t buttons;
    uint64_t buttonsChanged;
    uint64_t buttonsScroll;
    char pad3[0x8];

	/*
    void SetSubTickAngles(CUserCmd* cmd, Vector& angles)
    {
        for (int i = 0; i < this->csgoUserCmd.; i++)
        {
            auto* tick = this->csgoUserCmd.GetInputHistoryEntry(i);

            if (tick && tick->m_angle_message)
            {
                tick->m_angle_message->angles = angles;
            }
        }
    }
	*/
};
