#include "Builtins.h"

#include <string.h>

#include <stdlib.h>

//FORWARD DECLARATION
//forward declaration for class Parent
struct class_Parent_struct;
typedef struct class_Parent_struct* class_Parent;
struct class_Parent_struct the_class_Parent_struct;

//forward declaration for class Middle
struct class_Middle_struct;
typedef struct class_Middle_struct* class_Middle;
struct class_Middle_struct the_class_Middle_struct;

//forward declaration for class Child
struct class_Child_struct;
typedef struct class_Child_struct* class_Child;
struct class_Child_struct the_class_Child_struct;


//CLASS DEFINITIONS
//ivar table for class Parent
typedef struct obj_Parent_struct {
    class_Parent clazz;
    //instance vars inherited from class Parent
    obj_Int y;
    obj_Int x;
} * obj_Parent;

//method table for class Parent
struct class_Parent_struct {
    obj_Parent (*constructor)(obj_Int, obj_Int);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
};

//ivar table for class Middle
typedef struct obj_Middle_struct {
    class_Middle clazz;
    //instance vars inherited from class Middle
    obj_Int y;
    obj_Int z;
    obj_Int x;
} * obj_Middle;

//method table for class Middle
struct class_Middle_struct {
    obj_Middle (*constructor)(obj_Int);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
};

//ivar table for class Child
typedef struct obj_Child_struct {
    class_Child clazz;
    //instance vars inherited from class Child
    obj_Int z;
} * obj_Child;

//method table for class Child
struct class_Child_struct {
    obj_Child (*constructor)(void);
    obj_String (*STRING)(obj_Obj);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_Nothing (*foo)(obj_Child);
};

//constructor for Parent
obj_Parent new_Parent(obj_Int x, obj_Int y){
    obj_Parent this = (obj_Parent)malloc(sizeof(struct obj_Parent_struct));
    this->clazz = &the_class_Parent_struct;
    //local variables for this constructor
    //end local vars


    this->x = x;


    this->y = y;

    return this;
}

//METHOD SET FOR Parent
struct class_Parent_struct the_class_Parent_struct =
{
    new_Parent, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
};
//constructor for Middle
obj_Middle new_Middle(obj_Int z){
    obj_Middle this = (obj_Middle)malloc(sizeof(struct obj_Middle_struct));
    this->clazz = &the_class_Middle_struct;
    //local variables for this constructor
    //end local vars


    this->z = z;

    obj_Int r0 = int_literal(5);

    this->x = r0;

    obj_Int r1 = int_literal(10);

    this->y = r1;

    return this;
}

//METHOD SET FOR Middle
struct class_Middle_struct the_class_Middle_struct =
{
    new_Middle, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
};
//constructor for Child
obj_Child new_Child(){
    obj_Child this = (obj_Child)malloc(sizeof(struct obj_Child_struct));
    this->clazz = &the_class_Child_struct;
    //local variables for this constructor
    //end local vars

    obj_Int r0 = int_literal(15);

    this->z = r0;

    return this;
}

//METHOD SET FOR Child
//method foo for class Child
obj_Nothing Child_method_foo(obj_Child this){
    //local variables for this method
    //end local vars

    obj_Int r0 = this->z;


    r0->clazz->PRINT((obj_Obj) r0);

}

struct class_Child_struct the_class_Child_struct =
{
    new_Child, /* constructor */
    Obj_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    Child_method_foo,
};

//MAIN BODY
int main(){
    //local variables for main
    obj_Child test;
    //end local vars


    obj_Child r0 = the_class_Child_struct.constructor();
    test = (obj_Child) r0;


    obj_Child r1 = test;


    r1->clazz->foo((obj_Child) r1);

    return 0;
}
