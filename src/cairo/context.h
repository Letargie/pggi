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

#ifndef PHP_CAIRO_CONTEXT_DEF
#define PHP_CAIRO_CONTEXT_DEF

#include <cairo.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "exception.h"
#include "../gdk/rgba.h"
#include "surface.h"

/*
typedef struct _cairo_context_object {
	zend_object std;
	zval *surface;
	zval *matrix;
	zval *pattern;
	zval *font_face;
	zval *font_matrix;
	zval *font_options;
	zval *scaled_font;
	cairo_t *context;
} cairo_context_object;*/

ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_arc, 0, 0, 5)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, angle1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, angle2, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_curved_to, 0, 0, 6)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x3, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y3, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_rectangle, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_line_to, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_set_color, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, x, PGGI\\RGBA, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_set_source_rgba, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, x, Cairo\\Surface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pc_context_show_text, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()


/*************************/
/* Intern Data Structure */
/*************************/

typedef struct{
	cairo_t * intern;
	int to_destroy : 1;
} * pc_context_ptr, pc_context_t;

typedef struct{
	pc_context_ptr context_ptr;
	zend_object std;
} ze_context_object;

/**************/
/* properties */
/**************/

#define CONTEXT_LINE_WIDTH "lineWidth"

/*****************************/
/* Class information getters */
/*****************************/

/*==========================================================================*/
/** 
 * get the class entry
 */
zend_class_entry * pc_context_get_class_entry(void);

/*==========================================================================*/
/** 
 * get the object handler
 */
zend_object_handlers * pc_context_get_object_handlers();

/****************************/
/* Memory handling function */
/****************************/

pc_context_ptr pc_context_new(void);

static inline ze_context_object *php_context_fetch_object(zend_object *obj) {
	return (ze_context_object *)((char*)(obj) - XtOffsetOf(ze_context_object, std));
}

inline zend_object *php_context_reverse_object(ze_context_object *obj) {
	return (zend_object *)((char*)(obj) + sizeof(pc_context_ptr));
}

#define Z_CONTEXT_P(zv) php_context_fetch_object(Z_OBJ_P((zv)))

zend_object *pc_context_object_new(zend_class_entry *class_type);

void pc_context_dtor(pc_context_ptr intern);

void pc_context_object_free_storage(zend_object *object);
void pc_context_free_resource(zend_resource *rsrc);

/***************/
/* PHP Methods */
/***************/

#define CONTEXT_METHOD(name) \
PHP_METHOD(Context, name)

/* Context Functions */
PHP_FUNCTION(cairo_create);
PHP_FUNCTION(cairo_status);
PHP_FUNCTION(cairo_save);
PHP_FUNCTION(cairo_restore);
PHP_FUNCTION(cairo_get_target);
PHP_FUNCTION(cairo_push_group);
PHP_FUNCTION(cairo_push_group_with_content);
PHP_FUNCTION(cairo_pop_group);
PHP_FUNCTION(cairo_pop_group_to_source);
PHP_FUNCTION(cairo_get_group_target);
PHP_FUNCTION(cairo_set_source_rgb);
PHP_FUNCTION(cairo_set_source_rgba);
PHP_FUNCTION(cairo_set_source);
PHP_FUNCTION(cairo_set_source_surface);
PHP_FUNCTION(cairo_get_source);
PHP_FUNCTION(cairo_set_antialias);
PHP_FUNCTION(cairo_get_antialias);
PHP_FUNCTION(cairo_set_dash);
PHP_FUNCTION(cairo_get_dash_count);
PHP_FUNCTION(cairo_get_dash);
PHP_FUNCTION(cairo_set_fill_rule);
PHP_FUNCTION(cairo_get_fill_rule);
PHP_FUNCTION(cairo_set_line_cap);
PHP_FUNCTION(cairo_get_line_cap);
PHP_FUNCTION(cairo_set_line_cap);
PHP_FUNCTION(cairo_set_line_join);
PHP_FUNCTION(cairo_get_line_join);
PHP_FUNCTION(cairo_set_line_width);
PHP_FUNCTION(cairo_get_line_width);
PHP_FUNCTION(cairo_set_miter_limit);
PHP_FUNCTION(cairo_get_miter_limit);
PHP_FUNCTION(cairo_set_operator);
PHP_FUNCTION(cairo_get_operator);
PHP_FUNCTION(cairo_set_tolerance);
PHP_FUNCTION(cairo_get_tolerance);
PHP_FUNCTION(cairo_clip);
PHP_FUNCTION(cairo_clip_preserve);
PHP_FUNCTION(cairo_in_clip);
PHP_FUNCTION(cairo_clip_extents);
PHP_FUNCTION(cairo_clip_rectangle_list);
PHP_FUNCTION(cairo_reset_clip);
PHP_FUNCTION(cairo_fill);
PHP_FUNCTION(cairo_fill_preserve);
PHP_FUNCTION(cairo_fill_extents);
PHP_FUNCTION(cairo_in_fill);
PHP_FUNCTION(cairo_mask);
PHP_FUNCTION(cairo_mask_surface);
PHP_FUNCTION(cairo_paint);
PHP_FUNCTION(cairo_paint_with_alpha);
PHP_FUNCTION(cairo_stroke);
PHP_FUNCTION(cairo_stroke_preserve);
PHP_FUNCTION(cairo_stroke_extents);
PHP_FUNCTION(cairo_in_stroke);
PHP_FUNCTION(cairo_copy_page);
PHP_FUNCTION(cairo_show_page);


/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *pc_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * get properties handling function
 */
HashTable *pc_context_get_properties(zval *object);

/*==========================================================================*/
/** 
 * write property handling function
 */
void pc_context_write_property(zval *object, zval *member, zval *value, void **cache_slot);

/************************/
/* Class Initialization */
/************************/

/*==========================================================================*/
/** 
 * init the class
 */
void pc_context_init(int module_number);





#endif
