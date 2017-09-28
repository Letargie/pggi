<?php
/**
 * pggi-API v1.0 Docs build by Gasped [2017-08-12]
 * @package pggi
 */

include_once("../Gasped/DocArray.class.php");

$GWidgetDoc = new ClassDocArray();
$GWidgetDoc->setHeader(array("Base class for all widgets"))
		   ->addConstant("ALIGN_FILL"		, array("stretch to fill all space if possible, center if no meaningful way to stretch"))
		   ->addConstant("ALIGN_START"		, array("snap to left or top side, leaving space on right or bottom"))
		   ->addConstant("ALIGN_END"		, array("snap to right or bottom side, leaving space on left or top"))
		   ->addConstant("ALIGN_CENTER"		, array("center natural width of widget inside the allocation"))
		   ->addConstant("ALIGN_BASELINE"	, array("align the widget according to the baseline. Since 3.10."))
		   ->addMethod  ("__construct"		, array("create a new GWidget"))
		   ->addMethod  ("on"				, array("connect a callback to a signal the can receive",
													'@param $type long the signal'					,
													'@param $callback long the callback to connect'	,
													'@param $param optional parameters'				))
		   ->addMethod  ("hide"				, array("hide the GWidget"))
		   ->addMethod  ("show"				, array("show the GWidget"))
		   ->addMethod  ("showAll"			, array("show all widgets contained in the GWidget"))
		   ->addProperty("vAlign"			, array("@var How to distribute vertical space if widget gets extra space"))
		   ->addProperty("hAlign"			, array("@var How to distribute horizontal space if widget gets extra space"))
		   ->addProperty("marginStart"		, array("@var Margin on start of widget, horizontally. This property supports left-to-right and right-to-left text directions."))
		   ->addProperty("marginEnd"		, array("@var Margin on end of widget, horizontally. This property supports left-to-right and right-to-left text directions."))
		   ->addProperty("marginTop"		, array("@var Margin on top side of widget."))
		   ->addProperty("marginBottom"		, array("@var Margin on bottom side of widget."))
		   ->addProperty("hExpand"			, array("@var Whether to expand horizontally"))
		   ->addProperty("hExpandSet"		, array("@var Whether to use the 'hExpand' property"))
		   ->addProperty("vExpand"			, array("@var Whether to expand vertically."))
		   ->addProperty("vExpandSet"		, array("@var Whether to use the 'vExpand' property"))
		   ->addProperty("opacity"			, array("@var The requested opacity of the widget"))
		   ->addProperty("visible"			, array("@var Whether the widget is visible."))
		   ->addProperty("noShowAll"		, array("@var Whether showAll() should not affect this widget."))
		   ->addProperty("direction"		, array("@var The reading direction on a particular widget. This direction controls the primary direction for widgets containing text, and also the direction in which the children of a container are packed. The ability to set the direction is present in order so that correct localization into languages with right-to-left reading directions can be done. Generally, applications will let the default reading direction present, except for containers where the containers are arranged in an order that is explicitly visual rather than logical"))
		   ->addProperty("sensitive"		, array("@var Whether the widget responds to input."))
		   ->addProperty("name"				, array("@var The name of the widget."))
		   ->addProperty("canDefault"		, array("@var Whether the widget can be the default widget."))
		   ->addProperty("canFocus"			, array("@var Whether the widget can accept the input focus."));

