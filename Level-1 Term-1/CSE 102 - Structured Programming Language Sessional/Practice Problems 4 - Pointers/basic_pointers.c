#include <stdio.h>

int main () {

   /* actual variable declaration */
   int  var = 20;
   /* pointer variable declaration */
   int  *ip;
    /* store address of var in pointer variable*/
   ip = &var;

   printf("Address of var variable: %x\n", &var  );

   /*using ip variable print address stored in pointer variable */
   printf("Address stored in ip variable: %x\n", ip );

   /*using ip variable  access the value using the pointer */
   printf("Value of *ip variable: %d\n", *ip );

   return 0;
}

