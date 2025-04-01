#include "obj.h"

Obj get_object(uint16_t loc){
    Obj obj = {.range = OBJ_RANGE_NONE};
    Obj_Range obj_range = get_object_range(loc);
    obj.range = obj_range;
    return obj; 
}

Obj_Range get_object_range(uint16_t range){
    if(range < 70) return OBJ_RANGE_CLOSE;
    else if (range < 100) return OBJ_RANGE_MID;
    else if (range < 150) return OBJ_RANGE_FAR;
    return OBJ_RANGE_NONE;
}

bool obj_detected(Obj_Range obj_range){ return obj_range != OBJ_RANGE_NONE; }
bool obj_close(Obj_Range obj_range) { return obj_range == OBJ_RANGE_CLOSE; }
bool obj_mid(Obj_Range obj_range) { return obj_range == OBJ_RANGE_MID; }
bool obj_far(Obj_Range obj_range) { return obj_range == OBJ_RANGE_FAR; }