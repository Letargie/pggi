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


#include "gwindow.h"

static int le_gwindow;
static zend_object_handlers gwindow_object_handlers;
static zend_class_entry * gwindow_class_entry_ce;


zend_class_entry * gwindow_get_class_entry(){
	return gwindow_class_entry_ce;
}

zend_object_handlers * gwindow_get_object_handlers(){
	return &gwindow_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

GWINDOW_METHOD(__construct){
	ze_gapplication_object * ze_obj;
	zval * obj;
	ze_gwidget_object * widget;
	char *in;
	size_t in_len = 0;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "O|s", &obj, gapplication_get_class_entry(), &in, &in_len) == FAILURE)
		return;
	ze_obj = Z_GAPPLICATION_P(obj);
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gwindow_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_application_window_new(ze_obj->app_ptr->app);
	if(in_len > 0)
		gtk_window_set_title(GTK_WINDOW(widget->widget_ptr->intern), in);
	gapplication_add_windows(ze_obj->app_ptr, getThis());
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

GWINDOW_METHOD(setDefaultSize){
	ze_gwidget_object *ze_obj = NULL;
	long x, y;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l|l", &x, &y) == FAILURE)
		return;
	zval * self = getThis();
	ze_obj = Z_GWIDGET_P(self);
	gtk_window_set_default_size(GTK_WINDOW(ze_obj->widget_ptr->intern), x, y);
}

GWINDOW_METHOD(close){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_window_close(GTK_WINDOW(ze_obj->widget_ptr->intern));
}

GWINDOW_METHOD(setKeepAbove){
	ze_gwidget_object *ze_obj = NULL;
	int kept;
	zval * self = getThis();
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &kept) == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_window_set_keep_above(GTK_WINDOW(ze_obj->widget_ptr->intern), kept);
}

GWINDOW_METHOD(setKeepBelow){
	ze_gwidget_object *ze_obj = NULL;
	int kept;
	zval * self = getThis();
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "b", &kept) == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_window_set_keep_below(GTK_WINDOW(ze_obj->widget_ptr->intern), kept);
}

GWINDOW_METHOD(fullscreen){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_window_fullscreen(GTK_WINDOW(ze_obj->widget_ptr->intern));
}


GWINDOW_METHOD(unfullscreen){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	gtk_window_unfullscreen(GTK_WINDOW(ze_obj->widget_ptr->intern));
}

GWINDOW_METHOD(getScreen){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(self);
	GdkScreen * screen = gtk_window_get_screen(GTK_WINDOW(ze_obj->widget_ptr->intern));
	zend_object * screen_obj = gscreen_object_new(gscreen_get_class_entry());
	ze_gscreen_object * s = php_gscreen_fetch_object(screen_obj);
	s->screen_ptr = gscreen_new();
	s->screen_ptr->intern = screen;
	RETURN_OBJ(screen_obj);
}

/**
 * List of GWindow functions and methods with their arguments
 */
