#include <stdexcept>
#include "engine_settings.h"

namespace EngineSettings
{
    bool Settings::IsPbrON()
    {
        return pbr_light;
    }
    void Settings::SetPbr(bool value)
    {
        pbr_light = value;
    }

    static Settings * main_settings = nullptr;

    Settings * GetEngineSettings()
    {
        if(main_settings == nullptr)
        {
            throw std::out_of_range("No recource manager defined!");
        }
        else
        return main_settings;
    }

    void SetEngineSettings(Settings * settings)
    {
        main_settings = settings;
    }
}

