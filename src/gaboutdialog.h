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


#ifndef __GABOUT_DIALOG_DEF__
#define __GABOUT_DIALOG_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "hub.h"
#include "gdialog.h"

/*************************/
/* GAboutDialog Arginfos */
/*************************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gabout_dialog_show, 0, 0, 0)
	ZEND_ARG_OBJ_INFO(0, parent, gwindow_get_class_entry, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

/***************************/
/* GAboutWindow Properties */
/***************************/

#define GABOUT_DIALOG_PROGRAM_NAME			"programName"
#define GABOUT_DIALOG_VERSION				"version"
#define GABOUT_DIALOG_COPYRIGHT				"copyright"
#define GABOUT_DIALOG_COMMENTS				"comments"
#define GABOUT_DIALOG_LICENSE				"license"
#define GABOUT_DIALOG_WRAP_LICENSE			"wrapLicense"
#define GABOUT_DIALOG_LICENSE_TYPE			"licenseType"
#define GABOUT_DIALOG_WEBSITE				"website"
#define GABOUT_DIALOG_WEBSITE_LABEL			"websiteLabel"
#define GABOUT_DIALOG_AUTHORS				"authors"
#define GABOUT_DIALOG_ARTISTS				"artists"
#define GABOUT_DIALOG_DOCUMENTERS			"documenters"
#define GABOUT_DIALOG_TRANSLATOR_CREDITS	"translatorCredits"
#define GABOUT_DIALOG_LOGO					"logo"
#define GABOUT_DIALOG_LOGO_ICON_NAME		"logoIconName"

#define GABOUT_DIALOG_LICENSE_UNKNOWN		GTK_LICENSE_UNKNOWN
#define GABOUT_DIALOG_LICENSE_CUSTOM		GTK_LICENSE_CUSTOM
#define GABOUT_DIALOG_LICENSE_GPL_2_0		GTK_LICENSE_GPL_2_0
#define GABOUT_DIALOG_LICENSE_GPL_3_0		GTK_LICENSE_GPL_3_0
#define GABOUT_DIALOG_LICENSE_LGPL_2_1		GTK_LICENSE_LGPL_2_1
#define GABOUT_DIALOG_LICENSE_LGPL_3_0		GTK_LICENSE_LGPL_3_0
#define GABOUT_DIALOG_LICENSE_BSD			GTK_LICENSE_BSD
#define GABOUT_DIALOG_LICENSE_MIT_X11		GTK_LICENSE_MIT_X11
#define GABOUT_DIALOG_LICENSE_ARTISTIC		GTK_LICENSE_ARTISTIC
#define GABOUT_DIALOG_LICENSE_GPL_2_0_ONLY	GTK_LICENSE_GPL_2_0_ONLY
#define GABOUT_DIALOG_LICENSE_GPL_3_0_ONLY	GTK_LICENSE_GPL_3_0_ONLY
#define GABOUT_DIALOG_LICENSE_LGPL_2_1_ONLY	GTK_LICENSE_LGPL_2_1_ONLY
#define GABOUT_DIALOG_LICENSE_LGPL_3_0_ONLY	GTK_LICENSE_LGPL_3_0_ONLY
//#define GABOUT_DIALOG_LICENSE_AGPL_3_0		GTK_LICENSE_AGPL_3_0

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gabout_dialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gabout_dialog_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
void gabout_dialog_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GABOUT_DIALOG_METHOD(name) \
PHP_METHOD(GAboutDialog, name)

GABOUT_DIALOG_METHOD(__construct);
GABOUT_DIALOG_METHOD(showGAboutDialog);


/*************************************/
/* GAboutDialog class initialisation */
/*************************************/

void gabout_dialog_init(int module_number);

#endif
