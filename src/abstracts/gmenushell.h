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


#ifndef __GMENUSHELL_DEF__
#define __GMENUSHELL_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gcontainer.h"

zend_class_entry * gmenushell_get_class_entry();
zend_object_handlers * gmenushell_get_object_handlers();

/***********************/
/* GMenuShell Arginfos */
/***********************/

/*************************/
/* GMenuShell Properties */
/*************************/

#define GMENUSHELL_TAKE_FOCUS "takeFocus"

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gmenushell_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable *row_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
void gmenushell_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GMENUSHELL_METHOD(name) \
PHP_METHOD(GMenuShell, name)

GMENUSHELL_METHOD(__construct );
GMENUSHELL_METHOD(append      );
GMENUSHELL_METHOD(prepend     );
GMENUSHELL_METHOD(insert      );
GMENUSHELL_METHOD(deactivate  );
GMENUSHELL_METHOD(selectItem  );
GMENUSHELL_METHOD(selectFirst );
GMENUSHELL_METHOD(deselect    );
GMENUSHELL_METHOD(activateItem);
GMENUSHELL_METHOD(cancel      );

/***********************************/
/* GMenuShell class initialisation */
/***********************************/

void gmenushell_init(int module_number);

#endif
