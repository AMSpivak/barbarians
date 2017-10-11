#ifndef ANIMATION_SEQUENCE
#define ANIMATION_SEQUENCE

struct AnimationSequence
{
    AnimationSequence(size_t start = 0,size_t stop = 0):
                                start_frame(start)
                                ,end_frame(stop)
    {}

    size_t start_frame;
    size_t end_frame;
};


#endif
