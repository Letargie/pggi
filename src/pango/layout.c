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


#include "layout.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_layout;
static zend_object_handlers layout_object_handlers;
static zend_class_entry * layout_class_entry_ce;

zend_class_entry * pp_layout_get_class_entry(){
	return layout_class_entry_ce;
}

zend_object_handlers * pp_layout_get_object_handlers(){
	return &layout_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

pp_layout_ptr pp_layout_new(){
	pp_layout_ptr tor = ecalloc(1, sizeof(pp_layout_t));
	tor->to_destroy = 0;
	return tor;
}

zend_object *pp_layout_object_new(zend_class_entry *class_type){
	ze_layout_object *intern;
	intern = ecalloc(1, sizeof(ze_layout_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &layout_object_handlers;
	return &intern->std;
}

void pp_layout_dtor(pp_layout_ptr intern){
	if (intern->intern && intern->to_destroy){
		g_object_unref(intern->intern);
	}
	efree(intern);
}

void pp_layout_object_free_storage(zend_object *object){
	ze_layout_object *intern = php_layout_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->layout_ptr){
		pp_layout_dtor(intern->layout_ptr);
	}
	intern->layout_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void pp_layout_free_resource(zend_resource *rsrc) {
	pp_layout_ptr layout = (pp_layout_ptr) rsrc->ptr;
	pp_layout_dtor(layout);
}

/***************/
/* PHP Methods */
/***************/

LAYOUT_METHOD(__construct){
	zval * self = getThis();
	ze_layout_object * ze_obj;
	zval * context;
	ze_obj = Z_LAYOUT_P(self);
	ze_obj->layout_ptr = pp_layout_new();
	if(zend_parse_parameters(ZEND_NUM_ARGS(), "O", &context, pp_context_get_class_entry()) == FAILURE){
		if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &context, pc_context_get_class_entry()) == FAILURE)
			return;
		ze_context_object * obj = Z_CONTEXT_P(context);
		ze_obj->layout_ptr->intern = pango_cairo_create_layout(obj->context_ptr->intern);
	}else{
		ze_pp_context_object * obj = Z_PP_CONTEXT_P(context);
		ze_obj->layout_ptr->intern = pango_layout_new(obj->context_ptr->intern);
	}
	ze_obj->layout_ptr->to_destroy = 1;
}


LAYOUT_METHOD(getSize){
	zval * self = getThis();
	ze_layout_object * ze_obj;
	long width, height;
	int tmp_w, tmp_h;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ll", &width, &height) == FAILURE)
		return;
	ze_obj = Z_LAYOUT_P(self);
	pango_layout_get_size(ze_obj->layout_ptr->intern, &tmp_w, &tmp_h);
	width = tmp_w;
	height = tmp_h;
}



static const zend_function_entry pp_layout_class_functions[] = {
	PHP_ME(Layout, __construct, arginfo_layout_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(Layout, getSize    , arginfo_layout_get_size , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *pp_layout_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_layout_object * intern = Z_LAYOUT_P(object);
	pp_layout_ptr l = intern->layout_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	 if(!strcmp(member_val, LAYOUT_WIDTH)){
		ZVAL_LONG(rv, pango_layout_get_width(l->intern));
	}else if(!strcmp(member_val, LAYOUT_HEIGHT)){
		ZVAL_LONG(rv, pango_layout_get_height(l->intern));
	}else if(!strcmp(member_val, LAYOUT_WRAP)){
		ZVAL_LONG(rv, pango_layout_get_wrap(l->intern));
	}else if(!strcmp(member_val, LAYOUT_TEXT)){
		const char * tmp = pango_layout_get_text(l->intern);
		ZVAL_STRINGL(rv, estrdup(tmp), strlen(tmp));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *pp_layout_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_layout_object * intern = Z_LAYOUT_P(object);
	pp_layout_ptr l = intern->layout_ptr;
	G_H_UPDATE_LONG  (LAYOUT_WIDTH ,pango_layout_get_width (l->intern));
	G_H_UPDATE_LONG  (LAYOUT_HEIGHT,pango_layout_get_height(l->intern));
	G_H_UPDATE_LONG  (LAYOUT_WRAP  ,pango_layout_get_wrap  (l->intern));
	G_H_UPDATE_STRING(LAYOUT_TEXT  ,pango_layout_get_text  (l->intern));
	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE pp_layout_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_layout_object * intern = Z_LAYOUT_P(object);
	pp_layout_ptr l = intern->layout_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, LAYOUT_HEIGHT)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		pango_layout_set_width(l->intern, tmp_l);
	}else if(!strcmp(member_val, LAYOUT_WIDTH)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		pango_layout_set_height(l->intern, tmp_l);
	}else if(!strcmp(member_val, LAYOUT_WRAP)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case PANGO_WRAP_WORD     :
			case PANGO_WRAP_CHAR     :
			case PANGO_WRAP_WORD_CHAR:
				pango_layout_set_wrap(l->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the gravityHint property, needs to be a Context::GRAVITY_HINT_*");
				return;
				break;
		}
	}else if(!strcmp(member_val, LAYOUT_TEXT)){
		convert_to_string(value);
		pango_layout_set_text(l->intern, Z_STRVAL_P(value), Z_STRLEN_P(value));
	} else {
		PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
	}
	PHP_WRITE_PROP_HANDLER_RETURN(value);
}

/************************/
/* Class Initialization */
/************************/

#define DECLARE_LAYOUT_PROP(name) \
DECLARE_CLASS_PROPERTY(layout_class_entry_ce, name)

#define LAYOUT_CONSTANT(name, value) \
zend_declare_class_constant_long(layout_class_entry_ce, name, sizeof(name)-1, value);


void pp_layout_init(int module_number){
	zend_class_entry ce;
	le_layout = zend_register_list_destructors_ex(pp_layout_free_resource, NULL, "PGGI\\Pango\\Layout", module_number);

	memcpy(&layout_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	layout_object_handlers.offset         = XtOffsetOf(ze_layout_object, std);
	layout_object_handlers.free_obj       = pp_layout_object_free_storage;
	layout_object_handlers.clone_obj      = NULL;
	layout_object_handlers.read_property  = pp_layout_read_property;
	layout_object_handlers.get_properties = pp_layout_get_properties;
	layout_object_handlers.write_property = pp_layout_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Pango\\Layout", pp_layout_class_functions);
	ce.create_object = pp_layout_object_new;
	layout_class_entry_ce = zend_register_internal_class(&ce);

	LAYOUT_CONSTANT("WRAP_WORD"     , PANGO_WRAP_WORD     );
	LAYOUT_CONSTANT("WRAP_CHAR"     , PANGO_WRAP_CHAR     );
	LAYOUT_CONSTANT("WRAP_WORD_CHAR", PANGO_WRAP_WORD_CHAR);

	DECLARE_LAYOUT_PROP(LAYOUT_WIDTH );
	DECLARE_LAYOUT_PROP(LAYOUT_HEIGHT);
	DECLARE_LAYOUT_PROP(LAYOUT_WRAP  );
	DECLARE_LAYOUT_PROP(LAYOUT_TEXT  );
}

