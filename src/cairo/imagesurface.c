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


#include "imagesurface.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_image_surface;
static zend_object_handlers image_surface_object_handlers;
static zend_class_entry * image_surface_class_entry_ce;

zend_class_entry * pc_image_surface_get_class_entry(){
	return image_surface_class_entry_ce;
}

zend_object_handlers * pc_image_surface_get_object_handlers(){
	return &image_surface_object_handlers;
}

/***************/
/* PHP Methods */
/***************/

IMAGE_SURFACE_METHOD(__construct){
	long format, width, height;
	ze_surface_object *surface_object;
	char * data;
	int data_len = 0;
	zval * this = getThis();
	surface_object = Z_SURFACE_P(this);
	surface_object->surface_ptr = pc_surface_new();
	switch(ZEND_NUM_ARGS()){
		case 1 :
			if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &data, &data_len) == FAILURE)
				return;
			surface_object->surface_ptr->intern = cairo_image_surface_create_from_png(data);
			break;
		default :
			if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lll|s", &format, &width, &height, &data, &data_len) == FAILURE)
				return;
			switch(format){
				case CAIRO_FORMAT_ARGB32    :
				case CAIRO_FORMAT_RGB24     :
				case CAIRO_FORMAT_A8        :
				case CAIRO_FORMAT_A1        :
				case CAIRO_FORMAT_RGB16_565 :
				case CAIRO_FORMAT_RGB30     :
					break;
				default :
					zend_throw_exception_ex(pc_exception_get(), 0, "the format should be a FORMAT_*");
					return;
			}
			if (width < 1 || height < 1) {
				zend_throw_exception_ex(pc_exception_get(), 0, "invalid dimension for a ImageSurface");
				return;
			}

			if(data_len > 0){
				long stride = -1;
				stride = cairo_format_stride_for_width (format, width);
				if(stride <= 0){
					zend_throw_exception_ex(pc_exception_get(), 0, "could not calculate stride for ImageSurface");
					return;
				}
				surface_object->surface_ptr->intern = cairo_image_surface_create_for_data((unsigned char *)data, format, width, height, stride);
				zval prop;
				ZVAL_STRINGL(&prop, data, data_len); 
				zend_update_property(image_surface_class_entry_ce, this, "data", sizeof("data") - 1, &prop);
			}else
				surface_object->surface_ptr->intern = cairo_image_surface_create(format, width, height);
			break;
	}
	
	pc_exception(cairo_surface_status(surface_object->surface_ptr->intern));
}

IMAGE_SURFACE_METHOD(getData){
	ze_surface_object *surface_object;
	unsigned char *data;	
	long height, stride;
	zval * this = getThis();
	if(pggi_parse_method_parameters_none_throw(this) == FAILURE)
		return;
	surface_object = Z_SURFACE_P(this);
	cairo_surface_t * s = surface_object->surface_ptr->intern;
	pc_exception(cairo_surface_status(s));
	data = cairo_image_surface_get_data(s);
	height = cairo_image_surface_get_height(s);
	stride = cairo_image_surface_get_stride(s);
	RETURN_STRINGL((char *)data, height * stride);
}

IMAGE_SURFACE_METHOD(getFormat){
	ze_surface_object *surface_object;
	zval * this = getThis();
	if(pggi_parse_method_parameters_none_throw(this) == FAILURE)
		return;
	surface_object = Z_SURFACE_P(this);
	cairo_surface_t * s = surface_object->surface_ptr->intern;
	pc_exception(cairo_surface_status(s));
	RETURN_LONG(cairo_image_surface_get_format(s));
}

IMAGE_SURFACE_METHOD(getWidth){
	ze_surface_object *surface_object;
	zval * this = getThis();
	if(pggi_parse_method_parameters_none_throw(this) == FAILURE)
		return;
	surface_object = Z_SURFACE_P(this);
	cairo_surface_t * s = surface_object->surface_ptr->intern;
	pc_exception(cairo_surface_status(s));
	RETURN_LONG(cairo_image_surface_get_width(s));
}

IMAGE_SURFACE_METHOD(getHeight){
	ze_surface_object *surface_object;
	zval * this = getThis();
	if(pggi_parse_method_parameters_none_throw(this) == FAILURE)
		return;
	surface_object = Z_SURFACE_P(this);
	cairo_surface_t * s = surface_object->surface_ptr->intern;
	pc_exception(cairo_surface_status(s));
	RETURN_LONG(cairo_image_surface_get_height(s));
}

IMAGE_SURFACE_METHOD(getStride){
	ze_surface_object *surface_object;
	zval * this = getThis();
	if(pggi_parse_method_parameters_none_throw(this) == FAILURE)
		return;
	surface_object = Z_SURFACE_P(this);
	cairo_surface_t * s = surface_object->surface_ptr->intern;
	pc_exception(cairo_surface_status(s));
	RETURN_LONG(cairo_image_surface_get_stride(s));
}

