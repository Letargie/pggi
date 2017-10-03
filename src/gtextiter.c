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


#include "gtextiter.h"

/*****************************/
/* Class information getters */
/*****************************/

static int le_gtext_iter;
static zend_object_handlers gtext_iter_object_handlers;
static zend_class_entry * gtext_iter_class_entry_ce;

/****************************/
/* Memory handling function */
/****************************/

gtext_iter_ptr gtext_iter_new(){
	gtext_iter_ptr tor = ecalloc(1, sizeof(gtext_iter_t));
	return tor;
}

zend_object *gtext_iter_object_new(zend_class_entry *class_type){
	ze_gtext_iter_object *intern;
	intern = ecalloc(1, sizeof(ze_gtext_iter_object) + zend_object_properties_size(class_type));
	zend_object_std_init    (&intern->std, class_type);
	object_properties_init    (&intern->std, class_type);
	intern->std.handlers = &gtext_iter_object_handlers;
	return &intern->std;
}

void gtext_iter_dtor(gtext_iter_ptr intern){
	if (intern->intern){    
	/*unref text iter?*/
	}
	efree(intern);
}

void gtext_iter_object_free_storage(zend_object *object){
	ze_gtext_iter_object *intern = php_gtext_iter_fetch_object(object);
	if(!intern){
		return;
	}
	if(intern->iter_ptr){
		gtext_iter_dtor(intern->iter_ptr);
	}
	intern->iter_ptr = NULL;
	zend_object_std_dtor(&intern->std);
}

void gtext_iter_free_resource(zend_resource *rsrc) {
	gtext_iter_ptr iter = (gtext_iter_ptr) rsrc->ptr;
	gtext_iter_dtor(iter);
}

/***************/
/* PHP Methods */
/***************/

GTEXT_ITER_METHOD(__construct){
	return;
}

GTEXT_ITER_METHOD(getBuffer){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_OBJ(ze_obj->iter_ptr->buffer);
}


size_t pggi_utf32_utf8(unsigned char *buf, unsigned k){
	size_t retval = 0;

	/* assert(0x0 <= k <= 0x10FFFF); */

	if (k < 0x80) {
		buf[0] = k;
		retval = 1;
	} else if (k < 0x800) {
		buf[0] = 0xc0 | (k >> 6);
		buf[1] = 0x80 | (k & 0x3f);
		retval = 2;
	} else if (k < 0x10000) {
		buf[0] = 0xe0 | (k >> 12);
		buf[1] = 0x80 | ((k >> 6) & 0x3f);
		buf[2] = 0x80 | (k & 0x3f);
		retval = 3;
	} else {
		buf[0] = 0xf0 | (k >> 18);
		buf[1] = 0x80 | ((k >> 12) & 0x3f);
		buf[2] = 0x80 | ((k >> 6) & 0x3f);
		buf[3] = 0x80 | (k & 0x3f);
		retval = 4;
	}
	/* UTF-8 has been restricted to max 4 bytes since RFC 3629 */

	return retval;
}


GTEXT_ITER_METHOD(getChar){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	unsigned char tmp[4]; 
	int size = pggi_utf32_utf8(tmp, gtk_text_iter_get_char(ze_obj->iter_ptr->intern));
	RETURN_STRINGL((char*)tmp, size);
}

GTEXT_ITER_METHOD(getSlice){
	zval * beg, * end;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "zz", &beg, &end) == FAILURE)
		return;
	ze_gtext_iter_object * beg_obj = Z_GTEXT_ITER_P(beg),
	                     * end_obj = Z_GTEXT_ITER_P(end);
	RETURN_STRING(gtk_text_iter_get_slice(beg_obj->iter_ptr->intern, end_obj->iter_ptr->intern));
}

GTEXT_ITER_METHOD(getText){
	zval * beg, * end;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "zz", &beg, &end) == FAILURE)
		return;
	ze_gtext_iter_object * beg_obj = Z_GTEXT_ITER_P(beg),
	                     * end_obj = Z_GTEXT_ITER_P(end);
	RETURN_STRING(gtk_text_iter_get_text(beg_obj->iter_ptr->intern, end_obj->iter_ptr->intern));
}

GTEXT_ITER_METHOD(getVisibleSlice){
	zval * beg, * end;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "zz", &beg, &end) == FAILURE)
		return;
	ze_gtext_iter_object * beg_obj = Z_GTEXT_ITER_P(beg),
	                     * end_obj = Z_GTEXT_ITER_P(end);
	RETURN_STRING(gtk_text_iter_get_visible_slice(beg_obj->iter_ptr->intern, end_obj->iter_ptr->intern));
}

