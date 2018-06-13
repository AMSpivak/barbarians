#ifndef ENGINE_SETTINGS
#define ENGINE_SETTINGS

namespace EngineSettings
{

class Settings
{
    private:
    bool pbr_light = false;
    public:
    bool IsPbrON();
    void SetPbr(bool value);
};

Settings * GetEngineSettings();
void SetEngineSettings(Settings * settings);

}

#endif