IMAGE_SURFACE_METHOD(writeToPNG){
	ze_surface_object *surface_object;
	zval * this = getThis();
	char * data;
	size_t data_len;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE)
		return;
	surface_object = Z_SURFACE_P(this);
	cairo_surface_t * s = surface_object->surface_ptr->intern;
	cairo_surface_write_to_png(s, data);
	pc_exception(cairo_surface_status(s));
}

PHP_FUNCTION(strideForWidth){
	long format, width;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &format, &width) == FAILURE)
		return;
	RETURN_LONG(cairo_format_stride_for_width(format, width));
}

static const zend_function_entry pc_image_surface_class_functions[] = {
	PHP_ME(ImageSurface, __construct   , arginfo_image_surface_construct   , ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(ImageSurface, getData       , arginfo_pggi_get_long             , ZEND_ACC_PUBLIC)
	PHP_ME(ImageSurface, getWidth      , arginfo_pggi_get_long             , ZEND_ACC_PUBLIC)
	PHP_ME(ImageSurface, getHeight     , arginfo_pggi_get_long             , ZEND_ACC_PUBLIC)
	PHP_ME(ImageSurface, getStride     , arginfo_pggi_get_long             , ZEND_ACC_PUBLIC)
	PHP_ME(ImageSurface, writeToPNG    , arginfo_image_surface_write_to_png, ZEND_ACC_PUBLIC)

	PHP_FE(strideForWidth              , arginfo_image_surface_stride_for_width)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *pc_image_surface_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	/*ze_surface_object * intern = Z_SURFACE_P(object);
	pc_surface_ptr c = intern->surface_ptr;
	const char * tmp;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	*/
	return pc_surface_read_property(object, member, type, cache_slot, rv);
}

HashTable *pc_image_surface_get_properties(zval *object){
	/*G_H_UPDATE_INIT(pc_surface_get_properties(object));
	const char * tmp;
	ze_surface_object * intern = Z_SURFACE_P(object);
	pc_surface_ptr c = intern->surface_ptr;

	G_H_UPDATE_LONG("Format", cairo_image_surface_get_format(c->intern));
	G_H_UPDATE_LONG("Width" , cairo_image_surface_get_width (c->intern));
	G_H_UPDATE_LONG("Height", cairo_image_surface_get_height(c->intern));
	G_H_UPDATE_LONG("Stride", cairo_image_surface_get_stride(c->intern));
	return G_H_UPDATE_RETURN;*/
	return pc_surface_get_properties(object);
}

PHP_WRITE_PROP_HANDLER_TYPE pc_image_surface_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	/*ze_surface_object * intern = Z_SURFACE_P(object);
	pc_surface_ptr c = intern->surface_ptr;

	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);*/
	PHP_WRITE_PROP_HANDLER_RETURN(pc_surface_write_property(object, member, value, cache_slot));
}

/********************************/
/* GWidget Class Initialization */
/********************************/

#define DECLARE_IMAGE_SURFACE_PROP(name) \
DECLARE_CLASS_PROPERTY(image_surface_class_entry_ce, name)

#define DECLARE_IMAGE_SURFACE_PRIVATE_PROP(name) \
zend_declare_property_null(image_surface_class_entry_ce, name, sizeof(name)-1, ZEND_ACC_PRIVATE)

#define IMAGE_SURFACE_CONSTANT(name, value) \
DECLARE_CLASS_CONSTANT(image_surface_class_entry_ce, name, value);


void pc_image_surface_init(int module_number){
	zend_class_entry ce;
	le_image_surface = zend_register_list_destructors_ex(pc_surface_free_resource, NULL, "PGGI\\Cairo\\ImageSurface", module_number);

	memcpy(&image_surface_object_handlers, pc_surface_get_object_handlers(), sizeof(zend_object_handlers));
	image_surface_object_handlers.read_property  = pc_image_surface_read_property;
	image_surface_object_handlers.get_properties = pc_image_surface_get_properties;
	image_surface_object_handlers.write_property = pc_image_surface_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Cairo\\ImageSurface", pc_image_surface_class_functions);
	image_surface_class_entry_ce = zend_register_internal_class_ex(&ce, pc_surface_get_class_entry());

	DECLARE_IMAGE_SURFACE_PROP(IMAGE_SURFACE_DATA);

	IMAGE_SURFACE_CONSTANT("FORMAT_ARGB32"   , CAIRO_FORMAT_ARGB32   );
	IMAGE_SURFACE_CONSTANT("FORMAT_RGB24"    , CAIRO_FORMAT_RGB24    );
	IMAGE_SURFACE_CONSTANT("FORMAT_A8"       , CAIRO_FORMAT_A8       );
	IMAGE_SURFACE_CONSTANT("FORMAT_A1"       , CAIRO_FORMAT_A1       );
	IMAGE_SURFACE_CONSTANT("FORMAT_RGB16_565", CAIRO_FORMAT_RGB16_565);
	IMAGE_SURFACE_CONSTANT("FORMAT_RGB30"    , CAIRO_FORMAT_RGB30    );

}

