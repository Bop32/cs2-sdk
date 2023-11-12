#pragma once
#include <fnv/FNV1A.hpp>
#include <cstdint>

#define STRINGTOKEN_MURMURHASH_SEED 0x31415926

#pragma pack(push, 8)
class CUtlStringToken
{
public:
	explicit CUtlStringToken(const char* szKeyName)
	{
		uHashCode = FNV1A::Hash(szKeyName);
		szDebugName = szKeyName;
	}

	constexpr CUtlStringToken(const uint32_t uHashCode, const char* szKeyName) :
		uHashCode(uHashCode), szDebugName(szKeyName)
	{
	}

	bool operator==(const CUtlStringToken& other) const
	{
		return (other.uHashCode == uHashCode);
	}

	bool operator!=(const CUtlStringToken& other) const
	{
		return (other.uHashCode != uHashCode);
	}

	bool operator<(const CUtlStringToken& other) const
	{
		return (uHashCode < other.uHashCode);
	}

public:
	std::uint32_t uHashCode = 0U; // 0x00
	const char* szDebugName = nullptr; // 0x08 //   @Todo: for some reason retards keep this even for non-debug builds, it can be changed later
};
#pragma pack(pop)

// helper to create a string token at compile-time
CUtlStringToken MakeStringToken(const char* szKeyName)
{
	return { FNV1A::HashConst(szKeyName, STRINGTOKEN_MURMURHASH_SEED), szKeyName };
}
