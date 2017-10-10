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

#include "pggi.h"

//gdk events

/*only work in MINIT_FUNCTION*/
#define REGISTER_CONST_LONG(      const_name,                       value) \
    zend_register_long_constant(  const_name, sizeof(const_name)-1, value, CONST_CS | CONST_PERSISTENT, module_number)
#define REGISTER_CONST_STRING(    const_name,                       value) \
    zend_register_string_constant(const_name, sizeof(const_name)-1, value, CONST_CS | CONST_PERSISTENT, module_number)

static const zend_function_entry pggi_functions[] = {
	PHP_FE_END
};

PHP_MINIT_FUNCTION(pggi){

	const char * differencies = gtk_check_version(GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
	if(differencies){
		printf("The Gtk version used in runtime is incompatible with the compiled version of the extension pggi\n");
		return SUCCESS;
	}
	/*
	if(strcmp(sapi_module.name, "cli")){
		printf("This extension should only be use in a cli context\n");
		return SUCCESS;
	}*/

	cairo_minit(module_number);

	/* Whenever a container has some form of natural row it may align children in that row along a common typographical baseline.
	* If the amount of verical space in the row is taller than the total requested height of the baseline-aligned children 
	* then it can use a GtkBaselinePosition to select where to put the baseline inside the extra availible space.
	*/
	#if GTK_MINOR_VERSION >= 10
	REGISTER_CONST_LONG("PGGI\\BASELINE_POSITION_TOP"             , GTK_BASELINE_POSITION_TOP          );
	REGISTER_CONST_LONG("PGGI\\BASELINE_POSITION_CENTER"          , GTK_BASELINE_POSITION_CENTER       );
	REGISTER_CONST_LONG("PGGI\\BASELINE_POSITION_BOTTOM"          , GTK_BASELINE_POSITION_BOTTOM       );
	#endif
	/*
	* See GEntry
	*/
	REGISTER_CONST_LONG("PGGI\\DELETE_CHARS"                      , GTK_DELETE_CHARS                   );
	REGISTER_CONST_LONG("PGGI\\DELETE_WORD_ENDS"                  , GTK_DELETE_WORD_ENDS               );
	REGISTER_CONST_LONG("PGGI\\DELETE_WORDS"                      , GTK_DELETE_WORDS                   );
	REGISTER_CONST_LONG("PGGI\\DELETE_DISPLAY_LINES"              , GTK_DELETE_DISPLAY_LINES           );
	REGISTER_CONST_LONG("PGGI\\DELETE_DISPLAY_LINE_ENDS"          , GTK_DELETE_DISPLAY_LINE_ENDS       );
	REGISTER_CONST_LONG("PGGI\\DELETE_PARAGRAPH_ENDS"             , GTK_DELETE_PARAGRAPH_ENDS          );
	REGISTER_CONST_LONG("PGGI\\DELETE_PARAGRAPHS"                 , GTK_DELETE_PARAGRAPHS              );
	REGISTER_CONST_LONG("PGGI\\DELETE_WHITESPACE"                 , GTK_DELETE_WHITESPACE              );
	/*
	* Focus movement types.
	*/
	REGISTER_CONST_LONG("PGGI\\DIR_TAB_FORWARD"                   , GTK_DIR_TAB_FORWARD                );
	REGISTER_CONST_LONG("PGGI\\DIR_TAB_BACKWARD"                  , GTK_DIR_TAB_BACKWARD               );
	REGISTER_CONST_LONG("PGGI\\DIR_UP"                            , GTK_DIR_UP                         );
	REGISTER_CONST_LONG("PGGI\\DIR_DOWN"                          , GTK_DIR_DOWN                       );
	REGISTER_CONST_LONG("PGGI\\DIR_LEFT"                          , GTK_DIR_LEFT                       );
	REGISTER_CONST_LONG("PGGI\\DIR_RIGHT"                         , GTK_DIR_RIGHT                      );
	/*
	* Used for justifying the text inside a GLabel widget.
	*/
	REGISTER_CONST_LONG("PGGI\\JUSTIFY_LEFT"                      , GTK_JUSTIFY_LEFT                   );
	REGISTER_CONST_LONG("PGGI\\JUSTIFY_RIGHT"                     , GTK_JUSTIFY_RIGHT                  );
	REGISTER_CONST_LONG("PGGI\\JUSTIFY_CENTER"                    , GTK_JUSTIFY_CENTER                 );
	REGISTER_CONST_LONG("PGGI\\JUSTIFY_FILL"                      , GTK_JUSTIFY_FILL                   );

	REGISTER_CONST_LONG("PGGI\\MOVEMENT_LOGICAL_POSITIONS"        , GTK_MOVEMENT_LOGICAL_POSITIONS     );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_VISUAL_POSITIONS"         , GTK_MOVEMENT_VISUAL_POSITIONS      );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_WORDS"                    , GTK_MOVEMENT_WORDS                 );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_DISPLAY_LINES"            , GTK_MOVEMENT_DISPLAY_LINES         );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_DISPLAY_LINE_ENDS"        , GTK_MOVEMENT_DISPLAY_LINE_ENDS     );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_PARAGRAPHS"               , GTK_MOVEMENT_PARAGRAPHS            );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_PARAGRAPH_ENDS"           , GTK_MOVEMENT_PARAGRAPH_ENDS        );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_PAGES"                    , GTK_MOVEMENT_PAGES                 );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_BUFFER_ENDS"              , GTK_MOVEMENT_BUFFER_ENDS           );
	REGISTER_CONST_LONG("PGGI\\MOVEMENT_HORIZONTAL_PAGES"         , GTK_MOVEMENT_HORIZONTAL_PAGES      );
	/*
	* Represents the orientation of widgets and other objects which can be switched between horizontal and vertical orientation on the fly
	*/
	REGISTER_CONST_LONG("PGGI\\ORIENTATION_HORIZONTAL"            , GTK_ORIENTATION_HORIZONTAL         );
	REGISTER_CONST_LONG("PGGI\\ORIENTATION_VERTICAL"              , GTK_ORIENTATION_VERTICAL           );
	/*
	* Represents the packing location GtkBox children.
	*/
	REGISTER_CONST_LONG("PGGI\\PACK_START"                        , GTK_PACK_START                     );
	REGISTER_CONST_LONG("PGGI\\PACK_END"                          , GTK_PACK_END                       );
	/*
	* Describes which edge of a widget a certain feature is positioned at.
	*/
	REGISTER_CONST_LONG("PGGI\\POS_LEFT"                          , GTK_POS_LEFT                       );
	REGISTER_CONST_LONG("PGGI\\POS_RIGHT"                         , GTK_POS_RIGHT                      );
	REGISTER_CONST_LONG("PGGI\\POS_TOP"                           , GTK_POS_TOP                        );
	REGISTER_CONST_LONG("PGGI\\POS_BOTTOM"                        , GTK_POS_BOTTOM                     );
	/*
	* Indicated the relief to be drawn around a GButton.
	*/
	REGISTER_CONST_LONG("PGGI\\RELIEF_NORMAL"                     , GTK_RELIEF_NORMAL                  );
	REGISTER_CONST_LONG("PGGI\\RELIEF_NONE"                       , GTK_RELIEF_NONE                    );

	REGISTER_CONST_LONG("PGGI\\SCROLL_STEPS"                      , GTK_SCROLL_STEPS                   );
	REGISTER_CONST_LONG("PGGI\\SCROLL_PAGES"                      , GTK_SCROLL_PAGES                   );
	REGISTER_CONST_LONG("PGGI\\SCROLL_ENDS"                       , GTK_SCROLL_ENDS                    );
	REGISTER_CONST_LONG("PGGI\\SCROLL_HORIZONTAL_STEPS"           , GTK_SCROLL_HORIZONTAL_STEPS        );
	REGISTER_CONST_LONG("PGGI\\SCROLL_HORIZONTAL_PAGES"           , GTK_SCROLL_HORIZONTAL_PAGES        );
	REGISTER_CONST_LONG("PGGI\\SCROLL_HORIZONTAL_ENDS"            , GTK_SCROLL_HORIZONTAL_ENDS         );
	/*
	* Scrolling types.
	*/
	REGISTER_CONST_LONG("PGGI\\SCROLL_NONE"                       , GTK_SCROLL_NONE                    );
	REGISTER_CONST_LONG("PGGI\\SCROLL_JUMP"                       , GTK_SCROLL_JUMP                    );
	REGISTER_CONST_LONG("PGGI\\SCROLL_STEP_BACKWARD"              , GTK_SCROLL_STEP_BACKWARD           );
	REGISTER_CONST_LONG("PGGI\\SCROLL_STEP_FORWARD"               , GTK_SCROLL_STEP_FORWARD            );
	REGISTER_CONST_LONG("PGGI\\SCROLL_PAGE_BACKWARD"              , GTK_SCROLL_PAGE_BACKWARD           );
	REGISTER_CONST_LONG("PGGI\\SCROLL_PAGE_FORWARD"               , GTK_SCROLL_PAGE_FORWARD            );
	REGISTER_CONST_LONG("PGGI\\SCROLL_STEP_UP"                    , GTK_SCROLL_STEP_UP                 );
	REGISTER_CONST_LONG("PGGI\\SCROLL_STEP_DOWN"                  , GTK_SCROLL_STEP_DOWN               );
	REGISTER_CONST_LONG("PGGI\\SCROLL_PAGE_UP"                    , GTK_SCROLL_PAGE_UP                 );
	REGISTER_CONST_LONG("PGGI\\SCROLL_PAGE_DOWN"                  , GTK_SCROLL_PAGE_DOWN               );
	REGISTER_CONST_LONG("PGGI\\SCROLL_STEP_LEFT"                  , GTK_SCROLL_STEP_LEFT               );
	REGISTER_CONST_LONG("PGGI\\SCROLL_STEP_RIGHT"                 , GTK_SCROLL_STEP_RIGHT              );
	REGISTER_CONST_LONG("PGGI\\SCROLL_PAGE_LEFT"                  , GTK_SCROLL_PAGE_LEFT               );
	REGISTER_CONST_LONG("PGGI\\SCROLL_PAGE_RIGHT"                 , GTK_SCROLL_PAGE_RIGHT              );
	REGISTER_CONST_LONG("PGGI\\SCROLL_START"                      , GTK_SCROLL_START                   );
	REGISTER_CONST_LONG("PGGI\\SCROLL_END"                        , GTK_SCROLL_END                     );
	/*
	* Used to control what selections users are allowed to make.
	*/
	REGISTER_CONST_LONG("PGGI\\SELECTION_NONE"                    , GTK_SELECTION_NONE                 );
	REGISTER_CONST_LONG("PGGI\\SELECTION_SINGLE"                  , GTK_SELECTION_SINGLE               );
	REGISTER_CONST_LONG("PGGI\\SELECTION_BROWSE"                  , GTK_SELECTION_BROWSE               );
	REGISTER_CONST_LONG("PGGI\\SELECTION_MULTIPLE"                , GTK_SELECTION_MULTIPLE             );
	/*
	* Used to change the appearance of an outline 
	*/
	REGISTER_CONST_LONG("PGGI\\SHADOW_NONE"                       , GTK_SHADOW_NONE                    );
	REGISTER_CONST_LONG("PGGI\\SHADOW_IN"                         , GTK_SHADOW_IN                      );
	REGISTER_CONST_LONG("PGGI\\SHADOW_OUT"                        , GTK_SHADOW_OUT                     );
	REGISTER_CONST_LONG("PGGI\\SHADOW_ETCHED_IN"                  , GTK_SHADOW_ETCHED_IN               );
	REGISTER_CONST_LONG("PGGI\\SHADOW_ETCHED_OUT"                 , GTK_SHADOW_ETCHED_OUT              );
	/*
	* Describes a widget state. Widget states are used to match the widget against CSS pseudo-classes. Note that GTK extends the regular CSS classes and sometimes uses different names.
	*/
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_NORMAL"                 , GTK_STATE_FLAG_NORMAL              );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_ACTIVE"                 , GTK_STATE_FLAG_ACTIVE              );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_PRELIGHT"               , GTK_STATE_FLAG_PRELIGHT            );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_SELECTED"               , GTK_STATE_FLAG_SELECTED            );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_INSENSITIVE"            , GTK_STATE_FLAG_INSENSITIVE         );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_INCONSISTENT"           , GTK_STATE_FLAG_INCONSISTENT        );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_FOCUSED"                , GTK_STATE_FLAG_FOCUSED             );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_BACKDROP"               , GTK_STATE_FLAG_BACKDROP            );
	#if GTK_MINOR_VERSION >= 8
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_DIR_LTR"                , GTK_STATE_FLAG_DIR_LTR             );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_DIR_RTL"                , GTK_STATE_FLAG_DIR_RTL             );
	#endif
	#if GTK_MINOR_VERSION >= 12
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_LINK"                   , GTK_STATE_FLAG_LINK                );
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_VISITED"                , GTK_STATE_FLAG_VISITED             );
	#endif
	#if GTK_MINOR_VERSION >= 14
	REGISTER_CONST_LONG("PGGI\\STATE_FLAG_CHECKED"                , GTK_STATE_FLAG_CHECKED             );
	#endif
	#if GTK_MINOR_VERSION >= 20
	REGISTER_CONST_LONG("PGGI\\PGGI_STATE_FLAG_DROP_ACTIVE"        , GTK_STATE_FLAG_DROP_ACTIVE        );
	#endif
	/*
	* Used to customize the appearance of a GToolbar. Note that setting the toolbar style overrides the user’s preferences for the default toolbar style.
	* Note that if the button has only a label set and PGGI_TOOLBAR_ICONS is used, the label will be visible, and vice versa.
	*/
	REGISTER_CONST_LONG("PGGI\\TOOLBAR_ICONS"                     , GTK_TOOLBAR_ICONS                  );
	REGISTER_CONST_LONG("PGGI\\TOOLBAR_TEXT"                      , GTK_TOOLBAR_TEXT                   );
	REGISTER_CONST_LONG("PGGI\\TOOLBAR_BOTH"                      , GTK_TOOLBAR_BOTH                   );
	REGISTER_CONST_LONG("PGGI\\TOOLBAR_BOTH_HORIZ"                , GTK_TOOLBAR_BOTH_HORIZ             );
	/*
	* Determines the direction of a sort.
	*/
	REGISTER_CONST_LONG("PGGI\\SORT_ASCENDING"                    , GTK_SORT_ASCENDING                 );
	REGISTER_CONST_LONG("PGGI\\SORT_DESCENDING"                   , GTK_SORT_DESCENDING                );


	REGISTER_CONST_LONG("PGGI\\SIGNAL_GWIDGET_DESTROY"            , gsignal_gwidget_destroy            );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GWIDGET_DRAW"               , gsignal_gwidget_draw               );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GWIDGET_KEY_PRESS_EVENT"    , gsignal_gwidget_key_press_event    );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GAPPLICATION_WINDOW_ADDED"  , gsignal_gapplication_window_added  );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GAPPLICATION_WINDOW_REMOVED", gsignal_gapplication_window_removed);
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GAPPLICATION_STARTUP"       , gsignal_gapplication_startup       );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GAPPLICATION_SHUTDOWN"      , gsignal_gapplication_shutdown      );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GAPPLICATION_ACTIVATE"      , gsignal_gapplication_activate      );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GCONTAINER_ADD"             , gsignal_gcontainer_add             );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GMENUITEM_ACTIVATE"         , gsignal_gmenuitem_activate         );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GCOMBO_BOX_CHANGED"         , gsignal_gcombo_box_changed         );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GCOMBO_BOX_MOVE_ACTIVE"     , gsignal_gcombo_box_move_active     );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GTEXT_BUFFER_CHANGED"       , gsignal_gtext_buffer_changed       );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_STYLE_CONTEXT_CHANGED"      , gsignal_gstyle_context_changed     );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GDIALOG_RESPONSE"           , gsignal_gdialog_response           );
	REGISTER_CONST_LONG("PGGI\\SIGNAL_GDIALOG_CLOSE"              , gsignal_gdialog_close              );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GTREE_VIEW_COLUMN_CHANGED"  , gsignal_gtree_view_column_changed  );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GTREE_VIEW_COLUMN_CLICKED"  , gsignal_gtree_view_column_clicked  );

	REGISTER_CONST_LONG("PGGI\\SIGNAL_GTREE_SELECTION_CHANGED"    , gsignal_gtree_selection_changed    );

	gexception_init          (module_number);
	gevent_init              (module_number);
	gevent_key_init          (module_number);

	rgba_init                (module_number);

	gcss_provider_init       (module_number);
	gscreen_init             (module_number);
	gstyle_context_init      (module_number);

	gapplication_init        (module_number);
	gwidget_init             (module_number);

	gcontainer_init          (module_number);
	gwindow_init             (module_number);
	gbox_init                (module_number);
	gbuttonbox_init          (module_number);

	gbutton_init             (module_number);

	glabel_init              (module_number);
	gimage_init              (module_number);

	gmenushell_init          (module_number);
	gmenubar_init            (module_number);
	gmenu_init               (module_number);
	gmenuitem_init           (module_number);

	gtext_view_init          (module_number);
	gtext_buffer_init        (module_number);

	gscroll_window_init      (module_number);

	gcombo_box_init          (module_number);
	gcombo_box_text_init     (module_number);

	gdialog_init             (module_number);
	gabout_dialog_init       (module_number);
	gmessage_dialog_init     (module_number);

	ggrid_init               (module_number);

	gtext_iter_init          (module_number);

	gtree_iter_init          (module_number);
	gtree_store_init         (module_number);

	gcell_renderer_init      (module_number);
	gcell_renderer_text_init (module_number);
	gtree_view_column_init   (module_number);
	gtree_selection_init     (module_number);
	gtree_view_init          (module_number);

	gfile_chooser_dialog_init(module_number);

	gentry_buffer_init       (module_number);
	gentry_init              (module_number);

	gdrawing_area_init       (module_number);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pggi){
	return SUCCESS;
}

PHP_MINFO_FUNCTION(pggi){
	char bufferC[128];
	char bufferL[128];
	snprintf(bufferC, 128, "%d.%d.%d", GTK_MAJOR_VERSION      , GTK_MINOR_VERSION      , GTK_MICRO_VERSION      );
	snprintf(bufferL, 128, "%d.%d.%d", gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());
	php_info_print_table_start();{
		php_info_print_table_row(2, "PGGI"                   , "enabled"    );
		php_info_print_table_row(2, "PGGI version"           , PGGI_VERSION );
		php_info_print_table_row(2, "Gtk compilation version", bufferC      );
		php_info_print_table_row(2, "Gtk runtime version"    , bufferL      );
		php_info_print_table_row(2, "Git reference"          , "$Id:  $"    );
	}
	php_info_print_table_end();
}

zend_module_entry pggi_module_entry = {
	STANDARD_MODULE_HEADER,
	"pggi",
	pggi_functions,
	PHP_MINIT(pggi),
	PHP_MSHUTDOWN(pggi),
	NULL,
	NULL,
	PHP_MINFO(pggi),
	PGGI_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PGGI
ZEND_GET_MODULE(pggi)
#endif

