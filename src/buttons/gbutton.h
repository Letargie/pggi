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


#ifndef __GBUTTON_DEF__
#define __GBUTTON_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../applications/gapplication.h"
#include "../abstracts/gcontainer.h"


#define GBUTTON_ALWAYS_SHOW_IMAGE "alwaysShowImage"
#define GBUTTON_IMAGE             "image"
#define GBUTTON_IMAGE_POSITION    "imagePosition"
#define GBUTTON_LABEL             "label"
#define GBUTTON_RELIEF            "relief"
#define GBUTTON_USE_UNDERLINE     "useUnderline"

PHP_METHOD(GButton, __construct);

zval *gbutton_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);
HashTable *gbutton_get_properties(zval *object);
PHP_WRITE_PROP_HANDLER_TYPE gbutton_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void gbutton_init(int module_number);

#endif
