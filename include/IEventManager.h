#ifndef _IEVENTMANAGER_H
#define _IEVENTMANAGER_H

enum ASN_INPUT_EVENTS_Types {
ASN_NONE = 0, // NULL
ASN_EVNT_EMPTY, //нет событий
ASN_EVNT_KEYPRESSED, //нажата кнопка
ASN_EVNT_KEYUP, //отжата кнопка
ASN_EVNT_MOUSEMOVE, //мышка сдвинулась
ASN_EVNT_MOUSEDOWN, //нажата кнопка мышки
ASN_EVNT_MOUSEUP, //отжата кнопка мышки
ASN_MOUSE_BUTTON_LEFT, //нажата лева€ кнопка мышки
ASN_MOUSE_BUTTON_RIGHT, //нажата права€ кнопка мышки
ASN_MOUSE_BUTTON_MIDDLE, //нажата средн€€ кнопка мышки
ASN_MOUSE_WHEEL_UP, //колЄсико мышки вертитс€ вверх
ASN_MOUSE_WHEEL_DOWN, //колЄсико мышки вертитс€ вниз
ASN_QUIT, // закрытие Device
ASN_WINDOW_ACTIVE_ON, //окно развернулось
ASN_WINDOW_ACTIVE_OFF, //окно свернулось
ASN_WINDOW_RESIZE, //окно изменило размер
ASN_WINDOW_MOUSE_FOCUS_ON, // курсор мышки в окне
ASN_WINDOW_MOUSE_FOCUS_OFF, // курсор мышки ушел из окона
ASN_WINDOW_INPUT_FOCUS_ON, // ввод в окно
ASN_WINDOW_INPUT_FOCUS_OFF, // окно не в фокусе ввода

ASN_EVENT_NUM
};

#include <string>

