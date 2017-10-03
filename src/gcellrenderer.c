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


#include "gcellrenderer.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gcell_renderer;
static zend_object_handlers gcell_renderer_object_handlers;
static zend_class_entry * gcell_renderer_class_entry_ce;

zend_class_entry * gcell_renderer_get_class_entry(){
	return gcell_renderer_class_entry_ce;
}

zend_object_handlers * gcell_renderer_get_object_handlers(){
	return &gcell_renderer_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gcell_renderer_ptr gcell_renderer_new(){
	gcell_renderer_ptr tor = ecalloc(1, sizeof(gcell_renderer_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gcell_renderer_object_new(zend_class_entry *class_type){
	ze_gcell_renderer_object *intern;
	intern = ecalloc(1, sizeof(ze_gcell_renderer_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &gcell_renderer_object_handlers;
	return &intern->std;
}

void gcell_renderer_dtor(gcell_renderer_ptr intern){
	if (intern->intern){	
	/*unref text buffer?*/
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gcell_renderer_object_free_storage(zend_object *object){
	ze_gcell_renderer_object *intern = php_gcell_renderer_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->cell_renderer_ptr){
		gcell_renderer_dtor(intern->cell_renderer_ptr);
	}
	intern->cell_renderer_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gcell_renderer_free_resource(zend_resource *rsrc) {
	gcell_renderer_ptr buffer = (gcell_renderer_ptr) rsrc->ptr;
	gcell_renderer_dtor(buffer);
}

/*
#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gcell_renderer_function(gpointer data, unsigned int type){
	zval retval;
	ze_gcell_renderer_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gcell_renderer_reverse_object(b);
		ZVAL_OBJ(&args[0], this);
		array = &b->cell_renderer_ptr->signals;
		value = (zend_hash_index_find(Z_ARRVAL_P(array), type));
		if(value != NULL){
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(value), zv){
				zval * function	= zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_NAME);
				zval * tmp = zend_hash_index_find(Z_ARRVAL_P(zv), INDEX_ON_FUNCTION_PARAM);
				if(tmp){
					ZVAL_COPY(&args[1], tmp);
				}else
					ZVAL_NULL(&args[1]);
				if(call_user_function(EG(function_table), NULL, function, &retval, 1, args) != SUCCESS){
					zend_error(E_ERROR, "Function call failed");
				}
			} ZEND_HASH_FOREACH_END();
		}
	}
}*/

/*********************************/
/* internal on-related functions */
/*********************************/

/*
void gcell_renderer_func_changed(GtkCellRenderer* buffer, gpointer data){
	gcell_renderer_function(data, gsignal_gcell_renderer_changed);
}*/

/***************/
/* PHP Methods */
/***************/

/*

GCELL_RENDERER_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gcell_renderer_object *ze_obj = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	if(!zend_is_callable(function, 0, NULL))
		zend_error(E_ERROR, "Function requires callable argument");
	this = getThis();
	if(!this)
		RETURN_NULL();
	ze_obj = Z_GCELL_RENDERER_P(this);
	switch(val){
		case gsignal_gcell_renderer_changed :
			break;
		default :
			zend_error(E_ERROR, "Signal unknown");
	}
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
	}
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->cell_renderer_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->cell_renderer_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gcell_renderer_changed :
				g_signal_connect(ze_obj->cell_renderer_ptr->intern, GSIGNAL_GCELL_RENDERER_CHANGED, G_CALLBACK (gcell_renderer_func_changed), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}*/

static const zend_function_entry gcell_renderer_class_functions[] = {
	//PHP_ME(GCellRenderer, on, arginfo_pggi_on, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gcell_renderer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	//ze_gcell_renderer_object * intern = Z_GCELL_RENDERER_P(object);
	//gcell_renderer_ptr b = intern->cell_renderer_ptr;
	//convert_to_string(member);
	//char * member_val = Z_STRVAL_P(member);
	//GtkCellRenderer * buffer = GTK_CELL_RENDERER(b->intern);
	/*if(!strcmp(member_val, GCELL_RENDERER_TEXT)){
		GtkTextIter * start, * end;
		gtk_cell_renderer_get_start_iter(buffer, start);
		gtk_cell_renderer_get_end_iter(buffer, end);
		tmp = gtk_cell_renderer_get_text(buffer, start, end, 1);
		if(tmp){
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else{
			ZVAL_NULL(rv);
		}
	}*/
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gcell_renderer_get_properties(zval *object){
	/*G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_gcell_renderer_object * intern = Z_GCELL_RENDERER_P(object);
	gcell_renderer_ptr b = intern->cell_renderer_ptr;
	GtkCellRenderer * buffer = GTK_CELL_RENDERER(b->intern);
	//G_H_UPDATE_BOOL(GCELL_RENDERER_VISIBLE, gtk_cell_renderer_get_text(renderer));
	//G_H_UPDATE_BOOL(GCELL_RENDERER_SENSIBLE, gtk_cell_renderer_get_text(renderer));
	G_H_UPDATE_LONG(GCELL_RENDERER_X_ALIGN, gtk_cell_renderer_get_text(renderer));
	G_H_UPDATE_LONG(GCELL_RENDERER_Y_ALIGN, gtk_cell_renderer_get_text(renderer));
	G_H_UPDATE_LONG(GCELL_RENDERER_X_PAD, gtk_cell_renderer_get_text(renderer));
	G_H_UPDATE_LONG(GCELL_RENDERER_Y_PAD, gtk_cell_renderer_get_text(renderer));
	return G_H_UPDATE_RETURN;*/
	return zend_std_get_properties(object);
}

void gcell_renderer_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	//ze_gcell_renderer_object * intern = Z_GCELL_RENDERER_P(object);
	//gcell_renderer_ptr b = intern->cell_renderer_ptr;
	//convert_to_string(member);
	//char * member_val = Z_STRVAL_P(member);
	//GtkCellRenderer * buffer = GTK_CELL_RENDERER(b->intern);
	switch(Z_TYPE_P(value)){
		/*case IS_FALSE :
		case IS_TRUE :
			if(!strcmp(member_val, GCELL_RENDERER_VISIBLE)){
				gtk_cell_renderer_set_text(buffer, Z_STRVAL_P(value), Z_STRLEN_P(value));
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;*/
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/************************************/
/* GCellRenderer Class Initialization */
/************************************/

#define DECLARE_GCELL_RENDERER_PROP(name) \
DECLARE_CLASS_PROPERTY(gcell_renderer_class_entry_ce, name)

#define GCELL_RENDERER_CONSTANT(name, value) \
zend_declare_class_constant_double(gcell_renderer_class_entry_ce, name, sizeof(name)-1, value);


void gcell_renderer_init(int module_number){
	zend_class_entry ce;
	le_gcell_renderer = zend_register_list_destructors_ex(gcell_renderer_free_resource, NULL, "PGGI\\GCellRenderer", module_number);

	memcpy(&gcell_renderer_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gcell_renderer_object_handlers.offset         = XtOffsetOf(ze_gcell_renderer_object, std);
	gcell_renderer_object_handlers.free_obj       = gcell_renderer_object_free_storage;
	gcell_renderer_object_handlers.clone_obj      = NULL;
	gcell_renderer_object_handlers.read_property  = gcell_renderer_read_property;
	gcell_renderer_object_handlers.get_properties = gcell_renderer_get_properties;
	gcell_renderer_object_handlers.write_property = gcell_renderer_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GCellRenderer", gcell_renderer_class_functions);
	ce.create_object = gcell_renderer_object_new;
	gcell_renderer_class_entry_ce = zend_register_internal_class(&ce);
	gcell_renderer_class_entry_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	//DECLARE_GCELL_RENDERER_PROP(GCELL_RENDERER_VISIBLE );
	//DECLARE_GCELL_RENDERER_PROP(GCELL_RENDERER_SENSIBLE);
	/*DECLARE_GCELL_RENDERER_PROP(GCELL_RENDERER_X_ALIGN );
	DECLARE_GCELL_RENDERER_PROP(GCELL_RENDERER_Y_ALIGN );
	DECLARE_GCELL_RENDERER_PROP(GCELL_RENDERER_X_PAD   );
	DECLARE_GCELL_RENDERER_PROP(GCELL_RENDERER_Y_PAD   );*/
}


