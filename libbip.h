/////////////////////////////////////////////////////////////
//
//  FUNCTIONS LIBRARY v.1.3
// for Amazfit Bip
//  (C) Maxim Volkov  2020 <Maxim.N.Volkov@ya.ru>
//
// SDK BipOS
// Library of functions for the BipOS application loader
//
/////////////////////////////////////////////////////////////
/****
v.1.4. - 11.2020
+ added function text_out_font - alias to forgiving function show_big_digit with prototype more appropriate to its functionality (thanks x27)
 the show_big_digit function has been deprecated and will be removed in future versions of the library
* in resource functions such as ElfGetSettingsSize, ElfReadSettings, ElfWriteSettings, show_elf_res_by_id, get_res_count, get_res_params, etc.
 + added new resource source - Font (index_listed == INDEX_FONT_RES)
+ added firmware function int get_battery_charge (), which measures and returns the battery charge in%, as well as two macros
 IS_CHARGE_PLUGGED - returns 0 if the watch is not charging, 1 if charging
 IS_CHARGE_COMPLETE - returns 0 if the watch is not fully charged, 1 if the watch is fully charged and the charger is not disconnected
+ added data structures passed by BipOS to the application when drawing main menu icons

v.1.3. - 12.07.2020 
+ added new firmware features:
 switch_gps_pressure_sensors, get_navi_data, is_gps_fixed - functions for working with GPS and atmospheric pressure
 add_notification, create_and_show_notification - functions for creating notifications (pop-up notifications)
 get_current_timestamp The value of the current UNIX timestamp
* improved functions for working with resources ElfGetSettingsSize, ElfReadSettings, ElfWriteSettings, show_elf_res_by_id, get_res_count, get_res_params, etc.
 ELF_INDEX_SELF can now be passed as an argument to index_listed to specify the application's own index.
+ added Greek language support for BipOS applications

v.1.2 - 06.01.2020
+ added firmware features:
 send_host_app_msg, send_host_app_data - sending messages from watch to phone
 get_ptr_screen_memory, - getting a pointer to the video memory area
 show_elf_res_by_id, get_res_params, - functions for handling clock resources (own and stock)
 getResCount,ReadElfResById
 
* fixed the following functions:
 pvPortMalloc - returns NULL in case of insufficient memory (there was a reload before)
 read_res_by_id - fixed a bug in the implementation, added the ability to read stock resources

* 

v.1.1 - 12.10.2019
+ added firmware features:
 + _rand, _srand (спасибо @Rainberd)
+ added library functions:
 + ElfReadSettings, 
 + ElfWriteSettings, 
 + ElfGetSettingsSize, 
 + show_elf_res_by_id
+ added structure Elf_proc_ (added parameter index_listed)

v.1.0 
- first version of the library

*****/

#ifndef __LIBBIP_H__
#define __LIBBIP_H__

#define LIBBIP_VERSION "1.4"

#define VIDEO_X 176
#define VIDEO_Y 176

#define NULL ((void *)0)

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef abssub
#define abssub(x, y) ((x) > (y) ? (x) - (y) : (y) - (x))
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef offsetof
#define offsetof(s, m) (int)&(((s *)0)->m)
#endif

#ifndef sizeof_member
#define sizeof_member(s, m) ((int)sizeof(((s *)0)->m))
#endif

/* 
Type        Size        Alignment
---------------------------------
char        1 byte      1 byte  
short       2 bytes     2 bytes  
int         4 bytes     4 bytes  
unsigned    4 bytes     4 bytes  
long        4 bytes     4 bytes  
long long   8 bytes     8 bytes  
float       4 bytes     4 bytes  
double      8 bytes     8 bytes  
pointer     4 bytes     4 bytes  
---------------------------------
*/

