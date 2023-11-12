class FNV1A
{
public:
    // Not guaranteed to be hashed at compile-time.
    static constexpr uint32_t Hash(const char* string)
    {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (size_t i = 0; string[i]; ++i)
        {
            hash = FNV_PRIME * (hash ^ static_cast< uint8_t >(string[i]));
        }

        return hash;
    }

    static uint32_t HashConst(const char* string, uint32_t seed)
    {
        const uint32_t m = 0x5bd1e995;
        const int r = 24;

        // Initialize the hash to a 'random' value

        uint32_t len = strlen(string);

        uint32_t h = seed ^ len;

        // Mix 4 bytes at a time into the hash

        const unsigned char* data = ( const unsigned char* )string;

        while (len >= 4)
        {
            uint32_t k = *( uint32_t* )data;

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            len -= 4;
        }

        // Handle the last few bytes of the input array

        switch (len)
        {
            case 3: h ^= data[2] << 16;
            case 2: h ^= data[1] << 8;
            case 1: h ^= data[0];
                h *= m;
        };

        // Do a few final mixes of the hash to ensure the last few
        // bytes are well-incorporated.

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    // Guaranteed to be hashed at compile-time.
    static consteval uint32_t HashConst(const char* string)
    {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (size_t i = 0; string[i]; ++i)
        {
            hash = FNV_PRIME * (hash ^ static_cast< uint8_t >(string[i]));
        }

        return hash;
    }



private:
    static constexpr uint32_t FNV_PRIME = 0x01000193, FNV_OFFSET_BASIS = 0x811c9dc5;
};
