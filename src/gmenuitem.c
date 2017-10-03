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


#include "gmenuitem.h"

static int le_gmenuitem;
static zend_object_handlers gmenuitem_object_handlers;
static zend_class_entry * gmenuitem_class_entry_ce;

/***************/
/* PHP Methods */
/***************/

GMENUITEM_METHOD(__construct){
	zend_string * label = NULL;
	ze_gwidget_object * ze_obj;
	zval * self = getThis();
	ze_obj = Z_GWIDGET_P(getThis());
		ze_obj->std.handlers = &gmenuitem_object_handlers;
	char * c = NULL;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|S", &label) == FAILURE)
		return;
	ze_obj->widget_ptr = gwidget_new();
	if(label){
		c = ZSTR_VAL(label);
		ze_obj->widget_ptr->intern = gtk_menu_item_new_with_label(c);
	}else
		ze_obj->widget_ptr->intern = gtk_menu_item_new();
	GCONTAINER_ADD_ELEMENT(ze_obj);
	g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), Z_GWIDGET_P(self));
}

GMENUITEM_METHOD(select){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	gtk_menu_item_select(GTK_MENU_ITEM(widget->widget_ptr->intern));
}

GMENUITEM_METHOD(deselect){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	gtk_menu_item_deselect(GTK_MENU_ITEM(widget->widget_ptr->intern));
}

GMENUITEM_METHOD(activate){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	if (pggi_parse_parameters_none_throw() == FAILURE)
		return;
	gtk_menu_item_activate(GTK_MENU_ITEM(widget->widget_ptr->intern));
}
/*
GMENUITEM_METHOD(toggle_size_request){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	// TODO CHANGING THIS FUNCTION
	gtk_menu_item_toggle_size_request(GTK_MENU_ITEM(widget->widget_ptr->intern), 1);
}

GMENUITEM_METHOD(toggle_size_allocate){
	ze_gwidget_object * widget;
	widget = Z_GWIDGET_P(getThis());
	//TODO CHANGING THIS FUNCTION
	gtk_menu_item_toggle_size_allocate(GTK_MENU_ITEM(widget->widget_ptr->intern), 1);
}*/

void gmenuitem_func_activate(GtkWidget* w, gpointer data){
	zval args[2];
	gwidget_function(data, gsignal_gmenuitem_activate, args, 2);
}

void gmenuitem_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param){
	switch(val){
		case gsignal_gmenuitem_activate :
			gwidget_adding_function(val, GSIGNAL_GMENUITEM_ACTIVATE, G_CALLBACK(gmenuitem_func_activate), ze_obj, function, param);
			break;
		default :
			gcontainer_on(val, ze_obj, function, param);
	}
}

GMENUITEM_METHOD(on){
	zval * function, * this, * param = NULL;
	long val;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	gmenuitem_on(val, Z_GWIDGET_P(this), function, param);
}


/**
 * List of GMenuItem functions and methods with their arguments
 */