// screen structure
struct regmenu_
{
  unsigned char curr_scr;  // screen number
  unsigned char swipe_scr; // additional screen number
  unsigned char overlay;   // overlay screen flag
  void *dispatch_func,     // screen gesture processing function
      *key_press,          // side button press function
      *scr_job_func,       // callback function of the screen refresh timer
      *scr_job_buff,       // variable for the screen refresh callback
      *show_menu_func,     // screen shaping function
      *show_menu_buff,     // variable for the display function
      *long_key_press;     // long press side button handling function
};

// the structure of the gesture when processing clicks on the screen and side button
struct gesture_
{
  char gesture;    // type gesture
  int touch_pos_x, // click coordinates  X
      touch_pos_y; //      Y
};

// type gesture
#define GESTURE_CLICK 1       // simple tap on the screen, the coordinates of the tap in touch_pos_x variables touch_pos_y
#define GESTURE_SWIPE_UP 2    // swipe from bottom to top
#define GESTURE_SWIPE_DOWN 3  // swipe from top to bottom
#define GESTURE_SWIPE_LEFT 4  // swipe from right to left
#define GESTURE_SWIPE_RIGHT 5 // swipe from left to right

// date time structure
struct datetime_
{
  unsigned short year;
  unsigned char month,
      day,
      hour,
      min,
      sec,
      weekday;
  unsigned char h24;
};

// animation type for show_menu_animate function
#define ANIMATE_LEFT 0  // animation of the screen transition from right to left
#define ANIMATE_RIGHT 1 // animation of the screen transition from left to right
#define ANIMATE_UP 2    // animation of the screen transition from bottom to top
#define ANIMATE_DOWN 3  // top-down screen transition animation

// colors
#define COLOR_BLACK 0x000000  // the black
#define COLOR_RED 0x0000FF    // red
#define COLOR_GREEN 0x00FF00  // green
#define COLOR_BLUE 0xFF0000   // blue
#define COLOR_YELLOW 0x00FFFF // yellow
#define COLOR_AQUA 0xFFFF00   // aquamarine
#define COLOR_PURPLE 0xFF00FF // purple
#define COLOR_WHITE 0xFFFFFF  // white
#define COLOR_MASK 0xFFFFFF   // color mask

// colors in short format
#define COLOR_SH_BLACK 0b0000  // the black
#define COLOR_SH_RED 0b0001    // red
#define COLOR_SH_GREEN 0b0010  // green
#define COLOR_SH_BLUE 0b0100   // blue
#define COLOR_SH_YELLOW 0b0011 // yellow
#define COLOR_SH_AQUA 0b0110   // aquamarine
#define COLOR_SH_PURPLE 0b0101 // purple
#define COLOR_SH_WHITE 0b0111  // white
#define COLOR_SH_MASK 0b1111   // color mask

// languages of text (locales)
#define locale_ru_RU 1049 // Russian
#define locale_en_US 1033 // English
#define locale_it_IT 1040 // Italian
#define locale_es_ES 3082 // Spanish
#define locale_fr_FR 1036 // French
#define locale_de_DE 1031 // German
#define locale_el_GR 1032 // Greek

// the language set for the mod menu, returned by get_stored_lang
#define OPT_LANG_COUNT 7   // number of languages
#define OPT_LANG_AUTO 0x00 // auto select by phone locale
#define OPT_LANG_RUS 0x01  // Russian
#define OPT_LANG_ENG 0x02  // English
#define OPT_LANG_ITA 0x03  // Italian
#define OPT_LANG_SPA 0x04  // Spanish
#define OPT_LANG_FRA 0x05  // French
#define OPT_LANG_DEU 0x06  // German
#define OPT_LANG_GRK 0x07  // Greek

// system settings data (function get_generic_data / set_generic_data)
#define GENERIC_DATA_LOCALE 36 // phone locale

//  menu structure
// settings menu
#define MAX_MENU_ITEM_NAME_LEN 64
#define MAX_MENU_ENCLOSURE 15 // maximum menu nesting
#define ITEMS_ON_PAGE 3
#define MAX_MENU_ITEMS_COUNT 55
#define MAX_LINES_PER_ITEM 3
// menu item display options
#define MENU_H_MARGIN 10

