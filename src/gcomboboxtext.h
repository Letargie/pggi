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


#ifndef __GCOMBO_BOX_TEXT_DEF__
#define __GCOMBO_BOX_TEXT_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gcombobox.h"

/***************************/
/* GComboBoxText: Arginfos */
/***************************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gcombo_box_text_remove, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gcombo_box_text_insert, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gcombo_box_text_pend, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 1)
ZEND_END_ARG_INFO()

/****************************/
/* GComboBoxText Properties */
/****************************/

/*****************************/
/* Object handling functions */
/*****************************/

/***************/
/* PHP Methods */
/***************/

#define GCOMBO_BOX_TEXT_METHOD(name) \
PHP_METHOD(GComboBoxText, name)

GCOMBO_BOX_TEXT_METHOD(__construct);
GCOMBO_BOX_TEXT_METHOD(append);
GCOMBO_BOX_TEXT_METHOD(prepend);
GCOMBO_BOX_TEXT_METHOD(insert);
GCOMBO_BOX_TEXT_METHOD(remove);
GCOMBO_BOX_TEXT_METHOD(removeAll);
GCOMBO_BOX_TEXT_METHOD(getActiveText);

/**************************************/
/* GComboBoxText class initialisation */
/**************************************/

void gcombo_box_text_init(int module_number);

#endif
