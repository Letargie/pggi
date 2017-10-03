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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "gfilechooserdialog.h"

static int le_gfile_chooser_dialog;
static zend_object_handlers gfile_chooser_dialog_object_handlers;
static zend_class_entry * gfile_chooser_dialog_class_entry_ce;

zend_class_entry * gfile_chooser_dialog_get_class_entry(){
	return gfile_chooser_dialog_class_entry_ce;
}

zend_object_handlers * gfile_chooser_dialog_get_object_handlers(){
	return &gfile_chooser_dialog_object_handlers;
}


/***************/
/* PHP Methods */
/***************/

GFILE_CHOOSER_DIALOG_METHOD(__construct){
	zval * parent = NULL;
	ze_gwidget_object * widget, * window;
	long action, bn1_action = 0, bn2_action = 0;
	zend_string * button_name1 = NULL, * button_name2 = NULL, * title = NULL;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l|SOSlSl", &action, &title, &parent, gwindow_get_class_entry(), &button_name1, &bn1_action, &button_name2, &bn2_action) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gfile_chooser_dialog_object_handlers;
	widget->widget_ptr = gwidget_new();
	window = Z_GWIDGET_P(parent);
	if(!button_name1)
		widget->widget_ptr->intern = gtk_file_chooser_dialog_new(ZSTR_VAL(title), GTK_WINDOW(window->widget_ptr->intern), action, NULL, NULL);
	else if(!button_name2)
		widget->widget_ptr->intern = gtk_file_chooser_dialog_new(ZSTR_VAL(title), GTK_WINDOW(window->widget_ptr->intern), action,
		                                                         ZSTR_VAL(button_name1), bn1_action, NULL);
	else	
		widget->widget_ptr->intern = gtk_file_chooser_dialog_new(ZSTR_VAL(title), GTK_WINDOW(window->widget_ptr->intern), action,
		                                                         ZSTR_VAL(button_name1), bn1_action,
		                                                         ZSTR_VAL(button_name2), bn2_action, NULL);
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

GFILE_CHOOSER_DIALOG_METHOD(selectFilename){
	ze_gwidget_object * widget;
	zend_string name;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &name) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	gtk_file_chooser_select_filename(GTK_FILE_CHOOSER(widget->widget_ptr->intern), ZSTR_VAL(&name));
}

GFILE_CHOOSER_DIALOG_METHOD(unselectFilename){
	ze_gwidget_object * widget;
	zend_string name;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &name) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	gtk_file_chooser_unselect_filename(GTK_FILE_CHOOSER(widget->widget_ptr->intern), ZSTR_VAL(&name));
}

GFILE_CHOOSER_DIALOG_METHOD(selectAll){
	ze_gwidget_object * widget;
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());;
	gtk_file_chooser_select_all(GTK_FILE_CHOOSER(widget->widget_ptr->intern));
}

GFILE_CHOOSER_DIALOG_METHOD(unselectAll){
	ze_gwidget_object * widget;
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widget->widget_ptr->intern));
}

GFILE_CHOOSER_DIALOG_METHOD(selectURI){
	ze_gwidget_object * widget;
	zend_string name;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &name) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	gtk_file_chooser_select_uri(GTK_FILE_CHOOSER(widget->widget_ptr->intern), ZSTR_VAL(&name));
}

GFILE_CHOOSER_DIALOG_METHOD(unselectURI){
	ze_gwidget_object * widget;
	zend_string name;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &name) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	gtk_file_chooser_unselect_uri(GTK_FILE_CHOOSER(widget->widget_ptr->intern), ZSTR_VAL(&name));
}

/**
 * List of GFileChooserDialog functions and methods with their arguments
 */
