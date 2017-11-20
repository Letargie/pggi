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


#include "context.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_context;
static zend_object_handlers context_object_handlers;
static zend_class_entry * context_class_entry_ce;

zend_class_entry * pp_context_get_class_entry(){
	return context_class_entry_ce;
}

zend_object_handlers * pp_context_get_object_handlers(){
	return &context_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

pp_context_ptr pp_context_new(){
	pp_context_ptr tor = ecalloc(1, sizeof(pp_context_t));
	tor->to_destroy = 0;
	return tor;
}

zend_object *pp_context_object_new(zend_class_entry *class_type){
	ze_pp_context_object *intern;
	intern = ecalloc(1, sizeof(ze_pp_context_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &context_object_handlers;
	return &intern->std;
}

void pp_context_dtor(pp_context_ptr intern){
	if (intern->intern && intern->to_destroy){
		g_object_unref(intern->intern);
	}
	efree(intern);
}

void pp_context_object_free_storage(zend_object *object){
	ze_pp_context_object *intern = php_pp_context_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->context_ptr){
		pp_context_dtor(intern->context_ptr);
	}
	intern->context_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void pp_context_free_resource(zend_resource *rsrc) {
	pp_context_ptr context = (pp_context_ptr) rsrc->ptr;
	pp_context_dtor(context);
}

/***************/
/* PHP Methods */
/***************/

PANGO_CONTEXT_METHOD(__construct){
	zval * self = getThis();
	ze_pp_context_object * ze_obj;
	zval * widget;
	// A fontmap
	// A widget
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &widget, gwidget_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_PP_CONTEXT_P(self);
	ze_gwidget_object * obj = Z_GWIDGET_P(widget);
	ze_obj->context_ptr = pp_context_new();
	ze_obj->context_ptr->intern = gtk_widget_create_pango_context(obj->widget_ptr->intern);
	//pango_font_map_create_context();// font map // gtk_widget_create_pango_context() // widget
	ze_obj->context_ptr->to_destroy = 1;
}

PANGO_CONTEXT_METHOD(getGravity){
	zval * self = getThis();
	ze_pp_context_object * ze_obj;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_PP_CONTEXT_P(self);
	RETURN_LONG(pango_context_get_gravity(ze_obj->context_ptr->intern));
}

static const zend_function_entry pp_context_class_functions[] = {
	PANGO_CONTEXT_ME(__construct, arginfo_pp_context_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PANGO_CONTEXT_ME(getGravity , arginfo_pggi_get_long       , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *pp_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_pp_context_object * intern = Z_PP_CONTEXT_P(object);
	pp_context_ptr c = intern->context_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	 if(!strcmp(member_val, CONTEXT_BASE_DIR)){
		ZVAL_LONG(rv, pango_context_get_base_dir(c->intern));
	}else if(!strcmp(member_val, CONTEXT_BASE_GRAVITY)){
		ZVAL_LONG(rv, pango_context_get_base_gravity(c->intern));
	}else if(!strcmp(member_val, CONTEXT_GRAVITY_HINT)){
		ZVAL_LONG(rv, pango_context_get_gravity_hint(c->intern));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *pp_context_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_pp_context_object * intern = Z_PP_CONTEXT_P(object);
	pp_context_ptr c = intern->context_ptr;
	G_H_UPDATE_LONG(CONTEXT_BASE_DIR    , pango_context_get_base_dir    (c->intern));
	G_H_UPDATE_LONG(CONTEXT_BASE_GRAVITY, pango_context_get_base_gravity(c->intern));
	G_H_UPDATE_LONG(CONTEXT_GRAVITY_HINT, pango_context_get_gravity_hint(c->intern));
	return G_H_UPDATE_RETURN;
}

void pp_context_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_pp_context_object * intern = Z_PP_CONTEXT_P(object);
	pp_context_ptr c = intern->context_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, CONTEXT_BASE_DIR)){
				switch(tmp_l){
					case PANGO_DIRECTION_LTR     :
					case PANGO_DIRECTION_RTL     :
					case PANGO_DIRECTION_TTB_LTR :
					case PANGO_DIRECTION_TTB_RTL :
					case PANGO_DIRECTION_WEAK_LTR:
					case PANGO_DIRECTION_WEAK_RTL:
					case PANGO_DIRECTION_NEUTRAL :
						pango_context_set_base_dir(c->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the baseDir property, needs to be a Context::DIRECTION_*");
						return;
						break;
				}
			}else if(!strcmp(member_val, CONTEXT_BASE_GRAVITY)){
				switch(tmp_l){
					case PANGO_GRAVITY_SOUTH:
					case PANGO_GRAVITY_EAST :
					case PANGO_GRAVITY_NORTH:
					case PANGO_GRAVITY_WEST :
					case PANGO_GRAVITY_AUTO :
						pango_context_set_base_gravity(c->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the baseGravity property, needs to be a Context::GRAVITY_*");
						return;
						break;
				}
			}else if(!strcmp(member_val, CONTEXT_GRAVITY_HINT)){
				switch(tmp_l){
					case PANGO_GRAVITY_HINT_NATURAL:
					case PANGO_GRAVITY_HINT_STRONG :
					case PANGO_GRAVITY_HINT_LINE   :
						pango_context_set_gravity_hint(c->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the gravityHint property, needs to be a Context::GRAVITY_HINT_*");
						return;
						break;
				}
			}else
				std_object_handlers.write_property(object, member, value, cache_slot);
			break;
		default:
			std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/********************************/
/* GWidget Class Initialization */
/********************************/

#define DECLARE_CONTEXT_PROP(name) \
DECLARE_CLASS_PROPERTY(context_class_entry_ce, name)

#define CONTEXT_CONSTANT(name, value) \
zend_declare_class_constant_long(context_class_entry_ce, name, sizeof(name)-1, value);


void pp_context_init(int module_number){
	zend_class_entry ce;
	le_context = zend_register_list_destructors_ex(pp_context_free_resource, NULL, "PGGI\\Pango\\Context", module_number);

	memcpy(&context_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	context_object_handlers.offset         = XtOffsetOf(ze_context_object, std);
	context_object_handlers.free_obj       = pp_context_object_free_storage;
	context_object_handlers.clone_obj      = NULL;
	context_object_handlers.read_property  = pp_context_read_property;
	context_object_handlers.get_properties = pp_context_get_properties;
	context_object_handlers.write_property = pp_context_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Pango\\Context", pp_context_class_functions);
	ce.create_object = pp_context_object_new;
	context_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_CONTEXT_PROP(CONTEXT_BASE_DIR    );
	DECLARE_CONTEXT_PROP(CONTEXT_BASE_GRAVITY);
	DECLARE_CONTEXT_PROP(CONTEXT_GRAVITY_HINT);

	CONTEXT_CONSTANT("DIRECTION_LTR"     , PANGO_DIRECTION_LTR     );
	CONTEXT_CONSTANT("DIRECTION_RTL"     , PANGO_DIRECTION_RTL     );
	CONTEXT_CONSTANT("DIRECTION_TTB_LTR" , PANGO_DIRECTION_TTB_LTR );
	CONTEXT_CONSTANT("DIRECTION_TTB_RTL" , PANGO_DIRECTION_TTB_RTL );
	CONTEXT_CONSTANT("DIRECTION_WEAK_LTR", PANGO_DIRECTION_WEAK_LTR);
	CONTEXT_CONSTANT("DIRECTION_WEAK_RTL", PANGO_DIRECTION_WEAK_RTL);
	CONTEXT_CONSTANT("DIRECTION_NEUTRAL" , PANGO_DIRECTION_NEUTRAL );

	CONTEXT_CONSTANT("GRAVITY_SOUTH", PANGO_GRAVITY_SOUTH);
	CONTEXT_CONSTANT("GRAVITY_EAST" , PANGO_GRAVITY_EAST );
	CONTEXT_CONSTANT("GRAVITY_NORTH", PANGO_GRAVITY_NORTH);
	CONTEXT_CONSTANT("GRAVITY_WEST" , PANGO_GRAVITY_WEST );
	CONTEXT_CONSTANT("GRAVITY_AUTO" , PANGO_GRAVITY_AUTO );

	CONTEXT_CONSTANT("GRAVITY_HINT_NATURAL", PANGO_GRAVITY_HINT_NATURAL);
	CONTEXT_CONSTANT("GRAVITY_HINT_STRONG" , PANGO_GRAVITY_HINT_STRONG );
	CONTEXT_CONSTANT("GRAVITY_HINT_LINE"   , PANGO_GRAVITY_HINT_LINE   );
}