// menu item statuses
#define MENU_ITEM_STYLE_NORMAL 0     // the usual menu item is the ">" icon. Run procedure via show_animate with return function parameter
#define MENU_ITEM_STYLE_LIST 1       // the usual menu item is the ">" icon. Running a procedure with the menu and index parameters
#define MENU_ITEM_STYLE_UNCHECKED 2  // the checkbox is not marked with a "0" icon. Running a procedure with the menu and index parameters
#define MENU_ITEM_STYLE_CHECKED 3    // the checkbox is marked with a "v". Running a procedure with the menu and index parameters
#define MENU_ITEM_STYLE_TOGGLE_ON 4  // switch is turned on icon "( o)". Running a procedure with the menu and index parameters
#define MENU_ITEM_STYLE_TOGGLE_OFF 5 // switch is turned off icon "(o )". Running a procedure with the menu and index parameters
#define MENU_ITEM_STYLE_LOCKED 6     // locked icon "padlock".
#define MENU_ITEM_STYLE_DISABLED 7   // item is currently not available "X" icon

struct menu_item_struct
{
  char name[MAX_MENU_ITEM_NAME_LEN + 1]; // menu item name
  void *show_func;                       // launch function
  int item_color;                        // menu item color
  int item_style;                        // menu bar status
  int item_param0;                       // arbitrary parameter of a menu item
};

struct menu_struct
{
  struct menu_item_struct items[MAX_MENU_ITEMS_COUNT]; // menu structure
  unsigned char page;                                  // number of the currently displayed page
  unsigned char item_count;                            // number of menu items
  void *show_func;                                     // address of the current screen display function
  void *ret_f[MAX_MENU_ENCLOSURE];                     // return to previous screen pointer
  int menu_level;                                      // current nesting level
};

#pragma pack(push, 1) // disable alignment of structure fields
struct res_struct
{                           // sizeof=32+4
  char res_type[5];         // NERES, HMRES
  char version;             //
  char magic[4];            // E8 09 1A D7 = 0xD71A09E8
  char reserved_0[4];       // 03 00 00 00
  char reserved_1[2];       // FF FF
  unsigned int reserved[4]; //  FF..FF
  unsigned int count;       //
};
#pragma pack(pop)

#define RES_MAGIC 0xD71A09E8
#define NERES_ADDRESS 0x2A0000

// graphics resource options
#pragma pack(push, 1) // disable alignment of structure fields
struct res_params_
{
  short width;  // width in px
  short height; // height in px
};
#pragma pack(push, 1) // disable alignment of structure fields

#define INDEX_MAIN_RES ((int)0xFFFF0000)
#define INDEX_FONT_RES ((int)0xFFFF0001)
#define ELF_INDEX_SELF ((int)0xFFFFFFFF)

typedef struct
{                                                // structure of the running process
  unsigned int process_id;                       // process id, assigned by the loader
  int index_listed;                              // index of the elf in the bootloader list
  void *base;                                    // pointer to the memory allocated for the process
  unsigned int size;                             // the amount of memory allocated for the process
  void *ret_f;                                   // process return point
  unsigned int ret_param0;                       // return function parameter
  void (*elf_finish)(void *param0);              // a pointer to the elf termination routine
  int (*entry_point)(void *param0, char **argv); // pointer to the elf startup procedure, entry point.Param0 = pointer to the proc structure of the running process
  void *param;                                   // custom parameter, can store anything like pointers instead of temp_buf_2 for background processes
  int argc;                                      // the number of parameters when starting the elf
  char **argv;                                   // parameters when starting elf
} Elf_proc_;

