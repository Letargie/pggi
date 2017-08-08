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


#include "gaboutdialog.h"

static int le_gabout_dialog;
static zend_object_handlers gabout_dialog_object_handlers;
static zend_class_entry * gabout_dialog_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GABOUT_DIALOG_METHOD(__construct){
	ze_gwidget_object * widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gabout_dialog_object_handlers;
	widget->widget_ptr = gwidget_new();
	widget->widget_ptr->intern = gtk_about_dialog_new();
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}

/**
 * List of GAboutDialog functions and methods with their arguments
 */
static const zend_function_entry gabout_dialog_class_functions[] = {
	PHP_ME(GAboutDialog, __construct	, arginfo_pggi_void	, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gabout_dialog_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval zobj;
	zend_long lval;

	ZVAL_NULL(rv);
	if(w){
		convert_to_string(member);
		char * member_val = Z_STRVAL_P(member);
		GtkAboutDialog * dialog = GTK_ABOUT_DIALOG(w->intern);
		if(!strcmp(member_val, GABOUT_DIALOG_PROGRAM_NAME)){
			const char * tmp = gtk_about_dialog_get_program_name(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GABOUT_DIALOG_VERSION)){
			const char * tmp = gtk_about_dialog_get_version(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GABOUT_DIALOG_COPYRIGHT)){
			const char * tmp = gtk_about_dialog_get_version(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GABOUT_DIALOG_COMMENTS)){
			const char * tmp = gtk_about_dialog_get_comments(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GABOUT_DIALOG_LICENSE)){
			const char * tmp = gtk_about_dialog_get_license(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GABOUT_DIALOG_WRAP_LICENSE))
			ZVAL_BOOL(rv, gtk_about_dialog_get_wrap_license(dialog));
		else if(!strcmp(member_val, GABOUT_DIALOG_LICENSE_TYPE)){
			ZVAL_LONG(rv, gtk_about_dialog_get_license_type(dialog));
		}else if(!strcmp(member_val, GABOUT_DIALOG_WEBSITE)){
			const char * tmp = gtk_about_dialog_get_website(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GABOUT_DIALOG_WEBSITE_LABEL)){
			const char * tmp = gtk_about_dialog_get_website_label(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}/*else if(!strcmp(member_val, GABOUT_DIALOG_AUTHORS)){

		}else if(!strcmp(member_val, GABOUT_DIALOG_ARTISTS)){
			
		}else if(!strcmp(member_val, GABOUT_DIALOG_DOCUMENTERS)){
			
		}*/else if(!strcmp(member_val, GABOUT_DIALOG_TRANSLATOR_CREDITS)){
			const char * tmp = gtk_about_dialog_get_translator_credits(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else if(!strcmp(member_val, GABOUT_DIALOG_LOGO_ICON_NAME)){
			const char * tmp = gtk_about_dialog_get_logo_icon_name(dialog);
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}/*else if(!strcmp(member_val, GABOUT_DIALOG_LOGO)){
			
		}*/else
			return gwindow_read_property(object, member, type, cache_slot, rv);
	}
	return rv;
}

HashTable *gabout_dialog_get_properties(zval *object){
	G_H_UPDATE_INIT(gwindow_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	if(!w){
		return NULL;
	}
	GtkAboutDialog * dialog = GTK_ABOUT_DIALOG(w->intern);
	G_H_UPDATE_STRING	(GABOUT_DIALOG_PROGRAM_NAME			, gtk_about_dialog_get_program_name			(dialog));
	G_H_UPDATE_STRING	(GABOUT_DIALOG_VERSION				, gtk_about_dialog_get_version				(dialog));
	G_H_UPDATE_STRING	(GABOUT_DIALOG_COPYRIGHT			, gtk_about_dialog_get_copyright			(dialog));
	G_H_UPDATE_STRING	(GABOUT_DIALOG_COMMENTS				, gtk_about_dialog_get_comments				(dialog));
	G_H_UPDATE_STRING	(GABOUT_DIALOG_LICENSE				, gtk_about_dialog_get_license				(dialog));
	G_H_UPDATE_BOOL		(GABOUT_DIALOG_WRAP_LICENSE			, gtk_about_dialog_get_wrap_license			(dialog));
	G_H_UPDATE_LONG		(GABOUT_DIALOG_LICENSE_TYPE			, gtk_about_dialog_get_license_type			(dialog));
	G_H_UPDATE_STRING	(GABOUT_DIALOG_WEBSITE				, gtk_about_dialog_get_website				(dialog));
	G_H_UPDATE_STRING	(GABOUT_DIALOG_WEBSITE_LABEL		, gtk_about_dialog_get_website_label		(dialog));
	// AUTHORS, ARTISTS, DOCUMENTERS, LOGO
	G_H_UPDATE_STRING	(GABOUT_DIALOG_TRANSLATOR_CREDITS	, gtk_about_dialog_get_translator_credits	(dialog));
	G_H_UPDATE_STRING	(GABOUT_DIALOG_LOGO_ICON_NAME		, gtk_about_dialog_get_logo_icon_name		(dialog));

	return G_H_UPDATE_RETURN;
}

void gabout_dialog_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkAboutDialog * dialog = GTK_ABOUT_DIALOG(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(member_val, GABOUT_DIALOG_PROGRAM_NAME))
				gtk_about_dialog_set_program_name(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_VERSION))
				gtk_about_dialog_set_version(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_COPYRIGHT))
				gtk_about_dialog_set_copyright(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_COMMENTS))
				gtk_about_dialog_set_comments(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_LICENSE))
				gtk_about_dialog_set_license(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_WEBSITE))
				gtk_about_dialog_set_website(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_WEBSITE_LABEL))
				gtk_about_dialog_set_website_label(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_TRANSLATOR_CREDITS))
				gtk_about_dialog_set_translator_credits(dialog, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GABOUT_DIALOG_LOGO_ICON_NAME))
				gtk_about_dialog_set_logo_icon_name(dialog, Z_STRVAL_P(value));
			else
				gwindow_write_property(object, member, value, cache_slot);
			break;
		case IS_LONG : // need to check license type
			if(!strcmp(member_val, GABOUT_DIALOG_LICENSE_TYPE)){
				switch(Z_LVAL_P(value)){
					case GABOUT_DIALOG_LICENSE_UNKNOWN:
					case GABOUT_DIALOG_LICENSE_CUSTOM :
					case GABOUT_DIALOG_LICENSE_GPL_2_0 :
					case GABOUT_DIALOG_LICENSE_GPL_3_0 :
					case GABOUT_DIALOG_LICENSE_BSD :
					case GABOUT_DIALOG_LICENSE_MIT_X11 :
					case GABOUT_DIALOG_LICENSE_ARTISTIC :
					case GABOUT_DIALOG_LICENSE_GPL_2_0_ONLY :
					case GABOUT_DIALOG_LICENSE_GPL_3_0_ONLY :
					case GABOUT_DIALOG_LICENSE_LGPL_2_1_ONLY :
					case GABOUT_DIALOG_LICENSE_LGPL_3_0_ONLY :
					//case GABOUT_DIALOG_LICENSE_AGPL_3_0 :
						gtk_about_dialog_set_license_type(dialog, Z_LVAL_P(value));
						break;
					default :
						// error
						break;
				}
			}else{
				gwindow_write_property(object, member, value, cache_slot);
			}
			break;
		case IS_TRUE :
		case IS_FALSE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 :0;
			if(!strcmp(member_val, GABOUT_DIALOG_WRAP_LICENSE))
				gtk_about_dialog_set_wrap_license(dialog, tmp_b);
			else
				gwindow_write_property(object, member, value, cache_slot);
			break;
		default :
			gwindow_write_property(object, member, value, cache_slot);
	}
}

