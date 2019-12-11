#include "Builtins.h"

#include <string.h>

#include <stdlib.h>

//FORWARD DECLARATION

//CLASS DEFINITIONS

//MAIN BODY
int main(){
    //local variables for main
    obj_Int x;
    //end local vars

    obj_Int r0 = int_literal(0);
    x = (obj_Int) r0;


    obj_Int r4 = int_literal(5);
    obj_Int r2 = x;

    obj_Int r3 = r4;

    obj_Boolean r1 = r2->clazz->LESS((obj_Int) r2, r3);

if(r1->value == 0){ goto r1loop_end;}
r1start_label: ;
    obj_Int r5 = x;

    obj_String r6 = str_literal("\n");
    obj_String r7 = r6;

    obj_Int r10 = int_literal(1);
    obj_Int r8 = x;

    obj_Int r9 = r10;
    x = (obj_Int) r8->clazz->PLUS((obj_Int) r8, r9);
    obj_String r11 = str_literal("new_x=");
    obj_String r12 = r11;

    obj_Int r13 = x;

    obj_String r14 = str_literal("\n========\n");
    obj_String r15 = r14;


    r5->clazz->PRINT((obj_Obj) r5);
    r7->clazz->PRINT((obj_String) r7);
    r12->clazz->PRINT((obj_String) r12);
    r13->clazz->PRINT((obj_Obj) r13);
    r15->clazz->PRINT((obj_String) r15);

    obj_Int r18 = int_literal(5);
    obj_Int r16 = x;

    obj_Int r17 = r18;
    r1 = r16->clazz->LESS((obj_Int) r16, r17);
if(r1->value == 1){ goto r1start_label; }
r1loop_end: ;
//end while loop

    obj_String r19 = str_literal("end\n");
    obj_String r20 = r19;


    r20->clazz->PRINT((obj_String) r20);

    return 0;
}
