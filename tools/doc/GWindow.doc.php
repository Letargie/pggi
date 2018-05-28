<?php

include_once("../Gasped/DocArray.class.php");

$GWindowDoc = new ClassDocArray();
$GWindowDoc->setHeader(array())
		   ->addMethod("__construct"	, array("create a new GWindow link to an application",
												'@param $app GApplication the application to link the window',
												'@param $title string the title use by the window'))
		   ->addMethod("setTitle"		, array("change the window title (obsolete function probably)",
												'@param $name string the new title of the window'))
		   ->addMethod("setDefaultSize" , array("set the default size of the window",
												'@param $x the new width of the window',
												'@param $y the new height of the window'))
		   ->addMethod("close"			, array("Requests that the window is closed, similar to what happens when a window manager close button is clicked."))
		   ->addMethod("setKeepAbove"	, array("Asks to keep window above, so that it stays on top. Note that you shouldn’t assume the window is definitely above afterward, because other entities could not keep it above, and not all window managers support keeping windows above. But normally the window will end kept above. Just don’t write code that crashes if not.
It’s permitted to call this function before showing a window, in which case the window will be kept above when it appears onscreen initially."))
		   ->addMethod("setKeepBelow"	, array("Asks to keep window below, so that it stays in bottom. Note that you shouldn’t assume the window is definitely below afterward, because other entities could not keep it below, and not all window managers support putting windows below. But normally the window will be kept below. Just don’t write code that crashes if not. It’s permitted to call this function before showing a window, in which case the window will be kept below when it appears onscreen initially"))
		   ->addMethod("fullscreen"		, array("Asks to place window in the fullscreen state. Note that you shouldn’t assume the window is definitely full screen afterward, because other entities could unfullscreen it again, and not all window managers honor requests to fullscreen windows. But normally the window will end up fullscreen. Just don’t write code that crashes if not."))
		   ->addMethod("unfullscreen"	, array("Asks to toggle off the fullscreen state for window . Note that you shouldn’t assume the window is definitely not full screen afterward, because other entities (e.g. the user or window manager) could fullscreen it again, and not all window managers honor requests to unfullscreen windows. But normally the window will end up restored to its normal state. Just don’t write code that crashes if not."));


;

/*
class GWindow extends GContainer {
	public $title;
	public $focusVisible;
	public $focusOnMap;
	public $decorated;
	public $deletable;
	public $mnemonicsVisible;
	public $resizable;
	public $defaultHeight;
	public $defaultWidth;
	public $hideTitlebarWhenMaximized;
	public $acceptFocus;
}*/
