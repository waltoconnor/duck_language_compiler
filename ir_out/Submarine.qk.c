#include "Builtins.h"

#include <string.h>

#include <stdlib.h>

//FORWARD DECLARATION
//forward declaration for class Submarine
struct class_Submarine_struct;
typedef struct class_Submarine_struct* class_Submarine;
struct class_Submarine_struct the_class_Submarine_struct;

//forward declaration for class AtomicSubmarine
struct class_AtomicSubmarine_struct;
typedef struct class_AtomicSubmarine_struct* class_AtomicSubmarine;
struct class_AtomicSubmarine_struct the_class_AtomicSubmarine_struct;

//forward declaration for class YellowSubmarine
struct class_YellowSubmarine_struct;
typedef struct class_YellowSubmarine_struct* class_YellowSubmarine;
struct class_YellowSubmarine_struct the_class_YellowSubmarine_struct;


//CLASS DEFINITIONS
//ivar table for class Submarine
typedef struct obj_Submarine_struct {
    class_Submarine clazz;
} * obj_Submarine;

//method table for class Submarine
struct class_Submarine_struct {
    obj_Submarine (*constructor)(void);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
};

//ivar table for class AtomicSubmarine
typedef struct obj_AtomicSubmarine_struct {
    class_AtomicSubmarine clazz;
} * obj_AtomicSubmarine;

//method table for class AtomicSubmarine
struct class_AtomicSubmarine_struct {
    obj_AtomicSubmarine (*constructor)(void);
    obj_String (*STRING)(obj_AtomicSubmarine);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
};

//ivar table for class YellowSubmarine
typedef struct obj_YellowSubmarine_struct {
    class_YellowSubmarine clazz;
} * obj_YellowSubmarine;

//method table for class YellowSubmarine
struct class_YellowSubmarine_struct {
    obj_YellowSubmarine (*constructor)(void);
    obj_String (*STRING)(obj_YellowSubmarine);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
};

//constructor for Submarine
obj_Submarine new_Submarine(){
    obj_Submarine this = (obj_Submarine)malloc(sizeof(struct obj_Submarine_struct));
    this->clazz = &the_class_Submarine_struct;
    //local variables for this constructor
    //end local vars

    return this;
}

//METHOD SET FOR Submarine
struct class_Submarine_struct the_class_Submarine_struct =
{
    new_Submarine, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
};
//constructor for AtomicSubmarine
obj_AtomicSubmarine new_AtomicSubmarine(){
    obj_AtomicSubmarine this = (obj_AtomicSubmarine)malloc(sizeof(struct obj_AtomicSubmarine_struct));
    this->clazz = &the_class_AtomicSubmarine_struct;
    //local variables for this constructor
    //end local vars

    return this;
}

//METHOD SET FOR AtomicSubmarine
//method STRING for class AtomicSubmarine
obj_String AtomicSubmarine_method_STRING(obj_AtomicSubmarine this){
    //local variables for this method
    //end local vars

    obj_String r0 = str_literal("Atomic");

    return (obj_String) r0;

}

struct class_AtomicSubmarine_struct the_class_AtomicSubmarine_struct =
{
    new_AtomicSubmarine, /* constructor */
    AtomicSubmarine_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
};
//constructor for YellowSubmarine
obj_YellowSubmarine new_YellowSubmarine(){
    obj_YellowSubmarine this = (obj_YellowSubmarine)malloc(sizeof(struct obj_YellowSubmarine_struct));
    this->clazz = &the_class_YellowSubmarine_struct;
    //local variables for this constructor
    //end local vars

    return this;
}

//METHOD SET FOR YellowSubmarine
//method STRING for class YellowSubmarine
obj_String YellowSubmarine_method_STRING(obj_YellowSubmarine this){
    //local variables for this method
    //end local vars

    obj_String r0 = str_literal("Yellow");

    return (obj_String) r0;

}

struct class_YellowSubmarine_struct the_class_YellowSubmarine_struct =
{
    new_YellowSubmarine, /* constructor */
    YellowSubmarine_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
};

//MAIN BODY
int main(){
    //local variables for main
    obj_Int y;
    obj_Submarine sub;
    obj_Int x;
    //end local vars


    obj_Submarine r0 = the_class_Submarine_struct.constructor();
    sub = (obj_Submarine) r0;


    obj_Int r1 = int_literal(10);
    x = (obj_Int) r1;


    obj_Int r2 = int_literal(7);
    y = (obj_Int) r2;



    obj_Int r7 = int_literal(5);
    obj_Int r5 = x;

    obj_Int r6 = r7;
    obj_Int r11 = int_literal(7);
    obj_Int r9 = y;

    obj_Int r10 = r11;
//and registers
    obj_Boolean r4 = r5->clazz->LESS((obj_Int) r5, r6);
    obj_Boolean r8 = r9->clazz->LESS((obj_Int) r9, r10);

    obj_Boolean r12 = r4->clazz->EQUALS(r4, lit_true); //left is true
    obj_Boolean r13 = r8->clazz->EQUALS(r8, lit_true); //right is true
    obj_Boolean r3 = ((r8->value == 1 && r4->value == 1) ? lit_true : lit_false);
    if(r3->value == 1){ goto r3trueblock; }else{ goto r3falseblock; }

r3trueblock: ; //need semicolon because declarations may not follow labels, so we insert an emtpy statement

    obj_AtomicSubmarine r14 = the_class_AtomicSubmarine_struct.constructor();
    sub = (obj_Submarine) r14;

    goto r3end;
r3falseblock: ; //need semicolon because declarations may not follow labels, so we insert an emtpy statement

    obj_YellowSubmarine r15 = the_class_YellowSubmarine_struct.constructor();
    sub = (obj_Submarine) r15;


r3end: ;

    obj_Submarine r16 = sub;


    r16->clazz->PRINT((obj_Obj) r16);

    return 0;
}
