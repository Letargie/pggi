<?php
namespace PGGI;
chdir(__DIR__);

// utils
function AddMenuItem($menu, $name = NULL, $callback = NULL, $param = NULL){
	$item = new GMenuItem($name);
	if($callback)
		$item->on(SIGNAL_GMENUITEM_ACTIVATE, $callback, $param);
	$menu->add($item);
}

class ReflectionReader{

	public $extensionsList;
	public $currentExtension;

	public $currentClass;

	public $currentSelectedBox;
	public $no_signal = false;

	public $extensions, $classes, $functions, $constants;

	public $text;

	public $function_label, $method_label;

	public $about_dialog;

	function __construct($app){
		
		$this->extensionsList = get_loaded_extensions();
		asort($this->extensionsList, SORT_STRING | SORT_FLAG_CASE);

		$this->extensions= new GComboBoxText();
		$this->classes	= new GComboBoxText();
		$this->functions	= new GComboBoxText();
		$this->constants	= new GComboBoxText();

		$this->fillExtension($this->extensionsList);
		$this->extensions->on(SIGNAL_GCOMBO_BOX_CHANGED, "PGGI\\on_extension_changed", $this);
		
		$this->classes->on(SIGNAL_GCOMBO_BOX_CHANGED, "PGGI\\on_class_changed", $this);
		$this->functions->on(SIGNAL_GCOMBO_BOX_CHANGED, "PGGI\\on_function_changed", $this);
		$this->constants->on(SIGNAL_GCOMBO_BOX_CHANGED, "PGGI\\on_constant_changed", $this);

		$choosebox = new GBox(ORIENTATION_VERTICAL, 20);
		$choosebox->packStart(new GLabel("Extensions :"));
		$choosebox->packStart($this->extensions);
		$choosebox->packStart(new GLabel("Classes :"));
		$choosebox->packStart($this->classes);
		$this->function_label = new GLabel("Functions :");
		$this->method_label = new GLabel("Methods :");
		$choosebox->packStart($this->function_label);
		$choosebox->packStart($this->method_label);
		$choosebox->packStart($this->functions);
		$choosebox->packStart(new GLabel("Constants :"));
		$choosebox->packStart($this->constants);
	
		$this->text = new GTextBuffer();
		$this->text->text = phpversion();
		$view = new GTextView($this->text);
		$scroll = new GScrollWindow();
		$scroll->add($view);
		$textbox = new GBox(ORIENTATION_VERTICAL);
		$textbox->packEnd($scroll, true, true);

		$contentbox = new GBox(ORIENTATION_HORIZONTAL, 5);
		$contentbox->packStart($choosebox);
		$contentbox->packStart($textbox, true, true);

		$bar = new GMenuBar();

		$fileMenu = new GMenu();
		AddMenuItem($fileMenu, "quit", "PGGI\\menu_quit", $app);

		$item = new GMenuItem("File");
		$item->submenu = $fileMenu;
		$bar->add($item);

		$helpMenu = new GMenu();
		AddMenuItem($helpMenu, "about", "PGGI\\menu_about", $this);

		$item = new GMenuItem("Help");
		$item->submenu = $helpMenu;
		$bar->add($item);


		$box = new GBox(ORIENTATION_VERTICAL);
		$box->packStart($bar);
		$box->packStart($contentbox);
		$box->marginStart = $box->marginTop = $box->marginBottom = $box->marginEnd = 10;
		$w = new GWindow($app, "PHP Reflection Reader");
		$w->setDefaultSize(1000, 700);
		$w->add($box);
		$w->showAll();
		$this->method_label->hide();
	}

	function fillExtension($list){
		$this->currentSelectedBox = $this->extensions;

		if(count($list) < 1)
			return;

		foreach($list as $data)
			$this->extensions->append($data);
	}

	function refillClasses(){
		$this->no_signal = true;
		$this->classes->removeAll();
		if(!count($this->currentExtension->getClassNames())){
			$this->classes->sensitive = false;
			$this->classes->append("none");
			$this->classes->active = 0;
		}else{
			$this->classes->sensitive = true;
			foreach($this->currentExtension->getClassNames() as $name)
				$this->classes->append($name);
			$this->classes->prepend("global");
			$this->classes->active = 0;
		}
		$this->no_signal = false;
	}

