/*
  +-----------------------------------------------------------+
  | Copyright (c) 2017 Collet Valentin                        |
  +-----------------------------------------------------------+
  | This source file is subject to version the BDS license,   |
  | that is bundled with this package in the file LICENSE     |
  +-----------------------------------------------------------+
  | Author: Collet Valentin <valentin@famillecollet.com>      |
  +-----------------------------------------------------------+
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "geventkey.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gevent_key;
static zend_object_handlers gevent_key_object_handlers;
static zend_class_entry * gevent_key_class_entry_ce;

zend_class_entry * gevent_key_get_class_entry(){
	return gevent_key_class_entry_ce;
}

zend_object_handlers * gevent_key_get_object_handlers(){
	return &gevent_key_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

GEVENT_KEY_METHOD(__construct){}

static const zend_function_entry gevent_key_class_functions[] = {
	PHP_ME(GEvent, __construct , arginfo_pggi_void, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gevent_key_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	return gevent_read_property(object, member, type, cache_slot, rv);
}

HashTable *gevent_key_get_properties(zval *object){
	G_H_UPDATE_INIT(gevent_get_properties(object));
	return G_H_UPDATE_RETURN;
}

void gevent_key_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	gevent_write_property(object, member, value, cache_slot);
}

/*******************************/
/* GEvent Class Initialization */
/*******************************/

#define DECLARE_GEVENT_KEY_PROP(name) \
DECLARE_CLASS_PROPERTY(gevent_key_class_entry_ce, name)

#define GEVENT_KEY_CONSTANT(name, value) \
zend_declare_class_constant_long(gevent_key_class_entry_ce, name, sizeof(name)-1, value);


