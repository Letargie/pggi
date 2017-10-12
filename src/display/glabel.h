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


#ifndef __GLABEL_DEF__
#define __GLABEL_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gwidget.h"

zend_class_entry * glabel_get_class_entry(void);

/*******************/
/* GLabel Arginfos */
/*******************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_glabel_construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 1)
ZEND_END_ARG_INFO()

/**********************/
/* GWindow Properties */
/**********************/

#define GLABEL_TRACK_VISITED_LINKS "trackVisitedLinks"
#define GLABEL_USE_MARKUP          "useMarkup"
#define GLABEL_USE_UNDERLINE       "useUnderline"
#define GLABEL_XALIGN              "xAlign"
#define GLABEL_YALIGN              "yAlign"
#define GLABEL_ELLIPSIZE           "ellipsize"
#define GLABEL_WIDTH_CHARS         "widthChars"
#define GLABEL_MAX_WIDTH_CHARS     "maxWidthChars"
#define GLABEL_LINES               "lines"
#define GLABEL_SELECTABLE          "selectable"
#define GLABEL_TEXT                "text"

/***************/
/* PHP Methods */
/***************/

#define GLABEL_METHOD(name) \
PHP_METHOD(GLabel, name)

GLABEL_METHOD(__construct);


/*****************************/
/* Object handling functions */
/*****************************/

zval *glabel_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *glabel_get_properties(zval *object);

void glabel_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/********************************/
/* GWindow class initialisation */
/********************************/

void glabel_init(int module_number);

#endif
