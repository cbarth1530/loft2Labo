#ifndef TIME_ALARM_H
#define TIME_ALARM_H

#define SECS_PER_MINUTE 60
#define SECS_PER_HOUR (SECS_PER_MINUTE * 60)
#define SECS_PER_DAY (SECS_PER_HOUR * 24)
#define SECS_PER_WEEK (SECS_PER_DAY * 7)

#define TIMEZONE_HRS (+1 * SECS_PER_HOUR)
#define TIMEZONE_MIN (+0 * SECS_PER_MINUTE)

#define ALL_DAY -1
#define MONDAY 4
#define TUESDAY 5
#define WEDNESDAY 6
#define THURSDAY 0
#define FRIDAY 1
#define SATURDAY 2
#define SUNDAY 3

#define MAX_PROCESSES 2

typedef void (*AlarmListener)(uint8_t);

struct Process
{
    uint8_t id;
    time_t interval;
    time_t time;
    AlarmListener listener;
};

Process PROCESSES[MAX_PROCESSES];
uint8_t PROCCESSES_COUNT = 0;

class TimeAlarms
{
public:
	static void alarmRepeat(uint8_t id, uint8_t hrs, uint8_t min, uint8_t sec, AlarmListener listener)
    {
        addRepeat(id, SECS_PER_DAY, toSecond(hrs, min, sec), listener);
    }
    
    static void alarmRepeat(uint8_t id, int8_t day, uint8_t hrs, uint8_t min, uint8_t sec, AlarmListener listener)
    {
        addRepeat(id, SECS_PER_WEEK, day * toSecond(hrs, min, sec), listener);
    }
	
	static void timerRepeat(int8_t id, time_t seconds, AlarmListener listener)
    {
		Process proc = { id, seconds, now() + seconds, listener };
		PROCESSES[PROCCESSES_COUNT++] = proc;
    }
	
	static void process()
    {
        time_t tnow = now();
        
		for (uint8_t i = 0; i < PROCCESSES_COUNT; ++i)
		{
			Process *p = &PROCESSES[i];
            
			if (tnow >= p->time)
            {
                p->listener(p->id);
                p->time += p->interval;
            }
		}
    }
	
	/*static String nowFormatedTime()
    {
        long now = System.currentTimeMillis() / 1000 + TIMEZONE_HRS + TIMEZONE_MIN;        
        return (now % SECS_PER_DAY / SECS_PER_HOUR) + ":" + (now % SECS_PER_HOUR / SECS_PER_MINUTE) + ":" + (now % SECS_PER_MINUTE);
    }*/
	
private:
	static long toSecond(uint8_t hrs, uint8_t min, uint8_t sec)
    {
        return hrs * SECS_PER_HOUR + min * SECS_PER_MINUTE + sec;
    }
	
	static void addRepeat(uint8_t id, time_t interval, time_t sec, AlarmListener listener)
    {
        time_t tnow = now();
        time_t time = tnow / interval * interval + sec - TIMEZONE_HRS - TIMEZONE_MIN;
        if (time <= tnow) time += interval;
		
		Process proc = { id, interval, time, listener };
		PROCESSES[PROCCESSES_COUNT++] = proc;
    }
};

#endif