void gevent_key_init(int module_number){
	zend_class_entry ce;
	le_gevent_key = zend_register_list_destructors_ex(gevent_free_resource, NULL, "PGGI\\GEventKey", module_number);

	memcpy(&gevent_key_object_handlers, gevent_get_object_handlers(), sizeof(zend_object_handlers));
	gevent_key_object_handlers.read_property  = gevent_key_read_property;
	gevent_key_object_handlers.get_properties = gevent_key_get_properties;
	gevent_key_object_handlers.write_property = gevent_key_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GEventKey", gevent_key_class_functions);
	gevent_key_class_entry_ce = zend_register_internal_class_ex(&ce, gevent_get_class_entry());

	DECLARE_GEVENT_KEY_PROP(GEVENT_KEY_KEYVAL);
	DECLARE_GEVENT_KEY_PROP(GEVENT_KEY_STATE);

	// I WONT DO ALL THE KEY :p GO SEE HERE :
	// https://git.gnome.org/browse/gtk+/tree/gdk/gdkkeysyms.h

	GEVENT_KEY_CONSTANT("KEY_VoidSymbol"  , GDK_KEY_VoidSymbol  );
	GEVENT_KEY_CONSTANT("KEY_BackSpace"   , GDK_KEY_BackSpace   );
	GEVENT_KEY_CONSTANT("KEY_Tab"         , GDK_KEY_Tab         );
	GEVENT_KEY_CONSTANT("KEY_Return"      , GDK_KEY_Return      );
	GEVENT_KEY_CONSTANT("KEY_Escape"      , GDK_KEY_Escape      );
	GEVENT_KEY_CONSTANT("KEY_Delete"      , GDK_KEY_Delete      );
	GEVENT_KEY_CONSTANT("KEY_Home"        , GDK_KEY_Home        );
	GEVENT_KEY_CONSTANT("KEY_Left"        , GDK_KEY_Left        );
	GEVENT_KEY_CONSTANT("KEY_Up"          , GDK_KEY_Up          );
	GEVENT_KEY_CONSTANT("KEY_Right"       , GDK_KEY_Right       );
	GEVENT_KEY_CONSTANT("KEY_Down"        , GDK_KEY_Down        );
	GEVENT_KEY_CONSTANT("KEY_Page_Up"     , GDK_KEY_Page_Up     );
	GEVENT_KEY_CONSTANT("KEY_Page_Down"   , GDK_KEY_Page_Down   );
	GEVENT_KEY_CONSTANT("KEY_Num_Lock"    , GDK_KEY_Num_Lock    );
	GEVENT_KEY_CONSTANT("KEY_F1"          , GDK_KEY_F1          );
	GEVENT_KEY_CONSTANT("KEY_F2"          , GDK_KEY_F2          );
	GEVENT_KEY_CONSTANT("KEY_F3"          , GDK_KEY_F3          );
	GEVENT_KEY_CONSTANT("KEY_F4"          , GDK_KEY_F4          );
	GEVENT_KEY_CONSTANT("KEY_F5"          , GDK_KEY_F5          );
	GEVENT_KEY_CONSTANT("KEY_F6"          , GDK_KEY_F6          );
	GEVENT_KEY_CONSTANT("KEY_F7"          , GDK_KEY_F7          );
	GEVENT_KEY_CONSTANT("KEY_F8"          , GDK_KEY_F8          );
	GEVENT_KEY_CONSTANT("KEY_F9"          , GDK_KEY_F9          );
	GEVENT_KEY_CONSTANT("KEY_F10"         , GDK_KEY_F10         );
	GEVENT_KEY_CONSTANT("KEY_F11"         , GDK_KEY_F11         );
	GEVENT_KEY_CONSTANT("KEY_F12"         , GDK_KEY_F12         );
	GEVENT_KEY_CONSTANT("KEY_Shift_L"     , GDK_KEY_Shift_L     );
	GEVENT_KEY_CONSTANT("KEY_Shift_R"     , GDK_KEY_Shift_R     );
	GEVENT_KEY_CONSTANT("KEY_Control_L"   , GDK_KEY_Control_L   );
	GEVENT_KEY_CONSTANT("KEY_Control_R"   , GDK_KEY_Control_R   );
	GEVENT_KEY_CONSTANT("KEY_Caps_Lock"   , GDK_KEY_Caps_Lock   );
	GEVENT_KEY_CONSTANT("KEY_Shift_Lock"  , GDK_KEY_Shift_Lock  );
	GEVENT_KEY_CONSTANT("KEY_Alt_L"       , GDK_KEY_Alt_R       );
	GEVENT_KEY_CONSTANT("KEY_space"       , GDK_KEY_space       );
	GEVENT_KEY_CONSTANT("KEY_exclam"      , GDK_KEY_exclam      );
	GEVENT_KEY_CONSTANT("KEY_quotedbl"    , GDK_KEY_quotedbl    );
	GEVENT_KEY_CONSTANT("KEY_numbersign"  , GDK_KEY_numbersign  );
	GEVENT_KEY_CONSTANT("KEY_dollar"      , GDK_KEY_dollar      );
	GEVENT_KEY_CONSTANT("KEY_percent"     , GDK_KEY_percent     );
	GEVENT_KEY_CONSTANT("KEY_ampersand"   , GDK_KEY_ampersand   );
	GEVENT_KEY_CONSTANT("KEY_apostrophe"  , GDK_KEY_apostrophe  );
	GEVENT_KEY_CONSTANT("KEY_quoteright"  , GDK_KEY_quoteright  );
	GEVENT_KEY_CONSTANT("KEY_parenleft"   , GDK_KEY_parenleft   );
	GEVENT_KEY_CONSTANT("KEY_parenright"  , GDK_KEY_parenright  );
	GEVENT_KEY_CONSTANT("KEY_asterisk"    , GDK_KEY_asterisk    );
	GEVENT_KEY_CONSTANT("KEY_plus"        , GDK_KEY_plus        );
	GEVENT_KEY_CONSTANT("KEY_comma"       , GDK_KEY_comma       );
	GEVENT_KEY_CONSTANT("KEY_minus"       , GDK_KEY_minus       );
	GEVENT_KEY_CONSTANT("KEY_period"      , GDK_KEY_period      );
	GEVENT_KEY_CONSTANT("KEY_slash"       , GDK_KEY_slash       );
	GEVENT_KEY_CONSTANT("KEY_1"           , GDK_KEY_1           );
	GEVENT_KEY_CONSTANT("KEY_2"           , GDK_KEY_2           );
	GEVENT_KEY_CONSTANT("KEY_3"           , GDK_KEY_3           );
	GEVENT_KEY_CONSTANT("KEY_4"           , GDK_KEY_4           );
	GEVENT_KEY_CONSTANT("KEY_5"           , GDK_KEY_5           );
	GEVENT_KEY_CONSTANT("KEY_6"           , GDK_KEY_6           );
	GEVENT_KEY_CONSTANT("KEY_7"           , GDK_KEY_7           );
	GEVENT_KEY_CONSTANT("KEY_8"           , GDK_KEY_8           );
	GEVENT_KEY_CONSTANT("KEY_9"           , GDK_KEY_9           );
	GEVENT_KEY_CONSTANT("KEY_colon"       , GDK_KEY_colon       );
	GEVENT_KEY_CONSTANT("KEY_semicolon"   , GDK_KEY_semicolon   );
	GEVENT_KEY_CONSTANT("KEY_less"        , GDK_KEY_less        );
	GEVENT_KEY_CONSTANT("KEY_equal"       , GDK_KEY_equal       );
	GEVENT_KEY_CONSTANT("KEY_greater"     , GDK_KEY_greater     );
	GEVENT_KEY_CONSTANT("KEY_question"    , GDK_KEY_question    );
	GEVENT_KEY_CONSTANT("KEY_at"          , GDK_KEY_at          );
	GEVENT_KEY_CONSTANT("KEY_A"           , GDK_KEY_A           );
	GEVENT_KEY_CONSTANT("KEY_B"           , GDK_KEY_B           );
	GEVENT_KEY_CONSTANT("KEY_C"           , GDK_KEY_C           );
	GEVENT_KEY_CONSTANT("KEY_D"           , GDK_KEY_D           );
	GEVENT_KEY_CONSTANT("KEY_E"           , GDK_KEY_E           );
	GEVENT_KEY_CONSTANT("KEY_F"           , GDK_KEY_F           );
	GEVENT_KEY_CONSTANT("KEY_G"           , GDK_KEY_G           );
	GEVENT_KEY_CONSTANT("KEY_H"           , GDK_KEY_H           );
	GEVENT_KEY_CONSTANT("KEY_I"           , GDK_KEY_I           );
	GEVENT_KEY_CONSTANT("KEY_J"           , GDK_KEY_J           );
	GEVENT_KEY_CONSTANT("KEY_K"           , GDK_KEY_K           );
	GEVENT_KEY_CONSTANT("KEY_L"           , GDK_KEY_L           );
	GEVENT_KEY_CONSTANT("KEY_M"           , GDK_KEY_M           );
	GEVENT_KEY_CONSTANT("KEY_N"           , GDK_KEY_N           );
	GEVENT_KEY_CONSTANT("KEY_O"           , GDK_KEY_O           );
	GEVENT_KEY_CONSTANT("KEY_P"           , GDK_KEY_P           );
	GEVENT_KEY_CONSTANT("KEY_Q"           , GDK_KEY_Q           );
	GEVENT_KEY_CONSTANT("KEY_R"           , GDK_KEY_R           );
	GEVENT_KEY_CONSTANT("KEY_S"           , GDK_KEY_S           );
	GEVENT_KEY_CONSTANT("KEY_T"           , GDK_KEY_T           );
	GEVENT_KEY_CONSTANT("KEY_U"           , GDK_KEY_U           );
	GEVENT_KEY_CONSTANT("KEY_V"           , GDK_KEY_V           );
	GEVENT_KEY_CONSTANT("KEY_W"           , GDK_KEY_W           );
	GEVENT_KEY_CONSTANT("KEY_X"           , GDK_KEY_X           );
	GEVENT_KEY_CONSTANT("KEY_Y"           , GDK_KEY_Y           );
	GEVENT_KEY_CONSTANT("KEY_Z"           , GDK_KEY_Z           );
	GEVENT_KEY_CONSTANT("KEY_bracketleft" , GDK_KEY_bracketleft );
	GEVENT_KEY_CONSTANT("KEY_backslash"   , GDK_KEY_backslash   );
	GEVENT_KEY_CONSTANT("KEY_bracketright", GDK_KEY_bracketright);
	GEVENT_KEY_CONSTANT("KEY_asciicircum" , GDK_KEY_asciicircum );
	GEVENT_KEY_CONSTANT("KEY_underscore"  , GDK_KEY_underscore  );
	GEVENT_KEY_CONSTANT("KEY_grave"       , GDK_KEY_grave       );
	GEVENT_KEY_CONSTANT("KEY_quoteleft"   , GDK_KEY_quoteleft   );
	GEVENT_KEY_CONSTANT("KEY_a"           , GDK_KEY_a           );
	GEVENT_KEY_CONSTANT("KEY_b"           , GDK_KEY_b           );
	GEVENT_KEY_CONSTANT("KEY_c"           , GDK_KEY_c           );
	GEVENT_KEY_CONSTANT("KEY_d"           , GDK_KEY_d           );
	GEVENT_KEY_CONSTANT("KEY_e"           , GDK_KEY_e           );
	GEVENT_KEY_CONSTANT("KEY_f"           , GDK_KEY_f           );
	GEVENT_KEY_CONSTANT("KEY_g"           , GDK_KEY_g           );
	GEVENT_KEY_CONSTANT("KEY_h"           , GDK_KEY_h           );
	GEVENT_KEY_CONSTANT("KEY_i"           , GDK_KEY_i           );
	GEVENT_KEY_CONSTANT("KEY_j"           , GDK_KEY_j           );
	GEVENT_KEY_CONSTANT("KEY_k"           , GDK_KEY_k           );
	GEVENT_KEY_CONSTANT("KEY_l"           , GDK_KEY_l           );
	GEVENT_KEY_CONSTANT("KEY_m"           , GDK_KEY_m           );
	GEVENT_KEY_CONSTANT("KEY_n"           , GDK_KEY_n           );
	GEVENT_KEY_CONSTANT("KEY_o"           , GDK_KEY_o           );
	GEVENT_KEY_CONSTANT("KEY_p"           , GDK_KEY_p           );
	GEVENT_KEY_CONSTANT("KEY_q"           , GDK_KEY_q           );
	GEVENT_KEY_CONSTANT("KEY_r"           , GDK_KEY_r           );
	GEVENT_KEY_CONSTANT("KEY_s"           , GDK_KEY_s           );
	GEVENT_KEY_CONSTANT("KEY_t"           , GDK_KEY_t           );
	GEVENT_KEY_CONSTANT("KEY_u"           , GDK_KEY_u           );
	GEVENT_KEY_CONSTANT("KEY_v"           , GDK_KEY_v           );
	GEVENT_KEY_CONSTANT("KEY_w"           , GDK_KEY_w           );
	GEVENT_KEY_CONSTANT("KEY_x"           , GDK_KEY_x           );
	GEVENT_KEY_CONSTANT("KEY_y"           , GDK_KEY_y           );
	GEVENT_KEY_CONSTANT("KEY_z"           , GDK_KEY_z           );
	GEVENT_KEY_CONSTANT("KEY_braceleft"   , GDK_KEY_braceleft   );
	GEVENT_KEY_CONSTANT("KEY_bar"         , GDK_KEY_bar         );
	GEVENT_KEY_CONSTANT("KEY_asciitilde"  , GDK_KEY_asciitilde  );
}


