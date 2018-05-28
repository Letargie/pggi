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


#include "gtoolbar.h"

static int le_gtoolbar;
static zend_object_handlers gtoolbar_object_handlers;
static zend_class_entry * gtoolbar_class_entry_ce;

zend_class_entry * gtoolbar_get_class_entry(){
	return gtoolbar_class_entry_ce;
}

zend_object_handlers * gtoolbar_get_object_handlers(){
	return &gtoolbar_object_handlers;
}

PHP_METHOD(GToolbar, __construct){
	ze_gwidget_object * widget;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gtoolbar_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_toolbar_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

PHP_METHOD(GToolbar, insert){
	ze_gwidget_object *ze_obj = NULL;
	ze_gwidget_object * data;
	zval * self = getThis();
	zval * obj;
	long pos = -1;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|bbl", &obj , gtool_item_get_class_entry(), &pos) == FAILURE)
		return;
	data = Z_GWIDGET_P(obj);
	ze_obj = Z_GWIDGET_P(self);
	gtk_toolbar_insert(GTK_TOOLBAR(ze_obj->widget_ptr->intern), GTK_TOOL_ITEM(data->widget_ptr->intern), pos);
	gcontainer_add_data(ze_obj->widget_ptr, obj);
}

static const zend_function_entry gtoolbar_class_functions[] = {
	PHP_ME(GToolbar, __construct, arginfo_pggi_void      , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GToolbar, insert     , arginfo_gtoolbar_insert, ZEND_ACC_PUBLIC)
	PHP_FE_END
};



zval *gtoolbar_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GTOOLBAR_STYLE)){
		ZVAL_LONG(rv, gtk_toolbar_get_style(GTK_TOOLBAR(w->intern)));
	}else if(!strcmp(member_val, GTOOLBAR_SHOW_ARROW)){
		ZVAL_BOOL(rv, gtk_toolbar_get_show_arrow(GTK_TOOLBAR(w->intern)));
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gtoolbar_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;

	G_H_UPDATE_LONG(GTOOLBAR_STYLE     , gtk_toolbar_get_style     (GTK_TOOLBAR(w->intern)));
	G_H_UPDATE_BOOL(GTOOLBAR_SHOW_ARROW, gtk_toolbar_get_show_arrow(GTK_TOOLBAR(w->intern)));

	return G_H_UPDATE_RETURN;
}

void gtoolbar_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	int tmp_b;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GTOOLBAR_STYLE))
				switch(tmp_l){
					case GTK_TOOLBAR_ICONS     :
					case GTK_TOOLBAR_TEXT      :
					case GTK_TOOLBAR_BOTH      :
					case GTK_TOOLBAR_BOTH_HORIZ:
						gtk_toolbar_set_style(GTK_TOOLBAR(w->intern), tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "The style property needs to be a TOOLBAR_*");
				}
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_FALSE :
		case IS_TRUE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GTOOLBAR_SHOW_ARROW))
				gtk_toolbar_set_show_arrow(GTK_TOOLBAR(w->intern), tmp_b);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		default:
			gcontainer_write_property(object, member, value, cache_slot);
	}
}


#define DECLARE_GTOOLBAR_PROP(name) \
DECLARE_CLASS_PROPERTY(gtoolbar_class_entry_ce, name)

void gtoolbar_init(int module_number){
	zend_class_entry ce;
	le_gtoolbar = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GToolbar", module_number);

	memcpy(&gtoolbar_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gtoolbar_object_handlers.read_property  = gtoolbar_read_property;
	gtoolbar_object_handlers.get_properties = gtoolbar_get_properties;
	gtoolbar_object_handlers.write_property = gtoolbar_write_property;	
	INIT_CLASS_ENTRY(ce, "PGGI\\GToolbar", gtoolbar_class_functions);
	ce.create_object 	= gwidget_object_new;
	gtoolbar_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	DECLARE_GTOOLBAR_PROP(GTOOLBAR_SHOW_ARROW);
	DECLARE_GTOOLBAR_PROP(GTOOLBAR_STYLE     );
}