/////// from SDL
typedef enum {
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	ASNK_UNKNOWN		= 0,
	ASNK_FIRST		= 0,
	ASNK_BACKSPACE		= 8,
	ASNK_TAB		= 9,
	ASNK_CLEAR		= 12,
	ASNK_RETURN		= 13,
	ASNK_PAUSE		= 19,
	ASNK_ESCAPE		= 27,
	ASNK_SPACE		= 32,
	ASNK_EXCLAIM		= 33,
	ASNK_QUOTEDBL		= 34,
	ASNK_HASH		= 35,
	ASNK_DOLLAR		= 36,
	ASNK_AMPERSAND		= 38,
	ASNK_QUOTE		= 39,
	ASNK_LEFTPAREN		= 40,
	ASNK_RIGHTPAREN		= 41,
	ASNK_ASTERISK		= 42,
	ASNK_PLUS		= 43,
	ASNK_COMMA		= 44,
	ASNK_MINUS		= 45,
	ASNK_PERIOD		= 46,
	ASNK_SLASH		= 47,
	ASNK_0			= 48,
	ASNK_1			= 49,
	ASNK_2			= 50,
	ASNK_3			= 51,
	ASNK_4			= 52,
	ASNK_5			= 53,
	ASNK_6			= 54,
	ASNK_7			= 55,
	ASNK_8			= 56,
	ASNK_9			= 57,
	ASNK_COLON		= 58,
	ASNK_SEMICOLON		= 59,
	ASNK_LESS		= 60,
	ASNK_EQUALS		= 61,
	ASNK_GREATER		= 62,
	ASNK_QUESTION		= 63,
	ASNK_AT			= 64,
	/*
	   Skip uppercase letters
	 */
	ASNK_LEFTBRACKET	= 91,
	ASNK_BACKSLASH		= 92,
	ASNK_RIGHTBRACKET	= 93,
	ASNK_CARET		= 94,
	ASNK_UNDERSCORE		= 95,
	ASNK_BACKQUOTE		= 96,
	ASNK_a			= 97,
	ASNK_b			= 98,
	ASNK_c			= 99,
	ASNK_d			= 100,
	ASNK_e			= 101,
	ASNK_f			= 102,
	ASNK_g			= 103,
	ASNK_h			= 104,
	ASNK_i			= 105,
	ASNK_j			= 106,
	ASNK_k			= 107,
	ASNK_l			= 108,
	ASNK_m			= 109,
	ASNK_n			= 110,
	ASNK_o			= 111,
	ASNK_p			= 112,
	ASNK_q			= 113,
	ASNK_r			= 114,
	ASNK_s			= 115,
	ASNK_t			= 116,
	ASNK_u			= 117,
	ASNK_v			= 118,
	ASNK_w			= 119,
	ASNK_x			= 120,
	ASNK_y			= 121,
	ASNK_z			= 122,
	ASNK_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	ASNK_WORLD_0		= 160,		/* 0xA0 */
	ASNK_WORLD_1		= 161,
	ASNK_WORLD_2		= 162,
	ASNK_WORLD_3		= 163,
	ASNK_WORLD_4		= 164,
	ASNK_WORLD_5		= 165,
	ASNK_WORLD_6		= 166,
	ASNK_WORLD_7		= 167,
	ASNK_WORLD_8		= 168,
	ASNK_WORLD_9		= 169,
	ASNK_WORLD_10		= 170,
	ASNK_WORLD_11		= 171,
	ASNK_WORLD_12		= 172,
	ASNK_WORLD_13		= 173,
	ASNK_WORLD_14		= 174,
	ASNK_WORLD_15		= 175,
	ASNK_WORLD_16		= 176,
	ASNK_WORLD_17		= 177,
	ASNK_WORLD_18		= 178,
	ASNK_WORLD_19		= 179,
	ASNK_WORLD_20		= 180,
	ASNK_WORLD_21		= 181,
	ASNK_WORLD_22		= 182,
	ASNK_WORLD_23		= 183,
	ASNK_WORLD_24		= 184,
	ASNK_WORLD_25		= 185,
	ASNK_WORLD_26		= 186,
	ASNK_WORLD_27		= 187,
	ASNK_WORLD_28		= 188,
	ASNK_WORLD_29		= 189,
	ASNK_WORLD_30		= 190,
	ASNK_WORLD_31		= 191,
	ASNK_WORLD_32		= 192,
	ASNK_WORLD_33		= 193,
	ASNK_WORLD_34		= 194,
	ASNK_WORLD_35		= 195,
	ASNK_WORLD_36		= 196,
	ASNK_WORLD_37		= 197,
	ASNK_WORLD_38		= 198,
	ASNK_WORLD_39		= 199,
	ASNK_WORLD_40		= 200,
	ASNK_WORLD_41		= 201,
	ASNK_WORLD_42		= 202,
	ASNK_WORLD_43		= 203,
	ASNK_WORLD_44		= 204,
	ASNK_WORLD_45		= 205,
	ASNK_WORLD_46		= 206,
	ASNK_WORLD_47		= 207,
	ASNK_WORLD_48		= 208,
	ASNK_WORLD_49		= 209,
	ASNK_WORLD_50		= 210,
	ASNK_WORLD_51		= 211,
	ASNK_WORLD_52		= 212,
	ASNK_WORLD_53		= 213,
	ASNK_WORLD_54		= 214,
	ASNK_WORLD_55		= 215,
	ASNK_WORLD_56		= 216,
	ASNK_WORLD_57		= 217,
	ASNK_WORLD_58		= 218,
	ASNK_WORLD_59		= 219,
	ASNK_WORLD_60		= 220,
	ASNK_WORLD_61		= 221,
	ASNK_WORLD_62		= 222,
	ASNK_WORLD_63		= 223,
	ASNK_WORLD_64		= 224,
	ASNK_WORLD_65		= 225,
	ASNK_WORLD_66		= 226,
	ASNK_WORLD_67		= 227,
	ASNK_WORLD_68		= 228,
	ASNK_WORLD_69		= 229,
	ASNK_WORLD_70		= 230,
	ASNK_WORLD_71		= 231,
	ASNK_WORLD_72		= 232,
	ASNK_WORLD_73		= 233,
	ASNK_WORLD_74		= 234,
	ASNK_WORLD_75		= 235,
	ASNK_WORLD_76		= 236,
	ASNK_WORLD_77		= 237,
	ASNK_WORLD_78		= 238,
	ASNK_WORLD_79		= 239,
	ASNK_WORLD_80		= 240,
	ASNK_WORLD_81		= 241,
	ASNK_WORLD_82		= 242,
	ASNK_WORLD_83		= 243,
	ASNK_WORLD_84		= 244,
	ASNK_WORLD_85		= 245,
	ASNK_WORLD_86		= 246,
	ASNK_WORLD_87		= 247,
	ASNK_WORLD_88		= 248,
	ASNK_WORLD_89		= 249,
	ASNK_WORLD_90		= 250,
	ASNK_WORLD_91		= 251,
	ASNK_WORLD_92		= 252,
	ASNK_WORLD_93		= 253,
	ASNK_WORLD_94		= 254,
	ASNK_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	ASNK_KP0		= 256,
	ASNK_KP1		= 257,
	ASNK_KP2		= 258,
	ASNK_KP3		= 259,
	ASNK_KP4		= 260,
	ASNK_KP5		= 261,
	ASNK_KP6		= 262,
	ASNK_KP7		= 263,
	ASNK_KP8		= 264,
	ASNK_KP9		= 265,
	ASNK_KP_PERIOD		= 266,
	ASNK_KP_DIVIDE		= 267,
	ASNK_KP_MULTIPLY	= 268,
	ASNK_KP_MINUS		= 269,
	ASNK_KP_PLUS		= 270,
	ASNK_KP_ENTER		= 271,
	ASNK_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	ASNK_UP			= 273,
	ASNK_DOWN		= 274,
	ASNK_RIGHT		= 275,
	ASNK_LEFT		= 276,
	ASNK_INSERT		= 277,
	ASNK_HOME		= 278,
	ASNK_END		= 279,
	ASNK_PAGEUP		= 280,
	ASNK_PAGEDOWN		= 281,

	/* Function keys */
	ASNK_F1			= 282,
	ASNK_F2			= 283,
	ASNK_F3			= 284,
	ASNK_F4			= 285,
	ASNK_F5			= 286,
	ASNK_F6			= 287,
	ASNK_F7			= 288,
	ASNK_F8			= 289,
	ASNK_F9			= 290,
	ASNK_F10		= 291,
	ASNK_F11		= 292,
	ASNK_F12		= 293,
	ASNK_F13		= 294,
	ASNK_F14		= 295,
	ASNK_F15		= 296,

	/* Key state modifier keys */
	ASNK_NUMLOCK		= 300,
	ASNK_CAPSLOCK		= 301,
	ASNK_SCROLLOCK		= 302,
	ASNK_RSHIFT		= 303,
	ASNK_LSHIFT		= 304,
	ASNK_RCTRL		= 305,
	ASNK_LCTRL		= 306,
	ASNK_RALT		= 307,
	ASNK_LALT		= 308,
	ASNK_RMETA		= 309,
	ASNK_LMETA		= 310,
	ASNK_LSUPER		= 311,		/* Left "Windows" key */
	ASNK_RSUPER		= 312,		/* Right "Windows" key */
	ASNK_MODE		= 313,		/* "Alt Gr" key */
	ASNK_COMPOSE		= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	ASNK_HELP		= 315,
	ASNK_PRINT		= 316,
	ASNK_SYSREQ		= 317,
	ASNK_BREAK		= 318,
	ASNK_MENU		= 319,
	ASNK_POWER		= 320,		/* Power Macintosh power key */
	ASNK_EURO		= 321,		/* Some european keyboards */
	ASNK_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */

	ASNK_LAST
} ASNK_Key;

