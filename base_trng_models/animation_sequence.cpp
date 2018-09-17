#include "animation_sequence.h"

#include <iostream>
#include <string>
#include <iomanip>


std::istream& operator>> ( std::istream& is, AnimationSequence & value)
{
	is>>value.start_frame>>value.end_frame>>value.m_loop>>value.m_jump;
    if(value.m_jump)
        is>>value.m_target_sequence;
    is >>std::quoted(value.m_start_message)>>std::quoted(value.m_end_message);
    std::cout<<"animation "<<value;
}

std::ostream& operator << ( std::ostream& os, const AnimationSequence & value)
{
    os<<value.start_frame<<" "<<value.end_frame<<" "<<value.m_loop<<" "<<value.m_jump;
    if(value.m_jump) 
        os<<" "<<value.m_target_sequence;
        os<<" "<<std::quoted(value.m_start_message)<<" "<<std::quoted(value.m_end_message)<<"\n";
	return os;
}