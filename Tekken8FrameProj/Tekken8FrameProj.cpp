// Tekken8FrameProj.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <Windows.h>
#include <iostream>
#include "Input.h"
#include "Macros.h"
#include "Memory.h"
#include "Methods.h"
#include "GameStructs.h"
#include <cstdlib>



//number of ms per frame (16.666 in reality, but 17 because better to be over-adjust than underadjust
//so inputs guaranteed at least 1 frame)
const int msPerFrame = 17;



DWORD64 lastMove = 0x9999;

DWORD64 currentMove = 0x32769;

DWORD64 currentParriedMove = 0x9999;



bool hasMoveChangedThisIteration(DWORD_PTR move_info_ptr) {


	DWORD64 readMove = readDwordFromMemory(tekkenHandle, move_info_ptr + moveIDOffset);

	//if the read move is different, then it is not true
	if (readMove != currentMove) {
		lastMove = currentMove;
		currentMove = readMove;
		return true;
	}

	//otherwise, return false
	return false;
	
}


/*
concept of autoblock script is to override the user's input to stop it as soon as the script detects the opponent has made an attack
to prevent them from being counterhit since the user cant react in time (of course dont override if the user's input would result
in a faster attack that would counterhit the opponent before)
For now, I am just making an auto-guard script
*/








/*
concept of autoblock script is to override the user's input to stop it as soon as the script detects the opponent has made an attack
to prevent them from being counterhit since the user cant react in time (of course dont override if the user's input would result
in a faster attack that would counterhit the opponent before)
For now, I am just making an auto-guard script
*/


//assumes that player is not pressing any buttons at all
//simply guards depending on move data (i mean throws technically aren't recorded as moves so we need to work on detecting that but i have auto throw break so doesnt matter)

/*
autoblocks simply assuming player is not pressing buttons, pass pointer of opponent's move info struct
*/


void autoBlockSimple(DWORD_PTR move_info_ptr, int side) {
	
	//old hit property offset, season 2 increased offsets by 0x20
	// DWORD_PTR hitPropertyOffset = 0x4F0;


	HitProperty hitPropertyValue = (HitProperty)readDwordFromMemory(tekkenHandle, move_info_ptr + hitPropertyOffset);


	//deprecated, since side switches depending on "internal" side, now pass side into function paramter
	//int side = 1 - getSide(move_info_ptr);

	//printf("The side at memory location %p is %i\n", move_info_ptr + sideOffset, side);




	/*
	non attacking move (default): C000001D, high: A000050F, mid: 8000020A, low: 20000112,
	mid unblockable(jin's cancelable charge-up mid unblockable at least) (maybe different for high/mid/low unblockables?): C000071A,
	high unblockable (kazuya laser in heat): 2000081B,
	special low: 60000402, special mid: 10000302,
	during_throw_animations(attacker): 60000A1D
	(maybe check to see if any other values are possible for some niche states)
	*/


	int moveTimer = 0;//getMoveTimer(move_info_ptr);
	int moveStartup = 0; //getMoveStartupMin(move_info_ptr);
	int moveStartupMax = 0; //getMoveStartupMax(move_info_ptr);


	printf("side: %i, movetimer: %i; moveStartup: %i; hitProperty: %x\n", side, moveTimer, moveStartup, (int)hitPropertyValue);
	

	//TODO: use move timer and startup frames to try to calculate the exact moment to low parry
	//one weakness of it is unblockables and charge up attacks, to solve this just manually put every charge up attack (imo) unless i can find an automation solution?
	//to fix unblockables, if unblockable then get startup frames/move timer and step when the timer is about to reach the startup frames
	switch (hitPropertyValue) {
	case HIGH:
		HoldInput("db", 1, side);
		printf("HIGH\n");
		break;
	case MID:
		printf("MID\n");
		HoldInput("b", 1, side);
		break;
	case LOW:
		HoldInput("db", 1, side);
		break;

	case UNBLOCKABLE_MID:
		//HoldInput("u", 1);
		//Sleep(30);
		//HoldInput("u", 30);

		break;

	case UNBLOCKABLE_HIGH:
		//50 frames cuz sometimes it randomly releases early, maybe because active window too long or smth?
		HoldInput("db", 50, side);
		break;
	case UNBLOCKABLE_LOW:
		//automatically do delayed hopkick when the timing is right
		HoldInput("uf", 3, side);
		Sleep(1 * msPerFrame);
		HoldInput("4", 3, side);
		break;
	case SPECIAL_MID:
		HoldInput("b", 1, side);
		break;
	case SPECIAL_LOW:
		HoldInput("db", 1, side);
		break;
	default:
		HoldInput("f", 1, side);
		printf("Val: %x\n", hitPropertyValue);
		break;

	}
}


