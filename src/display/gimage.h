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


#ifndef __GIMAGE_DEF__
#define __GIMAGE_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gwidget.h"

/*******************/
/* GImage Arginfos */
/*******************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gimage_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, image)
ZEND_END_ARG_INFO()

/*****************************/
/* Class information getters */
/*****************************/


zend_class_entry * gimage_get_class_entry(void);

/***************/
/* PHP Methods */
/***************/

#define GIMAGE_METHOD(name) \
PHP_METHOD(GImage, name)

GIMAGE_METHOD(__construct);
GIMAGE_METHOD(set        );

/*******************************/
/* GImage Class Initialization */
/*******************************/

void gimage_init(int module_number);

/* __GIMAGE_DEF__*/

#endif
