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


#ifndef __GNOTEBOOK_DEF__
#define __GNOTEBOOK_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gcontainer.h"

zend_class_entry * gnotebook_get_class_entry();

zend_object_handlers * gnotebook_get_object_handlers();

ZEND_BEGIN_ARG_INFO_EX(arginfo_gnotebook_pend_page, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, child, PGGI\\GWidget, 0)
	ZEND_ARG_OBJ_INFO(0, tabLabel, PGGI\\GWidget, 1)
	ZEND_ARG_OBJ_INFO(0, menuLabel, PGGI\\GWidget, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gnotebook_insert_page, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, child, PGGI\\GWidget, 0)
	ZEND_ARG_TYPE_INFO(0, pos, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, tabLabel, PGGI\\GWidget, 1)
	ZEND_ARG_OBJ_INFO(0, menuLabel, PGGI\\GWidget, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gnotebook_detach_tab, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, child, PGGI\\GWidget, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_gnotebook_page_num, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, child, PGGI\\GWidget, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gnotebook_reorder_child, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, child, PGGI\\GWidget, 0)
	ZEND_ARG_TYPE_INFO(0, pos, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define GNOTEBOOK_ENABLE_POPUP "enablePopup"
#define GNOTEBOOK_GROUP_NAME   "groupName"
#define GNOTEBOOK_PAGE         "page"
#define GNOTEBOOK_SCROLLABLE   "scrollable"
#define GNOTEBOOK_SHOW_BORDER  "showBorder"
#define GNOTEBOOK_SHOW_TABS    "showTabs"
#define GNOTEBOOK_TAB_POS      "tabPos"

PHP_METHOD(GNotebook, __construct);

zval *gnotebook_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

HashTable *gnotebook_get_properties(zval *object);

void gnotebook_write_property(zval *object, zval *member, zval *value, void **cache_slot);

void gnotebook_init(int module_number);

#endif
