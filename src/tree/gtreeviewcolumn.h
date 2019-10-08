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


#ifndef __GTREE_VIEW_COLUMN_DEF__
#define __GTREE_VIEW_COLUMN_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "gcellrenderer.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_view_column_add_cell_renderer_text, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, renderer, PGGI\\GCellRenderer, 0)
	ZEND_ARG_INFO(0, col)
ZEND_END_ARG_INFO()


/*************************************/
/* GTreeViewColumn Intern Data Structure */
/*************************************/

typedef struct{
	GtkTreeViewColumn * intern;
	zval signals;
} * gtree_view_column_ptr, gtree_view_column_t;

typedef struct{
	gtree_view_column_ptr tree_view_column_ptr;
	zend_object std;
} ze_gtree_view_column_object;

/**************************/
/* GTreeViewColumn properties */
/**************************/

#define GTREE_VIEW_COLUMN_SPACING     "spacing"
#define GTREE_VIEW_COLUMN_VISIBLE     "visible"
#define GTREE_VIEW_COLUMN_RESIZABLE   "resizable"
#define GTREE_VIEW_COLUMN_SIZING      "sizing"
#define GTREE_VIEW_COLUMN_FIXED_WIDTH "fixedWidth"
#define GTREE_VIEW_COLUMN_MIN_WIDTH   "minWidth"
#define GTREE_VIEW_COLUMN_MAX_WIDTH   "maxWidth"
#define GTREE_VIEW_COLUMN_TITLE       "title"
#define GTREE_VIEW_COLUMN_EXPAND      "expand"
#define GTREE_VIEW_COLUMN_CLICKABLE   "clickable"
#define GTREE_VIEW_COLUMN_ALIGNMENT   "alignment"
#define GTREE_VIEW_COLUMN_REORDERABLE "reorderable"
//#define GTREE_VIEW_COLUMN_WIDGET "widget"

/*****************************/
/* Class information getters */
/*****************************/

zend_class_entry * gtree_view_column_get_class_entry(void);

zend_object_handlers * gtree_view_column_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

static inline ze_gtree_view_column_object *php_gtree_view_column_fetch_object(zend_object *obj) {
	return (ze_gtree_view_column_object *)((char*)(obj) - XtOffsetOf(ze_gtree_view_column_object, std));
}

static inline zend_object *php_gtree_view_column_reverse_object(ze_gtree_view_column_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gtree_view_column_ptr));
}

#define Z_GTREE_VIEW_COLUMN_P(zv) php_gtree_view_column_fetch_object(Z_OBJ_P((zv)))

zend_object *gtree_view_column_object_new(zend_class_entry *class_type);

void gtree_view_column_dtor(gtree_view_column_ptr intern);

void gtree_view_column_object_free_storage(zend_object *object);
void gtree_view_column_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

void gtree_view_column_func_clicked(GtkTreeViewColumn* tree, gpointer data);

void gtree_view_column_function(gpointer data, unsigned int type);

/***************/
/* PHP Methods */
/***************/

#define GTREE_VIEW_COLUMN_METHOD(name) \
PHP_METHOD(GTreeViewColumn, name)

GTREE_VIEW_COLUMN_METHOD(on             );
GTREE_VIEW_COLUMN_METHOD(__construct    );
GTREE_VIEW_COLUMN_METHOD(packStart      );
GTREE_VIEW_COLUMN_METHOD(packEnd        );
GTREE_VIEW_COLUMN_METHOD(clear          );
GTREE_VIEW_COLUMN_METHOD(addAttribute   );
GTREE_VIEW_COLUMN_METHOD(clearAttributes);
GTREE_VIEW_COLUMN_METHOD(clicked        );
GTREE_VIEW_COLUMN_METHOD(cellIsVisible  );

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gtree_view_column_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gtree_view_column_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gtree_view_column_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************************/
/* GTreeViewColumn Class Initialization */
/************************************/

void gtree_view_column_init(int module_number);

/* __GTREE_VIEW_COLUMN_DEF__ */

#endif