/*
Automatically does leroy parry (and maybe change name if it works also on jin) since they are strong parries I won't check to see if the attack is parryable,
only check to see if it is a mid (or maybe high just to be safe in case of a uninterruptable high-mid string that deals chip?)

*/
void autoLeroyParry(DWORD_PTR move_info_ptr) {


	HitProperty hitPropertyValue = (HitProperty)readDwordFromMemory(tekkenHandle, move_info_ptr + hitPropertyOffset);


	DWORD_PTR isThrowOffset = 0x5B4;

	DWORD64 isThrowValue = readDwordFromMemory(tekkenHandle, move_info_ptr + isThrowOffset);

	bool isThrow = ((int)isThrowValue == 39);//(isThrowValue == 0x27);

	printf("isThrow: %i\n", isThrowValue);

	printf("isThrow: %i\n", isThrow);


	int side = getSide(move_info_ptr) ^ 1;
	int moveTimer = getMoveTimer(move_info_ptr);
	int moveStartup = getMoveStartupMin(move_info_ptr);
	int moveStartupMax = getMoveStartupMax(move_info_ptr);

	if (isThrow) {
		Sleep(100);
		//autoThrowBreak(move_info_ptr, 0, 0);
	}

	else if (hitPropertyValue == HIGH || hitPropertyValue == MID || hitPropertyValue == SPECIAL_MID) {






		printf("%i\n", side);
		//if move will hit in 4 frames (to account for processing delays), then parry


		//super basic, add chefck to fast strings later

		int parryDelay = 7;
		int parryTimer = moveStartup - parryDelay;
		if (moveTimer > parryTimer && currentParriedMove != currentMove && moveTimer < moveStartupMax) {
			if (moveStartup > 0) {
				HoldInput("b2", 3, side);
			}
			currentParriedMove = currentMove;
		}


		else if (moveTimer > moveStartupMax) {
			currentParriedMove = 0x99999;
		}
	}

	else if (hitPropertyValue == LOW || hitPropertyValue == SPECIAL_LOW) {


		HoldInput("df", 14, side);
		HoldInput("b", 1, side);
		printf("%i 333\n", side);
	}



	else {


		HoldInput("f", 1, side);
	}

}


/*
Automatically does jin parry. Requires special function as jin has strong parry and cannot parry successive hits like leroy.
So it will check to see if the next attack from the opponent has a long enough startup time, then the parry will probably work.
As the parry is repeatedly called, the next attack will also be parried and the next next attack will be checked as well.
So by induction the parry will work since it will parry the next hit until there are no more hits or the next hit is too quick
to be parried.

move_info_ptr is the pointer to the move_info struct of the jin player
*/

