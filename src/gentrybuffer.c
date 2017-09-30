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


#include "gentrybuffer.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gentry_buffer;
static zend_object_handlers gentry_buffer_object_handlers;
static zend_class_entry * gentry_buffer_class_entry_ce;

zend_class_entry * gentry_buffer_get_class_entry(){
	return gentry_buffer_class_entry_ce;
}

zend_object_handlers * gentry_buffer_get_object_handlers(){
	return &gentry_buffer_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

gentry_buffer_ptr gentry_buffer_new(){
	gentry_buffer_ptr tor = ecalloc(1, sizeof(gentry_buffer_t));
	array_init(&tor->signals);
	return tor;
}

zend_object *gentry_buffer_object_new(zend_class_entry *class_type){
	ze_gentry_buffer_object *intern;
	intern = ecalloc(1, sizeof(ze_gentry_buffer_object) + zend_object_properties_size(class_type));
	zend_object_std_init	(&intern->std, class_type);
	object_properties_init	(&intern->std, class_type);
	intern->std.handlers = &gentry_buffer_object_handlers;
	return &intern->std;
}

void gentry_buffer_dtor(gentry_buffer_ptr intern){
	zval *  zv, * tmp;
	if (intern->intern){	
	/*unref entry buffer?*/
	}
	zend_hash_destroy(Z_ARRVAL_P(&intern->signals));
	efree(intern);
}

void gentry_buffer_object_free_storage(zend_object *object){
	ze_gentry_buffer_object *intern = php_gentry_buffer_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->buffer_ptr){
		gentry_buffer_dtor(intern->buffer_ptr);
	}
	intern->buffer_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gentry_buffer_free_resource(zend_resource *rsrc) {
	gentry_buffer_ptr buffer = (gentry_buffer_ptr) rsrc->ptr;
	gentry_buffer_dtor(buffer);
}

/*
#define INDEX_ON_FUNCTION_NAME 1
#define INDEX_ON_FUNCTION_PARAM 2

void gentry_buffer_function(gpointer data, unsigned int type){
	zval retval;
	ze_gentry_buffer_object * b = data;
	zval * array, * value, * zv;
	zval args[2];
	if(type){
		zend_object * this = php_gentry_buffer_reverse_object(b);
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
}*/

/*********************************/
/* internal on-related functions */
/*********************************/

/*
void gentry_buffer_func_changed(GtkEntryBuffer* buffer, gpointer data){
	gentry_buffer_function(data, gsignal_gentry_buffer_changed);
}*/

/***************/
/* PHP Methods */
/***************/

GENTRY_BUFFER_METHOD(__construct){
	ze_gentry_buffer_object *ze_obj = NULL;
	zval * self = getThis();
	char *in = NULL;
	size_t in_len = -1;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &in, &in_len) == FAILURE)
		return;
	ze_obj = Z_GENTRY_BUFFER_P(self);
	ze_obj->buffer_ptr = gentry_buffer_new();
	ze_obj->buffer_ptr->intern = gtk_entry_buffer_new(in, in_len);
}


/*
GENTRY_BUFFER_METHOD(on){
	zval * function, * data,* narray, * this, * param = NULL;
	long val;
	ze_gentry_buffer_object *ze_obj = NULL;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz|z", &val ,&function, &param) == FAILURE)
		return;
	this = getThis();
	ze_obj = Z_GENTRY_BUFFER_P(this);
	switch(val){
		case gsignal_gentry_buffer_changed :
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
			case gsignal_gentry_buffer_changed :
				g_signal_connect(ze_obj->buffer_ptr->intern, GSIGNAL_GENTRY_BUFFER_CHANGED, G_CALLBACK (gentry_buffer_func_changed), (gpointer) ze_obj);
				break;
		}
	}else{
		zend_hash_next_index_insert(Z_ARRVAL_P(data), data_to_insert);
	}
}
*/
GENTRY_BUFFER_METHOD(getLength){
	ze_gentry_buffer_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GENTRY_BUFFER_P(self);
	RETURN_LONG(gtk_entry_buffer_get_length(ze_obj->buffer_ptr->intern));
}

GENTRY_BUFFER_METHOD(getBytes){
	ze_gentry_buffer_object *ze_obj = NULL;
	zval * self = getThis();
	if(pggi_parse_method_parameters_none(self) == FAILURE)
		return ;
	ze_obj = Z_GENTRY_BUFFER_P(self);
	RETURN_LONG(gtk_entry_buffer_get_bytes(ze_obj->buffer_ptr->intern));
}