/* Enumeration of valid key mods (possibly OR'd together) */
typedef enum {
	ASNM_NONE  = 0x0000,
	ASNM_LSHIFT= 0x0001,
	ASNM_RSHIFT= 0x0002,
	ASNM_LCTRL = 0x0040,
	ASNM_RCTRL = 0x0080,
	ASNM_LALT  = 0x0100,
	ASNM_RALT  = 0x0200,
	ASNM_LMETA = 0x0400,
	ASNM_RMETA = 0x0800,
	ASNM_NUM   = 0x1000,
	ASNM_CAPS  = 0x2000,
	ASNM_MODE  = 0x4000,
	ASNM_RESERVED = 0x8000
} ASNM_Mod;

#define ASNM_CTRL   (ASNM_LCTRL|ASNM_RCTRL)
#define ASNM_SHIFT  (ASNM_LSHIFT|ASNM_RSHIFT)
#define ASNM_ALT    (ASNM_LALT|ASNM_RALT)
#define ASNM_META   (ASNM_LMETA|ASNM_RMETA)

#include "ASN_Types.h"
#include "IEventListener.h"
#include "IManager.h"

class IEventManager: public IManager {
public:
~IEventManager(){printf("IEventManager::~IEventManager()");};

// читаем событи€ из очереди
virtual int GetInput()=0;

virtual IEventListener* AddEventListener( const char *ListenerType )=0;
virtual bool AddCustomEventListener( IEventListener *listener )=0;
virtual bool DelEventListener( UID listenID )=0;

virtual int GetLastKey()=0;
virtual UINT16 GetUnicodeKey()=0;
virtual unsigned short GetKeyModifier()=0;

//int LastKey;
//UINT16 UnicodeKey;			/* translated character */
//unsigned short KeyModifier;         /* Alt, Ctrl, итп */

};

#define EVENTER ((IEventManager*)ENGINE->GetPtrParam("EventManager"))

#endif