static const zend_function_entry gmenuitem_class_functions[] = {
	PHP_ME(GMenuItem, __construct, arginfo_gmenuitem_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GMenuItem, select     , arginfo_pggi_void          , ZEND_ACC_PUBLIC)
	PHP_ME(GMenuItem, deselect   , arginfo_pggi_void          , ZEND_ACC_PUBLIC)
	PHP_ME(GMenuItem, activate   , arginfo_pggi_void          , ZEND_ACC_PUBLIC)
	PHP_ME(GMenuItem, on         , arginfo_pggi_on            , ZEND_ACC_PUBLIC)
	//PHP_ME(GMenuItem, toggle_size_request, arginfo_pggi_void, ZEND_ACC_PUBLIC)
	//PHP_ME(GMenuItem, toggle_size_allocate, arginfo_pggi_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gmenuitem_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkMenuItem * menu = GTK_MENU_ITEM(w->intern);
	if(!strcmp(member_val, GMENUITEM_LABEL)){
		const char * tmp = gtk_menu_item_get_label(menu);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else if(!strcmp(member_val, GMENUITEM_ACCEL_PATH)){
		const char * tmp = gtk_menu_item_get_accel_path(menu);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else if(!strcmp(member_val, GMENUITEM_USE_UNDERLINE)){
		ZVAL_BOOL(rv, gtk_menu_item_get_use_underline(menu));
	}else if(!strcmp(member_val, GMENUITEM_RESERVE_INDICATOR)){
		ZVAL_BOOL(rv, gtk_menu_item_get_reserve_indicator(menu));
	}else if(!strcmp(member_val, GMENUITEM_SUBMENU)){
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);

	return rv;
}

HashTable *gmenuitem_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	GtkMenuItem * win = GTK_MENU_ITEM(w->intern);

	G_H_UPDATE_STRING(GMENUITEM_LABEL            , gtk_menu_item_get_label            (win));
	G_H_UPDATE_STRING(GMENUITEM_ACCEL_PATH       , gtk_menu_item_get_accel_path       (win));
	G_H_UPDATE_BOOL  (GMENUITEM_USE_UNDERLINE    , gtk_menu_item_get_use_underline    (win));
	G_H_UPDATE_BOOL  (GMENUITEM_RESERVE_INDICATOR, gtk_menu_item_get_reserve_indicator(win));

	return G_H_UPDATE_RETURN;
}

void gmenuitem_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	ze_gwidget_object * tmp_obj;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkMenuItem * menu = GTK_MENU_ITEM(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(member_val, GMENUITEM_LABEL))
				gtk_menu_item_set_label(menu, Z_STRVAL_P(value));
			else if(!strcmp(member_val, GMENUITEM_ACCEL_PATH))
				gtk_menu_item_set_accel_path(menu, Z_STRVAL_P(value));
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_FALSE :
		case IS_TRUE  :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GMENUITEM_USE_UNDERLINE))
				gtk_menu_item_set_use_underline(menu, tmp_b);
			else if(!strcmp(member_val, GMENUITEM_RESERVE_INDICATOR))
				gtk_menu_item_set_reserve_indicator(menu, tmp_b);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_OBJECT :
				if(!strcmp(member_val, GMENUITEM_SUBMENU)){
					tmp_obj = Z_GWIDGET_P(value);
					if(!tmp_obj){
						zend_throw_exception_ex(pggi_exception_get(), 0, "the submenu need to be a widget");
						return ;
					}
					w = tmp_obj->widget_ptr;
					std_object_handlers.write_property(object, member, value, cache_slot);
					gtk_menu_item_set_submenu(menu, w->intern);
				}else
					gcontainer_write_property(object, member, value, cache_slot);
			break;
		default :
			gcontainer_write_property(object, member, value, cache_slot);
	}
}

/**********************************/
/* GMenuItem class initialisation */
/**********************************/

#define DECLARE_GMENUITEM_PROP(name) \
DECLARE_CLASS_PROPERTY(gmenuitem_class_entry_ce, name)

void gmenuitem_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gmenuitem = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GMenuItem", module_number);

	memcpy(&gmenuitem_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gmenuitem_object_handlers.read_property  = gmenuitem_read_property;
	gmenuitem_object_handlers.get_properties = gmenuitem_get_properties;
	gmenuitem_object_handlers.write_property = gmenuitem_write_property;

	INIT_CLASS_ENTRY(ce, "PGGI\\GMenuItem", gmenuitem_class_functions);
	gmenuitem_class_entry_ce = zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	DECLARE_GMENUITEM_PROP(GMENUITEM_ACCEL_PATH       );
	DECLARE_GMENUITEM_PROP(GMENUITEM_LABEL            );
	DECLARE_GMENUITEM_PROP(GMENUITEM_SUBMENU          );
	DECLARE_GMENUITEM_PROP(GMENUITEM_USE_UNDERLINE    );
	DECLARE_GMENUITEM_PROP(GMENUITEM_RESERVE_INDICATOR);
}


