#include "Builtins.h"

#include <string.h>

#include <stdlib.h>

//FORWARD DECLARATION
//forward declaration for class Pt
struct class_Pt_struct;
typedef struct class_Pt_struct* class_Pt;
struct class_Pt_struct the_class_Pt_struct;

//forward declaration for class Rect
struct class_Rect_struct;
typedef struct class_Rect_struct* class_Rect;
struct class_Rect_struct the_class_Rect_struct;

//forward declaration for class Square
struct class_Square_struct;
typedef struct class_Square_struct* class_Square;
struct class_Square_struct the_class_Square_struct;


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
    obj_String (*STRING)(obj_Pt);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_Pt (*PLUS)(obj_Pt, obj_Pt);
    obj_Int (*_x)(obj_Pt);
    obj_Int (*_y)(obj_Pt);
};

//ivar table for class Rect
typedef struct obj_Rect_struct {
    class_Rect clazz;
    //instance vars inherited from class Rect
    obj_Pt ur;
    obj_Pt ll;
} * obj_Rect;

//method table for class Rect
struct class_Rect_struct {
    obj_Rect (*constructor)(obj_Pt, obj_Pt);
    obj_Nothing (*STRING)(obj_Rect);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_Rect (*translate)(obj_Rect, obj_Pt);
};

//ivar table for class Square
typedef struct obj_Square_struct {
    class_Square clazz;
    //instance vars inherited from class Rect
    obj_Pt ur;
    obj_Pt ll;
    //instance vars inherited from class Square
} * obj_Square;