	function refillFunctions(){
		$this->no_signal = true;
		$this->functions->removeAll();

		$data;
		if($this->currentClass != NULL)
			$data = $this->currentClass->getMethods();
		else
			$data = $this->currentExtension->getFunctions();

		if(!count($data)){
			$this->functions->sensitive = false;
			$this->functions->append("none");
			$this->functions->active = 0;
		}else{
			$this->functions->sensitive = true;
			foreach($data as $function)
				$this->functions->append($function->getName());
		}
		$this->no_signal = false;
	}

	function refillConstants(){
		$this->no_signal = true;
		$this->constants->removeAll();
		
		$data;
		if($this->currentClass != NULL)
			$data = $this->currentClass->getConstants();
		else
			$data = $this->currentExtension->getConstants();
		if(!count($data)){
			$this->constants->sensitive = false;
			$this->constants->append("none");
			$this->constants->active = 0;
		}else{
			$this->constants->sensitive = true;
			foreach($data as $const => $val)
				$this->constants->append($const);
		}
		$this->no_signal = false;
	}

	function displayExtensionContent(){
		$this->text->text = phpversion($this->currentSelectedBox->getActiveText());
		$this->function_label->show();
		$this->method_label->hide();
	}

	function displayClassContent(){
		if($this->currentSelectedBox->getActiveText() == "global"){
			$this->currentClass = NULL;
			$this->displayExtensionContent();
		}else{
			$this->function_label->hide();
			$this->method_label->show();
			$this->currentClass = new \ReflectionClass($this->currentSelectedBox->getActiveText());
			$this->text->text = $this->currentClass->__toString();
		}
	}

	function displayFunctionContent(){
		$ref;
		if($this->currentClass != NULL)
			$ref = $this->currentClass->getMethod($this->currentSelectedBox->getActiveText());
		else
			$ref = new \ReflectionFunction($this->currentSelectedBox->getActiveText());
		$this->text->text = $ref->__toString();
	}

	function displayConstantContent(){
		if($this->currentClass != NULL){
			$ref = $this->currentClass->getConstant($this->currentSelectedBox->getActiveText());
			$this->text->text = var_export($ref, true);
		}else
			foreach($this->currentExtension->getConstants() as $const => $val)
				if($const == $this->currentSelectedBox->getActiveText())
					$this->text->text = var_export($val, true);
	}
}

function menu_about($item, $data){
	$data->about_dialog = new GAboutDialog();
	$data->about_dialog->programName = "php reflection reader";
	$data->about_dialog->copyright = "Valentin Collet - 2017";
	$data->about_dialog->licenseType = GAboutDialog::LICENSE_BSD;
	$data->about_dialog->version = "1.0";
	$data->about_dialog->comments = "small reflection reader application, for all question please send me an email at <valentin@famillecollet.com> .";
	$data->about_dialog->translatorCredits = "Valentin Collet";
	$data->about_dialog->logoIconName = "logo icon name";
	$data->about_dialog->website = "https://github.com/Letargie/pggi";
	$data->about_dialog->websiteLabel = "sources sur github";
	$data->about_dialog->run();
}

function menu_quit($item, $data){
	$data->quit();
}

function on_extension_changed($box, $data){
	$data->currentExtension = new \ReflectionExtension($box->getActiveText());
	
	$data->currentSelectedBox = $data->extensions;
	$data->currentClass = NULL;

	$data->displayExtensionContent();
	$data->refillClasses();
	$data->refillFunctions();
	$data->refillConstants();
}

function on_class_changed($box, $data){
	if($data->no_signal)
		return;
	$data->currentSelectedBox = $data->classes;
	$data->displayClassContent();
	$data->refillFunctions();
	$data->refillConstants();
}

function on_function_changed($box, $data){
	if($data->no_signal)
		return;
	$data->currentSelectedBox = $data->functions;
	$data->displayFunctionContent();
	if($data->currentClass == NULL)
		$data->refillClasses();
	$data->refillConstants();
}

function on_constant_changed($box, $data){
	if($data->no_signal)
		return;
	$data->currentSelectedBox = $data->constants;
	$data->displayConstantContent();
	if($data->currentClass == NULL)
		$data->refillClasses();
	$data->refillFunctions();
}

function activate($app, $data){
	$app->reader = new ReflectionReader($app);
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE,"PGGI\\activate");
$a->run();