static const zend_function_entry gfile_chooser_dialog_class_functions[] = {
	PHP_ME(GFileChooserDialog, __construct     , arginfo_gfile_chooser_dialog_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GFileChooserDialog, selectFilename  , arginfo_gfile_chooser_dialog_select   , ZEND_ACC_PUBLIC)
	PHP_ME(GFileChooserDialog, unselectFilename, arginfo_gfile_chooser_dialog_select   , ZEND_ACC_PUBLIC)
	PHP_ME(GFileChooserDialog, selectAll       , arginfo_pggi_void                     , ZEND_ACC_PUBLIC)
	PHP_ME(GFileChooserDialog, unselectAll     , arginfo_pggi_void                     , ZEND_ACC_PUBLIC)
	PHP_ME(GFileChooserDialog, selectURI       , arginfo_gfile_chooser_dialog_select   , ZEND_ACC_PUBLIC)
	PHP_ME(GFileChooserDialog, unselectURI     , arginfo_gfile_chooser_dialog_select   , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/


zval *gfile_chooser_dialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkFileChooser * chooser = GTK_FILE_CHOOSER(GTK_FILE_CHOOSER_DIALOG(w->intern));
	if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_FILENAME)){
		const char * tmp = gtk_file_chooser_get_filename(chooser);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	//Need to check the action type before
	}/*else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CURRENT_NAME)){
		const char * tmp = gtk_file_chooser_get_current_name(chooser);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}*/else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CURRENT_FOLDER)){
		const char * tmp = gtk_file_chooser_get_current_folder(chooser);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_URI)){
		const char * tmp = gtk_file_chooser_get_uri(chooser);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CURRENT_FOLDER_URI)){
		const char * tmp = gtk_file_chooser_get_current_folder_uri(chooser);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_ACTION)){
		ZVAL_LONG(rv, gtk_file_chooser_get_action(chooser));
	}else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CREATE_FOLDERS)){
		ZVAL_BOOL(rv, gtk_file_chooser_get_create_folders(chooser));
	}else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_DO_OVERRIDE_CONFIRMATION))
		ZVAL_BOOL(rv, gtk_file_chooser_get_do_overwrite_confirmation(chooser));
	else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_LOCAL_ONLY))
		ZVAL_BOOL(rv, gtk_file_chooser_get_local_only(chooser));
	else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_SELECT_MULTIPLE))
		ZVAL_BOOL(rv, gtk_file_chooser_get_select_multiple(chooser));
	else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_SHOW_HIDDEN))
		ZVAL_BOOL(rv, gtk_file_chooser_get_show_hidden(chooser));
	else
		return gwindow_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gfile_chooser_dialog_get_properties(zval *object){
	G_H_UPDATE_INIT(gwindow_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkFileChooser * chooser = GTK_FILE_CHOOSER(GTK_FILE_CHOOSER_DIALOG(w->intern));

	G_H_UPDATE_LONG  (GFILE_CHOOSER_DIALOG_ACTION                  , gtk_file_chooser_get_action                   (chooser));
	G_H_UPDATE_BOOL  (GFILE_CHOOSER_DIALOG_CREATE_FOLDERS          , gtk_file_chooser_get_create_folders           (chooser));
	G_H_UPDATE_BOOL  (GFILE_CHOOSER_DIALOG_DO_OVERRIDE_CONFIRMATION, gtk_file_chooser_get_do_overwrite_confirmation(chooser));
	G_H_UPDATE_BOOL  (GFILE_CHOOSER_DIALOG_LOCAL_ONLY              , gtk_file_chooser_get_local_only               (chooser));
	G_H_UPDATE_BOOL  (GFILE_CHOOSER_DIALOG_SELECT_MULTIPLE         , gtk_file_chooser_get_select_multiple          (chooser));
	G_H_UPDATE_BOOL  (GFILE_CHOOSER_DIALOG_SHOW_HIDDEN             , gtk_file_chooser_get_show_hidden              (chooser));
	G_H_UPDATE_STRING(GFILE_CHOOSER_DIALOG_FILENAME                , gtk_file_chooser_get_filename                 (chooser));
	//Need to check the action type before
	//G_H_UPDATE_STRING(GFILE_CHOOSER_DIALOG_CURRENT_NAME            , gtk_file_chooser_get_current_name             (chooser));
	G_H_UPDATE_STRING(GFILE_CHOOSER_DIALOG_CURRENT_FOLDER          , gtk_file_chooser_get_current_folder           (chooser));
	G_H_UPDATE_STRING(GFILE_CHOOSER_DIALOG_URI                     , gtk_file_chooser_get_uri                      (chooser));
	G_H_UPDATE_STRING(GFILE_CHOOSER_DIALOG_CURRENT_FOLDER_URI      , gtk_file_chooser_get_current_folder_uri       (chooser));

	return G_H_UPDATE_RETURN;
}

void gfile_chooser_dialog_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkFileChooser * chooser = GTK_FILE_CHOOSER(GTK_FILE_CHOOSER_DIALOG(w->intern));
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			;
			char * tmp_s = Z_STRVAL_P(value);
			if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_FILENAME))
				gtk_file_chooser_set_filename(chooser, tmp_s);
			//Need to check the action type before
			//else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CURRENT_NAME))
				//gtk_file_chooser_set_current_name(chooser, tmp_s);
			else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CURRENT_FOLDER))
				gtk_file_chooser_set_current_folder(chooser, tmp_s);
			else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_URI))
				gtk_file_chooser_set_uri(chooser, tmp_s);
			else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CURRENT_FOLDER_URI))
				gtk_file_chooser_set_current_folder_uri(chooser, tmp_s);
			else
				gwindow_write_property(object, member, value, cache_slot);
			break;
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_ACTION))
				gtk_file_chooser_set_action(chooser, tmp_l);
			else
				gwindow_write_property(object, member, value, cache_slot);
			break;
		case IS_TRUE :
		case IS_FALSE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 :0;
			if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_CREATE_FOLDERS))
				gtk_file_chooser_set_create_folders(chooser, tmp_b);
			else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_DO_OVERRIDE_CONFIRMATION))
				gtk_file_chooser_set_do_overwrite_confirmation(chooser, tmp_b);
			else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_LOCAL_ONLY))
				gtk_file_chooser_set_local_only(chooser, tmp_b);
			else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_SELECT_MULTIPLE))
				gtk_file_chooser_set_select_multiple(chooser, tmp_b);
			else if(!strcmp(member_val, GFILE_CHOOSER_DIALOG_SHOW_HIDDEN))
				gtk_file_chooser_set_show_hidden(chooser, tmp_b);
			else
				gwindow_write_property(object, member, value, cache_slot);
			break;
		default :
			gwindow_write_property(object, member, value, cache_slot);
	}
}

