#pragma once

#define USE_GENERIC_MAIN
#undef REMOVE_LEADING_ZERO_FROM_TIME
#undef DEBUG_TIME

#define BG_IMAGE RESOURCE_ID_IMAGE_N7LOGO

#define FONT_NAME RESOURCE_ID_FONT_JUPITER_ALPHABOT_XEN_75

#define CLOCK_POS GRect(0, 40, 144, 168) /* probably taller than really needed */
#define BT_POS GRect(0, 120, 144, 168) /* probably taller than really needed */
#define DATE_POS GRect(0, 140, 144, 168) /* probably taller than really needed */
#define BAT_POS GRect(0, 140, 144, 168) /* probably taller than really needed */

/* for screen shots and font testing
#define DEBUG_TIME
#define DEBUG_TIME_SCREENSHOT
//#define DEBUG_TIME_PAUSE 2000
*/
