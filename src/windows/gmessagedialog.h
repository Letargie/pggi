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


#ifndef __GMESSAGE_DIALOG_DEF__
#define __GMESSAGE_DIALOG_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gdialog.h"

/***************************/
/* GMessageDialog Arginfos */
/***************************/

/*****************************/
/* GMessageWindow Properties */
/*****************************/


/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gmessage_dialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gmessage_dialog_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gmessage_dialog_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GMESSAGE_DIALOG_METHOD(name) \
PHP_METHOD(GMessageDialog, name)

GMESSAGE_DIALOG_METHOD(__construct);

/***************************************/
/* GMessageDialog class initialisation */
/***************************************/

void gmessage_dialog_init(int module_number);

#endif
