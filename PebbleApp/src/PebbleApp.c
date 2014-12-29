#include <pebble.h>


static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_message_layer;
// static GFont s_time_font;
// static GFont s_date_font;

// static BitmapLayer *s_background_layer;
// static GBitmap *s_background_bitmap;
static char messageBuffer[120];
enum {
  KEY_MESSAGE = 0,
  KEY_JUNK = 1,
};


// JS and APP
void send_int(uint8_t key, uint8_t cmd)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);

    app_message_outbox_send();
}

void process_tuple(Tuple *t)
{
  //Get key
  int key = t->key;

  //Get integer value, if present
  //int value = t->value->int32;
  //char source_value[20];
  //strcpy(source_value,t->value->cstring);
  //Get string value, if present
  char string_value[120];
  strcpy(string_value, t->value->cstring);
APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Received ");
APP_LOG(APP_LOG_LEVEL_DEBUG, string_value);
  //Decide what to do

  switch(key) {
    case KEY_MESSAGE:
      //quote received
      snprintf(messageBuffer, 120, "%s", string_value);
      text_layer_set_text(s_message_layer, (char*) &messageBuffer);
      break;
    case KEY_JUNK:
      //tag received

      //snprintf(meta_buffer, 28, "Source: %s", source_value);
      //text_layer_set_text(meta_layer, (char*) &meta_buffer);
      break;
  }

  //Set time this update came in
  //time_t temp = time(NULL);
  //struct tm *tm = localtime(&temp);
  //strftime(time_buffer, sizeof("Last updated: XX:XX"), "Last updated: %H:%M", tm);
  //text_layer_set_text(time_layer, (char*) &time_buffer);
}
// WEB
static void in_received_handler(DictionaryIterator *iter, void *context)
{
	(void) context;
    //Get data
    Tuple *t = dict_read_first(iter);
    while(t != NULL)
    {
        process_tuple(t);
        //Get next
        t = dict_read_next(iter);
    }
}



// TIME and DATE
static void update_time() {
	// Get a tm structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	// Create a long-lived buffer

	static char buffer[] = "00:00";
	static char bufferDate[] = "Day, Mon 00";
	// Write the current hours and minutes into the buffer
	if(clock_is_24h_style() == true) {
		//Use 2h hour format
		strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		//Use 12 hour format
		strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}
	strftime(bufferDate,sizeof("Day, Mon 00"),"%a, %b %d",tick_time);
	// Display this time on the TextLayer
	text_layer_set_text(s_time_layer,buffer);
	text_layer_set_text(s_date_layer,bufferDate);
}

static void main_window_load(Window *window) {
	//Create GBitmap, then set to created BitmapLayer
	// s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIREWOLF);
	// s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 68));
  //
	// bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	// layer_add_child(window_get_root_layer(window),
	// bitmap_layer_get_layer(s_background_layer));

	s_time_layer = text_layer_create(GRect(0, 0, 144,50));
	text_layer_set_text(s_time_layer, "00:00");
	text_layer_set_background_color(s_time_layer, GColorBlack);
	text_layer_set_text_color(s_time_layer, GColorClear);
	// s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PH_REGULAR_20));
	// text_layer_set_font(s_time_layer, s_time_font); //
	text_layer_set_font(s_time_layer,fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window),text_layer_get_layer(s_time_layer));

	s_date_layer = text_layer_create(GRect(0,50,144,24));
	text_layer_set_text(s_date_layer,"Day, Mon 00");
	text_layer_set_background_color(s_date_layer, GColorBlack);
	text_layer_set_text_color(s_date_layer, GColorClear);
	//s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PH_REGULAR_12));
	//text_layer_set_font(s_date_layer, s_date_font); //
	text_layer_set_font(s_date_layer,fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window),text_layer_get_layer(s_date_layer));

  s_message_layer = text_layer_create(GRect(0, 74, 144,70));
  text_layer_set_text(s_message_layer, "No Message...");
  text_layer_set_background_color(s_message_layer, GColorBlack);
  text_layer_set_text_color(s_message_layer, GColorClear);
  // s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PH_REGULAR_20));
  // text_layer_set_font(s_time_layer, s_time_font); //
  text_layer_set_font(s_message_layer,fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_message_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window),text_layer_get_layer(s_message_layer));

	update_time();
}

static void main_window_unload(Window *window) {
	//Unload GFont
//	fonts_unload_custom_font(s_time_font);
	//Destroy GBitmap
	//gbitmap_destroy(s_background_bitmap);
	//Destroy BitmapLayer
	//bitmap_layer_destroy(s_background_layer);
	// Destroy TextLayer
	text_layer_destroy(s_time_layer);
	//text_layer_destroy(s_min_layer);
	//text_layer_destroy(s_sometext_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_message_layer);

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	if(tick_time->tm_hour%2 == 0)
 {
     //Send an arbitrary message, the response will be handled by in_received_handler()
     send_int(60, 60);
 }
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

  //Register AppMessage events
	app_message_register_inbox_received(in_received_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());    //Largest possible input and output buffer sizes

	// Show the Window on the watch, with animated=true
	window_stack_push(s_main_window, true);

	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

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
