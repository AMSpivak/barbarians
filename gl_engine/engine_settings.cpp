#include <stdexcept>
#include "engine_settings.h"
#include <iostream>
#include <fstream>

namespace EngineSettings
{
    Settings::~Settings()
    {
        std::ofstream savefile;
        savefile.open ("engine.cfg");
        SaveSettings(savefile);
        savefile.close();
    }

    bool Settings::IsPbrON()
    {
        return pbr_light;
    }
    void Settings::SetPbr(bool value)
    {
        pbr_light = value;
    }

    void Settings::SetQualityFactor(float value)
    {
        m_quality_factor = value;
    }
    float Settings::GetQualityFactor()
    {
        return m_quality_factor;
    }

    void Settings::BeginNewFrame()
    {
        ++frame_number;
    }

    void Settings::SaveSettings(std::ostream &os)
    {
        os<<"pbr_light "<<pbr_light<<"\n"
        <<"quality_factor "<<m_quality_factor<<"\n";
    }
    void Settings::LoadSettings(std::istream& os)
    {
        
    }


    unsigned short Settings::GetFrame(){return frame_number;}

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

