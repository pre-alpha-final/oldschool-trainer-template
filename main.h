#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include ".\eat\exy_window.h"
#include ".\eat\exy_button.h"
#include ".\eat\exy_rbutton.h"
#include ".\eat\exy_listbox.h"
#include ".\eat\exy_edit.h"
#include ".\eat\exy_combo.h"
#include ".\eat\mem_functions.h"
#include ".\eat\file_functions.h"
#include ".\eat\exy_find.h"
#include ".\eat\exy_send.h"
#include ".\eat\ufmod.h"
#include ".\eat\exy_strutils.h"

//change this for your trainer
#define window1_width 516
#define window1_height 216
#define window2_width 571
#define window2_height 271
#define window2_x_offset 0
#define window2_y_offset 216
#define gamewindowname "Warcraft III"
//end of things you need to change

//windows, buttons and shit
extern char *programpath;
extern exywindow *window1; //we need to know this to run the timer

//debug functions
void ShowMessage(const char *mes);
void ShowMessage(int mes);
//end debug functions

#endif
