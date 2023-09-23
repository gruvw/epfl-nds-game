/*
 * Template Nintendo DS
 * May 2011
 */
#include <nds.h>
#include <stdio.h>

int main(void) {

    consoleDemoInit();
    printf("\nI love you Sarah ! <3");

    while(1){
        swiWaitForVBlank();
}
}
