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


#include "gspinner.h"


static int le_gspinner;
static zend_object_handlers gspinner_object_handlers;
static zend_class_entry * gspinner_class_entry_ce;

zend_class_entry * gspinner_get_class_entry(){
	return gspinner_class_entry_ce;
}

zend_object_handlers * gspinner_get_object_handlers(){
	return &gspinner_object_handlers;
}

/*****************/
/* PHP FUNCTIONS */
/*****************/

PHP_METHOD(GSpinner, __construct){
	ze_gwidget_object * ze_obj;
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_GWIDGET_P(getThis());
	ze_obj->widget_ptr = gwidget_new();
	ze_obj->widget_ptr->intern = gtk_spinner_new();
}

PHP_METHOD(GSpinner, stop){
	ze_gwidget_object * this;
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	this = Z_GWIDGET_P(getThis());
	gtk_spinner_stop(GTK_SPINNER(this->widget_ptr->intern));
}

PHP_METHOD(GSpinner, start){
	ze_gwidget_object * this;
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	this = Z_GWIDGET_P(getThis());
	gtk_spinner_start(GTK_SPINNER(this->widget_ptr->intern));
}

PHP_METHOD(GSpinner, isActive){
	ze_gwidget_object * this;
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	this = Z_GWIDGET_P(getThis());
	GValue tmp = G_VALUE_INIT;
	g_value_init (&tmp, G_TYPE_BOOLEAN);
	g_object_get(G_OBJECT(this->widget_ptr->intern),"active", &tmp);
	RETURN_BOOL(g_value_get_boolean(&tmp));
}



static const zend_function_entry gspinner_class_functions[] = {
	PHP_ME(GSpinner, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GSpinner, start      , arginfo_pggi_void, ZEND_ACC_PUBLIC)
	PHP_ME(GSpinner, stop       , arginfo_pggi_void, ZEND_ACC_PUBLIC)
	PHP_ME(GSpinner, isActive   , arginfo_pggi_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/***********************************/
/* GSpinner class initialisation */
/***********************************/


void gspinner_init(int module_number){
	zend_class_entry ce;
	le_gspinner = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GSpinner", module_number);

	memcpy(&gspinner_object_handlers, gwidget_get_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "PGGI\\GSpinner", gspinner_class_functions);
	ce.create_object = gwidget_object_new;
	gspinner_class_entry_ce = zend_register_internal_class_ex(&ce, gwidget_get_class_entry());
}


