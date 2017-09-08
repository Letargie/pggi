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


#include "gcombobox.h"

static int le_gcombo_box;
static zend_object_handlers gcombo_box_object_handlers;
static zend_class_entry * gcombo_box_class_entry_ce;


zend_class_entry * gcombo_box_get_class_entry(){
	return gcombo_box_class_entry_ce;
}

zend_object_handlers * gcombo_box_get_object_handlers(){
	return &gcombo_box_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

GCOMBO_BOX_METHOD(__construct){
	ze_gwidget_object * ze_obj;
	zval * self = getThis();
	if(!self)
		RETURN_NULL();
	ze_obj = Z_GWIDGET_P(getThis());
	ze_obj->std.handlers = &gcombo_box_object_handlers;
	ze_obj->widget_ptr = gwidget_new();
	ze_obj->widget_ptr->intern = gtk_combo_box_new();
	GCONTAINER_ADD_ELEMENT(ze_obj);
	g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), Z_GWIDGET_P(self));
}

void gcombo_box_func_changed(GtkWidget* w, gpointer data){
	gwidget_function(data, gsignal_gcombo_box_changed);
}

void gcombo_box_func_move_active(GtkWidget* w, gpointer data){
	gwidget_function(data, gsignal_gcombo_box_move_active);
}

void gcombo_box_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * param){
	zval * data, * narray;
	switch(val){
		case gsignal_gcombo_box_changed :
			gwidget_adding_function(val, GSIGNAL_GCOMBO_BOX_CHANGED, gcombo_box_func_changed, ze_obj, function, param);
			break;
		case gsignal_gcombo_box_move_active :
			gwidget_adding_function(val, GSIGNAL_GCOMBO_BOX_MOVE_ACTIVE, gcombo_box_func_move_active, ze_obj, function, param);
			break;
		default :
			gcontainer_on(val, ze_obj, function, param);
			return ;
	}
}

GCOMBO_BOX_METHOD(on){
	zval * function, * this, * param = NULL;
	long val;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE) {
        RETURN_NULL();
    }
	if(!zend_is_callable(function, 0, NULL))
		zend_error(E_ERROR, "Function requires callable argument");
	this = getThis();
	if(!this)
		RETURN_NULL();
	gcombo_box_on(val, Z_GWIDGET_P(this), function, param);
}


/**
 * List of GComboBox functions and methods with their arguments
 */
static const zend_function_entry gcombo_box_class_functions[] = {
	PHP_ME(GComboBox, __construct	, arginfo_pggi_void	, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR	)
	PHP_ME(GComboBox, on			, arginfo_pggi_on	, ZEND_ACC_PUBLIC					)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gcombo_box_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval zobj;
	zend_long lval;

	ZVAL_NULL(rv);
	if(w){
		convert_to_string(member);
		char * member_val = Z_STRVAL_P(member);
		GtkComboBox * menu = GTK_COMBO_BOX(w->intern);
		if(!strcmp(member_val, GCOMBO_BOX_ACTIVE)){
			ZVAL_BOOL(rv, gtk_combo_box_get_active(menu));
		}else
			return gcontainer_read_property(object, member, type, cache_slot, rv);
	}
	return rv;
}

HashTable *gcombo_box_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	if(!w){
		return NULL;
	}
	GtkComboBox * win = GTK_COMBO_BOX(w->intern);
	G_H_UPDATE_LONG(GCOMBO_BOX_ACTIVE, gtk_combo_box_get_active(win));

	return G_H_UPDATE_RETURN;
}

void gcombo_box_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	ze_gwidget_object * tmp_obj;
	long tmp_l;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkComboBox * menu = GTK_COMBO_BOX(w->intern);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			if(!strcmp(member_val, GCOMBO_BOX_ACTIVE))
				gtk_combo_box_set_active(menu, Z_LVAL_P(value));
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		default :
			gcontainer_write_property(object, member, value, cache_slot);
	}
}

/**********************************/
/* GComboBox class initialisation */
/**********************************/

#define DECLARE_GCOMBO_BOX_PROP(name) \
DECLARE_CLASS_PROPERTY(gcombo_box_class_entry_ce, name)

void gcombo_box_init(int module_number){
	DECLARE_CLASS_PROPERTY_INIT();
	le_gcombo_box = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gcombo_box", module_number);

	memcpy(&gcombo_box_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gcombo_box_object_handlers.read_property  = gcombo_box_read_property;
	gcombo_box_object_handlers.get_properties = gcombo_box_get_properties;
	gcombo_box_object_handlers.write_property = gcombo_box_write_property;

	INIT_CLASS_ENTRY(ce, "GComboBox", gcombo_box_class_functions);
	gcombo_box_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	DECLARE_GCOMBO_BOX_PROP(GCOMBO_BOX_ACTIVE);
}


