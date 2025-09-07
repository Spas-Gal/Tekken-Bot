#include "Methods.h"




void sleepOneFrame(DWORD_PTR move_info_ptr) {
	DWORD_PTR frameCountPointer = move_info_ptr + frameCountOffset;
	DWORD64 initialFrameValue = readDwordFromMemory(tekkenHandle, frameCountPointer);
	try {
		while (readDwordFromMemory(tekkenHandle, frameCountPointer) == initialFrameValue) {
			//do nothing
			//maybe refactor to more efficient instead of busy-wait?
		}
	}
	catch (int e) {
		printf("cannot find frame counter memory location, sleeping for 0.5 seconds...");
		Sleep(500);
	}

}


void sleepFrames(DWORD_PTR move_info_ptr, int frames) {

	for (int i = 0; i < frames; i++) {
		sleepOneFrame(move_info_ptr);
	}

}

DWORD64 getFrames(DWORD_PTR move_info_ptr) {

	DWORD64 frames = readDwordFromMemory(tekkenHandle, move_info_ptr + frameAdvantageOffset);

	return frames;
}
//gets current side the player is on (so side switches will change value of method)
//returns 0 for p1 side, returns 1 for p2 side
DWORD64 getSide(DWORD_PTR move_info_ptr) {

	DWORD64 side = readDwordFromMemory(tekkenHandle, move_info_ptr + sideOffset);


	//printf("The side at memory location %p is %i\n", move_info_ptr + sideOffset, side);
	return side;
}

DWORD64 getMoveTimer(DWORD_PTR move_info_ptr) {
	return readDwordFromMemory(tekkenHandle, move_info_ptr + moveTimerOffset);

}

/*
Gets minimum startup frames of move
*/
DWORD64 getMoveStartupMin(DWORD_PTR move_info_ptr) {
	DWORD_PTR moveStartupStruct = readDwordFromMemory(tekkenHandle, move_info_ptr + moveStartupStructOffset);
	return readDwordFromMemory(tekkenHandle, moveStartupStruct + moveStartupMinOffset);
}

/*
Gets maximum startuip frames of move
*/
DWORD64 getMoveStartupMax(DWORD_PTR move_info_ptr) {
	DWORD_PTR moveStartupStruct = readDwordFromMemory(tekkenHandle, move_info_ptr + moveStartupStructOffset);
	return readDwordFromMemory(tekkenHandle, moveStartupStruct + moveStartupMaxOffset);
}






