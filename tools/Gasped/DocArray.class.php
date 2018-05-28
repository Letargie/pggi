<?php
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

/**
 * Class corresponding a documentation
 */
abstract class DocArray{
	/**
	 * @var array contains all the documentation
	 */
	private $data;

	/*
	 * Add documentation lines with a specific index for an element
	 * @param $element string the array where to put data
	 * @param $index string the index for new documentation lines
	 * @param $value array the new lines
	 * @return DocArray the instance of the class
	 */
	protected function addElement($element, $index, $value){
		return $this->data[$element][$index] = $value;
	}

	/**
	 * Get an array containing the documentation
	 * @return array the documentation
	 */
	public function getData(){
		return $this->data;
	}
}

/**
 * Class corresponding to the documentation for an extension
 */
class ExtensionDocArray extends DocArray{

	/**
	 * Create a new ExtensionDocArray
	 */
	public function __construct(){
		$this->data = array("constants" => array(),
					  		"functions" => array(),
					  		"classes   "=> array());
	}

	/**
	 * Add a line of documentation for a constant
	 * @param $index string the index for new documentation lines
	 * @param $value array the new lines
	 * @return ExtensionDocArray the instance of the class
	 */
	public function addConstant($index, $value){
		$this->addElement("constants", $index, $value);
		return $this;
	}

	/**
	 * Add a line of documentation for a function
	 * @param $index string the index for new documentation lines
	 * @param $value array the new lines
	 * @return ExtensionDocArray the instance of the class
	 */
	public function addFunction($index, $value){
		$this->addElement("functions", $index, $value);
		return $this;
	}

	/**
	 * Add a line of documentation for a class
	 * @param $index string the index for new documentation lines
	 * @param $value array the new lines
	 * @return ExtensionDocArray the instance of the class
	 */
	public function addClass($index, $value){
		$this->addElement("classes", $index, $value);
		return $this;
	}
}

/**
 * Class corresponding to the documentation for a class
 */
class ClassDocArray extends DocArray{
	/**
	 * Create a new ClassDocArray
	 */
	public function __construct(){
		$this->data = array("constants" => array(),
					  		"methods"   => array(),
					  		"properties"=> array(),
							"header"	=> array());
	}

	/**
	 * Add a line of documentation for a constant
	 * @param $index string the index for new documentation lines
	 * @param $value array the new lines
	 * @return ClassDocArray the instance of the class
	 */
	public function addConstant($index, $value){
		$this->addElement("constants", $index, $value);
		return $this;
	}

	/**
	 * Add a line of documentation for a method
	 * @param $index string the index for new documentation lines
	 * @param $value array the new lines
	 * @return ClassDocArray the instance of the class
	 */
	public function addMethod($index, $value){
		$this->addElement("methods", $index, $value);
		return $this;
	}

	/**
	 * Add a line of documentation for a property
	 * @param $index string the index for new documentation lines
	 * @param $value array the new lines
	 * @return ClassDocArray the instance of the class
	 */
	public function addProperty($index, $value){
		$this->addElement("properties", $index, $value);
		return $this;
	}

	/**
	 * set a line of documentation for the class itself
	 * @param $value array the new lines
	 * @return ClassDocArray the instance of the class
	 */
	public function setHeader($value){
		$this->data["header"] = $value;
		return $this;
	}
}
