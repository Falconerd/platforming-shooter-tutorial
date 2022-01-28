#ifndef TIME_H
#define TIME_H

typedef struct time_state {
    float now;
    float previous;
    float delta;
    float last_frame;
    float frame_time;
    float frame_delay;
    uint16_t frame_count;
    uint16_t frame_rate;
} Time_State;

Time_State *time_init(float framerate);
void time_update(void);
void time_late_update(void);

#endif
