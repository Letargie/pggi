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


#include "gcellrenderertext.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gcell_renderer_text;
static zend_object_handlers gcell_renderer_text_object_handlers;
static zend_class_entry * gcell_renderer_text_class_entry_ce;

zend_class_entry * gcell_renderer_text_get_class_entry(){
	return gcell_renderer_text_class_entry_ce;
}

zend_object_handlers * gcell_renderer_text_get_object_handlers(){
	return &gcell_renderer_text_object_handlers;
}

/***************/
/* PHP Methods */
/***************/


GCELL_RENDERER_TEXT_METHOD(__construct){
	ze_gcell_renderer_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GCELL_RENDERER_P(self);
	ze_obj->cell_renderer_ptr = gcell_renderer_new();
	ze_obj->cell_renderer_ptr->intern = gtk_cell_renderer_text_new();
}


static const zend_function_entry gcell_renderer_text_class_functions[] = {
	PHP_ME(GCellRendererText, __construct, arginfo_pggi_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

/************************************/
/* GCellRendererText Class Initialization */
/************************************/


void gcell_renderer_text_init(int module_number){
	zend_class_entry ce;
	le_gcell_renderer_text = zend_register_list_destructors_ex(gcell_renderer_free_resource, NULL, "PGGI\\GCellRendererText", module_number);

	memcpy(&gcell_renderer_text_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	INIT_CLASS_ENTRY(ce, "PGGI\\GCellRendererText", gcell_renderer_text_class_functions);
	ce.create_object = gcell_renderer_object_new;
	gcell_renderer_text_class_entry_ce = zend_register_internal_class_ex(&ce, gcell_renderer_get_class_entry());

}