/*******************************************/
/* GFileChooserDialog class initialisation */
/*******************************************/

#define DECLARE_GFILE_CHOOSER_DIALOG_PROP(name) \
DECLARE_CLASS_PROPERTY(gfile_chooser_dialog_class_entry_ce, name)

#define GFILE_CHOOSER_DIALOG_CONSTANT(name, value) \
zend_declare_class_constant_double(gfile_chooser_dialog_class_entry_ce, name, sizeof(name)-1, value);

void gfile_chooser_dialog_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gfile_chooser_dialog = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gfile_chooser_dialog", module_number);

	memcpy(&gfile_chooser_dialog_object_handlers, gwindow_get_object_handlers(), sizeof(zend_object_handlers));
	gfile_chooser_dialog_object_handlers.read_property  = gfile_chooser_dialog_read_property;
	gfile_chooser_dialog_object_handlers.get_properties = gfile_chooser_dialog_get_properties;
	gfile_chooser_dialog_object_handlers.write_property = gfile_chooser_dialog_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GFileChooserDialog", gfile_chooser_dialog_class_functions);
	gfile_chooser_dialog_class_entry_ce	= zend_register_internal_class_ex(&ce, gdialog_get_class_entry());

	GFILE_CHOOSER_DIALOG_CONSTANT("ACTION_OPEN"         , GFILE_CHOOSER_DIALOG_ACTION_OPEN         );
	GFILE_CHOOSER_DIALOG_CONSTANT("ACTION_SAVE"         , GFILE_CHOOSER_DIALOG_ACTION_SAVE         );
	GFILE_CHOOSER_DIALOG_CONSTANT("ACTION_SELECT_FOLDER", GFILE_CHOOSER_DIALOG_ACTION_SELECT_FOLDER);
	GFILE_CHOOSER_DIALOG_CONSTANT("ACTION_CREATE_FOLDER", GFILE_CHOOSER_DIALOG_ACTION_CREATE_FOLDER);

	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_ACTION                  );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_CREATE_FOLDERS          );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_DO_OVERRIDE_CONFIRMATION);
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_LOCAL_ONLY              );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_SELECT_MULTIPLE         );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_SHOW_HIDDEN             );
	//DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_CURRENT_NAME            );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_FILENAME                );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_CURRENT_FOLDER          );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_URI                     );
	DECLARE_GFILE_CHOOSER_DIALOG_PROP(GFILE_CHOOSER_DIALOG_CURRENT_FOLDER_URI      );
}



