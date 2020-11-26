#ifndef PLAY_TIME_H
#define PLAY_TIME_H
namespace Dwarf
{
	struct PlayTime
	{
		PlayTime(unsigned int hour,
				 unsigned int minutes,
				 float secs)
		{
			this->hour    = hour;
			this->minutes = minutes;
			this->secs    = secs;
		}
		~PlayTime(){}
		unsigned int hour,minutes;
		float secs;
	};

};
#endif //PLAY_TIME_H