// firmware versions
#define LATIN_1_1_5_12 11512
#define LATIN_1_1_5_16 11516
#define LATIN_1_1_5_36 11536
#define LATIN_1_1_5_56 11556
#define NOT_LATIN_1_1_2_05 11205
#define UNI_LATIN 55555

// function statuses get_app_state
#define APP_STATE_BT_CON 0x200

// data structure containing alarm information
struct alarm_struct
{
  unsigned char index;   // bit_7 - enabled / disabled;bit_0..4 - alarm clock number
  unsigned char hours;   // clock
  unsigned char minutes; // minutes
  unsigned char repeat;  // bit_0 - bit_7 - days of the week from Monday to Friday
                         // value 128 - once;127 - every day;
                         // 31 - for workers;0 - not created;60 - on weekends
};

// data structure containing information about the number of the selected page and data of 10 alarms
struct alarm_10
{
  unsigned char page;             // current page setting alarms
  unsigned char field_0;          // unknown structure field, possibly not used
  struct alarm_struct alarms[10]; // array of 10 alarm clocks
};

// heart sensor data structures
// 1.1.5.12, 1.1.5.36
typedef struct
{
  int t_start;
  short last_hr;
  unsigned char field_2;
  unsigned char field_3;
  unsigned char field_4;
  unsigned char heart_rate; // frequency, beats / min;> 200 - value not available
  unsigned char ret_code;   // measurement status 0 - finished,> 0 measurement not finished
  unsigned char field_5;
} hrm_data_struct;

// 1.1.2.05
typedef struct
{
  int t_start;
  short last_hr;
  unsigned char heart_rate; // frequency, beats / min;> 200 - value not available
  unsigned char ret_code;   // measurement status 0 - finished,> 0 measurement not finished
} hrm_data_struct_legacy;

// navigation data (for the get_navi_data function)
typedef struct
{
  int ready;             // Data readiness: bit 0: pressure;bit 1: height;bit 2: latitude;bit 3: longitude
  unsigned int pressure; // pressure value in Pascals
  float altitude;        // height value in meters
  signed int latitude;   // modulus of longitude value in degrees multiplied by 3,000,000
  int ns;                // ns: 0-northern hemisphere;1-south
  signed int longitude;  // modulus of the value of longitude in degrees multiplied by 3,000,000
  int ew;                // ew: 2-western hemisphere;3-east;
} navi_struct_;

// hemispheres
#define NAVI_NORTH_HEMISPHERE 0 // North hemisphere
#define NAVI_SOUTH_HEMISPHERE 1 // Southern Hemisphere
#define NAVI_WEST_HEMISPHERE 2  // western hemisphere
#define NAVI_EAST_HEMISPHERE 3  // lobular hemisphere

// macros for checking the ready value
#define IS_NAVI_PRESS_READY(navi) (navi & 0x01)    // barometric pressure data readiness: 0 - not ready, 1 - ready
#define IS_NAVI_GPS_READY(navi) (navi & 0x0E)      // readiness coordinates: 0 - not ready, 1 - ready
#define IS_NAVI_GPS_ALT_READY(navi) (navi & 0x02)  // Altitude Data Ready (GPS): 0 - not ready, 1 - ready
#define IS_NAVI_GPS_LAT_READY(navi) (navi & 0x04)  // latitude data ready (GPS): 0 - not ready, 1 - ready
#define IS_NAVI_GPS_LONG_READY(navi) (navi & 0x08) // longitude data ready (GPS): 0 - not ready, 1 - ready

// usually the altitude, latitude and longitude data is ready when the GPS receiver is locked, so it is enough to check
// GPS ready status - IS_NAVI_GPS_READY (navi)

