/*
** Jupiter Mass watch face - Color or Mono
**
** Acknowledgements
**   * This is only a slightly modified version of Pebble Watchface tutorial
**       * http://developer.getpebble.com/getting-started/watchface-tutorial/part2/
**       * https://github.com/pebble-examples/classio-battery-connection/
**   * Ideas for additional extras from
**       * http://clintkprojects.blogspot.com/2014/11/pebble-watchface-saving-battery-life.html
**   * Font ISL Jupiter by Isurus Labs
**       * http://www.dafont.com/isl-jupiter.font
**   * Logo Vector by Karlika
**       * http://karlika.deviantart.com/art/Mass-Effect-Vector-Pack-178422616
**
** TODO
**   * Add Bluetooth status, connect/disconnected is easy.
**     What about Bluetooth off (airplane mode)?
**     Show icons (rather than text)?
**       * battery
**           * https://github.com/Remper/Simplicity2/blob/master/src/simplicity2.c
**           * https://github.com/tkafka/ModernExtra20/tree/master/resources/images
**           * https://github.com/foxel/pebble-simplef/tree/master/resources/images
**       * BT https://github.com/drwrose/rosewright/tree/master/resources
**   * Add shake/tap to display seconds
*/

#include <pebble.h>

    
//#define BG_IMAGE RESOURCE_ID_IMAGE_N7LOGO_BLACK
//#define BG_IMAGE RESOURCE_ID_IMAGE_N7LOGO_WHITE
#define BG_IMAGE RESOURCE_ID_IMAGE_N7LOGO

/* For colors, see http://developer.getpebble.com/tools/color-picker/#0000FF */
//#define FONT_COLOR GColorBlack
//#define FONT_COLOR COLOR_FALLBACK(GColorBlue, GColorBlack)
//#define FONT_COLOR GColorWhite
#define FONT_COLOR COLOR_FALLBACK(GColorBlue, GColorWhite)

#define FONT_BAT_COLOR GColorWhite

#define FONT_NAME RESOURCE_ID_FONT_JUPITER_60

#define CLOCK_POS GRect(0, 52, 144, 168) /* probably taller than really needed */
#define DATE_POS GRect(0, 140, 144, 168) /* probably taller than really needed */
#define BAT_POS GRect(0, 140, 144, 168) /* probably taller than really needed */


static Window    *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_battery_layer;

static GFont       s_time_font;
static GFont       s_date_font;
static BitmapLayer *s_background_layer;
static GBitmap     *s_background_bitmap;

static int last_day = -1;


#define MAX_DATE_STR "Thu, 00 Aug"
#define MAX_TIME_STR "00:00"
#define MAX_BAT_STR "Bat: ??%"


static void handle_battery(BatteryChargeState charge_state) {
    static char battery_text[] = MAX_BAT_STR;

    if (charge_state.is_charging) {
        snprintf(battery_text, sizeof(battery_text), "Charging");
        text_layer_set_text_color(s_battery_layer, COLOR_FALLBACK(GColorGreen, FONT_BAT_COLOR));
    } else {
        snprintf(battery_text, sizeof(battery_text), "Bat: %d%%", charge_state.charge_percent);
#ifdef PBL_PLATFORM_BASALT
        /* TODO Check charge level and change color? E.g. red at 10%/20% */
        if (charge_state.charge_percent <= 20)
        {
            text_layer_set_text_color(s_battery_layer, GColorRed);
        }
        else /* TODO different colors for different ranges */
        {
            /* TODO is this an expensive call ? */
            text_layer_set_text_color(s_battery_layer, FONT_BAT_COLOR);
        }
#endif
    }
    text_layer_set_text(s_battery_layer, battery_text);
}

static void update_date(struct tm *tick_time) {
    static char buffer[] = MAX_DATE_STR;  /* FIXME use same buffer one for date and time? */

    last_day = tick_time->tm_mday;
    strftime(buffer, sizeof(buffer), "%a, %d %b", tick_time); /* TODO review %d for day */
    text_layer_set_text(s_date_layer, buffer);
}

static void update_time() {
    // Get a tm structure
    time_t    temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    // Create a long-lived buffer
    static char buffer[] = MAX_TIME_STR;

    // Write the current hours and minutes into the buffer
    if(clock_is_24h_style() == true) {
        // 2h hour format
        strftime(buffer, sizeof(buffer), "%H:%M", tick_time);
    } else {
        // 12 hour format
        strftime(buffer, sizeof(buffer), "%I:%M", tick_time);
    }

    /* Update the date only when the day changes */
    if (last_day != tick_time->tm_mday)
    {
        update_date(tick_time);
    }

    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
    // Create GBitmap, then set to created BitmapLayer
    s_background_bitmap = gbitmap_create_with_resource(BG_IMAGE);
    
    s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

    // Create time TextLayer
    s_time_layer = text_layer_create(CLOCK_POS);
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, FONT_COLOR);
    text_layer_set_text(s_time_layer, "00:00");

    // Create GFont
    s_time_font = fonts_load_custom_font(resource_get_handle(FONT_NAME));

    // Apply to TextLayer
    text_layer_set_font(s_time_layer, s_time_font);
    /* Consider GTextAlignmentLeft (with monospaced font) in cases where colon is proportional */
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

    /* Create date TextLayer */
    s_date_layer = text_layer_create(DATE_POS);
    text_layer_set_background_color(s_date_layer, GColorClear);
    text_layer_set_text_color(s_date_layer, FONT_COLOR);
    text_layer_set_text(s_date_layer, MAX_DATE_STR);

    /* Create GFont */
    s_date_font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

    /* Apply to TextLayer */
    text_layer_set_font(s_date_layer, s_date_font);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));

    /* Battery level */
    s_battery_layer = text_layer_create(BAT_POS);
    text_layer_set_text_color(s_battery_layer, FONT_BAT_COLOR);
    text_layer_set_background_color(s_battery_layer, GColorClear);
    text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(s_battery_layer, GTextAlignmentLeft);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
    text_layer_set_text(s_battery_layer, MAX_BAT_STR);

    /* Make sure the time is displayed from the start */
    update_time();
    /* Ensure battery status is displayed from the start */
    handle_battery(battery_state_service_peek());
}

static void main_window_unload(Window *window) {
    /* Unload GFonts */
    fonts_unload_custom_font(s_time_font);

    /* Destroy GBitmap */
    gbitmap_destroy(s_background_bitmap);

    /* Destroy BitmapLayer */
    bitmap_layer_destroy(s_background_layer);

    /* Destroy TextLayers */
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_date_layer);
    text_layer_destroy(s_battery_layer);


    /* unsubscribe events */
    tick_timer_service_unsubscribe();
    battery_state_service_unsubscribe();
    /*
    bluetooth_connection_service_unsubscribe();
    */
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void init() {
    // Create main Window element and assign to pointer
    s_main_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
                                   .load = main_window_load,
                                   .unload = main_window_unload
                               });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

    /* Register events; TickTimerService, Battery */
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    battery_state_service_subscribe(handle_battery);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
