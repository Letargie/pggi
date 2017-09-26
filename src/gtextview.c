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


#include "gtextview.h"

static int le_gtext_view;
static zend_object_handlers gtext_view_object_handlers;
static zend_class_entry * gtext_view_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GTEXT_VIEW_METHOD(__construct){
	ze_gwidget_object * widget;
	zval * textbuffer = NULL;
	// for now we forced the passage of a textbuffer
	// it's absolutely not the final solution (beside, have to test parameters)
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "z", &textbuffer) == FAILURE)
		return;
	GtkTextBuffer * buffer;
	if(textbuffer){
		buffer = GTK_TEXT_BUFFER((Z_GTEXT_BUFFER_P(textbuffer))->buffer_ptr->intern);	
		widget = Z_GWIDGET_P(getThis());
		widget->std.handlers = &gtext_view_object_handlers;
		widget->widget_ptr = gwidget_new();
		widget->widget_ptr->intern = gtk_text_view_new_with_buffer(buffer);
		GCONTAINER_ADD_ELEMENT(widget);
		g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
	}
}

/**
 * List of GTextView functions and methods with their arguments
 */
static const zend_function_entry gtext_view_class_functions[] = {
	PHP_ME(GTextView, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gtext_view_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval zobj;
	zend_long lval;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	return gcontainer_read_property(object, member, type, cache_slot, rv);
}

HashTable *gtext_view_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	return G_H_UPDATE_RETURN;
}

void gtext_view_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		default :
			gcontainer_write_property(object, member, value, cache_slot);
	}
}

/**********************************/
/* GTextView class initialisation */
/**********************************/

#define DECLARE_GTEXT_VIEW_PROP(name) \
DECLARE_CLASS_PROPERTY(gtext_view_class_entry_ce, name)

void gtext_view_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gtext_view = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GTextView", module_number);

	memcpy(&gtext_view_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gtext_view_object_handlers.read_property  = gtext_view_read_property;
	gtext_view_object_handlers.get_properties = gtext_view_get_properties;
	gtext_view_object_handlers.write_property = gtext_view_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GTextView", gtext_view_class_functions);
	gtext_view_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

}


