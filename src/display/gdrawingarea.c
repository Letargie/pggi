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


#include "gdrawingarea.h"

static int le_gdrawing_area;
static zend_object_handlers gdrawing_area_object_handlers;
static zend_class_entry * gdrawing_area_class_entry_ce;


zend_class_entry * gdrawing_area_get_class_entry(){
	return gdrawing_area_class_entry_ce;
}

zend_object_handlers * gdrawing_area_get_object_handlers(){
	return &gdrawing_area_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

GDRAWING_AREA_METHOD(__construct){
	ze_gwidget_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GWIDGET_P(self);
	ze_obj->widget_ptr = gwidget_new();
	ze_obj->widget_ptr->intern = gtk_drawing_area_new();
	g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), Z_GWIDGET_P(self));
}

/**
 * List of GDrawingArea functions and methods with their arguments
 */
static const zend_function_entry gdrawing_area_class_functions[] = {
	PHP_ME(GDrawingArea, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*************************************/
/* GDrawingArea class initialisation */
/*************************************/

void gdrawing_area_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gdrawing_area = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GDrawingArea", module_number);

	memcpy(&gdrawing_area_object_handlers, gwidget_get_object_handlers(), sizeof(zend_object_handlers));

	INIT_CLASS_ENTRY(ce, "PGGI\\GDrawingArea", gdrawing_area_class_functions);
	gdrawing_area_class_entry_ce = zend_register_internal_class_ex(&ce, gwidget_get_class_entry());
}


