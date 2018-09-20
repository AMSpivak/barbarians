#ifndef ANIMATION_SEQUENCE
#define ANIMATION_SEQUENCE
#include <iostream>
#include <cstdio>

enum class AnimationCommand {kNone,kMessage,kStrike};
struct AnimationSequence
{
    AnimationSequence(size_t start = 0,size_t stop = 0):
                                start_frame(start)
                                ,end_frame(stop)
                                ,m_loop(true)
                                ,m_start_message(AnimationCommand::kNone,"")
                                ,m_end_message(AnimationCommand::kNone,"")
    {}

    size_t start_frame;
    size_t end_frame;
    bool m_loop;
    bool m_jump;
    std::string m_target_sequence;
    std::pair<AnimationCommand,std::string> m_start_message;
    std::pair<AnimationCommand,std::string> m_end_message;
};

std::istream& operator>> ( std::istream& is, AnimationSequence & value);

std::ostream& operator << ( std::ostream& os, const AnimationSequence & value);
#endif