/*************************************/
/* GAboutDialog class initialisation */
/*************************************/

#define DECLARE_GABOUT_DIALOG_PROP(name) \
DECLARE_CLASS_PROPERTY(gabout_dialog_class_entry_ce, name)


#define GABOUT_DIALOG_CONSTANT(name, value) \
zend_declare_class_constant_double(gabout_dialog_class_entry_ce, name, sizeof(name)-1, value);

void gabout_dialog_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gabout_dialog = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gabout", module_number);

	memcpy(&gabout_dialog_object_handlers, gdialog_get_object_handlers(), sizeof(zend_object_handlers));
	gabout_dialog_object_handlers.read_property  = gabout_dialog_read_property;
	gabout_dialog_object_handlers.get_properties = gabout_dialog_get_properties;
	gabout_dialog_object_handlers.write_property = gabout_dialog_write_property;

	INIT_CLASS_ENTRY(ce, "GAboutDialog", gabout_dialog_class_functions);
	gabout_dialog_class_entry_ce	= zend_register_internal_class_ex(&ce, gdialog_get_class_entry());

	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_PROGRAM_NAME		);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_VERSION			);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_COPYRIGHT			);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_COMMENTS			);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_LICENSE			);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_WRAP_LICENSE		);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_LICENSE_TYPE		);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_WEBSITE			);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_WEBSITE_LABEL		);
	//DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_AUTHORS			);
	//DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_ARTISTS			);
	//DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_DOCUMENTERS		);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_TRANSLATOR_CREDITS	);
	//DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_LOGO				);
	DECLARE_GABOUT_DIALOG_PROP(GABOUT_DIALOG_LOGO_ICON_NAME		);
	
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_UNKNOWN"		, GABOUT_DIALOG_LICENSE_UNKNOWN			);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_CUSTOM"		, GABOUT_DIALOG_LICENSE_CUSTOM			);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_GPL_2_0"		, GABOUT_DIALOG_LICENSE_GPL_2_0			);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_GPL_3_0"		, GABOUT_DIALOG_LICENSE_GPL_3_0			);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_LGPL_2_1"		, GABOUT_DIALOG_LICENSE_LGPL_2_1		);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_LGPL_3_0"		, GABOUT_DIALOG_LICENSE_LGPL_3_0		);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_BSD"			, GABOUT_DIALOG_LICENSE_BSD				);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_MIT_X11"		, GABOUT_DIALOG_LICENSE_MIT_X11			);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_ARTISTIC"		, GABOUT_DIALOG_LICENSE_ARTISTIC		);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_GPL_2_0_ONLY"	, GABOUT_DIALOG_LICENSE_GPL_2_0_ONLY	);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_GPL_3_0_ONLY"	, GABOUT_DIALOG_LICENSE_GPL_3_0_ONLY	);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_LGPL_2_1_ONLY", GABOUT_DIALOG_LICENSE_LGPL_2_1_ONLY	);
	GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_LGPL_3_0_ONLY", GABOUT_DIALOG_LICENSE_LGPL_3_0_ONLY	);
	//GABOUT_DIALOG_CONSTANT("GABOUT_DIALOG_LICENSE_AGPL_3_0"		, GABOUT_DIALOG_LICENSE_AGPL_3_0		);

}


