#ifndef OBJ_H
#define OBJ_H
#include <Arduino.h>

typedef enum {
    OBJ_RANGE_NONE,
    OBJ_RANGE_CLOSE,
    OBJ_RANGE_MID,
    OBJ_RANGE_FAR,
} Obj_Range;

typedef struct {
    Obj_Range range; 
} Obj;

Obj get_object(uint16_t loc);
Obj_Range get_object_range(uint16_t range);
bool obj_detected(Obj_Range obj);
bool obj_close(Obj_Range obj);
bool obj_mid(Obj_Range obj);
bool obj_far(Obj_Range obj);

#endif