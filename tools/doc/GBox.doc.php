<?php
/**
 * pggi-API v1.0 Docs build by Gasped [2017-08-12]
 * @package pggi
 */

include_once("../Gasped/DocArray.class.php");

$GBoxDoc = new ClassDocArray();
$GBoxDoc->setHeader(array("A container box"))
		->addMethod("__construct"		, array("Create a new GBox",
												'@param $orientation long orientation of the Box : a PGGI_ORIENTATION',
												'@param $spacing long the number of pixels to place by default between children.'))
		->addMethod('packStart'		, array("Adds child to box , packed with reference to the start of box.",
												"The child is packed after any other child packed with reference to the start of box .",
												'@param $object GWidget the widget to be add to the box.',
												'@param $expand boolean true if the new child is to be given extra space allocated to box. The extra space will be divided evenly between all children that use this option',
												'@param $fill boolean true if space given to child by the expand option is actually allocated to child , rather than just padding it. This parameter has no effect if expand is set to FALSE. A child is always allocated the full height of a horizontal GBox and the full width of a vertical GBox. This option affects the other dimension',
												'@param $padding long extra space in pixels to put between this child and its neighbors, over and above the global amount specified by “spacing” property. If child is a widget at one of the reference ends of box, then padding pixels are also put between child and the reference edge of box '))
		->addMethod('packEnd'			, array("Adds child to box , packed with reference to the end of box.",
												"The child is packed after (away from end of) any other child packed with reference to the end of box .",
												'@param $object GWidget the widget to be add to the box.',
												'@param $expand boolean true if the new child is to be given extra space allocated to box. The extra space will be divided evenly between all children that use this option',
												'@param $fill boolean true if space given to child by the expand option is actually allocated to child , rather than just padding it. This parameter has no effect if expand is set to FALSE. A child is always allocated the full height of a horizontal GBox and the full width of a vertical GBox. This option affects the other dimension',
												'@param $padding long extra space in pixels to put between this child and its neighbors, over and above the global amount specified by “spacing” property. If child is a widget at one of the reference ends of box, then padding pixels are also put between child and the reference edge of box '))
		->addProperty('homogeneous'		, array("@var Whether the children should all be the same size."))
		->addProperty('spacing'			, array("@var The amount of space between children."))
		->addProperty('baselinePosition', array("@var The position of the baseline aligned widgets if extra space is available."));
