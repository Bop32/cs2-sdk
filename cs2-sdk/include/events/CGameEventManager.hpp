class CGameEventManager
{
public:
    static void Set(void* rcx);
    static CGameEventManager* Get();

    //void* AddListener(void* unk, const char* eventName, unsigned __int8 unk1);

private:
    static void* gameEventManagerClass;
};