GTEXT_ITER_METHOD(getVisibleText){
	zval * beg, * end;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "zz", &beg, &end) == FAILURE)
		return;
	ze_gtext_iter_object * beg_obj = Z_GTEXT_ITER_P(beg),
	                     * end_obj = Z_GTEXT_ITER_P(end);
	RETURN_STRING(gtk_text_iter_get_visible_text(beg_obj->iter_ptr->intern, end_obj->iter_ptr->intern));
}

/*
GTEXT_ITER_METHOD(editable){
	zval * self = getThis();
	pggi_parse_method_parameters_none_throw(self);
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_editable(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(canInsert){
	zval * self = getThis();
	pggi_parse_method_parameters_none_throw(self);
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_can_insert(ze_obj->iter_ptr->intern) != 0);
}
*/
GTEXT_ITER_METHOD(startsWord){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_starts_word(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(endsWord){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_ends_word(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(insideWord){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_inside_word(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(startsLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_starts_line(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(endsLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_ends_line(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(startsSentence){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_starts_sentence(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(endsSentence){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_ends_sentence(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(insideSentence){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_inside_sentence(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(isCursorPosition){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_is_cursor_position(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(isEnd){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_is_end(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(isStart){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_BOOL(gtk_text_iter_is_start(ze_obj->iter_ptr->intern) != 0);
}

GTEXT_ITER_METHOD(getCharsInLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_LONG(gtk_text_iter_get_chars_in_line(ze_obj->iter_ptr->intern));
}

GTEXT_ITER_METHOD(getBytesInLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	RETURN_LONG(gtk_text_iter_get_bytes_in_line(ze_obj->iter_ptr->intern));
}

GTEXT_ITER_METHOD(forwardChar){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_char(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardChar){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_char(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardChars){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_chars(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardChars){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_chars(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(forwardLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_line(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_line(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardLines){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_lines(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardLines){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_lines(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardWordStart){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_word_start(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardWordEnd){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_word_end(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardWordStarts){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_word_starts(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(forwardWordEnds){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_word_ends(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardCursorPosition){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_cursor_position(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardCursorPosition){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_cursor_position(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardCursorPositions){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_cursor_positions(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(forwardCursorPositions){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_cursor_positions(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardSentenceStart){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_sentence_start(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardSentenceEnd){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_sentence_end(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardSentenceStarts){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_sentence_starts(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(forwardSentenceEnds){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_sentence_ends(ze_obj->iter_ptr->intern, number);
}


GTEXT_ITER_METHOD(forwardVisibleLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_visible_line(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardVisibleLine){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_visible_line(ze_obj->iter_ptr->intern);
}


GTEXT_ITER_METHOD(forwardVisibleLines){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_visible_lines(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardVisibleLines){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_visible_lines(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardVisibleWordStart){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_visible_word_start(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardVisibleWordEnd){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_visible_word_end(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardVisibleWordStarts){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_visible_word_starts(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(forwardVisibleWordEnds){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_visible_word_ends(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(backwardVisibleCursorPosition){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_visible_cursor_position(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardVisibleCursorPosition){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_visible_cursor_position(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(backwardVisibleCursorPositions){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_backward_visible_cursor_positions(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(forwardVisibleCursorPositions){
	zval * self = getThis();
	long number;
	if(pggi_parse_method_parameters_throw(ZEND_NUM_ARGS(), self, "l", &number) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_visible_cursor_positions(ze_obj->iter_ptr->intern, number);
}

GTEXT_ITER_METHOD(forwardEnd){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_to_end(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardLineEnd){
	zval * self = getThis();
	if(pggi_parse_method_parameters_none_throw(self) == FAILURE)
		return;
	ze_gtext_iter_object * ze_obj = Z_GTEXT_ITER_P(self);
	gtk_text_iter_forward_to_line_end(ze_obj->iter_ptr->intern);
}

GTEXT_ITER_METHOD(forwardFindChar);
GTEXT_ITER_METHOD(backwardFindChar);
GTEXT_ITER_METHOD(forwardSearch);
GTEXT_ITER_METHOD(backwardSearch);

GTEXT_ITER_METHOD(equals);
GTEXT_ITER_METHOD(compare);
GTEXT_ITER_METHOD(inRange);
GTEXT_ITER_METHOD(order);

/*
GTEXT_ITER_METHOD(forwardFindChar);
GTEXT_ITER_METHOD(backwardFindChar);
GTEXT_ITER_METHOD(forwardSearch);
GTEXT_ITER_METHOD(backwardSearch);*/

static const zend_function_entry gtext_iter_class_functions[] = {
    PHP_ME(GTextIter, __construct                   , arginfo_pggi_void         , ZEND_ACC_PRIVATE | ZEND_ACC_CTOR  )

    PHP_ME(GTextIter, getBuffer                     , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, getChar                       , arginfo_pggi_void         , ZEND_ACC_PUBLIC)

    PHP_ME(GTextIter, getSlice                      , arginfo_text_iter_interval, ZEND_ACC_PUBLIC  | ZEND_ACC_STATIC)
    PHP_ME(GTextIter, getText                       , arginfo_text_iter_interval, ZEND_ACC_PUBLIC  | ZEND_ACC_STATIC)
    PHP_ME(GTextIter, getVisibleSlice               , arginfo_text_iter_interval, ZEND_ACC_PUBLIC  | ZEND_ACC_STATIC)
    PHP_ME(GTextIter, getVisibleText                , arginfo_text_iter_interval, ZEND_ACC_PUBLIC  | ZEND_ACC_STATIC)

	// need a parameter boolean TODO
    //PHP_ME(GTextIter, editable                      , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    //PHP_ME(GTextIter, canInsert                     , arginfo_pggi_void         , ZEND_ACC_PUBLIC)

    PHP_ME(GTextIter, startsWord                    , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, endsWord                      , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, insideWord                    , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, startsLine                    , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, endsLine                      , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, startsSentence                , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, endsSentence                  , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, insideSentence                , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, isCursorPosition              , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, isEnd                         , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, isStart                       , arginfo_pggi_void         , ZEND_ACC_PUBLIC)

    PHP_ME(GTextIter, getCharsInLine                , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, getBytesInLine                , arginfo_pggi_void         , ZEND_ACC_PUBLIC)    

    PHP_ME(GTextIter, forwardChar                   , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardChar                  , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardChars                  , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardChars                 , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardLine                   , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardLine                  , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardLines                  , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardLines                 , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardWordStart             , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardWordEnd                , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardWordStarts            , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardWordEnds               , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardCursorPosition        , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardCursorPosition         , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardCursorPositions       , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardCursorPositions        , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardSentenceStart         , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardSentenceEnd            , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardSentenceStarts        , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardSentenceEnds           , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardVisibleWordStart      , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardVisibleWordEnd         , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardVisibleWordStarts     , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardVisibleWordEnds        , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardVisibleCursorPosition , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardVisibleCursorPosition  , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardVisibleCursorPositions, arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardVisibleCursorPositions , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardVisibleLine            , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardVisibleLine           , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardVisibleLines           , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, backwardVisibleLines          , arginfo_text_iter_mov_mult, ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardEnd                    , arginfo_pggi_void         , ZEND_ACC_PUBLIC)
    PHP_ME(GTextIter, forwardLineEnd                , arginfo_pggi_void         , ZEND_ACC_PUBLIC)

    //PHP_ME(GTextIter, equals                        , arginfo_pggi_compare      , ZEND_ACC_PUBLIC)
    //PHP_ME(GTextIter, compare                       , arginfo_pggi_compare      , ZEND_ACC_PUBLIC)
    //PHP_ME(GTextIter, order                         , arginfo_pggi_compare      , ZEND_ACC_PUBLIC)
    //PHP_ME(GTextIter, inRange                       , arginfo_text_iter_interval, ZEND_ACC_PUBLIC)

    PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *gtext_iter_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_gtext_iter_object * intern = Z_GTEXT_ITER_P(object);
	gtext_iter_ptr b = intern->iter_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTextIter * iter = b->intern;
	if(!strcmp(member_val, GTEXT_ITER_OFFSET)){
		ZVAL_LONG(rv, gtk_text_iter_get_offset(iter));
	}else if(!strcmp(member_val, GTEXT_ITER_LINE)){
		ZVAL_LONG(rv, gtk_text_iter_get_line(iter));
	}else if(!strcmp(member_val, GTEXT_ITER_LINE_OFFSET)){
		ZVAL_LONG(rv, gtk_text_iter_get_line_offset(iter));
	}else if(!strcmp(member_val, GTEXT_ITER_LINE_INDEX)){
		ZVAL_LONG(rv, gtk_text_iter_get_line_index(iter));
	}else if(!strcmp(member_val, GTEXT_ITER_VISIBLE_LINE_INDEX)){
		ZVAL_LONG(rv, gtk_text_iter_get_visible_line_index(iter));
	}else if(!strcmp(member_val, GTEXT_ITER_VISIBLE_LINE_OFFSET)){
		ZVAL_LONG(rv, gtk_text_iter_get_visible_line_offset(iter));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	return rv;
}

HashTable *gtext_iter_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_gtext_iter_object * intern = Z_GTEXT_ITER_P(object);
	gtext_iter_ptr b = intern->iter_ptr;
	GtkTextIter * iter = b->intern;
	G_H_UPDATE_LONG(GTEXT_ITER_OFFSET             , gtk_text_iter_get_offset             (iter));
	G_H_UPDATE_LONG(GTEXT_ITER_LINE               , gtk_text_iter_get_line               (iter));
	G_H_UPDATE_LONG(GTEXT_ITER_LINE_OFFSET        , gtk_text_iter_get_line_offset        (iter));
	G_H_UPDATE_LONG(GTEXT_ITER_LINE_INDEX         , gtk_text_iter_get_line_index         (iter));
	G_H_UPDATE_LONG(GTEXT_ITER_VISIBLE_LINE_INDEX , gtk_text_iter_get_visible_line_index (iter));
	G_H_UPDATE_LONG(GTEXT_ITER_VISIBLE_LINE_OFFSET, gtk_text_iter_get_visible_line_offset(iter));
	return G_H_UPDATE_RETURN;
}

void gtext_iter_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_gtext_iter_object * intern = Z_GTEXT_ITER_P(object);
	gtext_iter_ptr b = intern->iter_ptr;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	GtkTextIter * iter = b->intern;
	switch(Z_TYPE_P(value)){
		case IS_LONG :
		    tmp_l = Z_LVAL_P(value);
		    if(!strcmp(member_val, GTEXT_ITER_OFFSET)){
		        gtk_text_iter_set_offset(iter, tmp_l);
		    }else if(!strcmp(member_val, GTEXT_ITER_LINE)){
		        gtk_text_iter_set_line(iter, tmp_l);
		    }else if(!strcmp(member_val, GTEXT_ITER_LINE_OFFSET)){
		        gtk_text_iter_set_line_offset(iter, tmp_l);
		    }else if(!strcmp(member_val, GTEXT_ITER_LINE_INDEX)){
		        gtk_text_iter_set_line_index(iter, tmp_l);
		    }else if(!strcmp(member_val, GTEXT_ITER_VISIBLE_LINE_INDEX)){
		        gtk_text_iter_set_visible_line_index(iter, tmp_l);
		    }else if(!strcmp(member_val, GTEXT_ITER_VISIBLE_LINE_OFFSET)){
		        gtk_text_iter_set_visible_line_offset(iter, tmp_l);
		    }else
		        std_object_handlers.write_property(object, member, value, cache_slot);
		    break;
		default:
		    std_object_handlers.write_property(object, member, value, cache_slot);
	}
}

/************************************/
/* GTextIter Class Initialization */
/************************************/

/*
GTK_TEXT_SEARCH_VISIBLE_ONLY
GTK_TEXT_SEARCH_TEXT_ONLY
GTK_TEXT_SEARCH_CASE_INSENSITIVE
*/


#define DECLARE_GTEXT_ITER_PROP(name) \
DECLARE_CLASS_PROPERTY(gtext_iter_class_entry_ce, name)

#define DECLARE_GTEXT_ITER_PROP_PRIVATE(name) \
zend_declare_property_null(gtext_iter_class_entry_ce, name, sizeof(name) - 1, ZEND_ACC_PRIVATE);

#define GTEXT_ITER_CONSTANT(name, value) \
zend_declare_class_constant_double(gtext_iter_class_entry_ce, name, sizeof(name)-1, value);


void gtext_iter_init(int module_number){
	zend_class_entry ce;
	le_gtext_iter = zend_register_list_destructors_ex(gtext_iter_free_resource, NULL, "PGGI\\GTextIter", module_number);

	memcpy(&gtext_iter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	gtext_iter_object_handlers.offset         = XtOffsetOf(ze_gtext_iter_object, std);
	gtext_iter_object_handlers.free_obj       = gtext_iter_object_free_storage;
	gtext_iter_object_handlers.clone_obj      = NULL;
	gtext_iter_object_handlers.read_property  = gtext_iter_read_property;
	gtext_iter_object_handlers.get_properties = gtext_iter_get_properties;
	gtext_iter_object_handlers.write_property = gtext_iter_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\GTextIter", gtext_iter_class_functions);
	ce.create_object = gtext_iter_object_new;
	gtext_iter_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_GTEXT_ITER_PROP(GTEXT_ITER_OFFSET             );
	DECLARE_GTEXT_ITER_PROP(GTEXT_ITER_LINE               );
	DECLARE_GTEXT_ITER_PROP(GTEXT_ITER_LINE_OFFSET        );
	DECLARE_GTEXT_ITER_PROP(GTEXT_ITER_LINE_INDEX         );
	DECLARE_GTEXT_ITER_PROP(GTEXT_ITER_VISIBLE_LINE_INDEX );
	DECLARE_GTEXT_ITER_PROP(GTEXT_ITER_VISIBLE_LINE_OFFSET);

}


