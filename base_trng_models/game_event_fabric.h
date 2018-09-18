#ifndef GL_I_GAME_EVENT_FABRIC
#define GL_I_GAME_EVENT_FABRIC
#include "i_map_event.h"

std::shared_ptr<IMapEvent> CreateGameEvent(std::string event_type,);
#endif