// Types of notifications
#define NOTIFY_TYPE_NONE 0
#define NOTIFY_TYPE_SMS 5
#define NOTIFY_TYPE_EMAIL 6
#define NOTIFY_TYPE_MICHAT 7
#define NOTIFY_TYPE_FACEBOOK 8
#define NOTIFY_TYPE_TWITTER 9
#define NOTIFY_TYPE_MI 10
#define NOTIFY_TYPE_WHATSAPP 12
#define NOTIFY_TYPE_ALARM 16
#define NOTIFY_TYPE_INSTAGRAM 18
#define NOTIFY_TYPE_ALIPAY 22
#define NOTIFY_TYPE_CALENDAR 27
#define NOTIFY_TYPE_VIBER 29
#define NOTIFY_TYPE_TELEGRAM 31
#define NOTIFY_TYPE_SKYPE 33
#define NOTIFY_TYPE_VK 34
#define NOTIFY_TYPE_CALL 39
#define NOTIFY_TYPE_LOW_BAT 42

// font type for text_out_font function
#define FONT_DIGIT_LED_0 0
#define FONT_DIGIT_SQUARE_MINI_1 1
#define FONT_DIGIT_MIDDLE_2 2
#define FONT_DIGIT_BIG_3 3
#define FONT_DIGIT_BIG_RED_4 4
#define FONT_LETTER_MIDDLE_5 5
#define FONT_LETTER_BIG_6 6
#define FONT_DIGIT_ROUND_BIG_7 7
#define FONT_DIGIT_MIDDLE_8 8
#define FONT_DIGIT_BIG_9 9
#define FONT_FONT_DIGIT_SQUARE_MIDDLE_10 10
#define FONT_DIGIT_SMALL_11 11
#define FONT_DIGIT_TINY_12 12
#define FONT_DIGIT_MICRO_13 13
#define FONT_DIGIT_TINY_14 14
#define FONT_DIGIT_SQUARE_MICRO_15 15

// storage structure for resource numbers when passed as parameters
struct icons_
{
  short icon_main;
  short icon_label_ru;
  short icon_label_en;
};

// structure of rendering of main menu icons
struct menu_items_draw_
{
  struct regmenu_ *regmenu; // pointer to the structure of the new element
  int prev_screen;          // previous screen
  int prev_sscreen;         // previous subscreen
  int screen;               // current subscreen
  struct icons_ icon_res;   // pointer to resource numbers of menu icons
};

// main menu icons
#define MAIN_MENU_STATUS 2
#define MAIN_MENU_ACTIVITY 3
#define MAIN_MENU_WEATHER 4
#define MAIN_MENU_ALARM 5
#define MAIN_MENU_TIMER 6
#define MAIN_MENU_COMPASS 7
#define MAIN_MENU_OPTIONS 8
#define MAIN_MENU_ALIPAY 9

// Global Variables
unsigned char get_var_current_screen();                              // active screen number
unsigned char set_var_current_screen(unsigned char val);             //
unsigned char get_var_swipe_screen_active_number();                  // active subnumber of the screen
unsigned char set_var_swipe_screen_active_number(unsigned char val); //
unsigned char get_var_menu_overlay();                                // active overlay screen (notification, incoming call, etc.)
unsigned char set_var_menu_overlay(unsigned char val);               //
void *get_ptr_menu_dispatch_func();                                  // pointer to the touchscreen handler and the right click of the button
void *set_ptr_menu_dispatch_func(void *val);                         //
void *get_ptr_key_press_func();                                      // pointer to the button click handler
void *set_ptr_key_press_func(void *val);                             //
void *get_ptr_screen_job_func();                                     // pointer to screen timer handler
void *set_ptr_screen_job_func(void *val);                            //
void *get_ptr_temp_buf_1();                                          // pointer to screen data (lost when the overlay screen appears)
void *get_ptr_show_menu_func();                                      // pointer to the function for creating the current screen
void *set_ptr_show_menu_func(void *val);                             //
void *get_ptr_temp_buf_2();                                          // pointer to screen data (not lost when the overlay screen appears)
void *get_ptr_long_key_press_func();                                 // pointer to the handler for a long press on the button (does not work on all screens)
void *set_ptr_long_key_press_func(void *val);                        //
unsigned char get_var_left_side_menu_active();                       // variable value 1-call through the left menu;0-via main menu
int get_var_free_ram();                                              // amount of free memory
void *get_ptr_screen_memory();                                       // pointer to screen memory
void *get_ptr_free_list();                                           // pointer to a tree of free memory blocks

