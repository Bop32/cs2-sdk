#pragma once
#include <bindings/playercontroller.hpp>

class C_BaseEntity;

class CGameEntitySystem {
   public:
    static CGameEntitySystem* Get();

    template <typename T = C_BaseEntity>
    static auto GetBaseEntity(int index) {
        return static_cast<T*>(Get()->GetBaseEntityInternal(index));
    }

    static auto GetHighestEntityIndex() { return Get()->GetHighestEntityIndexInternal(); }
    static CCSPlayerController* GetLocalPlayerController();

   private:
    C_BaseEntity* GetBaseEntityInternal(int index);
    int GetHighestEntityIndexInternal();
};
