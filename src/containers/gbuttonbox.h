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


#ifndef __G_BUTTON_BOX_DEF__
#define __G_BUTTON_BOX_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gbox.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_gbuttonbox_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, orientation)
ZEND_END_ARG_INFO()


PHP_METHOD(GButtonBox, __construct);
PHP_METHOD(GButtonBox, getLayout);
PHP_METHOD(GButtonBox, setLayout);

void gbuttonbox_init(int module_number);

#endif