// Firmware functions
extern void reg_menu(void *regmenu_, int param);                                // register the created screen in the operating system
extern int show_menu_animate(void *show_menu_function, int param, int animate); // display a menu with screen shift animation, param is a parameter passed to the show_menu_function function
extern int show_menu(void *show_menu_function, int param);                      // display the menu without animation, param is a parameter passed to the show_menu_function function
extern int text_width(const char *text);                                        // returns the width of the text that will be displayed in the current font
extern void text_out(const char *text, int pos_x, int pos_y);                   // text output by coordinates, pos_x, pos_y coordinates of the upper left corner of the label
extern void text_out_center(const char *text, int pos_center_x, int pos_y);     // text output by coordinates, pos_x, pos_y coordinates of the middle of the upper edge of the label
extern int get_text_height();                                                   // returns the height of the text, current font
extern int get_last_text_pos_x();                                               // returns the X coordinate immediately after the last displayed text
                                                                                //
extern void set_bg_color(long color);                                           // select a background color for subsequent graphics operations
extern void set_fg_color(long color);                                           // color selection for subsequent graphic operations
extern void fill_screen_bg();                                                   // fill the screen with background color
extern int set_graph_callback_to_ram_1();                                       // this function must be used according to the example
extern void repaint_screen_lines(int from, int to);                             // update lines on screen (copy from video memory to display)
extern void repaint_screen();                                                   // refresh the whole screen ((copy from video memory to display))
extern void draw_horizontal_line(int pos_y, int from_x, int to_x);              // draw a horizontal line
extern void draw_rect(int from_x, int from_y, int to_x, int to_y);              // draw rectangle
extern void draw_vertical_line(int pos_x, int from_y, int to_y);                // draw the vertical line
extern void draw_filled_rect(int from_x, int from_y, int to_x, int to_y);       // draw the filled rectangle
extern void draw_filled_rect_bg(int from_x, int from_y, int to_x, int to_y);    // draw the background colored rectangle
extern void draw_rect_or_line(int from_x, int from_y, int to_x, int to_y);      // draw rectangle ???
                                                                                //
extern void load_font();                                                        // load font
extern int get_res_ret_language();                                              // get the clock interface language
extern void show_res_by_id(int res_ID, int pos_x, int pos_y);                   // display a graphic resource by its number
                                                                                //
extern int _sprintf(char *buf, const char *format, ...);                        // firmware built-in sprintf
                                                                                //
extern int _strlen(const char *str);                                            // firmware built-in strlen function
extern int _strcmp(const char *string1, const char *string2);                   // firmware built-in strcmp function
extern int _strncmp(const char *string1, const char *string2, int num);         // firmware built-in strncmp function
extern int _strcpy(char *destptr, const char *srcptr);                          // strcpy function built into the firmware
extern int _strncpy(char *destptr, const char *srcptr, int num);                // firmware built-in strncpy function
                                                                                //
extern int _memclr(void *buf, int len);                                         // built-in memclr function
extern int _memset(void *buf, int len, int val);                                // firmware built-in memset function
extern int _memcpy(void *dest, const void *srcptr, int num);                    // built-in memcpy function
extern int _memcmp(const void *p1, const void *p2, int size);                   // built-in memcmp function

extern int get_tick_count();                                // returns the number of ticks of the system timer since reboot (approximately 510 ticks per second)
extern int set_update_period(int enable, int period);       // starts the timer for the current screen.timer callback screen_job_func;cmd = 0 stop the timer, cmd = 1 set the timer for the number of ms equal to period
extern int set_display_state_value(int state_1, int state); // setting screen parameters
extern int set_close_timer(int delay_s);                    //

