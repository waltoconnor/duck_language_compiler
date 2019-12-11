#include "Builtins.h"

#include <string.h>

#include <stdlib.h>

//FORWARD DECLARATION
//forward declaration for class Schroedinger
struct class_Schroedinger_struct;
typedef struct class_Schroedinger_struct* class_Schroedinger;
struct class_Schroedinger_struct the_class_Schroedinger_struct;


//CLASS DEFINITIONS
//ivar table for class Schroedinger
typedef struct obj_Schroedinger_struct {
    class_Schroedinger clazz;
    //instance vars inherited from class Schroedinger
    obj_Boolean living;
} * obj_Schroedinger;

//method table for class Schroedinger
struct class_Schroedinger_struct {
    obj_Schroedinger (*constructor)(obj_Int);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
};

//constructor for Schroedinger
obj_Schroedinger new_Schroedinger(obj_Int box){
    obj_Schroedinger this = (obj_Schroedinger)malloc(sizeof(struct obj_Schroedinger_struct));
    this->clazz = &the_class_Schroedinger_struct;
    //local variables for this constructor
    //end local vars


    obj_Int r3 = int_literal(0);
    obj_Int r1 = box;

    obj_Int r2 = r3;
    obj_Boolean r0 = r1->clazz->LESS((obj_Int) r1, r2);
    if(r0->value == 1){ goto r0trueblock; }else{ goto r0falseblock; }

    this->living = lit_true;
r0trueblock: ; //need semicolon because declarations may not follow labels, so we insert an emtpy statement

    goto r0end;
r0falseblock: ; //need semicolon because declarations may not follow labels, so we insert an emtpy statement

    this->living = lit_false;

r0end: ;

    return this;
}

//METHOD SET FOR Schroedinger
struct class_Schroedinger_struct the_class_Schroedinger_struct =
{
    new_Schroedinger, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
};

//MAIN BODY
int main(){
    //local variables for main
    obj_Schroedinger hi;
    //end local vars

    obj_Int r2 = int_literal(5);
    obj_Int r1 = r2;

    obj_Schroedinger r0 = the_class_Schroedinger_struct.constructor(r1);
    hi = (obj_Schroedinger) r0;


    obj_Boolean r3 = hi->living;


    r3->clazz->PRINT((obj_Obj) r3);

    return 0;
}
