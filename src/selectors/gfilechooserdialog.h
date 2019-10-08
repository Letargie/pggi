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


#ifndef __GFILE_CHOOSER_DIALOG_DEF__
#define __GFILE_CHOOSER_DIALOG_DEF__

#include <gtk/gtk.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend.h"
#include "zend_API.h"
#include "../commons/hub.h"
#include "../windows/gdialog.h"

zend_class_entry * gfile_chooser_dialog_get_class_entry();

zend_object_handlers * gfile_chooser_dialog_get_object_handlers();

/*******************************/
/* GFileChooserDialog Arginfos */
/*******************************/

ZEND_BEGIN_ARG_INFO_EX(arginfo_gfile_chooser_dialog_construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, action, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 1)
	ZEND_ARG_OBJ_INFO(0, parent, PGGI\\GWindow, 1)
	ZEND_ARG_TYPE_INFO(0, button1_name, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, button1_action, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, button2_name, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, button2_action, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gfile_chooser_dialog_select, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()



#define GFILE_CHOOSER_DIALOG_ACTION_OPEN              GTK_FILE_CHOOSER_ACTION_OPEN
#define GFILE_CHOOSER_DIALOG_ACTION_SAVE              GTK_FILE_CHOOSER_ACTION_SAVE
#define GFILE_CHOOSER_DIALOG_ACTION_SELECT_FOLDER     GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER
#define GFILE_CHOOSER_DIALOG_ACTION_CREATE_FOLDER     GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER
#define GFILE_CHOOSER_DIALOG_ACTION                   "action"
#define GFILE_CHOOSER_DIALOG_CREATE_FOLDERS           "createFolders"
#define GFILE_CHOOSER_DIALOG_DO_OVERRIDE_CONFIRMATION "doOverrideConfirmation"
#define GFILE_CHOOSER_DIALOG_LOCAL_ONLY               "localOnly"
#define GFILE_CHOOSER_DIALOG_SELECT_MULTIPLE          "selectMultiple"
#define GFILE_CHOOSER_DIALOG_SHOW_HIDDEN              "showHidden"
#define GFILE_CHOOSER_DIALOG_CURRENT_NAME             "currentName"
#define GFILE_CHOOSER_DIALOG_FILENAME                 "fileName"
#define GFILE_CHOOSER_DIALOG_CURRENT_FOLDER           "currentFolder"
#define GFILE_CHOOSER_DIALOG_URI                      "uri"
#define GFILE_CHOOSER_DIALOG_CURRENT_FOLDER_URI       "currentFolderURI"

/*****************************/
/* Object handling functions */
/*****************************/

/*==========================================================================*/
/** 
 * Read property handling function
 */
zval *gfile_chooser_dialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);

/*==========================================================================*/
/** 
 * Get property handling function
 */
HashTable * gfile_chooser_dialog_get_properties(zval *object);

/*==========================================================================*/
/** 
 * Write property handling function
 */
PHP_WRITE_PROP_HANDLER_TYPE gfile_chooser_dialog_write_property(zval *object, zval *member, zval *value, void **cache_slot);


/***************/
/* PHP Methods */
/***************/

#define GFILE_CHOOSER_DIALOG_METHOD(name) \
PHP_METHOD(GFileChooserDialog, name)

GFILE_CHOOSER_DIALOG_METHOD(selectFilename);
GFILE_CHOOSER_DIALOG_METHOD(unselectFilename);
GFILE_CHOOSER_DIALOG_METHOD(selectAll);
GFILE_CHOOSER_DIALOG_METHOD(unselectAll);
GFILE_CHOOSER_DIALOG_METHOD(selectURI);
GFILE_CHOOSER_DIALOG_METHOD(unselectURI);

// left for later, functions using Files and returning GSList *

/*******************************************/
/* GFileChooserDialog class initialisation */
/*******************************************/

void gfile_chooser_dialog_init(int module_number);

#endif
