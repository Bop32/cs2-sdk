#pragma once

#include <bindings/baseentity.hpp>
#include <weapon/c_attributecontainer.hpp>
class C_BasePlayerWeapon : public C_BaseEntity
{
public:
    SCHEMA(int, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick")
    PSCHEMA(C_AttributeContainer, m_AttributeManager, "C_EconEntity", "m_AttributeManager")
    SCHEMA(int32_t, m_nFallbackPaintKit, "C_EconEntity", "m_nFallbackPaintKit")
    SCHEMA(int32_t, m_nFallbackStatTrak, "C_EconEntity", "m_nFallbackStatTrak")
    SCHEMA(int32_t, m_nFallbackSeed, "C_EconEntity", "m_nFallbackSeed")

    float GetInaccuracy()
    {
        using function_t = float(__fastcall*)(void*);
        static function_t fn = reinterpret_cast< function_t >(signatures::GetInaccuracy.GetPtrAs<void*>());

        return fn(this);
    }

    float GetSpread()
    {
        using function_t = float(__fastcall*)(void*);
        static function_t fn = reinterpret_cast< function_t >(signatures::GetSpread.GetPtrAs<void*>());

        return fn(this);
    }
	/*
	Vector CalculateSpread(CCSWeaponBaseVData* weaponInfo,int seed, float inaccuracy, float spread, bool revolver2 = false)
	{
		float      recoil_index, r1, r2, r3, r4, s1, c1, s2, c2;

		// if we have no bullets, we have no spread.
		if (weaponInfo->m_nNumBullets() <= 0)
			return {};

		// seed randomseed.
		g_csgo.RandomSeed((seed & 0xff) + 1);

		// generate needed floats.
		r1 = g_csgo.RandomFloat(0.f, 1.f);
		r2 = g_csgo.RandomFloat(0.f, math::pi_2);

		else
		{
			r3 = g_csgo.RandomFloat(0.f, 1.f);
			r4 = g_csgo.RandomFloat(0.f, math::pi_2);
		}

		// revolver secondary spread.
		if (item_def_index == REVOLVER && revolver2)
		{
			r1 = 1.f - (r1 * r1);
			r3 = 1.f - (r3 * r3);
		}

		// negev spread.
		else if (item_def_index == NEGEV && recoil_index < 3.f)
		{
			for (int i { 3 }; i > recoil_index; --i)
			{
				r1 *= r1;
				r3 *= r3;
			}

			r1 = 1.f - r1;
			r3 = 1.f - r3;
		}

		// get needed sine / cosine values.
		c1 = std::cos(r2);
		c2 = std::cos(r4);
		s1 = std::sin(r2);
		s2 = std::sin(r4);

		// calculate spread vector.
		return {
			(c1 * (r1 * inaccuracy)) + (c2 * (r3 * spread)),
			(s1 * (r1 * inaccuracy)) + (s2 * (r3 * spread)),
			0.f
		};
	}
	*/

};
