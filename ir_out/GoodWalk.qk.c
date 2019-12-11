#include "Builtins.h"

#include <string.h>

#include <stdlib.h>

//FORWARD DECLARATION
//forward declaration for class C1
struct class_C1_struct;
typedef struct class_C1_struct* class_C1;
struct class_C1_struct the_class_C1_struct;

//forward declaration for class C2
struct class_C2_struct;
typedef struct class_C2_struct* class_C2;
struct class_C2_struct the_class_C2_struct;

//forward declaration for class C3
struct class_C3_struct;
typedef struct class_C3_struct* class_C3;
struct class_C3_struct the_class_C3_struct;

//forward declaration for class C4
struct class_C4_struct;
typedef struct class_C4_struct* class_C4;
struct class_C4_struct the_class_C4_struct;


//CLASS DEFINITIONS
//ivar table for class C1
typedef struct obj_C1_struct {
    class_C1 clazz;
} * obj_C1;

//method table for class C1
struct class_C1_struct {
    obj_C1 (*constructor)(void);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_C1 (*foo)(obj_C1);
};

//ivar table for class C2
typedef struct obj_C2_struct {
    class_C2 clazz;
} * obj_C2;

//method table for class C2
struct class_C2_struct {
    obj_C2 (*constructor)(void);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_C1 (*foo)(obj_C2);
};

//ivar table for class C3
typedef struct obj_C3_struct {
    class_C3 clazz;
} * obj_C3;

//method table for class C3
struct class_C3_struct {
    obj_C3 (*constructor)(void);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_C2 (*foo)(obj_C3);
};

//ivar table for class C4
typedef struct obj_C4_struct {
    class_C4 clazz;
} * obj_C4;

//method table for class C4
struct class_C4_struct {
    obj_C4 (*constructor)(void);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_C3 (*foo)(obj_C4);
};

//constructor for C1
obj_C1 new_C1(){
    obj_C1 this = (obj_C1)malloc(sizeof(struct obj_C1_struct));
    this->clazz = &the_class_C1_struct;
    //local variables for this constructor
    //end local vars

    return this;
}

//METHOD SET FOR C1
//method foo for class C1
obj_C1 C1_method_foo(obj_C1 this){
    //local variables for this method
    //end local vars


    obj_C1 r0 = the_class_C1_struct.constructor();

    return (obj_C1) r0;

}

struct class_C1_struct the_class_C1_struct =
{
    new_C1, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    C1_method_foo,
};
//constructor for C2
obj_C2 new_C2(){
    obj_C2 this = (obj_C2)malloc(sizeof(struct obj_C2_struct));
    this->clazz = &the_class_C2_struct;
    //local variables for this constructor
    //end local vars

    return this;
}

//METHOD SET FOR C2
//method foo for class C2
obj_C1 C2_method_foo(obj_C2 this){
    //local variables for this method
    //end local vars


    obj_C1 r0 = the_class_C1_struct.constructor();

    return (obj_C1) r0;

}

struct class_C2_struct the_class_C2_struct =
{
    new_C2, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    C2_method_foo,
};
//constructor for C3
obj_C3 new_C3(){
    obj_C3 this = (obj_C3)malloc(sizeof(struct obj_C3_struct));
    this->clazz = &the_class_C3_struct;
    //local variables for this constructor
    //end local vars

    return this;
}

//METHOD SET FOR C3
//method foo for class C3
obj_C2 C3_method_foo(obj_C3 this){
    //local variables for this method
    //end local vars


    obj_C2 r0 = the_class_C2_struct.constructor();

    return (obj_C2) r0;

}

struct class_C3_struct the_class_C3_struct =
{
    new_C3, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    C3_method_foo,
};
//constructor for C4
obj_C4 new_C4(){
    obj_C4 this = (obj_C4)malloc(sizeof(struct obj_C4_struct));
    this->clazz = &the_class_C4_struct;
    //local variables for this constructor
    //end local vars

    return this;
}

//METHOD SET FOR C4
//method foo for class C4
obj_C3 C4_method_foo(obj_C4 this){
    //local variables for this method
    //end local vars


    obj_C3 r0 = the_class_C3_struct.constructor();

    return (obj_C3) r0;

}

struct class_C4_struct the_class_C4_struct =
{
    new_C4, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    C4_method_foo,
};

//MAIN BODY
int main(){
    //local variables for main
    obj_C4 x;
    //end local vars


    obj_C4 r0 = the_class_C4_struct.constructor();
    x = (obj_C4) r0;



    obj_Boolean r1 = lit_true;

if(r1->value == 0){ goto r1loop_end;}
r1start_label: ;
    obj_C1 r2 = x;

    x = (obj_C4) r2->clazz->foo((obj_C1) r2);


    r1 = lit_true;
if(r1->value == 1){ goto r1start_label; }
r1loop_end: ;
//end while loop

    return 0;
}
