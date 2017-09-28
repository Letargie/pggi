<?php

chdir(__DIR__);

include_once("../Gasped/DocArray.class.php"	);
include_once("GAboutDialog.doc.php"			);
include_once("GApplication.doc.php"			);
include_once("GBox.doc.php"					);
include_once("GButton.doc.php"				);
include_once("GComboBox.doc.php"			);
include_once("GComboBoxText.doc.php"		);
include_once("GContainer.doc.php"			);
include_once("GDialog.doc.php"				);
include_once("GImage.doc.php"				);
include_once("GLabel.doc.php"				);
include_once("GMenu.doc.php"				);
include_once("GMenubar.doc.php"				);
include_once("GMenuItem.doc.php"			);
include_once("GMenuShell.doc.php"			);
include_once("GScrollWindow.doc.php"		);
include_once("GTextBuffer.doc.php"			);
include_once("GTextView.doc.php"			);
include_once("GWidget.doc.php"				);
include_once("GWindow.doc.php"				);
include_once("PGGIException.doc.php"		);

$PGGIDoc = new ExtensionDocArray();
$PGGIDoc->addConstant("PGGI_BASELINE_POSITION_TOP"				, array("@var align the baseline at the top"))
		->addConstant("PGGI_BASELINE_POSITION_CENTER"			, array("@var center the baseline"))
		->addConstant("PGGI_BASELINE_POSITION_BOTTOM"			, array("@var align the baseline at the bottom"))
		->addConstant("PGGI_DELETE_CHARS"						, array("@var Delete characters."))
		->addConstant("PGGI_DELETE_WORD_ENDS"					, array("@var Delete only the portion of the word to the left/right of cursor if we’re in the middle of a word."))
		->addConstant("PGGI_DELETE_WORDS"						, array("@var Delete words."))
		->addConstant("PGGI_DELETE_DISPLAY_LINES"				, array("@var Delete display-lines.Display-lines refers to the visible lines, with respect to to the current line breaks. As opposed to paragraphs, which are defined by line breaks in the input."))
		->addConstant("PGGI_DELETE_DISPLAY_LINE_ENDS"			, array("@var Delete only the portion of the display-line to the left/right of cursor."))
		->addConstant("PGGI_DELETE_PARAGRAPH_ENDS"				, array("@var Delete to the end of the paragraph."))
		->addConstant("PGGI_DELETE_PARAGRAPHS"					, array("@var Delete entire line."))
		->addConstant("PGGI_DELETE_WHITESPACE"					, array("@var Delete only whitespace."))
		->addConstant("PGGI_DIR_TAB_FORWARD"					, array("@var Move forward."))
		->addConstant("PGGI_DIR_TAB_BACKWARD"					, array("@var Move backward."))
		->addConstant("PGGI_DIR_UP"								, array("@var Move up."))
		->addConstant("PGGI_DIR_DOWN"							, array("@var Move down."))
		->addConstant("PGGI_DIR_LEFT"							, array("@var Move left."))
		->addConstant("PGGI_DIR_RIGHT"							, array("@var Move right."))
		->addConstant("PGGI_JUSTIFY_LEFT"						, array("@var The text is placed at the left edge of the label."))
		->addConstant("PGGI_JUSTIFY_RIGHT"						, array("@var The text is placed at the right edge of the label."))
		->addConstant("PGGI_JUSTIFY_CENTER"						, array("@var The text is placed in the center of the label."))
		->addConstant("PGGI_JUSTIFY_FILL"						, array("@var The text is placed is distributed across the label."))
		->addConstant("PGGI_MOVEMENT_LOGICAL_POSITIONS"			, array("@var Move forward or back by graphemes"))
		->addConstant("PGGI_MOVEMENT_VISUAL_POSITIONS"			, array("@var Move left or right by graphemes"))
		->addConstant("PGGI_MOVEMENT_WORDS"						, array("@var Move forward or back by words"))
		->addConstant("PGGI_MOVEMENT_DISPLAY_LINES"				, array("@var Move up or down lines (wrapped lines)"))
		->addConstant("PGGI_MOVEMENT_DISPLAY_LINE_ENDS"			, array("@var Move to either end of a line"))
		->addConstant("PGGI_MOVEMENT_PARAGRAPHS"				, array("@var Move up or down paragraphs (newline-ended lines)"))
		->addConstant("PGGI_MOVEMENT_PARAGRAPH_ENDS"			, array("@var Move to either end of a paragraph"))
		->addConstant("PGGI_MOVEMENT_BUFFER_ENDS"				, array("@var Move by pages"))
		->addConstant("PGGI_MOVEMENT_BUFFER_ENDS"				, array("@var Move to ends of the buffer"))
		->addConstant("PGGI_MOVEMENT_HORIZONTAL_PAGES"			, array("@var Move horizontally by pages"))
		->addConstant("PGGI_ORIENTATION_HORIZONTAL"				, array("@var The element is in horizontal orientation."))
		->addConstant("PGGI_ORIENTATION_VERTICAL"				, array("@var The element is in vertical orientation."))
		->addConstant("PGGI_PACK_START"							, array("@var The child is packed into the start of the box"))
		->addConstant("PGGI_PACK_END"							, array("@var The child is packed into the end of the box"))
		->addConstant("PGGI_POS_LEFT"							, array("@var The feature is at the left edge."))
		->addConstant("PGGI_POS_RIGHT"							, array("@var The feature is at the right edge."))
		->addConstant("PGGI_POS_TOP"							, array("@var The feature is at the top edge."))
		->addConstant("PGGI_POS_BOTTOM"							, array("@var The feature is at the bottom edge."))
		->addConstant("PGGI_RELIEF_NORMAL"						, array("@var Draw a normal relief."))
		->addConstant("PGGI_RELIEF_NONE"						, array("@var No relief"))
		->addConstant("PGGI_SCROLL_STEPS"						, array("@var Scroll in steps."))
		->addConstant("PGGI_SCROLL_PAGES"						, array("@var Scroll by pages."))
		->addConstant("PGGI_SCROLL_ENDS"						, array("@var Scroll to ends."))
		->addConstant("PGGI_SCROLL_HORIZONTAL_STEPS"			, array("@var Scroll in horizontal steps."))
		->addConstant("PGGI_SCROLL_HORIZONTAL_PAGES"			, array("@var Scroll by horizontal pages."))
		->addConstant("PGGI_SCROLL_HORIZONTAL_ENDS"				, array("@var Scroll to the horizontal ends."))
		->addConstant("PGGI_SCROLL_NONE"						, array("@var No scrolling."))
		->addConstant("PGGI_SCROLL_JUMP"						, array("@var Jump to new location."))
		->addConstant("PGGI_SCROLL_STEP_BACKWARD"				, array("@var Step backward."))
		->addConstant("PGGI_SCROLL_STEP_FORWARD"				, array("@var Step forward."))
		->addConstant("PGGI_SCROLL_PAGE_BACKWARD"				, array("@var Page backward."))
		->addConstant("PGGI_SCROLL_PAGE_FORWARD"				, array("@var Page forward."))
		->addConstant("PGGI_SCROLL_STEP_UP"						, array("@var Step up."))
		->addConstant("PGGI_SCROLL_STEP_DOWN"					, array("@var Step down."))
		->addConstant("PGGI_SCROLL_PAGE_UP"						, array("@var Page up."))
		->addConstant("PGGI_SCROLL_PAGE_DOWN"					, array("@var Page down."))
		->addConstant("PGGI_SCROLL_STEP_LEFT"					, array("@var Step to the left."))
		->addConstant("PGGI_SCROLL_STEP_RIGHT"					, array("@var Step to the right."))
		->addConstant("PGGI_SCROLL_PAGE_LEFT"					, array("@var Page to the left."))
		->addConstant("PGGI_SCROLL_PAGE_RIGHT"					, array("@var Page to the right."))
		->addConstant("PGGI_SCROLL_START"						, array("@var Scroll to start."))
		->addConstant("PGGI_SCROLL_END"							, array("@var Scroll to end."))
		->addConstant("PGGI_SELECTION_NONE"						, array("@var No selection is possible."))
		->addConstant("PGGI_SELECTION_SINGLE"					, array("@var Zero or one element may be selected."))
		->addConstant("PGGI_SELECTION_BROWSE"					, array("@var Exactly one element is selected. In some circumstances, such as initially or during a search operation, it’s possible for no element to be selected with PGGI_SELECTION_BROWSE. What is really enforced is that the user can’t deselect a currently selected element except by selecting another element."))
		->addConstant("PGGI_SELECTION_MULTIPLE"					, array("@var Any number of elements may be selected. The Ctrl key may be used to enlarge the selection, and Shift key to select between the focus and the child pointed to. Some widgets may also allow Click-drag to select a range of elements.
	 "))
		->addConstant("PGGI_SHADOW_NONE"						, array("@var No outline."))
		->addConstant("PGGI_SHADOW_IN"							, array("@var The outline is bevelled inwards."))
		->addConstant("PGGI_SHADOW_OUT"							, array("@var The outline is bevelled outwards like a button."))
		->addConstant("PGGI_SHADOW_ETCHED_IN"					, array("@var The outline has a sunken 3d appearance."))
		->addConstant("PGGI_SHADOW_ETCHED_OUT"					, array("@var The outline has a raised 3d appearance."))
		->addConstant("PGGI_STATE_FLAG_NORMAL"					, array("@var State during normal operation."))
		->addConstant("PGGI_STATE_FLAG_ACTIVE"					, array("@var Widget is active."))
		->addConstant("PGGI_STATE_FLAG_PRELIGHT"				, array("@var Widget has a mouse pointer over it."))
		->addConstant("PGGI_STATE_FLAG_SELECTED"				, array("@var Widget is selected."))
		->addConstant("PGGI_STATE_FLAG_INSENSITIVE"				, array("@var Widget is insensitive."))
		->addConstant("PGGI_STATE_FLAG_INCONSISTENT"			, array("@var Widget is inconsistent."))
		->addConstant("PGGI_STATE_FLAG_FOCUSED"					, array("@var Widget has the keyboard focus."))
		->addConstant("PGGI_STATE_FLAG_BACKDROP"				, array("@var Widget is in a background toplevel window."))
		->addConstant("PGGI_STATE_FLAG_DIR_LTR"					, array("@var Widget is in left-to-right text direction. Since 3.8"))
		->addConstant("PGGI_STATE_FLAG_DIR_RTL"					, array("@var Widget is in right-to-left text direction. Since 3.8"))
		->addConstant("PGGI_STATE_FLAG_LINK"					, array("@var Widget is a link. Since 3.12"))
		->addConstant("PGGI_STATE_FLAG_VISITED"					, array("@var The location the widget points to has already been visited. Since 3.12"))
		->addConstant("PGGI_STATE_FLAG_CHECKED"					, array("@var Widget is checked. Since 3.14"))
		->addConstant("PGGI_TOOLBAR_ICONS"						, array("@var Buttons display only icons in the toolbar."))
		->addConstant("PGGI_TOOLBAR_TEXT"						, array("@var Buttons display only text labels in the toolbar."))
		->addConstant("PGGI_TOOLBAR_BOTH"						, array("@var Buttons display text and icons in the toolbar."))
		->addConstant("PGGI_TOOLBAR_BOTH_HORIZ"					, array("@var Buttons display icons and text alongside each other, rather than vertically stacked"))
		->addConstant("PGGI_SORT_ASCENDING"						, array("@var Sorting is in ascending order."))
		->addConstant("PGGI_SORT_DESCENDING"					, array("@var Sorting is in descending order."))
		->addConstant("PGGI_SIGNAL_GAPPLICATION_WINDOW_ADDED"	, array("@var "))
		->addConstant("PGGI_SIGNAL_GAPPLICATION_WINDOW_REMOVED"	, array("@var "))
		->addConstant("PGGI_SIGNAL_GAPPLICATION_STARTUP"		, array("@var "))
		->addConstant("PGGI_SIGNAL_GAPPLICATION_SHUTDOWN"		, array("@var "))
		->addConstant("PGGI_SIGNAL_GAPPLICATION_ACTIVATE"		, array("@var "))
		->addConstant("PGGI_SIGNAL_GCONTAINER_ADD"				, array("@var "))
		->addConstant("PGGI_SIGNAL_GMENUITEM_ACTIVATE"			, array("@var "))
		->addConstant("PGGI_SIGNAL_GCOMBO_BOX_CHANGED"			, array("@var "))
		->addConstant("PGGI_SIGNAL_GCOMBO_BOX_MOVE_ACTIVE"		, array("@var "))
		->addConstant("PGGI_SIGNAL_GTEXT_BUFFER_CHANGED"		, array("@var "))
		->addConstant("PGGI_SIGNAL_GDIALOG_RESPONSE"			, array("@var "))
		->addConstant("PGGI_SIGNAL_GDIALOG_CLOSE"				, array("@var "));

$PGGIDoc->addClass("GAboutDialog"	, $GAboutDialogDoc	->getData())
		->addClass("GApplication"	, $GApplicationDoc	->getData())
		->addClass("GBox"			, $GBoxDoc			->getData())
		->addClass("GComboBox"		, $GComboBoxDoc		->getData())
		->addClass("GComboBoxText"	, $GComboBoxTextDoc	->getData())
		->addClass("GContainer"		, $GContainerDoc	->getData())
		->addClass("GDialog"		, $GDialogDoc		->getData())
		->addClass("GImage"			, $GImageDoc		->getData())
		->addClass("GLabel"			, $GLabelDoc		->getData())
		->addClass("GMenu"			, $GMenuDoc			->getData())
		->addClass("GMenubar"		, $GMenubarDoc		->getData())
		->addClass("GMenuItem"		, $GMenuItemDoc		->getData())
		->addClass("GMenuShell"		, $GMenuShellDoc	->getData())
		->addClass("GScrollWindow"	, $GScrollWindowDoc	->getData())
		->addClass("GTextBuffer"	, $GTextBufferDoc	->getData())
		->addClass("GWidget"		, $GWidgetDoc		->getData())
		->addClass("GWindow"		, $GWindowDoc		->getData())
		->addClass("PGGIException"	, $PGGIExceptionDoc	->getData());
$PGGIDocumentationDocFile = $PGGIDoc->getData();

