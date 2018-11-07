#include "game_status.h"
namespace GameSettings
{
    const float HeroStatus::GetLife() const
    {
        return m_life;
    }

    void HeroStatus::SetLife(float value)
    {
        m_life = value;
    }

    HeroStatus * pStatus = nullptr;

    HeroStatus * GetHeroStatus()
    {
        return pStatus;
    }

    void SetHeroStatus(HeroStatus * value)
    {
        pStatus = value;
    }       
}