//TODO
GENTRY_BUFFER_METHOD(insertText);
GENTRY_BUFFER_METHOD(deleteText);
GENTRY_BUFFER_METHOD(emitInsertText);
GENTRY_BUFFER_METHOD(emitDeleteText);

static const zend_function_entry gentry_buffer_class_functions[] = {
	//PHP_ME(GEntryBuffer, on          , arginfo_pggi_on	          , ZEND_ACC_PUBLIC)
	PHP_ME(GEntryBuffer, __construct , arginfo_gentry_buffer_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(GEntryBuffer, getLength   , arginfo_pggi_get_long          , ZEND_ACC_PUBLIC)
	PHP_ME(GEntryBuffer, getBytes    , arginfo_pggi_get_long          , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gentry_buffer_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gentry_buffer_object * intern = Z_GENTRY_BUFFER_P(object);
	gentry_buffer_ptr b = intern->buffer_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkEntryBuffer * buffer = b->intern;
	if(!strcmp(member_val, GENTRY_BUFFER_TEXT)){
		tmp = gtk_entry_buffer_get_text(buffer);
		if(tmp){
			ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
		}else{
			ZVAL_NULL(rv);
		}
	}
	if(!strcmp(member_val, GENTRY_BUFFER_MAX_LENGTH)){
		ZVAL_LONG(rv, gtk_entry_buffer_get_max_length(buffer));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *gentry_buffer_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	const char * tmp;
	ze_gentry_buffer_object * intern = Z_GENTRY_BUFFER_P(object);
	gentry_buffer_ptr b = intern->buffer_ptr;
	GtkEntryBuffer * buffer = b->intern;
	G_H_UPDATE_STRING(GENTRY_BUFFER_TEXT      , gtk_entry_buffer_get_text      (buffer));
	G_H_UPDATE_LONG  (GENTRY_BUFFER_MAX_LENGTH, gtk_entry_buffer_get_max_length(buffer));
	return G_H_UPDATE_RETURN;
}

void gentry_buffer_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gentry_buffer_object * intern = Z_GENTRY_BUFFER_P(object);
	gentry_buffer_ptr b = intern->buffer_ptr;
	zval * tmp_member;
	long tmp_l;
	const char * tmp_s;
	double tmp_d;
	int tmp_b;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkEntryBuffer * buffer = b->intern;
	switch(Z_TYPE_P(value)){
		case IS_STRING :
			if(!strcmp(member_val, GENTRY_BUFFER_TEXT)){
				gtk_entry_buffer_set_text(buffer, Z_STRVAL_P(value), Z_STRLEN_P(value));
			}else 
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		case IS_LONG :
			if(!strcmp(member_val, GENTRY_BUFFER_MAX_LENGTH)){
				gtk_entry_buffer_set_max_length(buffer, Z_LVAL_P(value));
			}else 
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/************************************/
/* GEntryBuffer Class Initialization */
/************************************/

#define DECLARE_GENTRY_BUFFER_PROP(name) \
DECLARE_CLASS_PROPERTY(gentry_buffer_class_entry_ce, name)

#define GENTRY_BUFFER_CONSTANT(name, value) \
zend_declare_class_constant_double(gentry_buffer_class_entry_ce, name, sizeof(name)-1, value);


void gentry_buffer_init(int module_number){
	zend_class_entry ce;
	le_gentry_buffer = zend_register_list_destructors_ex(gentry_buffer_free_resource, NULL, "PGGI\\GEntryBuffer", module_number);

	memcpy(&gentry_buffer_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gentry_buffer_object_handlers.offset         = XtOffsetOf(ze_gentry_buffer_object, std);
	gentry_buffer_object_handlers.free_obj       = gentry_buffer_object_free_storage;
	gentry_buffer_object_handlers.clone_obj      = NULL;
	gentry_buffer_object_handlers.read_property  = gentry_buffer_read_property;
	gentry_buffer_object_handlers.get_properties = gentry_buffer_get_properties;
	gentry_buffer_object_handlers.write_property = gentry_buffer_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GEntryBuffer", gentry_buffer_class_functions);
	ce.create_object = gentry_buffer_object_new;
	gentry_buffer_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_GENTRY_BUFFER_PROP(GENTRY_BUFFER_TEXT);
	DECLARE_GENTRY_BUFFER_PROP(GENTRY_BUFFER_MAX_LENGTH);
}