void autoJinParry(DWORD_PTR move_info_ptr, int side) {


	HitProperty hitPropertyValue = (HitProperty)readDwordFromMemory(tekkenHandle, move_info_ptr + hitPropertyOffset);

	//pre season2 isThrowOffset
	//DWORD_PTR isThrowOffset = 0x5B4;

	//DWORD_PTR isThrowOffset = 0x5D4;

	DWORD64 isThrowValue = readDwordFromMemory(tekkenHandle, move_info_ptr + throwTechOffset);

	//bool isThrow = ((int)isThrowValue != 0xC000001D);//(isThrowValue == 0x27);

	//deprecated, since side is different "internally"
	//int side = 1 - getSide(move_info_ptr);
	printf("side: %i\n", side);
	int moveTimer = getMoveTimer(move_info_ptr);
	int moveStartup = getMoveStartupMin(move_info_ptr);
	int moveStartupMax = getMoveStartupMax(move_info_ptr);



	if (hitPropertyValue == HIGH) {
		HoldInput("db", 1, side);
	}
	else if (false) {//isThrow) {
		Sleep(100);
		//autoThrowBreak(move_info_ptr, 0, 0);
	}

	else if (hitPropertyValue == MID || hitPropertyValue == SPECIAL_MID) {



		


		//printf("%i\n", side);
		//if move will hit in 4 frames (to account for processing delays), then parry


		//super basic, add chefck to fast strings later

		int parryDelay = 10;
		int parryTimer = moveStartup - parryDelay;
		if (moveTimer > parryTimer && currentParriedMove != currentMove && moveTimer < moveStartupMax) {
			if (moveStartup > 0) {
				HoldInput("b13", 3, side);
			}
			currentParriedMove = currentMove;
		}


		else if (moveTimer > moveStartupMax) {
			currentParriedMove = 0x99999;
		}
	}

	else if (hitPropertyValue == LOW || hitPropertyValue == SPECIAL_LOW) {
		
		if (moveTimer - moveStartup < 14) {
			HoldInput("df", 1, side);
		}
		//printf("%i 333\n", side);
	}

	
	else if (hitPropertyValue == UNBLOCKABLE_HIGH) {
		HoldInput("db", 1, side);
	}

	else if (hitPropertyValue == UNBLOCKABLE_MID) {
		//HoldInput("df", 4, side);
		//Sleep(30);
	}
	
	else if (hitPropertyValue == UNBLOCKABLE_LOW) {
		//automatically do delayed hopkick when the timing is right
		HoldInput("uf", 3, side);
		Sleep(1 * msPerFrame);
		HoldInput("4", 3, side);

	}
	
	else {
		

		HoldInput("f", 1, side);
	}
	
	//printf("HitPropertyValue: %ui\n", hitPropertyValue);
	//printf("values: %ui %ui %ui %ui", HIGH, MID, LOW, SPECIAL_MID);

}


/*
Auto throw breaks since that's annoying, reads the thrwo break variable in the player's move list

move_info_ptr is pointer to player struct, probability is percent it will throwbreak, delay is desired frames to delay
*/

void autoThrowBreak(DWORD_PTR move_info_ptr, float probability, int delay) {

	//TODO: add delay properly (maybe make separate function so I can modularly delay all macros)
	// 
	// 
	// 
	//throw tech (for the player who is doing the throwing)
//no throw = C000001D, 1+2 = F000001E, generic throw and 2 = E000001F, 1 = D000001C, 

	
	DWORD64 throwTechValue = readDwordFromMemory(tekkenHandle, move_info_ptr + throwTechOffset);



	//no throw = C000001D, 1+2 = F000001E, generic throw and 2 = E000001F, 1 = D000001C, 

	//printf("throwtech: %i, 1+2: %i, 1: %i, 2: %i\n", throwTechValue, 0xF000001E, 0xD000001C, 0xE000001F);

	//side doesn't matter for throw break
	int side = getSide(move_info_ptr);

	
	switch (throwTechValue) {
	case 0xF000001E:
		Sleep(delay * msPerFrame);
		HoldInput("12", 10, side);
		break;
	case 0xE000001F:
		Sleep(delay * msPerFrame);
		HoldInput("2", 10, side);
		break;

	case 0xD000001C:
		Sleep(delay * msPerFrame);
		HoldInput("1", 10, side);
		break;
	default:
		break;

	}



}



