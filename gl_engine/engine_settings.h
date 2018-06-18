#ifndef ENGINE_SETTINGS
#define ENGINE_SETTINGS

namespace EngineSettings
{

class Settings
{
    private:
    bool pbr_light = false;
    unsigned short frame_number = 0;
    public:
    bool IsPbrON();
    void SetPbr(bool value);
    void BeginNewFrame();
    unsigned short GetFrame();
};



Settings * GetEngineSettings();
void SetEngineSettings(Settings * settings);



}

#endif
