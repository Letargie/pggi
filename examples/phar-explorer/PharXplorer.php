<?php


// For now this example is in construction, I'm commiting it for testing purpose

namespace PGGI;
chdir(__DIR__);

// utils
function AddMenuItem($menu, $name = NULL, $callback = NULL, $param = NULL){
	$item = new GMenuItem($name);
	if($callback)
		$item->on(SIGNAL_GMENUITEM_ACTIVATE, $callback, $param);
	$menu->add($item);
}

class PharReader{
	
	private $tree,
            $treeRoot,
            $treeCurrent,
			$window,
			$phar,
			$treeSelection,
			$app, $column, $cell;


	function __construct($app){
		$this->app = $app;
		$treeCurrent = $treeRoot = null;
		$contentbox = new GBox(ORIENTATION_HORIZONTAL, 5);

		$bar = new GMenuBar();

		$fileMenu = new GMenu();
		AddMenuItem($fileMenu, "load", "PGGI\\menu_load", $this);
		AddMenuItem($fileMenu, "quit", function($item, $data){
			$data->quit();
		}, $app);

		$item = new GMenuItem("File");
		$item->submenu = $fileMenu;
		$bar->add($item);

		$editMenu = new GMenu();
		AddMenuItem($editMenu, "extract"    , "PGGI\\menu_extract"    , $this);
		AddMenuItem($editMenu, "extract all", "PGGI\\menu_extract_all", $this);
		AddMenuItem($editMenu, "add dir"    , "PGGI\\menu_add_dir"    , $this);
		AddMenuItem($editMenu, "add file"   , "PGGI\\menu_file"       , $this);
		AddMenuItem($editMenu, "delete"     , "PGGI\\menu_delete"     , $this);
		AddMenuItem($editMenu, "edit"       , "PGGI\\menu_edit"       , $this);
		AddMenuItem($editMenu, "reload"     , "PGGI\\menu_reload"     , $this);

		$item = new GMenuItem("Edit");
		$item->submenu = $editMenu;
		$bar->add($item);

		$helpMenu = new GMenu();
		AddMenuItem($helpMenu, "about", "PGGI\\menu_about", $this);

		$item = new GMenuItem("Help");
		$item->submenu = $helpMenu;
		$bar->add($item);

		$this->tree = new GTreeStore(1);
		$this->view = new GTreeView();
		$this->treeSelection = $this->view->getSelection();
		$this->treeSelection->mode = SELECTION_MULTIPLE;

		$this->column = new GTreeViewColumn();
		$this->column->title = "Files";
		$this->cell = new GCellRendererText();

		$this->column->addCellRendererText($this->cell, 0);

		$this->view->appendColumn($this->column);
		$this->view->model = $this->tree;
		$scroll = new GScrollWindow();
		$scroll->add($this->view);
		$contentbox->packStart($scroll, true, true);

		$box = new GBox(ORIENTATION_VERTICAL);
		$box->packStart($bar);
		$box->packStart($contentbox, true, true);
		$box->marginStart = $box->marginTop = $box->marginBottom = $box->marginEnd = 10;
		$this->window = new GWindow($app, "PHP Phar explorer");
		$this->window->setDefaultSize(1000, 700);
		$this->window->add($box);
		$this->window->showAll();
	}

	function loadFile($name){
		$file = fopen($name, "r");
    	if(!$file)
			return;
		try{
			$this->pharFileName = $name;
			$this->phar = new \Phar($name);
		}catch(Exception $e){
			echo("error loading phar :\n $e \n");
		}
		$this->listFiles();
	}

	function listFiles(){
		$this->tree->clear();
		if(!isset($this->phar))
			return;
		foreach($this->phar as $file){
			$iter = $this->tree->appendRow();
			$iter->setValue(0, $file->getFilename());
			if($file->isDir()){
			    $this->listFolderContent($iter, $this->phar);
			}
		}
	}
	

	public function listFolderContent($iter, $phar){
        $directoryinterator = $phar->getChildren();
        foreach($directoryinterator as $file){
            /* @var $file PharFileInfo */
			$iter = $this->tree->appendRow($iter);
			$iter->setValue(0, $file->getFilename());
            if($file->isDir()){
                $this->ListFolderContent($iter, $directoryinterator);
            }
        }
    }

	function extractSelected($path){
		$dialog = new GFileChooserDialog(GFileChooserDialog::ACTION_SELECT_FOLDER, "Add directory", $this->window, "Cancel", GDialog::RESPONSE_TYPE_CANCEL, "Choose", GDialog::RESPONSE_TYPE_ACCEPT);
		$res = $dialog->run();
		if ($res == GDialog::RESPONSE_TYPE_ACCEPT){
			$this->treeSelection->forEach("PGGI\\foreach_extract", array($this->phar, $dialog->currentFolder));
			$this->listFiles();
		}
	}

	function deleteSelected(){
		$this->treeSelection->forEach("PGGI\\foreach_delete", array($this->phar));
		$this->listFiles();
	}

	function editSelected(){
		$this->treeSelection->forEach("PGGI\\foreach_edit", array($this->pharFileName, $this->app));
	}

	function extractAll($path){
		$dialog = new GFileChooserDialog(GFileChooserDialog::ACTION_SELECT_FOLDER, "Add directory", $this->window, "Cancel", GDialog::RESPONSE_TYPE_CANCEL, "Choose", GDialog::RESPONSE_TYPE_ACCEPT);
		$res = $dialog->run();
		if ($res == GDialog::RESPONSE_TYPE_ACCEPT){
			$this->phar->extractTo($dialog->currentFolder, null, true);
		}
	}

