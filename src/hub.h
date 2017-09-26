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


#if PHP_VERSION_ID >= 70200

#define PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX   ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#define PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX    ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX

#else

#define PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type     ,             allow_null) \
        ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type     ,  NULL     , allow_null)

#define PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX( name, return_reference, required_num_args,             classname, allow_null) \
        ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, IS_OBJECT, #classname, allow_null)

#endif


#ifndef __PGGI_HUB_DEF__
#define __PGGI_HUB_DEF__

/**
 * This file is meant to provide common informations for all our classes
 */


#include "gexception.h"

/*********************/
/* Common prototypes */
/*********************/

/*
#define ZEND_BEGIN_ARG_INFO_EX(name, _unused, return_reference, required_num_args)
#define ZEND_ARG_INFO(pass_by_ref, name)
#define ZEND_ARG_OBJ_INFO(pass_by_ref, name, classname, allow_null)
#define ZEND_ARG_ARRAY_INFO(pass_by_ref, name, allow_null)
#define ZEND_ARG_CALLABLE_INFO(pass_by_ref, name, allow_null)
#define ZEND_ARG_TYPE_INFO(pass_by_ref, name, type_hint, allow_null)
#define ZEND_ARG_VARIADIC_INFO(pass_by_ref, name)
*/

ZEND_BEGIN_ARG_INFO(arginfo_pggi_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_on, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_CALLABLE_INFO(0, callback, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_set, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_set_bool, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_set_long, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_set_string, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_gwidget, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, widget, PGGI\\GWidget, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_pggi_get_long, 0, 1, IS_LONG, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_pggi_get_string, 0, 1, IS_STRING, 0)
ZEND_END_ARG_INFO()

/************************/
/* Signals informations */
/************************/

/**
 * All types of signals are repertories here
 */
enum{
	gsignal_gapplication_window_added,
	gsignal_gapplication_window_removed,
	gsignal_gapplication_startup,
	gsignal_gapplication_shutdown,
	gsignal_gapplication_activate,

	gsignal_gwidget_destroy,

	gsignal_gcontainer_add,

	gsignal_gmenuitem_activate,

	gsignal_gcombo_box_changed,
	gsignal_gcombo_box_move_active,

	gsignal_gtext_buffer_changed,

	gsignal_gdialog_close,
	gsignal_gdialog_response,
	gsignal_gtree_view_column_changed,
	gsignal_gtree_view_column_clicked
} gsignals;

/**
 * Associating each signal with a string
 * Needed for GTK+ signal handling
 */

#define GSIGNAL_GAPPLICATION_WINDOW_ADDED   "window-added"
#define GSIGNAL_GAPPLICATION_WINDOW_REMOVED "window-removed"
#define GSIGNAL_GAPPLICATION_STARTUP        "startup"
#define GSIGNAL_GAPPLICATION_SHUTDOWN       "shutdown"
#define GSIGNAL_GAPPLICATION_ACTIVATE       "activate"

#define GSIGNAL_GWIDGET_DESTROY             "destroy"

#define GSIGNAL_GCONTAINER_ADD              "add"

#define GSIGNAL_GMENUITEM_ACTIVATE          "activate"

#define GSIGNAL_GCOMBO_BOX_CHANGED          "changed"
#define GSIGNAL_GCOMBO_BOX_MOVE_ACTIVE      "move-active"

#define GSIGNAL_GTEXT_BUFFER_CHANGED        "changed"

#define GSIGNAL_GDIALOG_CLOSE               "close"
#define GSIGNAL_GDIALOG_RESPONSE            "response"

#define GSIGNAL_GTREE_VIEW_COLUMN_CHANGED   "column-changed"
#define GSIGNAL_GTREE_VIEW_COLUMN_CLICKED   "clicked"

/****************************/
/* Utils parsing parameters */
/****************************/

// Not available for php versions below 7.2
#define pggi_parse_parameters_none_throw() \
    (EXPECTED(ZEND_NUM_ARGS() == 0) ? SUCCESS : zend_parse_parameters_throw(ZEND_NUM_ARGS(), ""))

// what I think is missing

#define pggi_parse_method_parameters_throw(                      num_arg, ptr, type, ...        ) \
        zend_parse_method_parameters_ex(ZEND_PARSE_PARAMS_THROW, num_arg, ptr, type, __VA_ARGS__)

#define pggi_parse_method_parameters_none(                                                    this_ptr)     \
    (EXPECTED(ZEND_NUM_ARGS() == 0) ? SUCCESS : zend_parse_method_parameters(ZEND_NUM_ARGS(), this_ptr,""))

#define pggi_parse_method_parameters_none_throw(                                                                           this_ptr)     \
    (EXPECTED(ZEND_NUM_ARGS() == 0) ? SUCCESS : zend_parse_method_parameters_ex(ZEND_PARSE_PARAMS_THROW , ZEND_NUM_ARGS(), this_ptr,""))


/*****************/
/* Utils defines */
/*****************/
/*
size_t pggi_utf32_utf8(unsigned char *buf, unsigned k){
	size_t retval = 0;
*/
	/* assert(0x0 <= k <= 0x10FFFF); */
/*
	if (k < 0x80) {
		buf[0] = k;
		retval = 1;
	} else if (k < 0x800) {
		buf[0] = 0xc0 | (k >> 6);
		buf[1] = 0x80 | (k & 0x3f);
		retval = 2;
	} else if (k < 0x10000) {
		buf[0] = 0xe0 | (k >> 12);
		buf[1] = 0x80 | ((k >> 6) & 0x3f);
		buf[2] = 0x80 | (k & 0x3f);
		retval = 3;
	} else {
		buf[0] = 0xf0 | (k >> 18);
		buf[1] = 0x80 | ((k >> 12) & 0x3f);
		buf[2] = 0x80 | ((k >> 6) & 0x3f);
		buf[3] = 0x80 | (k & 0x3f);
		retval = 4;
	}*/
	/* UTF-8 has been restricted to max 4 bytes since RFC 3629 */
/*
	return retval;
}*/

#define PGGI_READ_PROPERTY(oclass) \
	zval * oclass##_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)

#define READ_PROPERTY_NAME(oclass) \
	oclass##_read_property

#define PGGI_PARSE_PARAMETERS(parsing)                                      \
PARSE_CONSTRUCT_PARAMETERS(parsing)

#define DECLARE_CLASS_PROPERTY_INIT()                                       \
zend_class_entry ce;

#define DECLARE_CLASS_PROPERTY(ce, name)                                    \
	zend_declare_property_null(ce, name, sizeof(name)-1, ZEND_ACC_PUBLIC)

#define G_H_UPDATE_INIT(value)                                              \
	HashTable *props;                                                       \
	zval zv;                                                                \
	props = value;

#define G_H_UPDATE_RETURN                                                   \
	props

#define G_H_UPDATE(name)                                                    \
zend_hash_update(props, zend_string_init(name, sizeof(name)-1, 0), &zv)

#define G_H_UPDATE_BOOL(name, value)                                        \
do{                                                                         \
	ZVAL_BOOL(&zv, value);                                                  \
	G_H_UPDATE(name);                                                       \
}while(0)

#define G_H_UPDATE_LONG(name, value)                                        \
do{                                                                         \
	ZVAL_LONG(&zv, value);                                                  \
	G_H_UPDATE(name);                                                       \
}while(0)

#define G_H_UPDATE_DOUBLE(name, value)                                      \
do{                                                                         \
	ZVAL_DOUBLE(&zv, value);                                                \
	G_H_UPDATE(name);                                                       \
}while(0)

#define G_H_UPDATE_STRING(name, value)                                      \
do{                                                                         \
	const char * tmp = value;                                               \
	if(tmp)                                                                 \
		ZVAL_STRINGL(&zv, estrdup(tmp), strlen(tmp));                       \
	else                                                                    \
		ZVAL_NULL(&zv);                                                     \
	G_H_UPDATE(name);                                                       \
}while(0)

/* PGGI_HUB_DEF */
#endif
