#include "doom_nukem.h"

void 	animation_play(t_animation *anim)
{
	anim->play = 1;
	anim->counter = 0.0;
	anim->frame_cur = 0;
}

void	animation_next_frame(t_animation *anim, double dt)
{
	if (!anim->play)
		return ;
	anim->counter += dt;
	while (anim->counter >= anim->speed)
	{
		anim->counter = 0;
		anim->frame_cur++;
		if (anim->loop)
			anim->frame_cur %= anim->frame_nbr;
	}
	if (!anim->loop && anim->frame_cur >= anim->frame_nbr)
	{
		anim->play = 0;
		anim->frame_cur = 0;
	}
}
