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


#ifndef __GWIDGET_DEF__
#define __GWIDGET_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"

/*********************************/
/* GWidget Intern Data Structure */
/*********************************/

typedef struct{
	GtkWidget * intern;
	zval signals;
	zval data;
} * gwidget_ptr, gwidget_t;

typedef struct{
	gwidget_ptr widget_ptr;
	zend_object std;
} ze_gwidget_object;

/**********************/
/* GWidget properties */
/**********************/

#define GWIDGET_VALIGN			"vAlign"
#define GWIDGET_HALIGN		 	"hAlign"
#define GWIDGET_MARGIN_START	"marginStart"
#define GWIDGET_MARGIN_END		"marginEnd"
#define GWIDGET_MARGIN_TOP		"marginTop"
#define GWIDGET_MARGIN_BOTTOM	"marginBottom"
#define GWIDGET_HEXPAND			"hExpand"
#define GWIDGET_HEXPAND_SET		"hExpandSet"
#define GWIDGET_VEXPAND			"vExpand"
#define GWIDGET_VEXPAND_SET		"vExpandSet"
#define GWIDGET_VISIBLE			"visible"
#define GWIDGET_NO_SHOW_ALL		"noShowAll"
#define GWIDGET_DIRECTION		"direction"
#define GWIDGET_SENSITIVE		"sensitive"
#define GWIDGET_NAME			"name"
#define GWIDGET_CAN_DEFAULT		"canDefault"
#define GWIDGET_CAN_FOCUS		"canFocus"
#define GWIDGET_HAS_TOOLTIP		"hasTooltip"
#define GWIDGET_OPACITY			"opacity"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the GWidget class entry
 */
zend_class_entry * gwidget_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the GWidget object handler
 */
zend_object_handlers * gwidget_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

/*==========================================================================*/
/** 
 * this function need to be called when a widget is detroyed
 */
void widget_destructed(GtkWidget* this, gpointer data);

gwidget_ptr gwidget_new(void);

static inline ze_gwidget_object *php_gwidget_fetch_object(zend_object *obj) {
	return (ze_gwidget_object *)((char*)(obj) - XtOffsetOf(ze_gwidget_object, std));
}

inline zend_object *php_gwidget_reverse_object(ze_gwidget_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gwidget_ptr));
}

#define Z_GWIDGET_P(zv) php_gwidget_fetch_object(Z_OBJ_P((zv)))

zend_object *gwidget_object_new(zend_class_entry *class_type);

void gwidget_dtor(gwidget_ptr intern);

void gwidget_object_free_storage(zend_object *object);
void gwidget_free_resource(zend_resource *rsrc);

/*********************************/
/* internal on-related functions */
/*********************************/

typedef struct _on_data{
	zval 	* function,
			* data;
} * on_data_ptr, on_data_t;

void gwidget_func_destroy(GtkWidget* w, gpointer data);

void gwidget_adding_function(long val, char * name, void (*f)(GtkWidget *, void *) ,ze_gwidget_object * ze_obj, zval * function, zval * param);

void gwidget_function(gpointer data, unsigned int type);
void gwidget_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param);

/***************/
/* PHP Methods */
/***************/

#define GWIDGET_METHOD(name) \
PHP_METHOD(GWidget, name)


/**
 * TEST
 */
GWIDGET_METHOD(on			);
GWIDGET_METHOD(__construct	);
GWIDGET_METHOD(show			);
GWIDGET_METHOD(hide			);
GWIDGET_METHOD(showAll		);

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
PGGI_READ_PROPERTY(gwidget);
//zval *gwidget_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *gwidget_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void gwidget_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/********************************/
/* GWidget Class Initialization */
/********************************/

/*==========================================================================*/
/** 
 * init the GWidget class
 */
void gwidget_init(int module_number);

/* __GWIDGET_DEF__ */

#endif
