#ifndef ENUM_H_INCLUDED
#define ENUM_H_INCLUDED

const int NUMLOCALS = 61;
const int ITEMCOUNT = 4;


enum myEDGE
{
	NOEDGE = -1,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

enum TEXT_JUSTIFICATION
{
	LEFT_JUSTIFY = 0,
	RIGHT_JUSTIFY,
	CENTRE_JUSTIFY
};

enum LOCATION_TYPE
{
	NOTSET = 0,
	NMVtoML = 1,	// Numeric Many Value to Many Locations
	UNUSED = 2,	//
	MVtoML = 3,	// Many Value to Many Location, 
	OVtoOL = 4,	// One Value to One Location, not sure it's used
	MCVtoOL = 5	// Many concurrent values in one location, when you need to squeeze multiple objects into one particular area. (Euphoria Mines)
};

enum BOARD_LOCATIONS
{
	MORAL1 = 0,
	MORAL2,
	MORAL3,
	MORAL4,
	MORAL5,
	MORAL6,
	KNOWLEDGE1,
	KNOWLEDGE2,
	KNOWLEDGE3,
	KNOWLEDGE4,
	KNOWLEDGE5,
	KNOWLEDGE6,
	WASTESTARS,
	ICARITESTARS,
	EUPHORIASTARS,
	SUBSTARS,
	EF1,
	EF2,
	WF1,
	WF2,
	SF1,
	SF2,
	IF1U,
	IF2U,
	IF3U,
	WIND_SALON,
	CLOUD_MINE,
	IoHA,
	ETUNNEL,
	EF1U,
	EF2U,
	GENERATOR,
	ETUNNELU,
	AoFM,
	WTUNNEL,
	WF1U,
	WF2U,
	FARM,
	WTUNNELU,
	FPoHR,
	SUBTUNNEL,
	SF1U,
	SF2U,
	AQUIFER,
	SUBTUNNELU,
	WORKER_ACTIVATE,
	E_TRACK,
	S_TRACK,
	W_TRACK,
	I_TRACK,
	ET_TRACK,
	ST_TRACK,
	WT_TRACK,
	MARKET_TILES,
	MARKET_STARS,
	PLAYER_ARTIFACTS,
	PLAYER_DILEMMA,
	PLAYER_WORKERS,
	PLAYER_RESOURCES,
	PLAYER_RECRUITS,
	BLOCKED_LOCATIONS
};

struct myRGB
{
	int Red;
	int Green;
	int Blue;
	bool modify;
};

#endif

