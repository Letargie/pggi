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


#ifndef __PGGI_HUB_DEF__
#define __PGGI_HUB_DEF__

/**
 * This file is meant to provide common informations for all our classes
 */


#include "gexception.h"

/*********************/
/* Common prototypes */
/*********************/

ZEND_BEGIN_ARG_INFO(arginfo_pggi_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_on, 0, 0, 2)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pggi_set, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
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

	gsignal_gtext_buffer_changed
} gsignals;

/**
 * Associating each signal with a string
 * Needed for GTK+ signal handling
 */

#define GSIGNAL_GAPPLICATION_WINDOW_ADDED	"window_added"
#define GSIGNAL_GAPPLICATION_WINDOW_REMOVED	"window_removed"
#define GSIGNAL_GAPPLICATION_STARTUP		"startup"
#define GSIGNAL_GAPPLICATION_SHUTDOWN		"shutdown"
#define GSIGNAL_GAPPLICATION_ACTIVATE		"activate"

#define GSIGNAL_GWIDGET_DESTROY				"destroy"

#define GSIGNAL_GCONTAINER_ADD				"add"

#define GSIGNAL_GMENUITEM_ACTIVATE			"activate"

#define GSIGNAL_GCOMBO_BOX_CHANGED			"changed"
#define GSIGNAL_GCOMBO_BOX_MOVE_ACTIVE		"move_active"

#define GSIGNAL_GTEXT_BUFFER_CHANGED		"changed"

/*****************/
/* Utils defines */
/*****************/

#define PARSE_CONSTRUCT_PARAMETERS(parsing)									\
do{																			\
	zend_error_handling error_handling;										\
	zend_replace_error_handling(EH_THROW, NULL, &error_handling);			\
	if (parsing == FAILURE) {												\
		zend_restore_error_handling(&error_handling TSRMLS_CC);				\
	}																		\
	zend_restore_error_handling(&error_handling TSRMLS_CC);					\
}while(0)

#define PGGI_PARSE_PARAMETERS(parsing)										\
PARSE_CONSTRUCT_PARAMETERS(parsing)

#define DECLARE_CLASS_PROPERTY_INIT()										\
zend_class_entry ce;

#define DECLARE_CLASS_PROPERTY(ce, name) 									\
zend_declare_property_null(ce, name, sizeof(name)-1, ZEND_ACC_PUBLIC)

#define G_H_UPDATE_INIT(value)												\
HashTable *props;															\
zval zv;																	\
props = value;

#define G_H_UPDATE_RETURN													\
props

#define G_H_UPDATE(name) 													\
zend_hash_update(props, zend_string_init(name, sizeof(name)-1, 0), &zv)

#define G_H_UPDATE_BOOL(name, value)										\
do{																			\
	ZVAL_BOOL(&zv, value); 													\
	G_H_UPDATE(name);														\
}while(0)

#define G_H_UPDATE_LONG(name, value)										\
do{																			\
	ZVAL_LONG(&zv, value); 													\
	G_H_UPDATE(name);														\
}while(0)

#define G_H_UPDATE_DOUBLE(name, value)										\
do{																			\
	ZVAL_DOUBLE(&zv, value); 												\
	G_H_UPDATE(name);														\
}while(0)

#define G_H_UPDATE_STRING(name, value)										\
do{																			\
	const char * tmp = value;												\
	if(tmp)																	\
		ZVAL_STRINGL(&zv, estrdup(tmp), strlen(tmp));						\
	else																	\
		ZVAL_NULL(&zv);														\
	G_H_UPDATE(name);														\
}while(0)

/* PGGI_HUB_DEF */
#endif
