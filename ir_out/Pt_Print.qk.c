#include "Builtins.h"

#include <string.h>

#include <stdlib.h>

//FORWARD DECLARATION
//forward declaration for class Pt
struct class_Pt_struct;
typedef struct class_Pt_struct* class_Pt;
struct class_Pt_struct the_class_Pt_struct;


//CLASS DEFINITIONS
//ivar table for class Pt
typedef struct obj_Pt_struct {
    class_Pt clazz;
    //instance vars inherited from class Pt
    obj_Int y;
    obj_Int x;
} * obj_Pt;

//method table for class Pt
struct class_Pt_struct {
    obj_Pt (*constructor)(obj_Int, obj_Int);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Pt);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_Int (*_get_x)(obj_Pt);
    obj_Int (*_get_y)(obj_Pt);
    obj_Nothing (*translate)(obj_Pt, obj_Int, obj_Int);
    obj_Pt (*PLUS)(obj_Pt, obj_Pt);
};

//constructor for Pt
obj_Pt new_Pt(obj_Int x, obj_Int y){
    obj_Pt this = (obj_Pt)malloc(sizeof(struct obj_Pt_struct));
    this->clazz = &the_class_Pt_struct;
    //local variables for this constructor
    //end local vars


    this->x = x;


    this->y = y;

    return this;
}

//METHOD SET FOR Pt
//method _get_x for class Pt
obj_Int Pt_method__get_x(obj_Pt this){
    //local variables for this method
    //end local vars


    return (obj_Int) this->x;

}

//method _get_y for class Pt
obj_Int Pt_method__get_y(obj_Pt this){
    //local variables for this method
    //end local vars


    return (obj_Int) this->y;

}

//method translate for class Pt
obj_Nothing Pt_method_translate(obj_Pt this, obj_Int dx, obj_Int dy){
    //local variables for this method
    //end local vars

    obj_Int r0 = this->x;

    obj_Int r1 = dx;

    this->x = r0->clazz->PLUS((obj_Int) r0, r1);

    obj_Int r2 = this->y;

    obj_Int r3 = dy;

    this->y = r2->clazz->PLUS((obj_Int) r2, r3);

}

//method PLUS for class Pt
obj_Pt Pt_method_PLUS(obj_Pt this, obj_Pt other){
    //local variables for this method
    //end local vars

    obj_Int r2 = this->x;

    obj_Int r3 = other->x;
    obj_Int r5 = this->y;

    obj_Int r6 = other->y;
    obj_Int r1 = r2->clazz->PLUS((obj_Int) r2, r3);
    obj_Int r4 = r5->clazz->PLUS((obj_Int) r5, r6);

    obj_Pt r0 = the_class_Pt_struct.constructor(r1, r4);

    return (obj_Pt) r0;

}

//method PRINT for class Pt
obj_Obj Pt_method_PRINT(obj_Pt this){
    //local variables for this method
    //end local vars

    obj_String r0 = str_literal("Pt(");
    obj_String r1 = r0;


    r1->clazz->PRINT((obj_String) r1);

    obj_Int r2 = this->x;


    r2->clazz->PRINT((obj_Obj) r2);

    obj_String r3 = str_literal(",");
    obj_String r4 = r3;


    r4->clazz->PRINT((obj_String) r4);

    obj_Int r5 = this->y;


    r5->clazz->PRINT((obj_Obj) r5);

    obj_String r6 = str_literal(")");
    obj_String r7 = r6;


    r7->clazz->PRINT((obj_String) r7);


    return (obj_Obj) this;

}

struct class_Pt_struct the_class_Pt_struct =
{
    new_Pt, /* constructor */
    Obj_method_STRING,
    Pt_method_PRINT,
    Obj_method_EQUALS,
    Pt_method__get_x,
    Pt_method__get_y,
    Pt_method_translate,
    Pt_method_PLUS,
};

//MAIN BODY
int main(){
    //local variables for main
    //end local vars

    obj_Int r2 = int_literal(13);
    obj_Int r4 = int_literal(42);
    obj_Int r1 = r2;
    obj_Int r3 = r4;

    obj_Pt r0 = the_class_Pt_struct.constructor(r1, r3);
    obj_Pt r5 = r0;


    r5->clazz->PRINT((obj_Pt) r5);

    return 0;
}
