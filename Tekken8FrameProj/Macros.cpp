
#include "Macros.h"
#include "Input.h"

void autoWaveDash() {

	//SendKeyMultiple(3, df2Input, 10);
	if (GetAsyncKeyState('E') & 0x8000) {
		PressKey('D');
		Sleep(4);
		ReleaseKey('D');

		Sleep(14);

		PressKey('S');
		Sleep(4);

		//auto dorya
		if (GetAsyncKeyState('Q') & 0x8000) {
			PressKey('I');
		}
		PressKey('D');
		Sleep(10);

		//dorya
		ReleaseKey('I');

		ReleaseKey('S');
		Sleep(15);
		ReleaseKey('D');
		Sleep(4);
	}
}

void autoKBD() {
	if (GetAsyncKeyState('R') & 0x8000) {
		PressKey('A');

		Sleep(16);
		ReleaseKey('A');
		Sleep(16);


		while (GetAsyncKeyState('R') & 0x8000) {
			PressKey('A');

			Sleep(16);
			Sleep(200);

			PressKey('S');
			Sleep(16);
			ReleaseKey('S');
			Sleep(16);

			Sleep(50);
			ReleaseKey('A');
			Sleep(30);
		}
	}
}

void df2() {
	PressKey('S');
	PressKey('D');
	PressKey('I');
	Sleep(17);
	ReleaseKey('S');
	ReleaseKey('D');
	ReleaseKey('I');
}

void PEWGF() {
	//d
	PressKey('D');
	Sleep(16);

	//n
	ReleaseKey('D');
	Sleep(16);

	//df2

	PressKey('S');
	PressKey('D');
	PressKey('I');
	Sleep(40);
	ReleaseKey('S');
	ReleaseKey('D');
	ReleaseKey('I');

	Sleep(20);
}


void df2PEWGF() {
	//doesn't work, probably because the stun time varies after CH df2 based on various factors?
	PressKey('S');
	PressKey('D');
	PressKey('I');
	Sleep(40);
	ReleaseKey('S');
	ReleaseKey('D');
	ReleaseKey('I');

	Sleep(688);

	PEWGF();
}