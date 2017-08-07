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


#include "gmenubar.h"

static int le_gmenubar;
static zend_object_handlers gmenubar_object_handlers;
static zend_class_entry * gmenubar_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GMENUBAR_METHOD(__construct){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gmenubar_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_menu_bar_new();
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

/**
 * List of GMenubar functions and methods with their arguments
 */
static const zend_function_entry gmenubar_class_functions[] = {
	PHP_ME(GMenubar, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/******************************/
/* GMenubar class initialisation */
/******************************/

#define DECLARE_GMENUBAR_PROP(name) \
DECLARE_CLASS_PROPERTY(gmenubar_class_entry_ce, name)

void gmenubar_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gmenubar = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gmenubar", module_number);

	memcpy(&gmenubar_object_handlers, gmenushell_get_object_handlers(), sizeof(zend_object_handlers));

	INIT_CLASS_ENTRY(ce, "GMenubar", gmenubar_class_functions);
	gmenubar_class_entry_ce = zend_register_internal_class_ex(&ce, gmenushell_get_class_entry());
}


