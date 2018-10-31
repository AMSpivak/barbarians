#ifndef GL_I_GAME_EVENT_FABRIC
#define GL_I_GAME_EVENT_FABRIC
#include "i_map_event.h"
#include <memory>
namespace GameEvents
{
    enum class EventTypes {HeroStrike,HeroUse};
    std::shared_ptr<IMapEvent> CreateGameEvent(EventTypes event_type, const void * parameters);
}
#endif