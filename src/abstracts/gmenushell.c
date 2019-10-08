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


#include "gmenushell.h"

static int le_gmenushell;
static zend_object_handlers gmenushell_object_handlers;
static zend_class_entry * gmenushell_class_entry_ce;


zend_class_entry * gmenushell_get_class_entry(){
	return gmenushell_class_entry_ce;
}

zend_object_handlers * gmenushell_get_object_handlers(){
	return &gmenushell_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

/*NOT IMPLEMENTED YET*/
/*
GMENUSHELL_METHOD(append){
	ze_gwidget_object * self, * obj;
	self = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(prepend){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(insert){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(deactivate){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(selectItem){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(selectFirst){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(deselect){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(activateItem){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}

GMENUSHELL_METHOD(cancel){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
}*/


/**
 * List of GMenuShell functions and methods with their arguments
 */
static const zend_function_entry gmenushell_class_functions[] = {
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gmenushell_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkMenuShell * menu = GTK_MENU_SHELL(w->intern);
	if(!strcmp(member_val, GMENUSHELL_TAKE_FOCUS))
		ZVAL_BOOL(rv, gtk_menu_shell_get_take_focus(menu));
	else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gmenushell_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkMenuShell * menu = GTK_MENU_SHELL(w->intern);

	G_H_UPDATE_BOOL(GMENUSHELL_TAKE_FOCUS, gtk_menu_shell_get_take_focus(menu));

	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE gmenushell_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkMenuShell * menu = GTK_MENU_SHELL(w->intern);
	if(!strcmp(member_val, GMENUSHELL_TAKE_FOCUS)){
		convert_to_boolean(value);
		gtk_menu_shell_set_take_focus(menu, GET_BOOL_FROM_ZVAL(value));
	} else {
		PHP_WRITE_PROP_HANDLER_RETURN(gcontainer_write_property(object, member, value, cache_slot));
	}
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

/***********************************/
/* GMenuShell class initialisation */
/***********************************/

#define DECLARE_GMENUSHELL_PROP(name) \
DECLARE_CLASS_PROPERTY(gmenushell_class_entry_ce, name)

void gmenushell_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gmenushell = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GMenuShell", module_number);

	memcpy(&gmenushell_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gmenushell_object_handlers.read_property  = gmenushell_read_property;
	gmenushell_object_handlers.get_properties = gmenushell_get_properties;
	gmenushell_object_handlers.write_property = gmenushell_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GMenuShell", gmenushell_class_functions);
	gmenushell_class_entry_ce = zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());
	gmenushell_class_entry_ce->ce_flags |= ZEND_ACC_ABSTRACT;
	DECLARE_GMENUSHELL_PROP(GMENUSHELL_TAKE_FOCUS);
}


