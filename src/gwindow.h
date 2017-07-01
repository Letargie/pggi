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


#ifndef __GWINDOW_DEF__
#define __GWINDOW_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gapplication.h"
#include "gcontainer.h"

/********************/
/* GWindow Arginfos */
/********************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gwindow_set_default_size, 0, 0, 2)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gwindow_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, app)
	ZEND_ARG_INFO(0, title)
ZEND_END_ARG_INFO()

/**********************/
/* GWindow Properties */
/**********************/

#define GWINDOW_TITLE							"title"
#define GWINDOW_FOCUS_VISIBLE					"focusVisible"
#define GWINDOW_FOCUS_ON_MAP					"focusOnMap"
#define GWINDOW_DECORATED						"decorated"
#define GWINDOW_DELETABLE						"deletable"
#define GWINDOW_MNEMONICS_VISIBLE				"mnemonicsVisible"
#define GWINDOW_RESIZABLE						"resizable"
#define GWINDOW_DEFAULT_HEIGHT					"defaultHeight"
#define GWINDOW_DEFAULT_WIDTH					"defaultWidth"
#define GWINDOW_HIDE_TITLEBAR_WHEN_MAXIMIZED	"hideTitlebarWhenMaximized"

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gwindow_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable *row_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
void gwindow_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GWINDOW_METHOD(name) \
PHP_METHOD(GWindow, name)

GWINDOW_METHOD(__construct		);
GWINDOW_METHOD(setTitle			);
GWINDOW_METHOD(setDefaultSize	);
GWINDOW_METHOD(close			);

/********************************/
/* GWindow class initialisation */
/********************************/

void gwindow_init(int module_number);

#endif
