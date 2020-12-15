
#include "de_platform_interface.h"

internal os_event_list
GenerateEventList(memory_arena& arena, u32 size)
{
    os_event_list result;
    
    result.events = 
        (os_event*)arena.ArenaAlloc(size * sizeof(os_event));
    result.writeOffset = 0;
    result.readOffset = 0;
    result.size = size;
    
    return result;
}

internal void
PushOSEvent(os_event_list* list,
            os_event event)
{
    list->events[list->writeOffset] = event;
    
    ++list->writeOffset;
}

inline os_event
OSEvent(os_event_type type)
{
    os_event event = {};
    event.eventType = type;
    return event;
}

internal os_event
KeyEvent(key_code keyCode,
         key_state keyState)
{
    os_event event = {};
    
    event.eventType = EVENT_KEY;
    event.keyCode = keyCode;
    event.keyState = keyState; 
    
    return event;
}

internal os_event
GetNextOSEvent(os_event_list* list)
{
    ++list->readOffset;
    return list->events[list->readOffset - 1];
}

internal void
ClearEventList(os_event_list* list)
{
    list->writeOffset = 0;
    list->readOffset = 0;
    
    MemorySet(list->events, 0, list->size * sizeof(os_event));
}