static const zend_function_entry gwindow_class_functions[] = {
	PHP_ME(GWindow, __construct   , arginfo_gwindow_construct       , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GWindow, setDefaultSize, arginfo_gwindow_set_default_size, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, close         , arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, setKeepAbove  , arginfo_pggi_set_bool           , ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, setKeepBelow  , arginfo_pggi_set_bool           , ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, fullscreen    , arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, unfullscreen  , arginfo_pggi_void               , ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, getScreen     , arginfo_gwindow_get_screen      , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gwindow_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkWindow * win = GTK_WINDOW(w->intern);
	if(!strcmp(member_val, GWINDOW_TITLE)){
		const char * tmp = gtk_window_get_title(GTK_WINDOW(intern->widget_ptr->intern));
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else if(!strcmp(member_val, GWINDOW_FOCUS_VISIBLE))
		ZVAL_BOOL(rv, gtk_window_get_focus_visible(win));
	else if(!strcmp(member_val, GWINDOW_FOCUS_ON_MAP))
		ZVAL_BOOL(rv, gtk_window_get_focus_on_map(win));
	else if(!strcmp(member_val, GWINDOW_DECORATED))
		ZVAL_BOOL(rv, gtk_window_get_decorated(win));
	else if(!strcmp(member_val, GWINDOW_DELETABLE))
		ZVAL_BOOL(rv, gtk_window_get_deletable(win));
	else if(!strcmp(member_val, GWINDOW_MNEMONICS_VISIBLE))
		ZVAL_BOOL(rv, gtk_window_get_mnemonics_visible(win));
	else if(!strcmp(member_val, GWINDOW_RESIZABLE))
		ZVAL_BOOL(rv, gtk_window_get_resizable(win));
	else if(!strcmp(member_val, GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED))
		ZVAL_BOOL(rv, gtk_window_get_hide_titlebar_when_maximized(win));
	else if(!strcmp(member_val, GWINDOW_ACCEPT_FOCUS))
		ZVAL_BOOL(rv, gtk_window_get_accept_focus(win));
	else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gwindow_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkWindow * win = GTK_WINDOW(w->intern);

	G_H_UPDATE_STRING(GWINDOW_TITLE                       , gtk_window_get_title                       (win));
	G_H_UPDATE_BOOL  (GWINDOW_FOCUS_VISIBLE               , gtk_window_get_focus_visible               (win));
	G_H_UPDATE_BOOL  (GWINDOW_FOCUS_ON_MAP                , gtk_window_get_focus_on_map                (win));
	G_H_UPDATE_BOOL  (GWINDOW_DECORATED                   , gtk_window_get_decorated                   (win));
	G_H_UPDATE_BOOL  (GWINDOW_DELETABLE                   , gtk_window_get_deletable                   (win));
	G_H_UPDATE_BOOL  (GWINDOW_MNEMONICS_VISIBLE           , gtk_window_get_mnemonics_visible           (win));
	G_H_UPDATE_BOOL  (GWINDOW_RESIZABLE                   , gtk_window_get_resizable                   (win));
	G_H_UPDATE_BOOL  (GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED, gtk_window_get_hide_titlebar_when_maximized(win));
	G_H_UPDATE_BOOL  (GWINDOW_ACCEPT_FOCUS                , gtk_window_get_accept_focus                (win));

	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE gwindow_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkWindow * win = GTK_WINDOW(w->intern);
	if(!strcmp(member_val, GWINDOW_TITLE)){
		convert_to_string(value);
		gtk_window_set_title(win, Z_STRVAL_P(value));
	}else if(!strcmp(member_val, GWINDOW_FOCUS_VISIBLE)){
		convert_to_boolean(value);
		gtk_window_set_focus_visible(win, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWINDOW_FOCUS_ON_MAP)){
		convert_to_boolean(value);
		gtk_window_set_focus_on_map(win, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWINDOW_DECORATED)){
		convert_to_boolean(value);
		gtk_window_set_decorated(win, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWINDOW_DELETABLE)){
		convert_to_boolean(value);
		gtk_window_set_deletable(win, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWINDOW_MNEMONICS_VISIBLE)){
		convert_to_boolean(value);
		gtk_window_set_mnemonics_visible(win, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWINDOW_RESIZABLE)){
		convert_to_boolean(value);
		gtk_window_set_resizable(win, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED)){
		convert_to_boolean(value);
		gtk_window_set_hide_titlebar_when_maximized(win, GET_BOOL_FROM_ZVAL(value));
	}else if(!strcmp(member_val, GWINDOW_ACCEPT_FOCUS)){
		convert_to_boolean(value);
		gtk_window_set_accept_focus(win, GET_BOOL_FROM_ZVAL(value));
	} else {
		PHP_WRITE_PROP_HANDLER_RETURN(gcontainer_write_property(object, member, value, cache_slot));
	}
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

/********************************/
/* GWindow class initialisation */
/********************************/

#define DECLARE_GWINDOW_PROP(name) \
DECLARE_CLASS_PROPERTY(gwindow_class_entry_ce, name)

void gwindow_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gwindow = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GWindow", module_number);

	memcpy(&gwindow_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gwindow_object_handlers.read_property  = gwindow_read_property;
	gwindow_object_handlers.get_properties = gwindow_get_properties;
	gwindow_object_handlers.write_property = gwindow_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GWindow", gwindow_class_functions);
	gwindow_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());
	// JUST FOR REFLECTION PURPOSE
	DECLARE_GWINDOW_PROP(GWINDOW_TITLE                       );
	DECLARE_GWINDOW_PROP(GWINDOW_FOCUS_VISIBLE               );
	DECLARE_GWINDOW_PROP(GWINDOW_FOCUS_ON_MAP                );
	DECLARE_GWINDOW_PROP(GWINDOW_DECORATED                   );
	DECLARE_GWINDOW_PROP(GWINDOW_DELETABLE                   );
	DECLARE_GWINDOW_PROP(GWINDOW_MNEMONICS_VISIBLE           );
	DECLARE_GWINDOW_PROP(GWINDOW_RESIZABLE                   );
	DECLARE_GWINDOW_PROP(GWINDOW_DEFAULT_HEIGHT              );
	DECLARE_GWINDOW_PROP(GWINDOW_DEFAULT_WIDTH               );
	DECLARE_GWINDOW_PROP(GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED);
	DECLARE_GWINDOW_PROP(GWINDOW_ACCEPT_FOCUS                );
}


