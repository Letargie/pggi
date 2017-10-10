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


#include "exception.h"

static zend_class_entry * pc_exception_entry_ce;

zend_class_entry * pc_exception_get(){
	return pc_exception_entry_ce;
}

void pc_exception_init(int module_number){
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Cairo\\Exception", NULL);
	pc_exception_entry_ce = zend_register_internal_class_ex(&ce, zend_ce_exception);

}

void pc_exception(cairo_status_t status){
	if(status == CAIRO_STATUS_SUCCESS)
		return;
	char * error_message = estrdup(cairo_status_to_string(status));
	zend_throw_exception(pc_exception_entry_ce, error_message, status);
	efree(error_message);
	return;
}

void pc_trigger_error(cairo_status_t status){
	if(status == CAIRO_STATUS_SUCCESS)
		return;
	char * error_message = estrdup(cairo_status_to_string(status));
	php_error_docref(NULL, E_WARNING, "%s", error_message);
	efree(error_message);
	return;
}
