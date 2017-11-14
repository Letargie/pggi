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


#ifndef __GAPPLICATION_DEF__
#define __GAPPLICATION_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../abstracts/gwidget.h"

zend_class_entry * gapplication_get_class_entry();

zend_object_handlers * gapplication_get_object_handlers();

/************************************/
/* GApplication internal structures */
/************************************/

typedef struct{
	GtkApplication * app;
	zval signals;
	zval windows;
} * gapplication_ptr, gapplication_len;

typedef struct{
	gapplication_ptr app_ptr;
	zend_object std;
} ze_gapplication_object;


/*********************************/
/* GApplication memory functions */
/*********************************/

static inline ze_gapplication_object *php_gapplication_fetch_object(zend_object *obj) {
	return (ze_gapplication_object *)((char*)(obj) - XtOffsetOf(ze_gapplication_object, std));
}

static inline zend_object *php_gapplication_reverse_object(ze_gapplication_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(gapplication_ptr));
}

#define Z_GAPPLICATION_P(zv) php_gapplication_fetch_object(Z_OBJ_P((zv)))

void gapplication_add_windows(gapplication_ptr intern, zval * window);

gapplication_ptr gapplication_ctor();

void gapplication_dtor(gapplication_ptr intern);
zend_object * gapplication_object_new(zend_class_entry *class_type);
void gapplication_object_free_storage(zend_object *object);
void gapplication_free_resource(zend_resource *rsrc);

/********************************/
/* GApplication signal handling */
/********************************/

void gapplication_function(gpointer data, unsigned int type);
void gapplication_func_activate(GtkApplication* app, gpointer data);
void gapplication_func_startup (GtkApplication* app, gpointer data);
void gapplication_func_shutdown(GtkApplication* app, gpointer data);

/***************/
/* PHP METHODS */
/***************/

#define GAPPLICATION_METHOD(name) \
PHP_METHOD(GApplication,name)

GAPPLICATION_METHOD(__construct);
GAPPLICATION_METHOD(run        );
GAPPLICATION_METHOD(on         );
GAPPLICATION_METHOD(quit       );
GAPPLICATION_METHOD(hold       );

/******************************/
/* GApplication Initilializer */
/******************************/

void gapplication_init(int module_number);


#endif
