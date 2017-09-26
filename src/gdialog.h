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


#ifndef __GDIALOG_DEF__
#define __GDIALOG_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gwindow.h"

zend_class_entry * gdialog_get_class_entry();

zend_object_handlers * gdialog_get_object_handlers();

/********************/
/* GDialog Arginfos */
/********************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gdialog_construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 1)
	ZEND_ARG_OBJ_INFO(0, parent, PGGI\\GWindow, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/**********************/
/* GWindow Properties */
/**********************/

#define GDIALOG_FLAG_MODAL                  GTK_DIALOG_MODAL
#define GDIALOG_FLAG_DESTROY_WITH_PARENT    GTK_DIALOG_DESTROY_WITH_PARENT
#define GDIALOG_FLAG_USE_HEADER_BAR         GTK_DIALOG_USE_HEADER_BAR

#define GDIALOG_RESPONSE_TYPE_NONE          GTK_RESPONSE_NONE
#define GDIALOG_RESPONSE_TYPE_REJECT        GTK_RESPONSE_REJECT
#define GDIALOG_RESPONSE_TYPE_ACCEPT        GTK_RESPONSE_ACCEPT
#define GDIALOG_RESPONSE_TYPE_DELETE_EVENT  GTK_RESPONSE_DELETE_EVENT
#define GDIALOG_RESPONSE_TYPE_OK            GTK_RESPONSE_OK
#define GDIALOG_RESPONSE_TYPE_CANCEL        GTK_RESPONSE_CANCEL
#define GDIALOG_RESPONSE_TYPE_CLOSE         GTK_RESPONSE_CLOSE
#define GDIALOG_RESPONSE_TYPE_YES           GTK_RESPONSE_YES
#define GDIALOG_RESPONSE_TYPE_NO            GTK_RESPONSE_NO
#define GDIALOG_RESPONSE_TYPE_APPLY         GTK_RESPONSE_APPLY
#define GDIALOG_RESPONSE_TYPE_HELP          GTK_RESPONSE_HELP

#define GDIALOG_USE_HEADER_BAR  "useHeaderBar"
#define GDIALOG_BUTTON_SPACING  "buttonSpacing"
#define GDIALOG_AREA_BORDER     "areaBorder"
#define GDIALOG_AREA_SPACING    "areaSpacing"

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gdialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gdialog_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
void gdialog_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GDIALOG_METHOD(name) \
PHP_METHOD(GDialog, name)

GDIALOG_METHOD(__construct         );
GDIALOG_METHOD(run                 );
GDIALOG_METHOD(response            );
GDIALOG_METHOD(addButton           );
GDIALOG_METHOD(addButtons          );
GDIALOG_METHOD(setDefaultResponse  );
GDIALOG_METHOD(setResponseSensitive);
GDIALOG_METHOD(getResponseForWidget);
GDIALOG_METHOD(getWidgetForResponse);
GDIALOG_METHOD(getContentArea      );
GDIALOG_METHOD(getHeaderBar        );


/********************************/
/* GDialog class initialisation */
/********************************/

void gdialog_init(int module_number);

#endif
