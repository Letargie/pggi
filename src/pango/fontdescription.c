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


#include "fontdescription.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_font_description;
static zend_object_handlers font_description_object_handlers;
static zend_class_entry * font_description_class_entry_ce;

zend_class_entry * font_description_get_class_entry(){
	return font_description_class_entry_ce;
}

zend_object_handlers * font_description_get_object_handlers(){
	return &font_description_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

font_description_ptr font_description_new(){
	font_description_ptr tor = ecalloc(1, sizeof(font_description_t));
	return tor;
}

zend_object *font_description_object_new(zend_class_entry *class_type){
	ze_font_description_object *intern;
	intern = ecalloc(1, sizeof(ze_font_description_object) + zend_object_properties_size(class_type));
	zend_object_std_init  (&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &font_description_object_handlers;
	return &intern->std;
}

void font_description_dtor(font_description_ptr intern){
	if (intern->intern){
		pango_font_description_free(intern->intern);
	}
	efree(intern);
}

void font_description_object_free_storage(zend_object *object){
	ze_font_description_object *intern = php_font_description_fetch_object(object);
	if (!intern) {
		return;
	}
	if (intern->description_ptr){
		font_description_dtor(intern->description_ptr);
	}
	intern->description_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void font_description_free_resource(zend_resource *rsrc) {
	font_description_ptr font_description = (font_description_ptr) rsrc->ptr;
	font_description_dtor(font_description);
}

/***************/
/* PHP Methods */
/***************/


FONT_DESCRIPTION_METHOD(__construct){
	zval * self = getThis();
	ze_font_description_object * ze_obj;
	char * str;
	size_t str_len = 0;
	ze_obj = Z_FONT_DESCRIPTION_P(self);
	ze_obj->description_ptr = font_description_new();
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|s", &str, &str_len) == FAILURE)
		return;
	if(str_len == 0)
		ze_obj->description_ptr->intern = pango_font_description_new();
	else
		ze_obj->description_ptr->intern = pango_font_description_from_string(str);
}


FONT_DESCRIPTION_METHOD(hash){
	zval * self = getThis();
	ze_font_description_object * ze_obj;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_FONT_DESCRIPTION_P(self);
	RETURN_LONG(pango_font_description_hash(ze_obj->description_ptr->intern));
}

static const zend_function_entry font_description_class_functions[] = {
	PHP_ME(FontDescription, __construct, arginfo_font_description_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(FontDescription, hash       , arginfo_pggi_get_long             , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *font_description_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_font_description_object * intern = Z_FONT_DESCRIPTION_P(object);
	font_description_ptr d = intern->description_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	 if(!strcmp(member_val, FONT_DESCRIPTION_STRETCH)){
		ZVAL_LONG(rv, pango_font_description_get_stretch(d->intern));
	}else if(!strcmp(member_val, FONT_DESCRIPTION_STYLE)){
		ZVAL_LONG(rv, pango_font_description_get_style(d->intern));
	}else if(!strcmp(member_val, FONT_DESCRIPTION_VARIANT)){
		ZVAL_LONG(rv, pango_font_description_get_variant(d->intern));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *font_description_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_font_description_object * intern = Z_FONT_DESCRIPTION_P(object);
	font_description_ptr d = intern->description_ptr;
	G_H_UPDATE_LONG(FONT_DESCRIPTION_STRETCH, pango_font_description_get_stretch(d->intern));
	G_H_UPDATE_LONG(FONT_DESCRIPTION_STYLE  , pango_font_description_get_style  (d->intern));
	G_H_UPDATE_LONG(FONT_DESCRIPTION_VARIANT, pango_font_description_get_variant(d->intern));
	return G_H_UPDATE_RETURN;
}

void font_description_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_font_description_object * intern = Z_FONT_DESCRIPTION_P(object);
	font_description_ptr d = intern->description_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	switch(Z_TYPE_P(value)){
		case IS_LONG :
			tmp_l = Z_LVAL_P(value);
			if(!strcmp(member_val, FONT_DESCRIPTION_STRETCH)){
				switch(tmp_l){
					case PANGO_STRETCH_ULTRA_CONDENSED :
					case PANGO_STRETCH_EXTRA_CONDENSED :
					case PANGO_STRETCH_CONDENSED       :
					case PANGO_STRETCH_SEMI_CONDENSED  :
					case PANGO_STRETCH_NORMAL          :
					case PANGO_STRETCH_SEMI_EXPANDED   :
					case PANGO_STRETCH_EXPANDED        :
					case PANGO_STRETCH_EXTRA_EXPANDED  :
					case PANGO_STRETCH_ULTRA_EXPANDED  :
						pango_font_description_set_stretch(d->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the stretch property, needs to be a FontDescription::STRETCH_*");
						return;
						break;
				}
				
			}else if(!strcmp(member_val, FONT_DESCRIPTION_STYLE)){
				pango_font_description_set_style(d->intern, tmp_l);
			}else if(!strcmp(member_val, FONT_DESCRIPTION_VARIANT)){
				switch(tmp_l){
					case PANGO_VARIANT_NORMAL     :
					case PANGO_VARIANT_SMALL_CAPS :
						pango_font_description_set_variant(d->intern, tmp_l);
						break;
					default:
						zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the variant property, needs to be a FontDescription::VARIANT_*");
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

/************************/
/* Class Initialization */
/************************/

#define DECLARE_FONT_DESCRIPTION_PROP(name) \
DECLARE_CLASS_PROPERTY(font_description_class_entry_ce, name)

#define FONT_DESCRIPTION_CONSTANT(name, value) \
zend_declare_class_constant_long(font_description_class_entry_ce, name, sizeof(name)-1, value);


void font_description_init(int module_number){
	zend_class_entry ce;
	le_font_description = zend_register_list_destructors_ex(font_description_free_resource, NULL, "PGGI\\Pango\\FontDescription", module_number);

	memcpy(&font_description_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	font_description_object_handlers.offset         = XtOffsetOf(ze_font_description_object, std);
	font_description_object_handlers.free_obj       = font_description_object_free_storage;
	font_description_object_handlers.clone_obj      = NULL;
	font_description_object_handlers.read_property  = font_description_read_property;
	font_description_object_handlers.get_properties = font_description_get_properties;
	font_description_object_handlers.write_property = font_description_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Pango\\FontDescription", font_description_class_functions);
	ce.create_object = font_description_object_new;
	font_description_class_entry_ce = zend_register_internal_class(&ce);
	
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_FAMILY"       , PANGO_FONT_MASK_FAMILY       );
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_STYLE"        , PANGO_FONT_MASK_STYLE        );
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_VARIANT"      , PANGO_FONT_MASK_VARIANT      );
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_WEIGHT"       , PANGO_FONT_MASK_WEIGHT       );
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_SIZE"         , PANGO_FONT_MASK_SIZE         );
	FONT_DESCRIPTION_CONSTANT("FONT_MASK_GRAVITY"      , PANGO_FONT_MASK_GRAVITY      );
	
	FONT_DESCRIPTION_CONSTANT("STRETCH_ULTRA_CONDENSED", PANGO_STRETCH_ULTRA_CONDENSED);
	FONT_DESCRIPTION_CONSTANT("STRETCH_EXTRA_CONDENSED", PANGO_STRETCH_EXTRA_CONDENSED);
	FONT_DESCRIPTION_CONSTANT("STRETCH_CONDENSED"      , PANGO_STRETCH_CONDENSED      );
	FONT_DESCRIPTION_CONSTANT("STRETCH_SEMI_CONDENSED" , PANGO_STRETCH_SEMI_CONDENSED );
	FONT_DESCRIPTION_CONSTANT("STRETCH_NORMAL"         , PANGO_STRETCH_NORMAL         );
	FONT_DESCRIPTION_CONSTANT("STRETCH_SEMI_EXPANDED"  , PANGO_STRETCH_SEMI_EXPANDED  );
	FONT_DESCRIPTION_CONSTANT("STRETCH_EXPANDED"       , PANGO_STRETCH_EXPANDED       );
	FONT_DESCRIPTION_CONSTANT("STRETCH_EXTRA_EXPANDED" , PANGO_STRETCH_EXTRA_EXPANDED );
	FONT_DESCRIPTION_CONSTANT("STRETCH_ULTRA_EXPANDED" , PANGO_STRETCH_ULTRA_EXPANDED );
	
	FONT_DESCRIPTION_CONSTANT("WEIGHT_LIGHT"           , PANGO_WEIGHT_LIGHT           );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_ULTRALIGHT"      , PANGO_WEIGHT_ULTRALIGHT      );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_THIN"            , PANGO_WEIGHT_THIN            );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_ULTRAHEAVY"      , PANGO_WEIGHT_ULTRAHEAVY      );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_HEAVY"           , PANGO_WEIGHT_HEAVY           );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_ULTRABOLD"       , PANGO_WEIGHT_ULTRABOLD       );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_BOLD"            , PANGO_WEIGHT_BOLD            );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_SEMIBOLD"        , PANGO_WEIGHT_SEMIBOLD        );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_MEDIUM"          , PANGO_WEIGHT_MEDIUM          );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_NORMAL"          , PANGO_WEIGHT_NORMAL          );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_BOOK"            , PANGO_WEIGHT_BOOK            );
	FONT_DESCRIPTION_CONSTANT("WEIGHT_SEMILIGHT"       , PANGO_WEIGHT_SEMILIGHT       );
	
	FONT_DESCRIPTION_CONSTANT("VARIANT_NORMAL"         , PANGO_VARIANT_NORMAL          );
	FONT_DESCRIPTION_CONSTANT("VARIANT_SMALL_CAPS"     , PANGO_VARIANT_SMALL_CAPS      );

	DECLARE_FONT_DESCRIPTION_PROP(FONT_DESCRIPTION_STRETCH);
	DECLARE_FONT_DESCRIPTION_PROP(FONT_DESCRIPTION_STYLE  );
	DECLARE_FONT_DESCRIPTION_PROP(FONT_DESCRIPTION_VARIANT);
}