extern int log_printf(int debug_level, const char *format, ...); // writing to the device log, debug_level = 5
extern int vibrate(int count, int on_ms, int off_ms);            // vibration (background)

extern void *_pvPortMalloc(int size);                         // memory allocation firmware function
extern void vPortFree(void *pv);                              // free allocated memory
                                                              //
extern int get_left_side_menu_active();                       // returns the left_side_menu_active variable
                                                              //
extern int get_current_date_time(struct datetime_ *datetime); // returns data about the current date / time
extern int get_current_timestamp();                           // returns the value of the current UNIX timestamp

extern int show_watchface(); // dial display procedure
extern void __attribute__((deprecated("show_big_digit is deprecated use text_out_font instead")))
show_big_digit(int color, const char *digits, int pos_x, unsigned int pos_y, int space);         // display numbers in large print
extern void text_out_font(int font, const char *text, int pos_x, unsigned int pos_y, int space); // displaying numbers in an alternative font (alias to the show_big_digit function), for examples of fonts, see https://github.com/freebip/fwhack/blob/master/show_big_digit.md
extern void vTaskDelay(int delay_ms);                                                            // suspend the current process for a time of ms
                                                                                                 //
extern int read_flash(int addr, void *data, int size);                                           // read from flash memory
extern int write_flash(int addr, void *data, int size);                                          // write to flash memory (work neatly)
                                                                                                 //
extern int get_generic_data(int info, void *buffer);                                             // receiving clock settings data
extern int set_generic_data(int info, void *buffer);                                             // recording clock setting data
                                                                                                 //
extern int set_backlight_state(int state);                                                       // display backlight on / off
extern int set_backlight_percent(int percent);                                                   // setting the display brightness in% (from a range of values 0, 10, 20, 40, 60, 90)
extern int get_backlight_value();                                                                //

// random numbers
extern int _rand();                    // getting a random number
extern void _srand(unsigned int seed); // random number generator initialization

// telephone communication protocol
extern void send_host_app_msg(int msg);
extern void send_host_app_data(short a1, short channel, unsigned int size, void *data, int a2);

// system status check
extern int check_app_state(long long state); // system status check

extern int dispatch_left_side_menu(struct gesture_ *gest); // parsing quick menu swipes

extern void stm32_soft_reset(); // software restart, turning on the clock with a long press
extern int iwdg_reboot();       // software restart of the clock, the clock turns on

extern unsigned char get_last_heartrate(); // receiving the last measured value from the heart rate sensor
extern int set_hrm_mode(int hrm_mode);     // setting the pulse measurement mode 0x20 - single
extern void *get_hrm_struct();             // getting a pointer to heart sensor data

// Navigation functions
extern void switch_gps_pressure_sensors(int mode);           // enable / disable GPS sensors and barometer
extern navi_struct_ *get_navi_data(navi_struct_ *navi_data); // receiving GPS and barometer data
extern int is_gps_fixed();                                   // GPS readiness check

// Functions for creating notifications (pop-up notifications)
extern int add_notification(int notif_type, int timestamp, char *title, char *msg, char *app_name); // creating and adding a new notification to the notification list
extern int create_and_show_notification(int notif_type, char *title, char *msg, char *app_name);    // create and display the created notification (remains in the list)

// Battery check function
int get_battery_charge();                                           // returns battery charge in %
#define IS_CHARGE_PLUGGED get_app_state(APP_STATE_CHARGE_PLUGGED)   // macro, returns 0 if the watch is not charging, 1 if it is charging
#define IS_CHARGE_COMPLETE get_app_state(APP_STATE_CHARGE_COMPLETE) // macro, returns 0 if the watch is not fully charged, 1 if the watch is fully charged and the charger is not disconnected

