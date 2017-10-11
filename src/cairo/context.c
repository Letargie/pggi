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

zend_class_entry * pc_context_get_class_entry(){
	return context_class_entry_ce;
}

zend_object_handlers * pc_context_get_object_handlers(){
	return &context_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

pc_context_ptr pc_context_new(){
	pc_context_ptr tor = ecalloc(1, sizeof(pc_context_t));
	return tor;
}

zend_object *pc_context_object_new(zend_class_entry *class_type){
	ze_context_object *intern;
	intern = ecalloc(1, sizeof(ze_context_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &context_object_handlers;
	return &intern->std;
}

void pc_context_dtor(pc_context_ptr intern){
	if (intern->intern){
		
	}
	efree(intern);
}

void pc_context_object_free_storage(zend_object *object){
	ze_context_object *intern = php_context_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->context_ptr){
		pc_context_dtor(intern->context_ptr);
	}
	intern->context_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void pc_context_free_resource(zend_resource *rsrc) {
	pc_context_ptr context = (pc_context_ptr) rsrc->ptr;
	pc_context_dtor(context);
}

/***************/
/* PHP Methods */
/***************/

CONTEXT_METHOD(fill){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_fill(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}


CONTEXT_METHOD(paint){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_paint(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}


CONTEXT_METHOD(stroke){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_stroke(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(strokePreserve){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_stroke(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(relCurveTo){
	zval * self = getThis();
	double x1, y1, x2, y2, x3, y3;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddddd", &x1, &y1, &x2, &y2, &x3, &y3) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rel_curve_to(ze_obj->context_ptr->intern, x1, y1, x2, y2, x3, y3); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(relLineTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rel_line_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}


CONTEXT_METHOD(relMoveTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rel_move_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(curveTo){
	zval * self = getThis();
	double x1, y1, x2, y2, x3, y3;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddddd", &x1, &y1, &x2, &y2, &x3, &y3) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_curve_to(ze_obj->context_ptr->intern, x1, y1, x2, y2, x3, y3); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(lineTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_line_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}


CONTEXT_METHOD(moveTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_move_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(rectangle){
	zval * self = getThis();
	double x, y, width, height;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddd", &x, &y, &width, &height) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rectangle(ze_obj->context_ptr->intern, x, y, width, height); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(arc){
	zval * self = getThis();
	double x, y, radius, angle1, angle2;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ddddd", &x, &y, &radius, &angle1, &angle2) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_arc(ze_obj->context_ptr->intern, x, y, radius, angle1, angle2); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(arcNegative){
	zval * self = getThis();
	double x, y, radius, angle1, angle2;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ddddd", &x, &y, &radius, &angle1, &angle2) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_arc_negative(ze_obj->context_ptr->intern, x, y, radius, angle1, angle2); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 0, 0);
}

CONTEXT_METHOD(setColor){
	zval * self = getThis();
	zval * color;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &color, rgba_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	ze_rgba_object * c = Z_RGBA_P(color);
	gdk_cairo_set_source_rgba(ze_obj->context_ptr->intern, c->ptr->color);
	RETURN_ZVAL(self, 0, 0);
}


static const zend_function_entry pc_context_class_functions[] = {
	PHP_ME(Context, fill          , arginfo_pggi_void           , ZEND_ACC_PUBLIC) // to change to return self if it works
	PHP_ME(Context, paint         , arginfo_pggi_void           , ZEND_ACC_PUBLIC)
	PHP_ME(Context, stroke        , arginfo_pggi_void           , ZEND_ACC_PUBLIC)
	PHP_ME(Context, strokePreserve, arginfo_pggi_void           , ZEND_ACC_PUBLIC)
	PHP_ME(Context, arc           , arginfo_pc_context_arc      , ZEND_ACC_PUBLIC)
	PHP_ME(Context, arcNegative   , arginfo_pc_context_arc      , ZEND_ACC_PUBLIC)
	PHP_ME(Context, curveTo       , arginfo_pc_context_curved_to, ZEND_ACC_PUBLIC)
	PHP_ME(Context, lineTo        , arginfo_pc_context_line_to  , ZEND_ACC_PUBLIC)
	PHP_ME(Context, moveTo        , arginfo_pc_context_line_to  , ZEND_ACC_PUBLIC)
	PHP_ME(Context, relCurveTo    , arginfo_pc_context_curved_to, ZEND_ACC_PUBLIC)
	PHP_ME(Context, relLineTo     , arginfo_pc_context_line_to  , ZEND_ACC_PUBLIC)
	PHP_ME(Context, relMoveTo     , arginfo_pc_context_line_to  , ZEND_ACC_PUBLIC)
	PHP_ME(Context, rectangle     , arginfo_pc_context_rectangle, ZEND_ACC_PUBLIC)
	PHP_ME(Context, setColor      , arginfo_pc_context_set_color, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *pc_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	/*ze_context_object * intern = Z_CONTEXT_P(object);
	pc_context_ptr c = intern->context_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);*/

	return std_object_handlers.read_property(object, member, type, cache_slot, rv);
}

HashTable *pc_context_get_properties(zval *object){
	/*G_H_UPDATE_INIT(zend_std_get_properties(object));
	const char * tmp;
	ze_context_object * intern = Z_CONTEXT_P(object);
	pc_context_ptr c = intern->context_ptr;

	return G_H_UPDATE_RETURN;*/
	return zend_std_get_properties(object);
}

void pc_context_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	/*ze_context_object * intern = Z_CONTEXT_P(object);
	pc_context_ptr c = intern->context_ptr;

	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);*/
	switch(Z_TYPE_P(value)){
		
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


void pc_context_init(int module_number){
	zend_class_entry ce;
	le_context = zend_register_list_destructors_ex(pc_context_free_resource, NULL, "Cairo\\Context", module_number);

	memcpy(&context_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	context_object_handlers.offset         = XtOffsetOf(ze_context_object, std);
	context_object_handlers.free_obj       = pc_context_object_free_storage;
	context_object_handlers.clone_obj      = NULL;
	context_object_handlers.read_property  = pc_context_read_property;
	context_object_handlers.get_properties = pc_context_get_properties;
	context_object_handlers.write_property = pc_context_write_property;
	INIT_CLASS_ENTRY(ce, "Cairo\\Context", pc_context_class_functions);
	ce.create_object = pc_context_object_new;
	context_class_entry_ce = zend_register_internal_class(&ce);


}
