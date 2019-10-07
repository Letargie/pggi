# Changelog
All notable changes to this project will be documented in this file.

For now as the extension in its early development phase, the api will change a lot.

## [Unreleased] 0.5.0
## 0.4.0
### Added
	-new classes
		-GFixed
	-clicked event for GButton
	-type conversion when setting object properties
### Bugfixed
	-gwindow_init implicit-function-declaration (issue #7)
	-src/display/gspinner.c:69:2: warning: missing sentinel in functio… (issue #8)
	-duplicated macro name (issue #9)
	-src/gdk/gpixbuf.c:120:3: error: format not a string literal and … (issue #11)
	-write_function usage for php 7.4
## 0.3.0
* Mon May 28 2017 Valentin Collet <valentin@famillecollet.com> 0.3.0
### Added
	-mouse event handling
	-new classes
		-GToolButton
		-GDK\GWindow
		-GDK\GPixbuf
	-changing the file rights

## 0.2.0
* Sat November 11 2017 Valentin Collet <valentin@famillecollet.com> 0.2.0
### Added
	-css handling
	-key event handling
	-Pango
	-Cairo
	-new widgets
	-new examples
	-new classes
		- GTreeSelection
		- GEntry
		- GEntryBuffer
		- GEvent
		- GEventKey
		- GScreen
		- GCssProvider
		- GStyleContext
		- GMessageDialog
		- GSpinner
		- Pango\Context
		- Pango\FontDescription
		- Pango\Layout
		- Cairo\Context
		- Cairo\Surface
		- Cairo\ImageSurface
		- Cairo\Pattern
		- Cairo\PatternSolid
		- Cairo\PatternSurface
###Bugfixed
	- Fixing -Wall warnings
	- Multiple bug fix for properties usage
	- GTreeIter functions not working
	- segfault in 'on' function when passing a closure as a callback in
		- GApplication
		- GWidgets
		- GTextBuffer
		- GTreeViewColumn
	- wrong parameter in GFileChooserDialog
	- wrong signal used in GTreeViewColumn
	- probably an handful of other bugs
## [0.1.0]
* Tue September 26 2017 Valentin Collet <valentin@famillecollet.com> 0.1.0
### Added
	-new classes
		-GApplication
		-GWidget
		-GContainer
		-GBox
		-GButtonBox
		-GComboBox
		-GGrid
		-GWindow
		-GScrollWindow
		-GDialog
		-GAboutDialog
		-GFileChooserDialog
		-GImage
		-GLabel
		-GMenu
		-GMenuBar
		-GMenuItem
		-GTextView
		-GTextBuffer
		-GTextIter
		-GTreeView
		-GTreeStore
		-GTreeIter
		-GTreeViewColumn
		-GCellRenderer
		-GCellRendererText
		
