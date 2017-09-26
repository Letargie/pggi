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


#include "gbox.h"

static int le_gbox;
static zend_object_handlers gbox_object_handlers;
static zend_class_entry * gbox_class_entry_ce;

zend_class_entry * gbox_get_class_entry(){
	return gbox_class_entry_ce;
}

zend_object_handlers * gbox_get_object_handlers(){
	return &gbox_object_handlers;
}

PHP_METHOD(GBox, __construct){
	long orientation, spacing = 0;
	ze_gwidget_object * widget;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &orientation, &spacing) == FAILURE)
		return;
	widget = Z_GWIDGET_P(getThis());
	widget->std.handlers = &gbox_object_handlers;
	switch(orientation){
		case GTK_ORIENTATION_HORIZONTAL:
		case GTK_ORIENTATION_VERTICAL  :
			widget->widget_ptr = gwidget_new();
			widget->widget_ptr->intern = gtk_box_new(orientation, spacing);
			break;
		default :
			zend_throw_exception_ex(pggi_exception_get(), 0, "the orientation need to be an ORIENTATION_*");
			
	}
	GCONTAINER_ADD_ELEMENT(widget);
	g_signal_connect(widget->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), widget);
}


PHP_METHOD(GBox, packStart){
	ze_gwidget_object *ze_obj = NULL;
	ze_gwidget_object * data;
	zval * self = getThis();
	zval * obj;
	int expand = 0;
	int fill = 1;
	long padding = 0;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|bbl", &obj , gwidget_get_class_entry(), &expand, &fill, &padding) == FAILURE)
		return;
	data = Z_GWIDGET_P(obj);
	ze_obj = Z_GWIDGET_P(self);
	gtk_box_pack_start(GTK_BOX(ze_obj->widget_ptr->intern), data->widget_ptr->intern, expand, fill, padding);
	gcontainer_add_data(ze_obj->widget_ptr, obj);
}


PHP_METHOD(GBox, packEnd){
	ze_gwidget_object *ze_obj = NULL;
	ze_gwidget_object * data;
	zval * self = getThis();
	zval * obj;
	int expand = 0;
	int fill = 1;
	long padding = 0;
	if(zend_parse_parameters(ZEND_NUM_ARGS(), "O|bbl", &obj, gwidget_get_class_entry() ,&expand, &fill, &padding) == FAILURE)
		return;
	data = Z_GWIDGET_P(obj);
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		gtk_box_pack_end(GTK_BOX(ze_obj->widget_ptr->intern), data->widget_ptr->intern, expand, fill, padding);
		gcontainer_add_data(ze_obj->widget_ptr, obj);
	}
}



static const zend_function_entry gbox_class_functions[] = {
	PHP_ME(GBox, __construct, arginfo_gbox_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GBox, packStart  , arginfo_gbox_pack     , ZEND_ACC_PUBLIC)
	PHP_ME(GBox, packEnd    , arginfo_gbox_pack     , ZEND_ACC_PUBLIC)
	PHP_FE_END
};



zval *gbox_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GBOX_SPACING)){
		ZVAL_LONG(rv, gtk_box_get_spacing(GTK_BOX(w->intern)));
	}else if(!strcmp(member_val, GBOX_BASELINE_POSITION)){
		ZVAL_LONG(rv, gtk_box_get_baseline_position(GTK_BOX(w->intern)));
	}else if(!strcmp(member_val, GBOX_HOMOGENEOUS)){
		ZVAL_BOOL(rv, gtk_box_get_homogeneous(GTK_BOX(w->intern)));
	}else
		return gcontainer_read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gbox_get_properties(zval *object){
	G_H_UPDATE_INIT(gcontainer_get_properties(object));
	const char * tmp;
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;

	G_H_UPDATE_LONG(GBOX_SPACING          , gtk_box_get_spacing          (GTK_BOX(w->intern)));
	G_H_UPDATE_LONG(GBOX_BASELINE_POSITION, gtk_box_get_baseline_position(GTK_BOX(w->intern)));
	G_H_UPDATE_BOOL(GBOX_HOMOGENEOUS      , gtk_box_get_homogeneous      (GTK_BOX(w->intern)));

	return G_H_UPDATE_RETURN;
}

void gbox_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	zval * tmp_member;
	long tmp_l;
	const char * tmp_s;
	double tmp_d;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, GBOX_SPACING))
				gtk_box_set_spacing(GTK_BOX(w->intern), tmp_l);
			else if(!strcmp(member_val, GBOX_BASELINE_POSITION))
				switch(tmp_l){
					case GTK_BASELINE_POSITION_TOP :
					case GTK_BASELINE_POSITION_CENTER :
					case GTK_BASELINE_POSITION_BOTTOM :
						gtk_box_set_baseline_position(GTK_BOX(w->intern), tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "the baselinePosition need to be a BASELINE_POSITION_*");
						break;
				}
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		case IS_FALSE :
		case IS_TRUE :
			tmp_b = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
			if(!strcmp(member_val, GBOX_HOMOGENEOUS))
				gtk_box_set_homogeneous(GTK_BOX(w->intern), tmp_b);
			else
				gcontainer_write_property(object, member, value, cache_slot);
			break;
		default:
			gcontainer_write_property(object, member, value, cache_slot);
	}
}


#define DECLARE_GBOX_PROP(name) \
DECLARE_CLASS_PROPERTY(gbox_class_entry_ce, name)

void gbox_init(int module_number){
	zend_class_entry ce;
	le_gbox = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "PGGI\\GBox", module_number);

	memcpy(&gbox_object_handlers, gcontainer_get_object_handlers(), sizeof(zend_object_handlers));
	gbox_object_handlers.read_property  = gbox_read_property;
	gbox_object_handlers.get_properties = gbox_get_properties;
	gbox_object_handlers.write_property = gbox_write_property;	
	INIT_CLASS_ENTRY(ce, "PGGI\\GBox", gbox_class_functions);
	ce.create_object 	= gwidget_object_new;
	gbox_class_entry_ce	= zend_register_internal_class_ex(&ce, gcontainer_get_class_entry());

	DECLARE_GBOX_PROP(GBOX_HOMOGENEOUS      );
	DECLARE_GBOX_PROP(GBOX_SPACING          );
	DECLARE_GBOX_PROP(GBOX_BASELINE_POSITION);
}


