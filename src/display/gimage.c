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


#include "gimage.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gimage;
static zend_object_handlers gimage_object_handlers;
static zend_class_entry * gimage_class_entry_ce;

zend_class_entry * gimage_get_class_entry(){
	return gimage_class_entry_ce;
}

/***************/
/* PHP Methods */
/***************/

GIMAGE_METHOD(__construct){
	zend_string * image = NULL;
	ze_gwidget_object * ze_obj;
	zval * self = getThis();
	if(!self)
		RETURN_NULL();
	ze_obj = Z_GWIDGET_P(getThis());
	ze_obj->std.handlers = &gimage_object_handlers;
	char * c = NULL;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &image) == FAILURE)
		return;
	c = ZSTR_VAL(image);
	ze_obj->widget_ptr = gwidget_new();
	ze_obj->widget_ptr->intern = gtk_image_new_from_file(c);
	g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), ze_obj);
}

GIMAGE_METHOD(set){
	zend_string * image = NULL;
	ze_gwidget_object * ze_obj;
	zval * self = getThis();
	ze_obj = Z_GWIDGET_P(self);
	char * c = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &image) == FAILURE)
		return;
	c = ZSTR_VAL(image);
	gtk_image_set_from_file(GTK_IMAGE(ze_obj->widget_ptr->intern), c);
}

static const zend_function_entry gimage_class_functions[] = {
	PHP_ME(GImage, set        , arginfo_pggi_set_string , ZEND_ACC_PUBLIC)
	PHP_ME(GImage, __construct, arginfo_gimage_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*******************************/
/* GImage Class Initialization */
/*******************************/

void gimage_init(int module_number){
	zend_class_entry ce;
	le_gimage = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GImage", module_number);

	memcpy(&gimage_object_handlers, gwidget_get_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "PGGI\\GImage", gimage_class_functions);
	ce.create_object = gwidget_object_new;
	gimage_class_entry_ce = zend_register_internal_class_ex(&ce, gwidget_get_class_entry());
}


