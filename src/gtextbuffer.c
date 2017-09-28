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


#include "gtextbuffer.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gtext_buffer;
static zend_object_handlers gtext_buffer_object_handlers;
static zend_class_entry * gtext_buffer_class_entry_ce;

zend_class_entry * gtext_buffer_get_class_entry(){
	return gtext_buffer_class_entry_ce;
}

zend_object_handlers * gtext_buffer_get_object_handlers(){
	return &gtext_buffer_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gtext_buffer_ptr gtext_buffer_new(){
	gtext_buffer_ptr tor = ecalloc(1, sizeof(gtext_buffer_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gtext_buffer_object_new(zend_class_entry *class_type){
	ze_gtext_buffer_object *intern;
	intern = ecalloc(1, sizeof(ze_gtext_buffer_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gtext_buffer_object_handlers;
	return &intern->std;
}

void gtext_buffer_dtor(gtext_buffer_ptr intern){
	zval *  zv, * tmp;
	if (intern->intern){	
	/*unref text buffer?*/
	}/*
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&intern->signals), zv){
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(zv), tmp){
			zval_ptr_dtor(tmp);		
		} ZEND_HASH_FOREACH_END();
		zend_hash_destroy(Z_ARRVAL_P(zv));
	} ZEND_HASH_FOREACH_END();*/
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gtext_buffer_object_free_storage(zend_object *object){
	ze_gtext_buffer_object *intern = php_gtext_buffer_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->buffer_ptr){
		gtext_buffer_dtor(intern->buffer_ptr);
	}
	intern->buffer_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gtext_buffer_free_resource(zend_resource *rsrc) {
	gtext_buffer_ptr buffer = (gtext_buffer_ptr) rsrc->ptr;
	gtext_buffer_dtor(buffer);
}


#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gtext_buffer_function(gpointer data, unsigned int type){
	zval retval;
	ze_gtext_buffer_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gtext_buffer_reverse_object(b);
		ZVAL_OBJ(&args[0], this);
		array = &b->buffer_ptr->signals;
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
}

/*********************************/
/* internal on-related functions */
/*********************************/


void gtext_buffer_func_changed(GtkTextBuffer* buffer, gpointer data){
	gtext_buffer_function(data, gsignal_gtext_buffer_changed);
}

/***************/
/* PHP Methods */
/***************/

GTEXT_BUFFER_METHOD(__construct){
	ze_gtext_buffer_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GTEXT_BUFFER_P(self);
	ze_obj->buffer_ptr = gtext_buffer_new();
	ze_obj->buffer_ptr->intern = gtk_text_buffer_new(NULL);
}



GTEXT_BUFFER_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gtext_buffer_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GTEXT_BUFFER_P(this);
	switch(val){
		case gsignal_gtext_buffer_changed :
			break;
		default :
			zend_error(E_ERROR, "Signal unknown");
	}
	zval * data_to_insert = ecalloc(1,sizeof(zval));
	array_init(data_to_insert);
	zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_NAME, function);
	zval_addref_p(function);
	if(param){
		zend_hash_index_add(Z_ARRVAL_P(data_to_insert), INDEX_ON_FUNCTION_PARAM, param);
		zval_addref_p(param);
	}
	data = zend_hash_index_find(Z_ARRVAL_P(&ze_obj->buffer_ptr->signals), val);
	if(data == NULL){
		narray = ecalloc(1,sizeof(zval));
		array_init(narray);
		zend_hash_index_add(Z_ARRVAL_P(&ze_obj->buffer_ptr->signals), val, narray);
		zend_hash_next_index_insert(Z_ARRVAL_P(narray), data_to_insert);
		switch(val){
			case gsignal_gtext_buffer_changed :
				g_signal_connect(ze_obj->buffer_ptr->intern, GSIGNAL_GTEXT_BUFFER_CHANGED, G_CALLBACK (gtext_buffer_func_changed), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}


GTEXT_BUFFER_METHOD(getLineCount){
	ze_gtext_buffer_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GTEXT_BUFFER_P(self);
	RETURN_LONG(gtk_text_buffer_get_line_count(GTK_TEXT_BUFFER(ze_obj->buffer_ptr->intern)));
}

GTEXT_BUFFER_METHOD(getCharCount){
	ze_gtext_buffer_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GTEXT_BUFFER_P(self);
	RETURN_LONG(gtk_text_buffer_get_char_count(GTK_TEXT_BUFFER(ze_obj->buffer_ptr->intern)));
}

static const zend_function_entry gtext_buffer_class_functions[] = {
	PHP_ME(GTextBuffer, on          , arginfo_pggi_on	   , ZEND_ACC_PUBLIC)
	PHP_ME(GTextBuffer, __construct , arginfo_pggi_void	   , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GTextBuffer, getLineCount, arginfo_pggi_get_long, ZEND_ACC_PUBLIC)
	PHP_ME(GTextBuffer, getCharCount, arginfo_pggi_get_long, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gtext_buffer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gtext_buffer_object * intern = Z_GTEXT_BUFFER_P(object);
	gtext_buffer_ptr b = intern->buffer_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTextBuffer * buffer = GTK_TEXT_BUFFER(b->intern);
	if(!strcmp(member_val, GTEXT_BUFFER_TEXT)){
		GtkTextIter * start, * end;
		gtk_text_buffer_get_start_iter(buffer, start);
		gtk_text_buffer_get_end_iter(buffer, end);
		tmp = gtk_text_buffer_get_text(buffer, start, end, 1);
		if(tmp){
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else{
			ZVAL_NULL(rv);
		}
	}
	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gtext_buffer_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	const char * tmp;
	ze_gtext_buffer_object * intern = Z_GTEXT_BUFFER_P(object);
	gtext_buffer_ptr b = intern->buffer_ptr;
	GtkTextBuffer * buffer = GTK_TEXT_BUFFER(b->intern);
	GtkTextIter * start, * end;
	gtk_text_buffer_get_start_iter(buffer, start);
	gtk_text_buffer_get_end_iter(buffer, end);
	G_H_UPDATE_STRING	(GTEXT_BUFFER_TEXT, gtk_text_buffer_get_text(buffer, start, end, 1));
	return G_H_UPDATE_RETURN;
}

void gtext_buffer_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gtext_buffer_object * intern = Z_GTEXT_BUFFER_P(object);
	gtext_buffer_ptr b = intern->buffer_ptr;
	zval * tmp_member;
	long tmp_l;
	const char * tmp_s;
	double tmp_d;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTextBuffer * buffer = GTK_TEXT_BUFFER(b->intern);
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(member_val, GTEXT_BUFFER_TEXT)){
				gtk_text_buffer_set_text(buffer, Z_STRVAL_P(value), Z_STRLEN_P(value));
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/************************************/
/* GTextBuffer Class Initialization */
/************************************/

#define DECLARE_GTEXT_BUFFER_PROP(name) \
DECLARE_CLASS_PROPERTY(gtext_buffer_class_entry_ce, name)

#define GTEXT_BUFFER_CONSTANT(name, value) \
zend_declare_class_constant_double(gtext_buffer_class_entry_ce, name, sizeof(name)-1, value);


void gtext_buffer_init(int module_number){
	zend_class_entry ce;
	le_gtext_buffer = zend_register_list_destructors_ex(gtext_buffer_free_resource, NULL, "PGGI\\GTextBuffer", module_number);

	memcpy(&gtext_buffer_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gtext_buffer_object_handlers.offset         = XtOffsetOf(ze_gtext_buffer_object, std);
	gtext_buffer_object_handlers.free_obj       = gtext_buffer_object_free_storage;
	gtext_buffer_object_handlers.clone_obj      = NULL;
	gtext_buffer_object_handlers.read_property  = gtext_buffer_read_property;
	gtext_buffer_object_handlers.get_properties = gtext_buffer_get_properties;
	gtext_buffer_object_handlers.write_property = gtext_buffer_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GTextBuffer", gtext_buffer_class_functions);
	ce.create_object = gtext_buffer_object_new;
	gtext_buffer_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_GTEXT_BUFFER_PROP(GTEXT_BUFFER_TEXT);
}