	function launchFileChooser($name, $action){
		$dialog = new GFileChooserDialog($action, $name, $this->window, "Cancel", GDialog::RESPONSE_TYPE_CANCEL, "Choose", GDialog::RESPONSE_TYPE_ACCEPT);
		$res = $dialog->run();
		if ($res == GDialog::RESPONSE_TYPE_ACCEPT){
			$this->loadFile($dialog->fileName);
		}
	}

	function addDirectory(){
		/*$dialog = new GFileChooserDialog(GFileChooserDialog::ACTION_CREATE_FOLDER, "Add directory", $this->window, "Cancel", GDialog::RESPONSE_TYPE_CANCEL, "Create", GDialog::RESPONSE_TYPE_ACCEPT);
		$res = $dialog->run();
		if ($res == GDialog::RESPONSE_TYPE_ACCEPT){
			$this->phar->addEmptyDir($dialog->fileName);
			var_dump($dialog);
		}
		$this->listFiles();*/
		echo("for now the add Directory function isn't working\n");
	}

	function addFile(){
		/*$dialog = new GFileChooserDialog(GFileChooserDialog::ACTION_OPEN, "Add File", $this->window, "Cancel", GDialog::RESPONSE_TYPE_CANCEL, "Choose", GDialog::RESPONSE_TYPE_ACCEPT);
		$res = $dialog->run();
		if ($res == GDialog::RESPONSE_TYPE_ACCEPT){
			var_dump($dialog->fileName);
			$this->treeSelection->forEach("PGGI\\foreach_addFile", array($this->phar, $name));
		}
		$this->listFiles();*/
		echo("for now the add File function isn't working\n");
	}
}

function foreach_extract($iter, $data){
	try{
		$name = "";
		$parent = $iter->getParent();
		while($parent != false){
			$name = $parent->getValue(0)."/".$name;
			$parent = $parent->getParent();
		}
		$name .= $iter->getValue(0);
		$data[0]->extractTo($data[1], $name, true);
	}catch(Exception $e){
		echo("error in extracting : \n $e \n");
	}
}


function foreach_addFile($iter, $data){
	$name = "";
	$parent = $iter->getParent();
	while($parent != false){
		$name = $parent->getValue(0)."/".$name;
		$parent = $parent->getParent();
	}
	$data[0]->addFile($data[1], $name);
}

function foreach_edit($iter, $data){
	$name = "";
	$parent = $iter->getParent();
	while($parent != false){
		$name = $parent->getValue(0)."/".$name;
		$parent = $parent->getParent();
	}
	$name = "phar://".$data[0]."/".$name.$iter->getValue(0);
	var_dump($name);
	$win = new GWindow($data[1], $iter->getValue(0));
	$scroll = new GScrollWindow();
	$buffer = new GTextBuffer();
	try{
		$buffer->text = file_get_contents($name);
	}catch(Exception $e){
		echo("couldn't open the file\n $e\n");
	}
	$scroll->add(new GTextView($buffer));
	$box = new GBox(1, 2);
	$box->packEnd($scroll, true, true);
	$win->add($box);
	$win->showAll();
}


function foreach_delete($iter, $data){
	$name = "";
	$parent = $iter->getParent();
	while($parent != false){
		$name = $parent->getValue(0)."/".$name;
		$parent = $parent->getParent();
	}
	$name .= $iter->getValue(0);
	$data[0]->delete($name);
}


function menu_about($item, $data){
	$data->about_dialog = new GAboutDialog();
	$data->about_dialog->programName = "php phar explorer";
	$data->about_dialog->copyright = "Valentin Collet - 2017";
	$data->about_dialog->licenseType = GAboutDialog::LICENSE_BSD;
	$data->about_dialog->version = "1.0";
	$data->about_dialog->comments = "phar explorer, for all question please send me an email at <valentin@famillecollet.com> .";
	$data->about_dialog->translatorCredits = "Valentin Collet";
	$data->about_dialog->logoIconName = "logo icon name";
	$data->about_dialog->website = "https://github.com/Letargie/pggi";
	$data->about_dialog->websiteLabel = "sources sur github";
	$data->about_dialog->run();
}

function menu_load($item, $data){
	$data->launchFileChooser("Open File", GFileChooserDialog::ACTION_OPEN);
}

function menu_extract($item, $data){
	// have to decide of the extraction path, maybe a parameter?
	$data->extractSelected("extractionPath");
}

function menu_extract_all($item, $data){
	$data->extractAll("extractionPath");
}

function menu_add_dir($item, $data){
	$data->addDirectory();
}

function menu_reload($item, $data){
	$data->listFiles();
}

function menu_file($item, $data){
	$data->addFile();
}

function menu_edit($item, $data){
	$data->editSelected();
}

function menu_delete($item, $data){
	$data->deleteSelected();
}



//#!/usr/bin/php
/*
$load_parameters = "";

//Try to load required libraries
if(!extension_loaded('pggi'))
    $load_parameters .= "-d extension=pggi." . PHP_SHLIB_SUFFIX . " ";

if(!extension_loaded('phar'))
    $load_parameters .= "-d extension=phar." . PHP_SHLIB_SUFFIX . " ";

if(ini_get("phar.readonly") == 1)
    $load_parameters .= "-d phar.readonly=0 ";

// If not on windows and phar.readonly is set to 1 we reload PHP CLI with
// phar.readonly set to 0 so writing and creating phar files is possible.
if(stripos(PHP_OS, "win") === false && $load_parameters != ""){
    shell_exec($_SERVER["_"] . " $load_parameters " . __FILE__ . " > /dev/null &");
    exit;
}*/
/*
if(ini_get("phar.readonly") == 1){
	shell_exec(" $load_parameters " . __FILE__ . " > /dev/null &");
    exit;
}*/

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, function($app, $data){
	$app->reader = new PharReader($app);
});
$a->run();





