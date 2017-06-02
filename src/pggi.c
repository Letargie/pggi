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

#include "pggi.h"

static const zend_function_entry pggi_functions[] = {
	PHP_FE_END
};

PHP_MINIT_FUNCTION(pggi){
	gapplication_init	(module_number);
	gwidget_init		(module_number);
	gwindow_init		(module_number);
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pggi){
	return SUCCESS;
}

PHP_MINFO_FUNCTION(pggi){
	php_info_print_table_start();{
		php_info_print_table_row(2, "PGGI", "enabled");
	}
	php_info_print_table_end();
}

zend_module_entry pggi_module_entry = {
	STANDARD_MODULE_HEADER,
	"pggi",
	pggi_functions,
	PHP_MINIT(pggi),
	PHP_MSHUTDOWN(pggi),
	NULL,
	NULL,
	PHP_MINFO(pggi),
	PGGI_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_XMLWRITER
ZEND_GET_MODULE(pggi)
#endif

