#pragma once


//hitProperty enums for high/mid/low/special/unblockable/throwanim
//can use unordered map instead, but for now use enum
/*
non attacking move (default): C000001D, high: A000050F, mid: 8000020A, low: 20000112,
mid unblockable(jin's cancelable charge-up mid unblockable at least) (maybe different for high/mid/low unblockables?): C000071A,
high unblockable (kazuya laser in heat): 2000081B,
special low: 60000402, special mid: 10000302,
during_throw_animations(attacker): 60000A1D
(maybe check to see if any other values are possible for some niche states)
*/
enum HitProperty {
	NONE = 0x0,

	HIGH = 0xA000050F,
	MID = 0x8000020A,
	LOW = 0x20000112,

	UNBLOCKABLE_MID = 0xC000071A,
	UNBLOCKABLE_HIGH = 0x2000081B,
	UNBLOCKABLE_LOW = 0x2000091A,

	SPECIAL_MID = 0x10000302,
	SPECIAL_LOW = 0x60000402,

	DURING_THROW_ATTACKER = 0x60000A1D
};