int main()
{
	/*
	DEBUG
	*/
	/*
	while (true) {
		autoWaveDash();
		autoKBD();

		if (GetAsyncKeyState('F') & 0x8000) {
			df2PEWGF();
		}
	}*/
	/*
	* END DEBUG
	*/
	std::cout << "Tekken 8 Frame Project STARTs\n";

	


	initTekkenHandle();
	initModuleAddresses();

	//inject the gui into the process
	//uninjectDLL();
	
	//cstdlib function, uninject at normal exit (termination/force close wont trigger atexit/uninjection)
	//std::atexit(uninjectDLL);

	


	bool isPressed = false;

	bool cheatToggle = true;

	bool counterHitToggle = false;


	//render the gui in a separate thread (will automatically close if main terminal is closed, or can just close gui and keep main terminal open)
	//std::thread guiThread(overlay::render);



	//main loop
	while (true) {



		//if (keyDown(VK_))
		if (false) {//(!overlay::cheatEnabled) {
			break;
		}

		/*
	P1 addresses (for online)
	*/

		DWORD_PTR p1Offset = 0x09B1DDE0;
		//switch below with the p2 offset if something is broken
		DWORD_PTR p1InfoOffset = 0x30;

		//printf("Base address pointer: %p\n", tekkenModulePointer + p1Offset);

		DWORD_PTR p1Pointer = readDwordFromMemory(tekkenHandle, tekkenModulePointer + p1Offset);

		//printf("p1Pointer: %p\n", p1Pointer);

		DWORD_PTR p1InfoPointer = readDwordFromMemory(tekkenHandle, p1Pointer + p1InfoOffset);
		//printf("p1nfoPointer: %p\n", p1InfoPointer);


		/*
		P2 addresses
		*/
		DWORD_PTR p2Offset = 0x09B1DDE0;
		//switch below with p1 offset if something is broken 
		DWORD_PTR p2InfoOffset = 0x38;

		//printf("Base address pointer: %p\n", tekkenModulePointer + p2Offset);

		DWORD_PTR p2Pointer = readDwordFromMemory(tekkenHandle, tekkenModulePointer + p2Offset);

		//printf("p2Pointer: %p\n", p2Pointer);

		DWORD_PTR p2InfoPointer = readDwordFromMemory(tekkenHandle, p2Pointer + p2InfoOffset);

		//printf("p2InfoPointer: %p\n", p2InfoPointer);

		//maybe move above into a separate while loop that updates less often?


		//DWORD_PTR p1MovePointer = readDwordFromMemory(tekkenHandle, p1InfoPointer + 0x3C0);

		//DWORD64 p1MoveID = readDwordFromMemory(tekkenHandle, p1InfoPointer + moveIDOffset);

		//int p1StartupFrames = (int)readDwordFromMemory(tekkenHandle, p1MovePointer + 0x158);


		//DWORD_PTR p2MovePointer = readDwordFromMemory(tekkenHandle, p2InfoPointer + 0x3C0);

		//DWORD64 p2MoveID = readDwordFromMemory(tekkenHandle, p2InfoPointer + moveIDOffset);

		//int p2StartupFrames = (int)readDwordFromMemory(tekkenHandle, p2MovePointer + 0x158);
		//printf("startup: %i\n", p2StartupFrames);
		int side = getSide(p1InfoPointer);

	

		//change to p2 for online
		//update current and last moves
		if (false) {//p1MoveID != currentMove) {
			lastMove = currentMove;
			//currentMove = p1MoveID;

		}


		//autoparry

		if (keyDown(VK_NUMPAD1)) {
			//NOTE: SOMETIMES P1 AND P2 ARE SWITCHED (MAYBE INTERNAL SERVER-SIDE SIDE?), so pass in the side
			autoJinParry(p1InfoPointer, side);
			//autoLeroyParry(p2InfoPointer);
		}

		//autoblock
		if (keyDown(VK_SHIFT)) {

			autoBlockSimple(p2InfoPointer, side);

		}

		//autothrowbreak

		autoThrowBreak(p1InfoPointer, 30, 4);



		//auto-counterhit
		//startup frames should have minimum +3 frames to startup if you dont want to trade (to account for input delay/processing)
		//+2 frames if trades are acceptable

		if (keyDown(VK_NUMPAD2)) {
			counterHitToggle = !counterHitToggle;
			Sleep(300);
		}
		if (counterHitToggle) {

			//printf("p1 move id: %i\n", p1MoveID);

			//change 0x301 to the moving backward move id
			if (false) {//(p2StartupFrames >= 17 && ((int)p1MoveID == 0x8001 || (int)p1MoveID == 1472)) { //23 is ewgf for some reason

				ReleaseKey('A');
				HoldInput("df2", 3, side);
				Sleep(16);

			}
		}

		autoWaveDash();
		autoKBD();

		if (GetAsyncKeyState('F') & 0x8000) {
			df2PEWGF();
		}


		/*
		 * Send all data to the GUI
		 */

		//overlay::p1Frames.store((int)getFrames(p1InfoPointer), std::memory_order_relaxed);

		//overlay::p2Frames.store((int)getFrames(p2InfoPointer), std::memory_order_relaxed);



		//int p1Frames = overlay::p1Frames.load(std::memory_order_relaxed);

		
		//printf("%i\n", p1Frames);

	}



    


}

