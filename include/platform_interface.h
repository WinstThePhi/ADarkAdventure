#ifndef PLATFORM_INTERFACE_H
#define PLATFORM_INTERFACE_H

#define NUM_OF_OS_EVENT   2

typedef enum key_code
{
#define Key(name) KEY_##name,
    KEY_NULL,
#include "key_list.inc"
#undef Key
    KEY_MAX
} key_code;

typedef enum OS_event_type
{
    EVENT_NULL,
    WINDOW_CLOSE,
    KEY_PRESS,
    KEY_RELEASE
} OS_event_type;

typedef struct OS_event OS_event;

struct OS_event
{
    OS_event_type eventType;
    
    union
    {
        key_code keyCode;
    };
};

typedef struct OS_event_list
{
    OS_event* events;
    
    u32 size;
    
    u32 writeOffset;
    u32 readOffset;
} OS_event_list;

typedef struct key_data
{
    b32 isDown;
} key_data;

#undef NUM_OF_KEYS
#undef NUM_OF_OS_EVENT

#endif
