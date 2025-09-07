#pragma once
#include <Windows.h>
#include "Memory.h"


//old ptr offsets (for some reason offsets increased by 0x20 in season 2
// const DWORD_PTR moveIDOffset = 0x528;
//const DWORD_PTR moveTimerOffset = 0x38C;
//const DWORD_PTR moveStartupStructOffset = 0x3C0;
//const DWORD_PTR moveStartupMinOffset = 0x158;
//const DWORD_PTR moveStartupMaxOffset = 0x15C;
//const DWORD_PTR hitPropertyOffset = 0x4F0;
// 
	//pre season 2 throw tech offset
	//DWORD_PTR throwTechOffset = 0x668;

//side offset is different change in season 2 for some reason
//const DWORD_PTR sideOffset = 0x342C;
// 
//
//pointer offsets (for 1 level above)
//maybe use #defines instead to practice using them?

const DWORD_PTR moveIDOffset = 0x548;
const DWORD_PTR moveTimerOffset = 0x3AC;
const DWORD_PTR moveStartupStructOffset = 0x3E0;
const DWORD_PTR moveStartupMinOffset = 0x158;
const DWORD_PTR moveStartupMaxOffset = 0x15C;
const DWORD_PTR sideOffset = 0x3570;
const DWORD_PTR hitPropertyOffset = 0x510;
const DWORD_PTR throwTechOffset = 0x688;
const DWORD_PTR frameCountOffset = 0x3AC;

const DWORD_PTR frameAdvantageOffset = 0x1FC0;


void sleepOneFrame(DWORD_PTR move_info_ptr);

void sleepFrames(DWORD_PTR move_info_ptr, int frames);

DWORD64 getFrames(DWORD_PTR move_info_ptr);

DWORD64 getSide(DWORD_PTR move_info_ptr);

DWORD64 getMoveTimer(DWORD_PTR move_info_ptr);

DWORD64 getMoveStartupMin(DWORD_PTR move_info_ptr);

DWORD64 getMoveStartupMax(DWORD_PTR move_info_ptr);

