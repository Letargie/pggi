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


#ifndef __GMENUITEM_DEF__
#define __GMENUITEM_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../applications/gapplication.h"
#include "../abstracts/gcontainer.h"

/**********************/
/* GMenuItem Arginfos */
/**********************/


ZEND_BEGIN_ARG_INFO_EX(arginfo_gmenuitem_construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 1)
ZEND_END_ARG_INFO()


/************************/
/* GMenuItem Properties */
/************************/

#define GMENUITEM_ACCEL_PATH        "accelPath"
#define GMENUITEM_LABEL             "label"
#define GMENUITEM_SUBMENU           "submenu"
#define GMENUITEM_USE_UNDERLINE     "useUnderline"
#define GMENUITEM_RESERVE_INDICATOR "reserveIndicator"

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gmenuitem_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable *gmenuitem_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
void gmenuitem_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

void gmenuitem_func_activate(GtkWidget* w, gpointer data);

void gmenuitem_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param);

#define GMENUITEM_METHOD(name) \
PHP_METHOD(GMenuItem, name)

GMENUITEM_METHOD(on);

GMENUITEM_METHOD(__construct);
GMENUITEM_METHOD(select     );
GMENUITEM_METHOD(deselect   );
GMENUITEM_METHOD(activate   );
//GMENUITEM_METHOD(toggle_size_request);
//GMENUITEM_METHOD(toggle_size_allocate);

/***********************************/
/* GMenuItem class initialisation */
/***********************************/

void gmenuitem_init(int module_number);

#endif
