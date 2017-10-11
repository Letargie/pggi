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

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"

#if PHP_VERSION_ID >= 70200

#define PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX   ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#define PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX    ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX

#else

#define PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type     ,             allow_null) \
        ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type     ,  NULL     , allow_null)

#define PGGI_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX( name, return_reference, required_num_args,             classname, allow_null) \
        ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, IS_OBJECT, #classname, allow_null)

#endif


#ifndef __CAIRO_HUB_DEF__
#define __CAIRO_HUB_DEF__

/**
 * This file is meant to provide common informations for all our classes
 */

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

ZEND_BEGIN_ARG_INFO(arginfo_cairo_void, 0)
ZEND_END_ARG_INFO()

PGGI_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_cairo_get_long, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_cairo_set, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_cairo_set_double, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/****************************/
/* Utils parsing parameters */
/****************************/

// Not available for php versions below 7.2
#define cairo_parse_parameters_none_throw() \
    (EXPECTED(ZEND_NUM_ARGS() == 0) ? SUCCESS : zend_parse_parameters_throw(ZEND_NUM_ARGS(), ""))

// what I think is missing

#define cairo_parse_method_parameters_throw(                     num_arg, ptr, type, ...        ) \
        zend_parse_method_parameters_ex(ZEND_PARSE_PARAMS_THROW, num_arg, ptr, type, __VA_ARGS__)

#define cairo_parse_method_parameters_none(                                                   this_ptr)     \
    (EXPECTED(ZEND_NUM_ARGS() == 0) ? SUCCESS : zend_parse_method_parameters(ZEND_NUM_ARGS(), this_ptr,""))

#define cairo_parse_method_parameters_none_throw(                                                                          this_ptr)     \
    (EXPECTED(ZEND_NUM_ARGS() == 0) ? SUCCESS : zend_parse_method_parameters_ex(ZEND_PARSE_PARAMS_THROW , ZEND_NUM_ARGS(), this_ptr,""))


/*****************/
/* Utils defines */
/*****************/

#define DECLARE_CLASS_PROPERTY_INIT()                                       \
zend_class_entry ce;

#define DECLARE_CLASS_PROPERTY(ce, name)                                    \
	zend_declare_property_null(ce, name, sizeof(name)-1, ZEND_ACC_PUBLIC)

#define DECLARE_CLASS_CONST(ce, const_name, value)                                 \
	zend_declare_class_constant_long(ce, const_name, sizeof(const_name)-1, value); \

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
