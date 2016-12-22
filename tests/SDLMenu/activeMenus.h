#ifndef ACTIVEMENUS_H
#define ACTIVEMENUS_H
#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "timeHelpers.h"
#include <SDL.h>

void genericCancelDisplay();
void genericCancelProcessButton();
void genericAcceptChildProcessButton();
void genericUpdateVisibleDisplay();

void mainMenuShowDisplay();
void mainMenuProcessButton();

void mainChildStartMotorDisplay();
void mainChildDisplayTimeDisplay();
void mainChildSetTimeDisplay();
void mainChildSetAlarmDisplay();
void mainChildToggleShabbosDisplay();

void displayTimeChildDisplay();
void displayTimeSlot();
int getTimeStart();
int getTimeMax();
void displaySetTime();
void changeTime();

void toggleShabbosOnDisplay();
void toggleShabbosOffDisplay();
void changeShabbosOn();
void changeShabbosOff();

#endif