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


#include "gtoolitem.h"

static int le_gtool_item;
static zend_object_handlers gtool_item_object_handlers;
static zend_class_entry * gtool_item_class_entry_ce;

zend_class_entry * gtool_item_get_class_entry(){
	return gtool_item_class_entry_ce;
}

zend_object_handlers * gtool_item_get_object_handlers(){
	return &gtool_item_object_handlers;
}

PHP_METHOD(GToolItem, __construct){
	ze_gwidget_object * widget;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gtool_item_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_tool_item_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}


static const zend_function_entry gtool_item_class_functions[] = {
	PHP_ME(GToolItem, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};



zval *gtool_item_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GTOOL_ITEM_EXPAND)){
		ZVAL_BOOL(rv, gtk_tool_item_get_expand(GTK_TOOL_ITEM(w->intern)));
	}else if(!strcmp(member_val, GTOOL_ITEM_HOMOGENEOUS)){
		ZVAL_BOOL(rv, gtk_tool_item_get_homogeneous(GTK_TOOL_ITEM(w->intern)));
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gtool_item_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;

	G_H_UPDATE_BOOL  (GTOOL_ITEM_EXPAND      , gtk_tool_item_get_expand      (GTK_TOOL_ITEM(w->intern)));
	G_H_UPDATE_BOOL  (GTOOL_ITEM_HOMOGENEOUS , gtk_tool_item_get_homogeneous (GTK_TOOL_ITEM(w->intern)));

	return G_H_UPDATE_RETURN;
}

void gtool_item_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_FALSE :
		case IS_TRUE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GTOOL_ITEM_HOMOGENEOUS))
				gtk_tool_item_set_homogeneous(GTK_TOOL_ITEM(w->intern), tmp_b);
			else if(!strcmp(member_val, GTOOL_ITEM_EXPAND))
				gtk_tool_item_set_expand(GTK_TOOL_ITEM(w->intern), tmp_b);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		default:
			gcontainer_write_property(object, member, value, cache_slot);
	}
}


#define DECLARE_GTOOL_ITEM_PROP(name) \
DECLARE_CLASS_PROPERTY(gtool_item_class_entry_ce, name)

void gtool_item_init(int module_number){
	zend_class_entry ce;
	le_gtool_item = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GToolItem", module_number);

	memcpy(&gtool_item_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gtool_item_object_handlers.read_property  = gtool_item_read_property;
	gtool_item_object_handlers.get_properties = gtool_item_get_properties;
	gtool_item_object_handlers.write_property = gtool_item_write_property;	
	INIT_CLASS_ENTRY(ce, "PGGI\\GToolItem", gtool_item_class_functions);
	ce.create_object 	= gwidget_object_new;
	gtool_item_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	DECLARE_GTOOL_ITEM_PROP(GTOOL_ITEM_HOMOGENEOUS );
	DECLARE_GTOOL_ITEM_PROP(GTOOL_ITEM_EXPAND      );
}


