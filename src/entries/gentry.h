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

#ifndef __GENTRY_DEF__
#define __GENTRY_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gwidget.h"
#include "gentrybuffer.h"

zend_class_entry * gentry_get_class_entry(void);

zend_object_handlers * gentry_get_object_handlers(void);

/*******************/
/* GEntry Arginfos */
/*******************/


ZEND_BEGIN_ARG_INFO_EX(arginfo_gentry_construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, widget, PGGI\\GEntryBuffer, 0)
ZEND_END_ARG_INFO()


/*********************/
/* GEntry Properties */
/*********************/

#define GENTRY_BUFFER              "buffer"
#define GENTRY_TEXT                "text"
#define GENTRY_VISIBILITY          "visibility"
#define GENTRY_INVISIBLE_CHAR      "invisibleChar"
#define GENTRY_MAX_LENGTH          "maxLength"
#define GENTRY_ACTIVATES_DEFAULT   "activatesDefault"
#define GENTRY_HAS_FRAME           "hasFrame"
#define GENTRY_WIDTH_CHARS         "widthChars"
#define GENTRY_MAX_WIDTH_CHARS     "maxWidthChars"
#define GENTRY_ALIGNMENT           "aligment"
#define GENTRY_PLACEHOLDER_TEXT    "placeholderText"
#define GENTRY_OVERWRITE_MODE      "overwriteMode"
#define GENTRY_PROGRESS_FRACTION   "progressFraction"
#define GENTRY_PROGRESS_PULSE_STEP "progressPulseStep"


/*****************/
/* PHP FUNCTIONS */
/*****************/

#define GENTRY_METHOD(name) \
PHP_METHOD(GEntry, name)

GENTRY_METHOD(__construct); // | buffer
GENTRY_METHOD(on);
GENTRY_METHOD(getTextLength);
GENTRY_METHOD(unsetInvisibleChar);
GENTRY_METHOD(progressPulse);

void gentry_func_activate(GtkWidget* entry, gpointer data);

void gentry_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param);

/*****************************/
/* Object handling functions */
/*****************************/

zval *gentry_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *gentry_get_properties(zval *object);

PHP_WRITE_PROP_HANDLER_TYPE gentry_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/*******************************/
/* GEntry class initialisation */
/*******************************/

void gentry_init(int module_number);

#endif

