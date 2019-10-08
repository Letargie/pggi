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


#include "gexception.h"

static zend_class_entry * gexception_entry_ce;

zend_class_entry * pggi_exception_get(){
	return gexception_entry_ce;
}

void gexception_init(int module_number){
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "PGGI\\Exception", NULL);
	gexception_entry_ce = zend_register_internal_class_ex(&ce, zend_ce_exception);

}


