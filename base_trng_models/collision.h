#ifndef COLLISION
#define COLLISION
#include <utility>

inline float CollisionOnAxe(float start1,float end1,float start2,float end2)
{
    if(start1 >end1)
        std::swap(start1, end1);
    if(start2 >end2)
        std::swap(start2, end2);

    float start = start1 > start2 ? start1 : start2;
    float end = end1 < end2 ? end1 : end2;

    return start < end ? end - start : 0;   
}

inline float CollisionOnAxe(std::pair<float,float> projection1,std::pair<float,float> projection2)
{
    if(projection1.first >projection1.second)
        std::swap(projection1.first,projection1.second);
    if(projection2.first > projection2.second)
        std::swap(projection2.first,projection2.second);

    float start = projection1.first > projection2.first ? projection1.first : projection2.first;
    float end = projection1.second < projection2.second ? projection1.second : projection2.second;

    return start < end ? end - start : 0;   
}



#endif