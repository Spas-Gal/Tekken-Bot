#pragma once
#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <thread>
#include "Methods.h"




extern std::unordered_map<char, WORD> scanCodeMap;

extern std::unordered_map<char, char> notationToInputMapP1;


void PressKey(WORD scanCode);



void ReleaseKey(WORD scanCode);

void HoldKey(WORD scanCode, int milliseconds);


WORD scanCode(char charValue);


void ReleaseAllKeys();

void BlockAndReleaseKeysNonBlocking(DWORD64 durationMs);

//from a string (e.g. "df2" or "12") presses all inputs in the string for desired number of frames
void HoldInput(std::string inputString, int frames, int side);

bool keyDown(int vKey);
