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

zend_class_entry * pc_context_get_class_entry(void){
	return context_class_entry_ce;
}

zend_object_handlers * pc_context_get_object_handlers(void){
	return &context_object_handlers;
}

/****************************/
/* Memory handling function */
/****************************/

pc_context_ptr pc_context_new(){
	pc_context_ptr tor = ecalloc(1, sizeof(pc_context_t));
	tor->to_destroy = 0;
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
	if (intern->intern && intern->to_destroy){
		cairo_destroy(intern->intern);
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

/*==========================================================================*/
/**
 * Creates a new Context with all graphics state parameters set to default values and with a target surface passed in parameters.
 * @constructor
 * @public
 *
 * @param PGGI\Cairo\Surface The target of the drawing
 *
 * @throw PGGI\Cairo\Exception  If memory cannot be allocate or if the surface does not support writing
 */
CONTEXT_METHOD(__construct){
	zval * self = getThis();
	ze_context_object * ze_obj;
	zval * surface;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &surface, pc_surface_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	ze_surface_object * obj = Z_SURFACE_P(surface);
	ze_obj->context_ptr = pc_context_new();
	ze_obj->context_ptr->intern = cairo_create(obj->surface_ptr->intern);
	ze_obj->context_ptr->target = surface;
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	ze_obj->context_ptr->to_destroy = 1;
}

/*==========================================================================*/
/**
 * A drawing operator that fills the current path according to the current fill rule,
 * (each sub-path is implicitly closed before being filled).
 * After fill(), the current path will be cleared from the Context. <cairoSetFillRule() Not available yet> and fillPreserve().
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(fill){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_fill(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * A drawing operator that fills the current path according to the current fill rule,
 * (each sub-path is implicitly closed before being filled).
 * Unlike fill(), fillPreserve() preserves the path within the cairo context.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(fillPreserve){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_fill_preserve(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Establishes a new clip region by intersecting the current clip region with the current path 
 * as it would be filled by fill() and according to the current fill rule <(see setFillRule()) not yet available>.
 *
 * After clip(), the current path will be cleared from the cairo context.
 *
 * The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.
 *
 * Calling clip() can only make the clip region smaller, never larger.
 * But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling clip() within a save()/restore() pair.
 * The only other means of increasing the size of the clip region is <resetClip() not yet available>.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(clip){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_clip(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Establishes a new clip region by intersecting the current clip region with the current path 
 * as it would be filled by fill() and according to the current fill rule <(see setFillRule()) not yet available>.
 *
 * Unlike clip(), clipPreserve() preserves the path within the cairo context.
 *
 * The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.
 *
 * Calling clipPreserve() can only make the clip region smaller, never larger.
 * But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling clipPreserve() within a save()/restore() pair.
 * The only other means of increasing the size of the clip region is <resetClip() not yet available>.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(clipPreserve){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_clip_preserve(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Clears the current path. After this call there will be no path and no current point.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(newPath){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_new_path(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Begin a new sub-path. Note that the existing path is not affected. After this call there will be no current point.
 *
 * In many cases, this call is not needed since new sub-paths are frequently started with moveTo().
 *
 * A call to newSubPath() is particularly useful when beginning a new sub-path with one of the arc() calls.
 * This makes things easier as it is no longer necessary to manually compute the arc's initial coordinates for a call to moveTo().
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(newSubPath){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_new_sub_path(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Adds a line segment to the path from the current point to the beginning of the current sub-path,
 * (the most recent point passed to moveTo()), and closes this sub-path. 
 * After this call the current point will be at the joined endpoint of the sub-path.
 *
 * The behavior of closePath() is distinct from simply calling lineTo() with the equivalent coordinate in the case of stroking.
 * When a closed sub-path is stroked, there are no caps on the ends of the sub-path. 
 * Instead, there is a line join connecting the final and initial segments of the sub-path.
 *
 * If there is no current point before the call to closePath(), this function will have no effect.
 *
 * Note: As of cairo version 1.2.4 any call to closePath() will place an explicit MOVE_TO element into the path immediately after the CLOSE_PATH element, (which can be seen in copyPath() for example).
 *  This can simplify path processing in some cases as it may not be necessary to save the "last move_to point" during processing as the MOVE_TO immediately after the CLOSE_PATH will provide that point.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(closePath){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_close_path(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * A drawing operator that paints the current source everywhere within the current clip region.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(paint){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_paint(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. 
 * After stroke(), the current path will be cleared from the cairo context. See setLineWidth(), setLineJoin(), setLineCap(), setDash(), and strokePreserve().
 *
 * Note: Degenerate segments and sub-paths are treated specially and provide a useful result. These can result in two different situations:
 *
 * Zero-length "on" segments set in setDash(). If the cap style is CAIRO_LINE_CAP_ROUND or CAIRO_LINE_CAP_SQUARE then these segments will be drawn as circular dots or squares respectively.
 * In the case of CAIRO_LINE_CAP_SQUARE, the orientation of the squares is determined by the direction of the underlying path.
 *
 * A sub-path created by moveTo() followed by either a closePath() or one or more calls to lineTo() to the same coordinate as the moveTo(). 
 * If the cap style is CAIRO_LINE_CAP_ROUND then these sub-paths will be drawn as circular dots. 
 * Note that in the case of CAIRO_LINE_CAP_SQUARE a degenerate sub-path will not be drawn at all, (since the correct orientation is indeterminate).
 *
 * In no case will a cap style of CAIRO_LINE_CAP_BUTT cause anything to be drawn in the case of either degenerate segments or sub-paths.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(stroke){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_stroke(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Makes a copy of the current state of the context and saves it on an internal stack of saved states for it . 
 * When restore() is called, the context will be restored to the saved state. 
 * Multiple calls to save() and restore() can be nested; each call to restore() restores the state from the matching paired save().
 *
 * It isn't necessary to clear all saved states before a context is freed. 
 * If the reference count of a context drops to zero, any saved states will be freed along with the context.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(save){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_save(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Restores the context to the state saved by a preceding call to save() and removes that state from the stack of saved states.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(restore){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_restore(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. 
 * Unlike stroke(), strokePreserve() preserves the path within the cairo context.
 * @public
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(strokePreserve){
	ze_context_object * ze_obj;
	zval * self = getThis();
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_stroke(ze_obj->context_ptr->intern);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Relative-coordinate version of curveTo(). All offsets are relative to the current point. 
 * Adds a cubic Bézier spline to the path from the current point to a point offset from the current point by (dx3 , dy3 ),
 * using points offset by (x1 , y1) and (dx2 , dy2 ) as the control points. After this call the current point will be offset by (dx3 , dy3 ).
 *
 * Given a current point of (x, y), relCurveTo(cr , dx1 , dy1 , dx2 , dy2 , dx3 , dy3 ) is logically equivalent to curveTo(cr , x+dx1 , y+dy1 , x+dx2 , y+dy2 , x+dx3 , y+dy3 ).
 * @public
 *
 * @param double $x1
 * @param double $y1
 * @param double $x2
 * @param double $y2
 * @param double $x3
 * @param double $y3
 *
 * @throw PGGI\Cairo\Exception If a problem occured
 *
 * @return self
 */
CONTEXT_METHOD(relCurveTo){
	zval * self = getThis();
	double x1, y1, x2, y2, x3, y3;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddddd", &x1, &y1, &x2, &y2, &x3, &y3) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rel_curve_to(ze_obj->context_ptr->intern, x1, y1, x2, y2, x3, y3); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Relative-coordinate version of lineTo(). Adds a line to the path from the current point to a point that is offset from the current point by (dx , dy ) in user space.
 * After this call the current point will be offset by (dx , dy ).
 *
 * Given a current point of (x, y), relLineTo(cr , dx , dy ) is logically equivalent to lineTo(cr , x + dx , y + dy ).
 * @public
 *
 * @param double $x
 * @param double $y
 *
 * @throw PGGI\Cairo\Exception It is an error to call this function with no current point. Doing so will cause the context to shutdown.
 *
 * @return self
 */
CONTEXT_METHOD(relLineTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rel_line_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Modifies the current transformation matrix (CTM) by scaling the X and Y user-space axes by x and y respectively.
 * The scaling of the axes takes place after any existing transformation of user space.
 * @public
 *
 * @param double $x
 * @param double $y
 *
 * @throw PGGI\Cairo\Exception It is an error to call this function with no current point. Doing so will cause the context to shutdown.
 *
 * @return self
 */
CONTEXT_METHOD(scale){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_scale(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Modifies the current transformation matrix (CTM) by rotating the user-space axes by angle radians. 
 * The rotation of the axes takes places after any existing transformation of user space.
 * The rotation direction for positive angles is from the positive X axis toward the positive Y axis.
 * @public
 *
 * @param double $angle
 *
 * @throw PGGI\Cairo\Exception It an error occured
 *
 * @return self
 */
CONTEXT_METHOD(rotate){
	zval * self = getThis();
	double angle;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &angle) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rotate(ze_obj->context_ptr->intern, angle); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Modifies the current transformation matrix (CTM) by translating the user-space origin by (tx , ty ).
 * This offset is interpreted as a user-space coordinate according to the CTM in place before the new call to translate().
 * In other words, the translation of the user-space origin takes place after any existing transformation.
 * @public
 *
 * @param double $x
 * @param double $y
 *
 * @throw PGGI\Cairo\Exception If an error occured.
 *
 * @return self
 */
CONTEXT_METHOD(translate){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_translate(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Begin a new sub-path. After this call the current point will offset by (x , y ).
 *
 * Given a current point of (x, y), relMoveTo(cr , dx , dy ) is logically equivalent to MoveTo(cr , x + dx , y + dy ).
 * @public
 *
 * @param double $x
 * @param double $y
 *
 * @throw PGGI\Cairo\Exception It is an error to call this function with no current point.
 *
 * @return self
 */
CONTEXT_METHOD(relMoveTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rel_move_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Adds a cubic Bézier spline to the path from the current point to position (x3 , y3 ) in user-space coordinates,
 * using (x1 , y1 ) and (x2 , y2 ) as the control points. After this call the current point will be (x3 , y3 ).
 *
 * If there is no current point before the call to curveTo() this function will behave as if preceded by a call to moveTo( x1 , y1 ).
 * @public
 *
 * @param double $x1
 * @param double $y1
 * @param double $x2
 * @param double $y2
 * @param double $x3
 * @param double $y3
 *
 * @throw PGGI\Cairo\Exception If an error occured
 *
 * @return self
 */
CONTEXT_METHOD(curveTo){
	zval * self = getThis();
	double x1, y1, x2, y2, x3, y3;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddddd", &x1, &y1, &x2, &y2, &x3, &y3) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_curve_to(ze_obj->context_ptr->intern, x1, y1, x2, y2, x3, y3); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Adds a line to the path from the current point to position (x , y ) in user-space coordinates. After this call the current point will be (x , y ).
 *
 * If there is no current point before the call to cairo_lineTo() this function will behave as moveTo(x , y ).
 * @public
 *
 * @param double $x
 * @param double $y
 *
 * @throw PGGI\Cairo\Exception If an error occured
 *
 * @return self
 */
CONTEXT_METHOD(lineTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_line_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Begin a new sub-path. After this call the current point will be (x , y ).
 * @public
 *
 * @param double $x
 * @param double $y
 *
 * @throw PGGI\Cairo\Exception If an error occured
 *
 * @return self
 */
CONTEXT_METHOD(moveTo){
	zval * self = getThis();
	double x, y;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &x, &y) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_move_to(ze_obj->context_ptr->intern, x, y); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Adds a closed sub-path rectangle of the given size to the current path at position (x , y ) in user-space coordinates.
 * This function is logically equivalent to:
 * ```
 * $Cr->moveTo( x, y);
 * $Cr->relLineTo (width, 0);
 * $Cr->relLineTo (0, height);
 * $Cr->relLineTo (-width, 0);
 * $Cr->closePath ();
 * ```
 * @public
 *
 * @param double $x
 * @param double $y
 * @param double $width
 * @param double $height
 *
 * @throw PGGI\Cairo\Exception If an error occured
 *
 * @return self
 */
CONTEXT_METHOD(rectangle){
	zval * self = getThis();
	double x, y, width, height;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddd", &x, &y, &width, &height) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_rectangle(ze_obj->context_ptr->intern, x, y, width, height); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Adds a circular arc of the given radius to the current path.
 * The arc is centered at (xc , yc ), begins at angle1 and proceeds in the direction of increasing angles to end at angle2.
 * If angle2 is less than angle1 it will be progressively increased by 2*M_PI until it is greater than angle1.
 *
 * If there is a current point, an initial line segment will be added to the path to connect the current point to the beginning of the arc.
 * If this initial line is undesired, it can be avoided by calling newSubPath() before calling arc().
 *
 * Angles are measured in radians. An angle of 0.0 is in the direction of the positive X axis (in user space). 
 * An angle of M_PI/2.0 radians (90 degrees) is in the direction of the positive Y axis (in user space).
 * Angles increase in the direction from the positive X axis toward the positive Y axis. 
 * So with the default transformation matrix, angles increase in a clockwise direction.
 *
 * (To convert from degrees to radians, use degrees * (M_PI / 180.).)
 *
 * This function gives the arc in the direction of increasing angles; see arcNegative() to get the arc in the direction of decreasing angles.
 *
 * The arc is circular in user space.
 * To achieve an elliptical arc, you can scale the current transformation matrix by different amounts in the X and Y directions.
 * For example, to draw an ellipse in the box given by x , y , width , height :
 * ```
 * $Cr->save();
 * $Cr->translate(x + width / 2., y + height / 2.);
 * $Cr->scale(width / 2., height / 2.);
 * $Cr->arc(0., 0., 1., 0., 2 * M_PI);
 * $Cr->restore();
 * ```
 * @public
 *
 * @param double $x
 * @param double $y
 * @param double $radius
 * @param double $angle1
 * @param double $angle2
 *
 * @throw PGGI\Cairo\Exception If an error occured
 *
 * @return self
 */
CONTEXT_METHOD(arc){
	zval * self = getThis();
	double x, y, radius, angle1, angle2;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ddddd", &x, &y, &radius, &angle1, &angle2) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_arc(ze_obj->context_ptr->intern, x, y, radius, angle1, angle2);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Adds a circular arc of the given radius to the current path.
 * The arc is centered at (xc , yc ), begins at angle1 and proceeds in the direction of decreasing angles to end at angle2.
 * If angle2 is greater than angle1 it will be progressively decreased by 2*M_PI until it is less than angle1 .
 * See arc() for more details. This function differs only in the direction of the arc between the two angles.
 * @public
 *
 * @param double $x
 * @param double $y
 * @param double $radius
 * @param double $angle1
 * @param double $angle2
 *
 * @throw PGGI\Cairo\Exception If an error occured
 *
 * @return self
 */
CONTEXT_METHOD(arcNegative){
	zval * self = getThis();
	double x, y, radius, angle1, angle2;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ddddd", &x, &y, &radius, &angle1, &angle2) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_arc_negative(ze_obj->context_ptr->intern, x, y, radius, angle1, angle2); 
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

/*==========================================================================*/
/**
 * Sets the source pattern within the context to an color. This color will then be used for any subsequent drawing operation until a new source pattern is set.
 *
 * The color components are floating point numbers in the range 0 to 1,
 * If the values passed in are outside that range, they will be clamped.
 *
 * The default source pattern is opaque black, (that is, it is equivalent to setColor(0.0, 0.0, 0.0)).

 * @public
 *
 * @param double $color
 *
 * @throw PGGI\Cairo\Exception If an error occured
 *
 * @return self
 */
CONTEXT_METHOD(setColor){
	zval * self = getThis();
	zval * color;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &color, rgba_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	ze_rgba_object * c = Z_RGBA_P(color);
	gdk_cairo_set_source_rgba(ze_obj->context_ptr->intern, c->ptr->color);
	RETURN_ZVAL(self, 1, 0);
}

CONTEXT_METHOD(setSourceRGBA){
	zval * self = getThis();
	double r, g, b, a = -1;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ddd|d", &r, &g, &b, &a) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	if(a != -1)
		cairo_set_source_rgba(ze_obj->context_ptr->intern,r, g, b, a);
	else
		cairo_set_source_rgb(ze_obj->context_ptr->intern,r, g, b);
	RETURN_ZVAL(self, 1, 0);
}

CONTEXT_METHOD(setSource){
	zval * self = getThis();
	zval * obj;
	ze_context_object * ze_obj;
	ze_obj = Z_CONTEXT_P(self);
	double r, g, b, a=-1, x, y;
	switch(ZEND_NUM_ARGS()){
		case 1 :
			if(zend_parse_parameters(ZEND_NUM_ARGS(), "O", &obj, pc_pattern_get_class_entry()) != FAILURE){
				ze_pattern_object * c = Z_PATTERN_P(obj);
				cairo_set_source(ze_obj->context_ptr->intern, c->pattern_ptr->intern);
			}else if(zend_parse_parameters(ZEND_NUM_ARGS(), "O", &obj, pc_surface_get_class_entry()) != FAILURE){
				ze_surface_object * c = Z_SURFACE_P(obj);
				cairo_set_source_surface(ze_obj->context_ptr->intern, c->surface_ptr->intern, 0, 0);
				ze_obj->context_ptr->target = obj;
			}else if(zend_parse_parameters(ZEND_NUM_ARGS(), "O", &obj, rgba_get_class_entry()) != FAILURE){
				ze_rgba_object * c = Z_RGBA_P(obj);
				gdk_cairo_set_source_rgba(ze_obj->context_ptr->intern, c->ptr->color);
			}else if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &obj, gpixbuf_get_class_entry()) != FAILURE){
				ze_gpixbuf_object * c = Z_GPIXBUF_P(obj);
				gdk_cairo_set_source_pixbuf(ze_obj->context_ptr->intern, c->pixbuf_ptr->intern, 0, 0);
			}else
				return;
			break;
		case 4 :
		case 3 :
		default :
			//generate a warning for now I know that
			if(zend_parse_parameters(ZEND_NUM_ARGS(), "ddd|d", &r, &g, &b, &a) != FAILURE){
				if(a != -1)
					cairo_set_source_rgba(ze_obj->context_ptr->intern, r, g, b, a);
				else
					cairo_set_source_rgb(ze_obj->context_ptr->intern, r, g, b);
			}else if(zend_parse_parameters(ZEND_NUM_ARGS(), "Odd", &obj, pc_surface_get_class_entry(), &x, &y) != FAILURE){
				ze_surface_object * c = Z_SURFACE_P(obj);
				ze_obj->context_ptr->target = obj;
				cairo_set_source_surface(ze_obj->context_ptr->intern, c->surface_ptr->intern, x, y);
			}else if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &obj, gpixbuf_get_class_entry(), &x, &y) != FAILURE){
				ze_gpixbuf_object * c = Z_GPIXBUF_P(obj);
				gdk_cairo_set_source_pixbuf(ze_obj->context_ptr->intern, c->pixbuf_ptr->intern, x, y);
			}else
				return;		
	}
	RETURN_ZVAL(self, 1, 0);
}

CONTEXT_METHOD(setDash){
	double offset = 0.0;
	long num_dashes = 0;
	double *dashes_array;
	zval * dashes = NULL;
	int i = 0;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "a|d", &dashes, &offset) == FAILURE)
		return;
	zval * self = getThis();
	ze_context_object * ze_obj;
	ze_obj = Z_CONTEXT_P(self);
	num_dashes = zend_hash_num_elements(Z_ARRVAL_P(dashes));
	dashes_array = emalloc(num_dashes * sizeof(double));
	zval * zv;
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(dashes), zv){
		if(Z_TYPE_P(zv) != IS_DOUBLE){
			convert_to_double(zv);
		}
		dashes_array[i++] = Z_DVAL_P(zv);
	} ZEND_HASH_FOREACH_END();
	cairo_set_dash(ze_obj->context_ptr->intern, dashes_array, i, offset);
	efree(dashes_array);
	pc_exception(cairo_status(ze_obj->context_ptr->intern));
	RETURN_ZVAL(self, 1, 0);
}

CONTEXT_METHOD(getDash){
	zval sub_array;
	double *dashes = NULL;
	double offset = 0;
	int num_dashes, i;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	zval * self = getThis();
	ze_context_object * ze_obj;
	ze_obj = Z_CONTEXT_P(self);

	num_dashes = cairo_get_dash_count(ze_obj->context_ptr->intern);
	dashes = emalloc(num_dashes * sizeof(double));

	cairo_get_dash(ze_obj->context_ptr->intern, dashes, &offset);

	array_init(&sub_array);
	for(i = 0; i < num_dashes; i++){
		add_next_index_double(&sub_array, dashes[i]);
	}
	efree(dashes);
	
	/* Put dashes and offset into return */
	array_init(return_value);
	add_assoc_zval(return_value, "dashes", &sub_array);
	add_assoc_double(return_value, "offset", offset);
}

CONTEXT_METHOD(showLayout){
	zval * self = getThis();
	zval * layout;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &layout, pp_layout_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	ze_layout_object * l = Z_LAYOUT_P(layout);
	pango_cairo_show_layout(ze_obj->context_ptr->intern, l->layout_ptr->intern);
}

CONTEXT_METHOD(updateLayout){
	zval * self = getThis();
	zval * layout;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &layout, pp_layout_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	ze_layout_object * l = Z_LAYOUT_P(layout);
	pango_cairo_update_layout(ze_obj->context_ptr->intern, l->layout_ptr->intern);
}


CONTEXT_METHOD(updateContext){
	zval * self = getThis();
	zval * layout;
	ze_context_object * ze_obj;
	if(zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &layout, pp_context_get_class_entry()) == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	ze_pp_context_object * c = Z_PP_CONTEXT_P(layout);
	pango_cairo_update_context(ze_obj->context_ptr->intern, c->context_ptr->intern);
}


CONTEXT_METHOD(getTarget){
	zval * self = getThis();
	ze_context_object * ze_obj;
	if(pggi_parse_parameters_none_throw() == FAILURE)
		return;
	ze_obj = Z_CONTEXT_P(self);
	cairo_surface_t * target = cairo_get_target(ze_obj->context_ptr->intern);
	zend_object * obj = pc_surface_object_new(pc_surface_get_class_entry());
	ze_surface_object * tmp = php_surface_fetch_object(obj);
	tmp->surface_ptr = pc_surface_new();
	tmp->surface_ptr->intern = target;
	RETURN_OBJ(obj);
}



static const zend_function_entry pc_context_class_functions[] = {
	PHP_ME(Context, fill          , arginfo_pggi_void                 , ZEND_ACC_PUBLIC) // to change to return self if it works
	PHP_ME(Context, fillPreserve  , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, paint         , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, clip          , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, clipPreserve  , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, newPath       , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, newSubPath    , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, closePath     , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, stroke        , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, strokePreserve, arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, save          , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, restore       , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, arc           , arginfo_pc_context_arc            , ZEND_ACC_PUBLIC)
	PHP_ME(Context, arcNegative   , arginfo_pc_context_arc            , ZEND_ACC_PUBLIC)
	PHP_ME(Context, curveTo       , arginfo_pc_context_curved_to      , ZEND_ACC_PUBLIC)
	PHP_ME(Context, lineTo        , arginfo_pc_context_line_to        , ZEND_ACC_PUBLIC)
	PHP_ME(Context, moveTo        , arginfo_pc_context_line_to        , ZEND_ACC_PUBLIC)
	PHP_ME(Context, relCurveTo    , arginfo_pc_context_curved_to      , ZEND_ACC_PUBLIC)
	PHP_ME(Context, relLineTo     , arginfo_pc_context_line_to        , ZEND_ACC_PUBLIC)
	PHP_ME(Context, relMoveTo     , arginfo_pc_context_line_to        , ZEND_ACC_PUBLIC)
	PHP_ME(Context, scale         , arginfo_pc_context_scale          , ZEND_ACC_PUBLIC)
	PHP_ME(Context, rotate        , arginfo_pc_context_rotate         , ZEND_ACC_PUBLIC)
	PHP_ME(Context, translate     , arginfo_pc_context_translate      , ZEND_ACC_PUBLIC)
	PHP_ME(Context, rectangle     , arginfo_pc_context_rectangle      , ZEND_ACC_PUBLIC)
	PHP_ME(Context, setColor      , arginfo_pc_context_set_color      , ZEND_ACC_PUBLIC)
	PHP_ME(Context, setSource     , NULL                              , ZEND_ACC_PUBLIC)
	PHP_ME(Context, setSourceRGBA , arginfo_pc_context_set_source_rgba, ZEND_ACC_PUBLIC)
	PHP_ME(Context, setDash       , arginfo_pc_context_set_dash       , ZEND_ACC_PUBLIC)
	PHP_ME(Context, getDash       , arginfo_pggi_void                 , ZEND_ACC_PUBLIC)
	PHP_ME(Context, updateContext , arginfo_pc_context_update_context , ZEND_ACC_PUBLIC)
	PHP_ME(Context, updateLayout  , arginfo_pc_context_update_layout  , ZEND_ACC_PUBLIC)
	PHP_ME(Context, showLayout    , arginfo_pc_context_show_layout    , ZEND_ACC_PUBLIC)
	PHP_ME(Context, __construct   , arginfo_pc_context_construct      , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(Context, getTarget     , arginfo_pc_context_get_target     , ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/*****************************/
/* Object handling functions */
/*****************************/

zval *pc_context_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv){
	ze_context_object * intern = Z_CONTEXT_P(object);
	pc_context_ptr c = intern->context_ptr;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	 if(!strcmp(member_val, CONTEXT_LINE_CAP)){
		ZVAL_LONG(rv, cairo_get_line_cap(c->intern));
	}else if(!strcmp(member_val, CONTEXT_LINE_JOIN)){
		ZVAL_LONG(rv, cairo_get_line_join(c->intern));
	}else if(!strcmp(member_val, CONTEXT_FILL_RULE)){
		ZVAL_LONG(rv, cairo_get_fill_rule(c->intern));
	}else if(!strcmp(member_val, CONTEXT_LINE_WIDTH)){
		ZVAL_DOUBLE(rv, cairo_get_line_width(c->intern));
	}else if(!strcmp(member_val, CONTEXT_OPERATOR)){
		ZVAL_LONG(rv, cairo_get_operator(c->intern));
	}else
		return std_object_handlers.read_property(object, member, type, cache_slot, rv);
	pc_exception(cairo_status(c->intern));
	return rv;
}

HashTable *pc_context_get_properties(zval *object){
	G_H_UPDATE_INIT(zend_std_get_properties(object));
	ze_context_object * intern = Z_CONTEXT_P(object);
	pc_context_ptr c = intern->context_ptr;
	G_H_UPDATE_LONG  (CONTEXT_FILL_RULE , cairo_get_fill_rule (c->intern));
	G_H_UPDATE_LONG  (CONTEXT_LINE_CAP  , cairo_get_line_cap  (c->intern));
	G_H_UPDATE_LONG  (CONTEXT_LINE_JOIN , cairo_get_line_join (c->intern));
	G_H_UPDATE_DOUBLE(CONTEXT_LINE_WIDTH, cairo_get_line_width(c->intern));
	G_H_UPDATE_LONG  (CONTEXT_OPERATOR  , cairo_get_operator  (c->intern));
	pc_exception(cairo_status(c->intern));
	return G_H_UPDATE_RETURN;
}

PHP_WRITE_PROP_HANDLER_TYPE pc_context_write_property(zval *object, zval *member, zval *value, void **cache_slot){
	ze_context_object * intern = Z_CONTEXT_P(object);
	pc_context_ptr c = intern->context_ptr;
	double tmp_d;
	long tmp_l;
	convert_to_string(member);
	char * member_val = Z_STRVAL_P(member);
	if(!strcmp(member_val, CONTEXT_OPERATOR)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case CAIRO_OPERATOR_CLEAR:
			case CAIRO_OPERATOR_IN:
			case CAIRO_OPERATOR_OUT:
			case CAIRO_OPERATOR_ATOP:
			case CAIRO_OPERATOR_DEST:
			case CAIRO_OPERATOR_DEST_OVER:
			case CAIRO_OPERATOR_DEST_IN:
			case CAIRO_OPERATOR_DEST_OUT :
			case CAIRO_OPERATOR_DEST_ATOP:
			case CAIRO_OPERATOR_XOR:
			case CAIRO_OPERATOR_ADD:
			case CAIRO_OPERATOR_SATURATE:
			case CAIRO_OPERATOR_MULTIPLY :
			case CAIRO_OPERATOR_SCREEN:
			case CAIRO_OPERATOR_OVERLAY:
			case CAIRO_OPERATOR_DARKEN:
			case CAIRO_OPERATOR_LIGHTEN:
			case CAIRO_OPERATOR_COLOR_DODGE:
			case CAIRO_OPERATOR_COLOR_BURN:
			case CAIRO_OPERATOR_HARD_LIGHT:
			case CAIRO_OPERATOR_SOFT_LIGHT:
			case CAIRO_OPERATOR_DIFFERENCE:
			case CAIRO_OPERATOR_EXCLUSION:
			case CAIRO_OPERATOR_HSL_HUE:
			case CAIRO_OPERATOR_HSL_SATURATION:
			case CAIRO_OPERATOR_HSL_COLOR:
			case CAIRO_OPERATOR_HSL_LUMINOSITY:
				// If the operator is valid
				cairo_set_operator(c->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the operator property, needs to be a Context::OPERATOR_*");
				break;
		}
	}else if(!strcmp(member_val, CONTEXT_LINE_CAP)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case CAIRO_LINE_CAP_BUTT  :
			case CAIRO_LINE_CAP_ROUND :
			case CAIRO_LINE_CAP_SQUARE:
				cairo_set_line_cap(c->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the lineCaps property, needs to be a Context::LINE_CAP_*");
				return;
				break;
		}
	}else if(!strcmp(member_val, CONTEXT_LINE_JOIN)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case CAIRO_LINE_JOIN_MITER:
			case CAIRO_LINE_JOIN_ROUND:
			case CAIRO_LINE_JOIN_BEVEL:
				cairo_set_line_join(c->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the lineJoin property, needs to be a Context::LINE_JOIN_*");
				return;
				break;
		}
	}else if(!strcmp(member_val, CONTEXT_FILL_RULE)){
		convert_to_long(value);
		tmp_l = Z_LVAL_P(value);
		switch(tmp_l){
			case CAIRO_FILL_RULE_WINDING :
			case CAIRO_FILL_RULE_EVEN_ODD:
				cairo_set_fill_rule(c->intern, tmp_l);
				break;
			default:
				zend_throw_exception_ex(pggi_exception_get(), 0, "Can't change the fillRule property, needs to be a Context::FILL_RULE_*");
				return;
				break;
		}
	}else if(!strcmp(member_val, CONTEXT_LINE_WIDTH)){
		convert_to_double(value);
		tmp_d = Z_DVAL_P(value);
		cairo_set_line_width(c->intern, tmp_d);
	} else {
		PHP_WRITE_PROP_HANDLER_RETURN(std_object_handlers.write_property(object, member, value, cache_slot));
	}
	pc_exception(cairo_status(c->intern));
	PHP_WRITE_PROP_HANDLER_RETURN(value);
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
	le_context = zend_register_list_destructors_ex(pc_context_free_resource, NULL, "PGGI\\Cairo\\Context", module_number);

	memcpy(&context_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	context_object_handlers.offset         = XtOffsetOf(ze_context_object, std);
	context_object_handlers.free_obj       = pc_context_object_free_storage;
	context_object_handlers.clone_obj      = NULL;
	context_object_handlers.read_property  = pc_context_read_property;
	context_object_handlers.get_properties = pc_context_get_properties;
	context_object_handlers.write_property = pc_context_write_property;
	INIT_CLASS_ENTRY(ce, "PGGI\\Cairo\\Context", pc_context_class_functions);
	ce.create_object = pc_context_object_new;
	context_class_entry_ce = zend_register_internal_class(&ce);

	DECLARE_CONTEXT_PROP(CONTEXT_LINE_WIDTH);
	DECLARE_CONTEXT_PROP(CONTEXT_FILL_RULE );
	DECLARE_CONTEXT_PROP(CONTEXT_LINE_CAP  );
	DECLARE_CONTEXT_PROP(CONTEXT_LINE_JOIN );
	DECLARE_CONTEXT_PROP(CONTEXT_OPERATOR  );

	CONTEXT_CONSTANT("FILL_RULE_WINDING"      , CAIRO_FILL_RULE_WINDING      );
	CONTEXT_CONSTANT("FILL_RULE_EVEN_ODD"     , CAIRO_FILL_RULE_EVEN_ODD     );

	CONTEXT_CONSTANT("LINE_CAP_BUTT"          , CAIRO_LINE_CAP_BUTT          );
	CONTEXT_CONSTANT("LINE_CAP_ROUND"         , CAIRO_LINE_CAP_ROUND         );
	CONTEXT_CONSTANT("LINE_CAP_SQUARE"        , CAIRO_LINE_CAP_SQUARE        );

	CONTEXT_CONSTANT("LINE_JOIN_MITER"        , CAIRO_LINE_JOIN_MITER        );
	CONTEXT_CONSTANT("LINE_JOIN_ROUND"        , CAIRO_LINE_JOIN_ROUND        );
	CONTEXT_CONSTANT("LINE_JOIN_BEVEL"        , CAIRO_LINE_JOIN_BEVEL        );

	CONTEXT_CONSTANT("OPERATOR_CLEAR"         , CAIRO_OPERATOR_CLEAR         );
	CONTEXT_CONSTANT("OPERATOR_IN"            , CAIRO_OPERATOR_IN            );
	CONTEXT_CONSTANT("OPERATOR_OUT"           , CAIRO_OPERATOR_OUT           );
	CONTEXT_CONSTANT("OPERATOR_ATOP"          , CAIRO_OPERATOR_ATOP          );
	CONTEXT_CONSTANT("OPERATOR_DEST"          , CAIRO_OPERATOR_DEST          );
	CONTEXT_CONSTANT("OPERATOR_DEST_OVER"     , CAIRO_OPERATOR_DEST_OVER     );
	CONTEXT_CONSTANT("OPERATOR_DEST_IN"       , CAIRO_OPERATOR_DEST_IN       );
	CONTEXT_CONSTANT("OPERATOR_DEST_OUT"      , CAIRO_OPERATOR_DEST_OUT      );
	CONTEXT_CONSTANT("OPERATOR_DEST_ATOP"     , CAIRO_OPERATOR_DEST_ATOP     );
	CONTEXT_CONSTANT("OPERATOR_XOR"           , CAIRO_OPERATOR_XOR           );
	CONTEXT_CONSTANT("OPERATOR_ADD"           , CAIRO_OPERATOR_ADD           );
	CONTEXT_CONSTANT("OPERATOR_SATURATE"      , CAIRO_OPERATOR_SATURATE      );
	CONTEXT_CONSTANT("OPERATOR_MULTIPLY"      , CAIRO_OPERATOR_MULTIPLY      );
	CONTEXT_CONSTANT("OPERATOR_SCREEN"        , CAIRO_OPERATOR_SCREEN        );
	CONTEXT_CONSTANT("OPERATOR_OVERLAY"       , CAIRO_OPERATOR_OVERLAY       );
	CONTEXT_CONSTANT("OPERATOR_DARKEN"        , CAIRO_OPERATOR_DARKEN        );
	CONTEXT_CONSTANT("OPERATOR_LIGHTEN"       , CAIRO_OPERATOR_LIGHTEN       );
	CONTEXT_CONSTANT("OPERATOR_COLOR_DODGE"   , CAIRO_OPERATOR_COLOR_DODGE   );
	CONTEXT_CONSTANT("OPERATOR_COLOR_BURN"    , CAIRO_OPERATOR_COLOR_BURN    );
	CONTEXT_CONSTANT("OPERATOR_HARD_LIGHT"    , CAIRO_OPERATOR_HARD_LIGHT    );
	CONTEXT_CONSTANT("OPERATOR_SOFT_LIGHT"    , CAIRO_OPERATOR_SOFT_LIGHT    );
	CONTEXT_CONSTANT("OPERATOR_DIFFERENCE"    , CAIRO_OPERATOR_DIFFERENCE    );
	CONTEXT_CONSTANT("OPERATOR_EXCLUSION"     , CAIRO_OPERATOR_EXCLUSION     );
	CONTEXT_CONSTANT("OPERATOR_HSL_HUE"       , CAIRO_OPERATOR_HSL_HUE       );
	CONTEXT_CONSTANT("OPERATOR_HSL_SATURATION", CAIRO_OPERATOR_HSL_SATURATION);
	CONTEXT_CONSTANT("OPERATOR_HSL_COLOR"     , CAIRO_OPERATOR_HSL_COLOR     );
	CONTEXT_CONSTANT("OPERATOR_HSL_LUMINOSITY", CAIRO_OPERATOR_HSL_LUMINOSITY);
}

