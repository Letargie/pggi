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

/***************/
/* PHP Methods */
/***************/

GWINDOW_METHOD(__construct){
	ze_gapplication_object * ze_obj;
	zval * obj;
	ze_gwidget_object * widget;
	zval * title = NULL;

	zend_error_handling error_handling;
	zend_replace_error_handling(EH_THROW, NULL, &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &obj, &title) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	ze_obj = Z_GAPPLICATION_P(obj);
	widget = Z_GWIDGET_P(getThis());
	// necessary for personnalised handlers, warning if you bypass the constructor
	widget->std.handlers = &gwindow_object_handlers;
	if(!ze_obj || !ze_obj->app_ptr)
		zend_throw_exception(zend_exception_get_default(), "Bad Parameter, GApplication is unitialised", 0);
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_application_window_new(ze_obj->app_ptr->app);
	if(!title)
		gtk_window_set_title(GTK_WINDOW(widget->widget_ptr->intern),NULL);
	else{
		if(Z_TYPE_P(title) != IS_STRING)
			zend_throw_exception(zend_exception_get_default(), "Bad Parameter, invalid title should be string", 0);
		gtk_window_set_title(GTK_WINDOW(widget->widget_ptr->intern),ZSTR_VAL(Z_STR_P(title)));
	}
	gapplication_add_windows(ze_obj->app_ptr, getThis());
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

GWINDOW_METHOD(setTitle){
	ze_gwidget_object *ze_obj = NULL;
	gwidget_ptr a;
	char * name;
	size_t name_len;
	zend_error_handling error_handling;
	zend_replace_error_handling(EH_THROW, NULL, &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &name, &name_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
    }
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		a = ze_obj->widget_ptr;
		gtk_window_set_title(GTK_WINDOW(ze_obj->widget_ptr->intern), name);
	}
}

GWINDOW_METHOD(setDefaultSize){
	ze_gwidget_object *ze_obj = NULL;
	long x, y;
	zend_error_handling error_handling;
	zend_replace_error_handling(EH_THROW, NULL, &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l|l", &x, &y) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
    }
	zend_restore_error_handling(&error_handling TSRMLS_CC);
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_set_default_size(GTK_WINDOW(ze_obj->widget_ptr->intern), x, y);
	}
}

GWINDOW_METHOD(close){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_close(GTK_WINDOW(ze_obj->widget_ptr->intern));
	}
}

GWINDOW_METHOD(setKeepAbove){
	ze_gwidget_object *ze_obj = NULL;
	int kept;
	PGGI_PARSE_PARAMETERS(zend_parse_parameters(ZEND_NUM_ARGS(), "b", &kept));
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_set_keep_above(GTK_WINDOW(ze_obj->widget_ptr->intern), kept);
	}
}

GWINDOW_METHOD(setKeepBelow){
	ze_gwidget_object *ze_obj = NULL;
	int kept;
	PGGI_PARSE_PARAMETERS(zend_parse_parameters(ZEND_NUM_ARGS(), "b", &kept));
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_set_keep_below(GTK_WINDOW(ze_obj->widget_ptr->intern), kept);
	}
}

GWINDOW_METHOD(fullscreen){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_fullscreen(GTK_WINDOW(ze_obj->widget_ptr->intern));
	}
}


GWINDOW_METHOD(unfullscreen){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_window_unfullscreen(GTK_WINDOW(ze_obj->widget_ptr->intern));
	}
}


/**
 * List of GWindow functions and methods with their arguments
 */
