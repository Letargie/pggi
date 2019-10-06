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


#include "gscrollwindow.h"

static int le_gscroll_window;
static zend_object_handlers gscroll_window_object_handlers;
static zend_class_entry * gscroll_window_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GSCROLL_WINDOW_METHOD(__construct){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return ;
	widget->std.handlers = &gscroll_window_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_scrolled_window_new(NULL, NULL);
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

/**
 * List of GScrollWindow functions and methods with their arguments
 */
static const zend_function_entry gscroll_window_class_functions[] = {
	PHP_ME(GScrollWindow, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gscroll_window_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	/*ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkScrolledWindow * win = GTK_SCROLLED_WINDOW(w->intern);*/
	return gcontainer_read_property(object, member, type, cache_slot, rv);
}

HashTable *gscroll_window_get_properties(zval *object){
	/*G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkScrolledWindow * win = GTK_SCROLLED_WINDOW(w->intern);
	
	return G_H_UPDATE_RETURN;*/
	return gcontainer_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE gscroll_window_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	/*ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkScrolledWindow * win = GTK_SCROLLED_WINDOW(w->intern);*/
	PHP_WRITE_PROP_HANDLER_RETURN(gcontainer_write_property(object, member, value, cache_slot));
}

/**************************************/
/* GScrollWindow class initialisation */
/**************************************/

#define DECLARE_GSCROLL_WINDOW_PROP(name) \
DECLARE_CLASS_PROPERTY(gscroll_window_class_entry_ce, name)

void gscroll_window_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gscroll_window = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GScrollWindow", module_number);

	memcpy(&gscroll_window_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gscroll_window_object_handlers.read_property  = gscroll_window_read_property;
	gscroll_window_object_handlers.get_properties = gscroll_window_get_properties;
	gscroll_window_object_handlers.write_property = gscroll_window_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GScrollWindow", gscroll_window_class_functions);
	gscroll_window_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());
}


