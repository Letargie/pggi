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


#include "gmenu.h"

static int le_gmenu;
static zend_object_handlers gmenu_object_handlers;
static zend_class_entry * gmenu_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GMENU_METHOD(__construct){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gmenu_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_menu_new();
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

/**
 * List of GMenu functions and methods with their arguments
 */
static const zend_function_entry gmenu_class_functions[] = {
	PHP_ME(GMenu, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/******************************/
/* GMenu class initialisation */
/******************************/

#define DECLARE_GMENU_PROP(name) \
DECLARE_CLASS_PROPERTY(gmenu_class_entry_ce, name)

void gmenu_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gmenu = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gmenu", module_number);

	memcpy(&gmenu_object_handlers, gmenushell_get_object_handlers(), sizeof(zend_object_handlers));

	INIT_CLASS_ENTRY(ce, "GMenu", gmenu_class_functions);
	gmenu_class_entry_ce = zend_register_internal_class_ex(&ce, gmenushell_get_class_entry());
}


