#include <signatures/signatures.hpp>
#include <cstdint>
#include <module/module.hpp>

namespace offsets
{
    //CModule client(CConstants::CLIENT_LIB);

    /*
    void SetForceJump(uint32_t value)
    {
        auto dwForceJump = signatures::GetForceJump.GetPtrAs<uint32_t*>();
        *dwForceJump = value;
    }
    */

    /*
    void SetForceJump(uint32_t value)
    {
        uint32_t* dwForceJump = ( uint32_t* )(client.m_Begin + 0x1693100);

        //auto dwForceJump = signatures::GetForceJump.GetPtrAs<uint32_t*>();
        *dwForceJump = value;
    }
    */
}