static const zend_function_entry gwindow_class_functions[] = {
	PHP_ME(GWindow, __construct		, arginfo_gwindow_construct			, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GWindow, setTitle		, arginfo_pggi_set					, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, setDefaultSize	, arginfo_gwindow_set_default_size	, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, close			, arginfo_pggi_void					, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, setKeepAbove	, arginfo_pggi_set					, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, setKeepBelow	, arginfo_pggi_set					, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, fullscreen		, arginfo_pggi_void					, ZEND_ACC_PUBLIC)
	PHP_ME(GWindow, unfullscreen	, arginfo_pggi_void					, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gwindow_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval zobj;
	zend_long lval;

	ZVAL_NULL(rv);
	if(w){
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
	}
	return rv;
}

HashTable *gwindow_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	if(!w){
		return NULL;
	}
	GtkWindow * win = GTK_WINDOW(w->intern);

	G_H_UPDATE_STRING	(GWINDOW_TITLE							, gtk_window_get_title							(win));
	G_H_UPDATE_BOOL		(GWINDOW_FOCUS_VISIBLE					, gtk_window_get_focus_visible					(win));
	G_H_UPDATE_BOOL		(GWINDOW_FOCUS_ON_MAP					, gtk_window_get_focus_on_map					(win));
	G_H_UPDATE_BOOL		(GWINDOW_DECORATED						, gtk_window_get_decorated						(win));
	G_H_UPDATE_BOOL		(GWINDOW_DELETABLE						, gtk_window_get_deletable						(win));
	G_H_UPDATE_BOOL		(GWINDOW_MNEMONICS_VISIBLE				, gtk_window_get_mnemonics_visible				(win));
	G_H_UPDATE_BOOL		(GWINDOW_RESIZABLE						, gtk_window_get_resizable						(win));
	G_H_UPDATE_BOOL		(GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED	, gtk_window_get_hide_titlebar_when_maximized	(win));
	G_H_UPDATE_BOOL		(GWINDOW_ACCEPT_FOCUS					, gtk_window_get_accept_focus					(win));

	return G_H_UPDATE_RETURN;
}

void gwindow_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkWindow * win = GTK_WINDOW(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(member_val, GWINDOW_TITLE))
				gtk_window_set_title(win, Z_STRVAL_P(value));
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_TRUE :
		case IS_FALSE :
			tmp_b = Z_LVAL_P(value);
			if(!strcmp(member_val, GWINDOW_FOCUS_VISIBLE))
				gtk_window_set_focus_visible(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_FOCUS_ON_MAP))
				gtk_window_set_focus_on_map(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_DECORATED))
				gtk_window_set_decorated(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_DELETABLE))
				gtk_window_set_deletable(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_MNEMONICS_VISIBLE))
				gtk_window_set_mnemonics_visible(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_RESIZABLE))
				gtk_window_set_resizable(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED))
				gtk_window_set_hide_titlebar_when_maximized(win, tmp_l);
			else if(!strcmp(member_val, GWINDOW_ACCEPT_FOCUS))
				gtk_window_set_accept_focus(win, tmp_l);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		default :
			gcontainer_write_property(object, member, value, cache_slot);
	}
}

/********************************/
/* GWindow class initialisation */
/********************************/

#define DECLARE_GWINDOW_PROP(name) \
DECLARE_CLASS_PROPERTY(gwindow_class_entry_ce, name)

void gwindow_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gwindow = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gwindow", module_number);

	memcpy(&gwindow_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gwindow_object_handlers.read_property  = gwindow_read_property;
	gwindow_object_handlers.get_properties = gwindow_get_properties;
	gwindow_object_handlers.write_property = gwindow_write_property;

	INIT_CLASS_ENTRY(ce, "GWindow", gwindow_class_functions);
	gwindow_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());
	// JUST FOR REFLECTION PURPOSE
	DECLARE_GWINDOW_PROP(GWINDOW_TITLE							);
	DECLARE_GWINDOW_PROP(GWINDOW_FOCUS_VISIBLE					);
	DECLARE_GWINDOW_PROP(GWINDOW_FOCUS_ON_MAP					);
	DECLARE_GWINDOW_PROP(GWINDOW_DECORATED						);
	DECLARE_GWINDOW_PROP(GWINDOW_DELETABLE						);
	DECLARE_GWINDOW_PROP(GWINDOW_MNEMONICS_VISIBLE				);
	DECLARE_GWINDOW_PROP(GWINDOW_RESIZABLE						);
	DECLARE_GWINDOW_PROP(GWINDOW_DEFAULT_HEIGHT					);
	DECLARE_GWINDOW_PROP(GWINDOW_DEFAULT_WIDTH					);
	DECLARE_GWINDOW_PROP(GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED	);
	DECLARE_GWINDOW_PROP(GWINDOW_ACCEPT_FOCUS					);
}