// Library functions
// working with language
extern int get_selected_locale();         // getting the locale based on the selected mod menu language
extern int get_stored_lang();             // getting the menu language selected in the mod menu
extern int store_selected_lang(int lang); // write menu language value mod
extern int get_system_locale();           // getting the system locale

// other functions
// drawing progress bar in the middle of the screen
extern void draw_progressbar(int pos_y, int width, int height, int color_bg, int color_fg, int progress_max, int progress, int border, int repaint, const char *text);
extern void dump_mem(void *address, int len); // memory dump to the device log in ti-txt format
extern int get_res_count();                   // getting the amount of resources
extern void *pvPortMalloc(int size);          // memory allocation library function (recommended instead of _pvPortMalloc)

// menu
extern int add_menu_item(struct menu_struct *menu, char *name, void *show_func, int color, int style); // adding a menu item
extern int clear_menu(struct menu_struct *menu);                                                       // clear custom menu
extern void draw_menu(struct menu_struct *menu);                                                       // display custom menu
extern int dispatch_menu(struct menu_struct *menu, void *param);                                       // custom menu handler
extern int push_ret_f(struct menu_struct *menu, void *ret_f);                                          // saving the function to return to the previous menu item in the menu chain stack
extern void *pop_ret_f(struct menu_struct *menu);                                                      // popping the return to previous menu item from the menu chain stack

// bootloader functions
extern int load_elf_by_index(int index_listed, void *ret_f, unsigned int ret_param0, int argc, void **argv); // loading and launching an application by its index
extern int load_elf_by_name(char *name, void *ret_f, unsigned int ret_param0, int argc, void **argv);        // loading and launching an application by its name
extern void *init_elf_loader(int force_scan);                                                                // initialization of the application loader.force_scan - force scan of resources in search of elves
extern Elf_proc_ *get_proc_by_addr(void *addr);                                                              // returns a pointer to the structure of the running process, calculating by the character address
extern void elf_finish(void *addr);                                                                          // application termination function, the address of the main application screen display function must be passed as a parameter
extern int ElfReadSettings(int index_listed, void *buffer, int offset, int len);                             // reading the settings section of a particular elf
extern int ElfWriteSettings(int index_listed, void *buffer, int offset, int len);                            // writing a section of settings for a specific elf
extern int ElfGetSettingsSize(int index_listed);                                                             // returns the size of the settings section .elf.settings stored in resources
extern int show_elf_res_by_id(int index_listed, int res_id, int pos_x, int pos_y);                           // displays on the screen the graphic resource of a specific elf contained in the .elf.resources section
                                                                                                             // resource numbering starts at 0 for each elf
extern int get_res_params(int index_listed, int res_id, struct res_params_ *res_params);                     // returns a res_params_ structure with the dimensions of the graphic resource
extern int get_res_count(int index_listed);                                                                  // returns the number of available resources in the source
extern int read_res_by_id(int res_id, int offset, void *buffer, int len);                                    // reading the data of a standard resource starting at offset len length by its res_id number
extern int read_elf_res_by_id(int index_listed, int res_id, int offset, void *buffer, int len);              // reading resource data (standard and application) starting at offset len length by its res_id number
extern int get_fw_version();                                                                                 // returns the firmware version

// resources
#define BIG_COLON 573
#define ICON_RECYCLE 293
#define ICON_ALARM 227
#define ICON_CALEND 228
#define ARROW_UP 318
#define ARROW_DOWN 316
#define CHECK_BOX_ON 74
#define CHECK_BOX_OFF 73
#define PAGE_UP_ARROW 408
#define PAGE_DOWN_ARROW 407
#define ICON_CANCEL_RED 416
#define ICON_OK_GREEN 417
#define MENU_ITEM_ARROW 72
#define ICON_DISABLED 75
#define ICON_TOGGLE_ON 77
#define ICON_TOGGLE_OFF 76
#define MENU_ITEM_LOCKED 103
#define ICON_SUNNY 71

#endif