//method table for class Square
struct class_Square_struct {
    obj_Square (*constructor)(obj_Pt, obj_Int);
    obj_Nothing (*STRING)(obj_Rect);
    obj_Obj (*PRINT)(obj_Obj);
    obj_Boolean (*EQUALS)(obj_Obj, obj_Obj);
    obj_Rect (*translate)(obj_Rect, obj_Pt);
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
//method STRING for class Pt
obj_String Pt_method_STRING(obj_Pt this){
    //local variables for this method
    //end local vars

    obj_String r0 = str_literal("(");
    obj_Int r3 = this->x;

    obj_String r1 = r0;

    obj_String r2 = r3->clazz->STRING((obj_Int) r3);
    obj_String r6 = str_literal(",");
    obj_String r4 = r1->clazz->PLUS((obj_String) r1, r2);

    obj_String r5 = r6;
    obj_Int r9 = this->y;

    obj_String r7 = r4->clazz->PLUS((obj_String) r4, r5);

    obj_String r8 = r9->clazz->STRING((obj_Int) r9);
    obj_String r12 = str_literal(")");
    obj_String r10 = r7->clazz->PLUS((obj_String) r7, r8);

    obj_String r11 = r12;

    return (obj_String) r10->clazz->PLUS((obj_String) r10, r11);

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

//method _x for class Pt
obj_Int Pt_method__x(obj_Pt this){
    //local variables for this method
    //end local vars


    return (obj_Int) this->x;

}

//method _y for class Pt
obj_Int Pt_method__y(obj_Pt this){
    //local variables for this method
    //end local vars


    return (obj_Int) this->y;

}

struct class_Pt_struct the_class_Pt_struct =
{
    new_Pt, /* constructor */
    Pt_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    Pt_method_PLUS,
    Pt_method__x,
    Pt_method__y,
};
//constructor for Rect
obj_Rect new_Rect(obj_Pt ll, obj_Pt ur){
    obj_Rect this = (obj_Rect)malloc(sizeof(struct obj_Rect_struct));
    this->clazz = &the_class_Rect_struct;
    //local variables for this constructor
    //end local vars


    this->ll = ll;


    this->ur = ur;

    return this;
}

//METHOD SET FOR Rect
//method translate for class Rect
obj_Rect Rect_method_translate(obj_Rect this, obj_Pt delta){
    //local variables for this method
    //end local vars

    obj_Pt r2 = this->ll;

    obj_Pt r3 = delta;
    obj_Pt r5 = this->ur;

    obj_Pt r6 = delta;
    obj_Pt r1 = r2->clazz->PLUS((obj_Pt) r2, r3);
    obj_Pt r4 = r5->clazz->PLUS((obj_Pt) r5, r6);

    obj_Rect r0 = the_class_Rect_struct.constructor(r1, r4);

    return (obj_Rect) r0;

}

//method STRING for class Rect
obj_Nothing Rect_method_STRING(obj_Rect this){
    //local variables for this method
    obj_Pt ul;
    obj_Pt lr;
    //end local vars

    obj_Pt r2 = this->ur;

    obj_Pt r4 = this->ll;

    obj_Int r1 = r2->clazz->_y((obj_Pt) r2);
    obj_Int r3 = r4->clazz->_x((obj_Pt) r4);

    obj_Pt r0 = the_class_Pt_struct.constructor(r1, r3);
    lr = (obj_Pt) r0;


    obj_Pt r7 = this->ll;

    obj_Pt r9 = this->ur;

    obj_Int r6 = r7->clazz->_x((obj_Pt) r7);
    obj_Int r8 = r9->clazz->_y((obj_Pt) r9);

    obj_Pt r5 = the_class_Pt_struct.constructor(r6, r8);
    ul = (obj_Pt) r5;


    obj_String r10 = str_literal("(");
    obj_Pt r13 = this->ll;

    obj_String r11 = r10;

    obj_String r12 = r13->clazz->STRING((obj_Pt) r13);
    obj_String r16 = str_literal(", ");
    obj_String r14 = r11->clazz->PLUS((obj_String) r11, r12);

    obj_String r15 = r16;
    obj_Pt r19 = ul;

    obj_String r17 = r14->clazz->PLUS((obj_String) r14, r15);

    obj_String r18 = r19->clazz->STRING((obj_Pt) r19);
    obj_String r22 = str_literal(",");
    obj_String r20 = r17->clazz->PLUS((obj_String) r17, r18);

    obj_String r21 = r22;
    obj_Pt r25 = this->ur;

    obj_String r23 = r20->clazz->PLUS((obj_String) r20, r21);

    obj_String r24 = r25->clazz->STRING((obj_Pt) r25);
    obj_String r28 = str_literal(",");
    obj_String r26 = r23->clazz->PLUS((obj_String) r23, r24);

    obj_String r27 = r28;
    obj_Pt r31 = lr;

    obj_String r29 = r26->clazz->PLUS((obj_String) r26, r27);

    obj_String r30 = r31->clazz->STRING((obj_Pt) r31);
    obj_String r34 = str_literal(")");
    obj_String r32 = r29->clazz->PLUS((obj_String) r29, r30);

    obj_String r33 = r34;

    return (obj_Nothing) r32->clazz->PLUS((obj_String) r32, r33);

}

struct class_Rect_struct the_class_Rect_struct =
{
    new_Rect, /* constructor */
    Rect_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    Rect_method_translate,
};
//constructor for Square
obj_Square new_Square(obj_Pt ll, obj_Int side){
    obj_Square this = (obj_Square)malloc(sizeof(struct obj_Square_struct));
    this->clazz = &the_class_Square_struct;
    //local variables for this constructor
    //end local vars


    this->ll = ll;

    obj_Pt r2 = this->ll;

    obj_Int r3 = r2->clazz->_x((obj_Pt) r2);

    obj_Int r4 = side;
    obj_Pt r6 = this->ll;

    obj_Int r7 = r6->clazz->_y((obj_Pt) r6);

    obj_Int r8 = side;
    obj_Int r1 = r3->clazz->PLUS((obj_Int) r3, r4);
    obj_Int r5 = r7->clazz->PLUS((obj_Int) r7, r8);

    obj_Pt r0 = the_class_Pt_struct.constructor(r1, r5);

    this->ur = r0;

    return this;
}

//METHOD SET FOR Square
struct class_Square_struct the_class_Square_struct =
{
    new_Square, /* constructor */
    Rect_method_STRING,
    Obj_method_PRINT,
    Obj_method_EQUALS,
    Rect_method_translate,
};

//MAIN BODY
int main(){
    //local variables for main
    obj_Rect a_square;
    //end local vars

    obj_Int r4 = int_literal(3);
    obj_Int r6 = int_literal(3);
    obj_Int r3 = r4;
    obj_Int r5 = r6;

    obj_Pt r2 = the_class_Pt_struct.constructor(r3, r5);
    obj_Int r8 = int_literal(5);
    obj_Pt r1 = r2;
    obj_Int r7 = r8;

    obj_Square r0 = the_class_Square_struct.constructor(r1, r7);
    a_square = (obj_Rect) r0;


    obj_Int r13 = int_literal(2);
    obj_Int r15 = int_literal(2);
    obj_Int r12 = r13;
    obj_Int r14 = r15;

    obj_Pt r11 = the_class_Pt_struct.constructor(r12, r14);
    obj_Rect r9 = a_square;

    obj_Pt r10 = r11;
    a_square = (obj_Rect) r9->clazz->translate((obj_Rect) r9, r10);


    obj_Rect r16 = a_square;


    r16->clazz->PRINT((obj_Obj) r16);

    return 0;
}
