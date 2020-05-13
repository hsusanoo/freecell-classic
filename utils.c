//
// Created by haita on 2020/05/07.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void printr(char *string) {
    printf(ANSI_COLOR_RED"%s"ANSI_COLOR_RESET, string);
}

void clrscr() {
    system(CLR);
}
//
//void wrapEnable() {
//    printf(TOGGLE_WRAP_ENABLE);
//}
//
//void wrapDisable() {
//    printf(TOGGLE_WRAP_DISABLE);
//}