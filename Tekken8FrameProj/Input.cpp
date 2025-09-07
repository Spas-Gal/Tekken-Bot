#include "Input.h"

const int msPerFrame = 16;

std::unordered_map<char, WORD> scanCodeMap = {
	// Digits and punctuation from the top row
	{'1', 0x02}, {'2', 0x03}, {'3', 0x04}, {'4', 0x05},
	{'5', 0x06}, {'6', 0x07}, {'7', 0x08}, {'8', 0x09},
	{'9', 0x0A}, {'0', 0x0B}, {'-', 0x0C}, {'=', 0x0D},

	// Letters (uppercase)
	{'Q', 0x10}, {'W', 0x11}, {'E', 0x12}, {'R', 0x13},
	{'T', 0x14}, {'Y', 0x15}, {'U', 0x16}, {'I', 0x17},
	{'O', 0x18}, {'P', 0x19},

	// Brackets
	{'[', 0x1A}, {']', 0x1B},

	// More letters
	{'A', 0x1E}, {'S', 0x1F}, {'D', 0x20}, {'F', 0x21},
	{'G', 0x22}, {'H', 0x23}, {'J', 0x24}, {'K', 0x25},
	{'L', 0x26},

	// Punctuation
	{';', 0x27}, {'\'', 0x28}, {'`', 0x29},
	{'\\', 0x2B},

	// Final row of letters and punctuation
	{'Z', 0x2C}, {'X', 0x2D}, {'C', 0x2E}, {'V', 0x2F},
	{'B', 0x30}, {'N', 0x31}, {'M', 0x32},
	{',', 0x33}, {'.', 0x34}, {'/', 0x35},

	// Keypad asterisk (using '*' character)
	{'*', 0x37},

	// Space character
	{' ', 0x39}
};

std::unordered_map<char, char> notationToInputMapP1 = {

	//convert numbers to attack buttons
	{'1', 'U'}, {'2', 'I'}, {'3', 'J'}, {'4', 'K'},

	//convert upper and lowercase directions to buttons
	{'u', 'W'}, {'b', 'A'}, {'d', 'S'}, {'f', 'D'},

	{'U', 'W'}, {'B', 'A'}, {'D', 'S'}, {'F', 'D'}

	
};


std::unordered_map<char, char> notationToInputMapP2 = {

	//convert numbers to attack buttons
	{'1', 'U'}, {'2', 'I'}, {'3', 'J'}, {'4', 'K'},

	//convert upper and lowercase directions to buttons
	{'u', 'W'}, {'b', 'D'}, {'d', 'S'}, {'f', 'A'},

	{'U', 'W'}, {'B', 'D'}, {'D', 'S'}, {'F', 'A'}


};


// Global hook handle.
HHOOK g_hHook = NULL;

// Define the virtual key codes for the keys you want to block.
// (Using ASCII values for letters, e.g. 'W' == 0x57, etc.)
const std::unordered_set<int> blockKeys = { 'W', 'A', 'S', 'D', 'U', 'I', 'J', 'K' };

// Low-level keyboard hook procedure.
// It intercepts key events and blocks those for which the virtual key code is in our set.
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		KBDLLHOOKSTRUCT* pKbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
		if (blockKeys.find(pKbd->vkCode) != blockKeys.end()) {
			// For key down events, block the key by returning a non-zero value.
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
				return 1;  // Block the key press.
			}
		}
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}


// Simulate pressing a key using its DirectInput scan code.
void PressKey(WORD scanCode) {
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = 0;            // We're not using virtual-key codes.
	input.ki.wScan = scanCodeMap.at(scanCode);   // DirectInput scan code.
	input.ki.dwFlags = KEYEVENTF_SCANCODE; // Specify that we're using scan codes.
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;    // Can be set to a custom value if needed.

	UINT sent = SendInput(1, &input, sizeof(INPUT));
	if (sent != 1) {
		//std::cerr << "Error: Failed to send key press input." << std::endl;
	}
}

// Simulate releasing a key using its DirectInput scan code.
void ReleaseKey(WORD scanCode) {
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = 0;
	input.ki.wScan = scanCodeMap.at(scanCode);
	// Combine KEYEVENTF_SCANCODE with KEYEVENTF_KEYUP to indicate key release.
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	UINT sent = SendInput(1, &input, sizeof(INPUT));
	if (sent != 1) {
		//std::cerr << "Error: Failed to send key release input." << std::endl;
	}
}

/// <summary>
/// Holds down a key for designated number of milliseconds then releases it
/// </summary>
/// <param name="scanCode"></param>
/// <param name="milliseconds"></param>
void HoldKey(WORD scanCode, int milliseconds) {
	INPUT input[2];
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0;            // We're not using virtual-key codes.
	input[0].ki.wScan = scanCodeMap.at(scanCode);   // DirectInput scan code.
	input[0].ki.dwFlags = KEYEVENTF_SCANCODE; // Specify that we're using scan codes.
	input[0].ki.time = 0;
	input[0].ki.dwExtraInfo = 0;    // Can be set to a custom value if needed.

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0;            // We're not using virtual-key codes.
	input[1].ki.wScan = scanCodeMap.at(scanCode);   // DirectInput scan code.
	// Combine KEYEVENTF_SCANCODE with KEYEVENTF_KEYUP to indicate key release.
	input[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	input[1].ki.time = milliseconds;
	input[1].ki.dwExtraInfo = 0;

	SendInput(2, input, sizeof(INPUT));

}




/// <summary>
/// Gets the scanCode of a char
/// </summary>
/// <param name="charValue"> the char to get the scan code of</param>
/// <returns> the scan code value as a WORD</returns>
WORD scanCode(char charValue) {
	return scanCodeMap.at(charValue);
}




void HoldInput(std::string inputString, int frames, int side) {
	
	std::unordered_map<char, char>* notationToInputMapPtr;

	//0 for p1, 1 for p2 
	if (side == 0) {
		notationToInputMapPtr = &notationToInputMapP1;
	}
	else {
		notationToInputMapPtr = &notationToInputMapP2;
	}


	for (int i = 0; i < inputString.length(); i++) {
		char currentChar = inputString.at(i);

		//if the char is in the map
		if ((*notationToInputMapPtr).count(currentChar) > 0) {

			char keyToPress = (*notationToInputMapPtr).at(currentChar);

			//printf("keyToPress: %c", keyToPress);
			PressKey(keyToPress);
		}

		
	}


	//sleepFrames(frames);

	Sleep(frames * msPerFrame);

	for (int i = 0; i < inputString.length(); i++) {
		char currentChar = inputString.at(i);

		//if the char is in the map
		if ((*notationToInputMapPtr).count(currentChar) > 0) {

			char keyToPress = (*notationToInputMapPtr).at(currentChar);
			ReleaseKey(keyToPress);
		}


	}
	

}




bool keyDown(int vKey) {
	// GetAsyncKeyState returns a SHORT value. The high-order bit is set if the key is currently down.
	return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}