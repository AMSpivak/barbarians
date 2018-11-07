#ifndef GAME_STATUS
#define GAME_STATUS
#include <string>
namespace GameSettings
{

    class HeroStatus
    {
        private:
        std::string m_save;
        std::string m_level;
        float m_life;
        public:
        HeroStatus():m_life(1.0f){}
        ~HeroStatus(){}
        const float GetLife() const;
        void SetLife(float value);       
    };

    HeroStatus * GetHeroStatus();
    void SetHeroStatus(HeroStatus * value);

}



#endif