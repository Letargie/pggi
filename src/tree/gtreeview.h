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


#ifndef __GTREE_VIEW_DEF__
#define __GTREE_VIEW_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gcontainer.h"
#include "gtreestore.h"
#include "gtreeviewcolumn.h"
#include "gtreeselection.h"

/**********************/
/* GTreeView Arginfos */
/**********************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_view_scroll_to_point, 0, 0, 2)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gtree_view_pend, 0, 0, 1)
	ZEND_ARG_INFO(0, column)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_gtree_view_get_selection, 0, 0, gtree_selection_get_class_entry(), 0)
ZEND_END_ARG_INFO()

/************************/
/* GTreeView Properties */
/************************/

#define GTREE_VIEW_ACTIVATE_ON_SINGLE_CLICK "activateOnSingleClick"
#define GTREE_VIEW_ENABLE_GRID_LINES        "enableGridLines"
#define GTREE_VIEW_ENABLE_SEARCH            "enableSearch"
#define GTREE_VIEW_ENABLE_TREE_LINES        "enableTreeLines"
//#define GTREE_VIEW_EXPANDER_COLUMN        "expanderColumn"
#define GTREE_VIEW_FIXED_HEIGHT_MODE        "fixedHeightMode"
#define GTREE_VIEW_HEADERS_CLICKABLE        "headersClickable"
#define GTREE_VIEW_HEADERS_VISIBLE          "headersVisible"
#define GTREE_VIEW_HOVER_EXPAND             "hoverExpand"
#define GTREE_VIEW_HOVER_SELECTION          "hoverSelection"
#define GTREE_VIEW_LEVEL_INDENTATION        "levelIndentation"
#define GTREE_VIEW_MODEL                    "model"
#define GTREE_VIEW_REORDERABLE              "reorderable"
#define GTREE_VIEW_RUBBER_BANDING           "rubberBanding"
#define GTREE_VIEW_SEARCH_COLUMN            "searchColumn"
#define GTREE_VIEW_SHOW_EXPANDERS           "showExpanders"
#define GTREE_VIEW_TOOLTIP_COLUMN           "tooltipColumn"

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gtree_view_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gtree_view_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gtree_view_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GTREE_VIEW_METHOD(name) \
PHP_METHOD(GTreeView, name)

GTREE_VIEW_METHOD(__construct  );
GTREE_VIEW_METHOD(on           );
GTREE_VIEW_METHOD(scrollToPoint);
GTREE_VIEW_METHOD(expandAll    );
GTREE_VIEW_METHOD(collapseAll  );
GTREE_VIEW_METHOD(getSelection );

/**********************************/
/* GTreeView class initialisation */
/**********************************/

void gtree_view_init(int module_number);

#endif
