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


#include "gcontainer.h"


static int le_gcontainer;
static zend_object_handlers gcontainer_object_handlers;
static zend_class_entry * gcontainer_class_entry_ce;

zend_class_entry * gcontainer_get_class_entry(){
	return gcontainer_class_entry_ce;
}

zend_object_handlers * gcontainer_get_object_handlers(){
	return &gcontainer_object_handlers;
}

void gcontainer_add_data(gwidget_ptr intern, zval * data){
	zend_hash_next_index_insert(Z_ARRVAL_P(&intern->data), data);
	zval_addref_p(data);
}

/*****************/
/* PHP FUNCTIONS */
/*****************/

PHP_METHOD(GContainer, __construct){
	ze_gwidget_object *ze_obj = NULL;
	zval * self = getThis();
	if(self){
		ze_obj = Z_GWIDGET_P(self);
		ze_obj->std.handlers = &gcontainer_object_handlers;
		g_signal_connect(ze_obj->widget_ptr->intern, "destroy", G_CALLBACK (widget_destructed), ze_obj);
	}
}

PHP_METHOD(GContainer, add){
	ze_gwidget_object * data;
	ze_gwidget_object * this;
	zval * obj;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &obj) == FAILURE) {
        RETURN_NULL();
    }
	data = Z_GWIDGET_P(obj);
	this = Z_GWIDGET_P(getThis());
	if(!this)
		RETURN_NULL();
	gtk_container_add(GTK_CONTAINER(this->widget_ptr->intern), data->widget_ptr->intern);
	gcontainer_add_data(this->widget_ptr, obj);
}

void gcontainer_func_add(GtkContainer* container, gpointer data){
	gwidget_function(data, gsignal_gcontainer_add);
}

void gcontainer_on(long val,ze_gwidget_object * ze_obj, zval * function, zval * this){
	zval * data, * narray;
	switch(val){
		case gsignal_gcontainer_add :
			break;
		default :
			zend_error(E_ERROR, "Signal unknown");
	}
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->widget_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->widget_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), function);
		zval_addref_p(function);
		switch(val){
			case gsignal_gcontainer_add :
				g_signal_connect(ze_obj->widget_ptr->intern, GSIGNAL_GCONTAINER_ADD, G_CALLBACK (gcontainer_func_add), (gpointer) this);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), function);
		/*zval_addref_p(function);*/
	}
}

PHP_METHOD(GContainer, on){
	zval * function, * this;
	long val;
	ze_gwidget_object *ze_obj = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz", &val ,&function) == FAILURE) {
        RETURN_NULL();
    }
	if(!zend_is_callable(function, 0, NULL))
		zend_error(E_ERROR, "Function requires callable argument");
	this = getThis();
	if(!this)
		RETURN_NULL();
	ze_obj = Z_GWIDGET_P(this);
	gcontainer_on(val, ze_obj, function, this);
}

/*****************************/
/* Object handling functions */
/*****************************/

zval *gcontainer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	const char * tmp;
	ZVAL_NULL(rv);
	if(!w)
		return rv;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, GCONTAINER_BORDER_WIDTH)){
		ZVAL_LONG(rv, gtk_container_get_border_width(GTK_CONTAINER(w->intern)));
		return rv;
	}
	return gwidget_read_property(object, member, type, cache_slot, rv);
}

#define G_H_UPDATE(name) \
zend_hash_update(props, zend_string_init(name, sizeof(name)-1, 0), &zv)

HashTable *gcontainer_get_properties(zval *object){
	G_H_UPDATE_INIT(gwidget_get_properties(object));
	const char * tmp;
	ze_gwidget_object * intern = Z_GWIDGET_P(object);
	gwidget_ptr w = intern->widget_ptr;
	if(!w){
		return NULL;
	}
	G_H_UPDATE_LONG(GCONTAINER_BORDER_WIDTH, gtk_container_get_border_width(GTK_CONTAINER(w->intern)));
	return G_H_UPDATE_RETURN;
}

void gcontainer_write_property(zval *object, zval *member, zval *value, void **cache_slot){
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
			if(!strcmp(member_val, GCONTAINER_BORDER_WIDTH))
				gtk_container_set_border_width(GTK_CONTAINER(w->intern), tmp_l);
			else
				gwidget_write_property(object, member, value, cache_slot);
			break;
		default:
			gwidget_write_property(object, member, value, cache_slot);
	}
}


static const zend_function_entry gcontainer_class_functions[] = {
	PHP_ME(GContainer, add			, arginfo_gcontainer_add	, ZEND_ACC_PUBLIC)
	PHP_ME(GContainer, __construct	, arginfo_pggi_void			, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

/***********************************/
/* GContainer class initialisation */
/***********************************/


void gcontainer_init(int module_number){
	zend_class_entry ce;
	le_gcontainer = zend_register_list_destructors_ex(gwidget_free_resource, NULL, "gcontainer", module_number);

	memcpy(&gcontainer_object_handlers, gwidget_get_object_handlers(), sizeof(zend_object_handlers));
	gcontainer_object_handlers.read_property  = gcontainer_read_property;
	gcontainer_object_handlers.get_properties = gcontainer_get_properties;
	gcontainer_object_handlers.write_property = gcontainer_write_property;
	INIT_CLASS_ENTRY(ce, "GContainer", gcontainer_class_functions);
	ce.create_object = gwidget_object_new;
	gcontainer_class_entry_ce = zend_register_internal_class_ex(&ce, gwidget_get_class_entry());

	DECLARE_CLASS_PROPERTY(gcontainer_class_entry_ce, GCONTAINER_BORDER_WIDTH);
}


