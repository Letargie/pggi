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

/*
                                _ 
                               | |
  __ _  __ _ ___ _ __   ___  __| |
 / _` |/ _` / __| '_ \ / _ \/ _` |
| (_| | (_| \__ \ |_) |  __/ (_| |
 \__, |\__,_|___/ .__/ \___|\__,_|
  __/ |         | |               
 |___/          |_|               

Generate a stub for php extensions to make documentation
*/
class Gasped{

	/**
	 * Build a php doc block from an array of lines
	 * @param $indent string the indentation to use for the doc block
	 * @param $lines array the lines to use in the doc block
	 * @return string the doc block generated
	 */
	protected function generateDocBlock($indent, $lines){
		$result = $indent.'/**'."\n";
		foreach( $lines as $line ){
			$result.= $indent.' * '.$line."\n";
		}
		$result.= $indent.' */'."\n";
		return $result;
	}

	/**
	 * Build the php code of a function
	 * @param $function ReflectionFunction the function to use
	 * @return string the code php of the function
	 */
	public function buildFunction($function){
		$result = 'function '.$function->getName().'(';
		$result.= $this->buildParameters($function->getParameters());
		$result.= ') {}'.PHP_EOL;
		return $result;
	}

	/**
	 * Build the php code of a class
	 * @param $className string the name of the class
	 * @param $comments array the data of the comments
	 * @return string the code php fo the class
	 */
	public function buildClass($className, $comments) {
		$result = '';
		if( class_exists($className) ){
			$refClass = new ReflectionClass($className);
			$className = $refClass->getName();
			if( isset($comments) &&
				isset($comments["header"])){
				$docLines = array();
				foreach($comments["header"] as $comment)
					$docLines[] = $comment;
				$result.= $this->generateDocBlock("", $docLines);
			}
			$result.= 'class '.$className.' ';
			if( $refClass->getParentClass() ){
				$result.= 'extends '.$refClass->getParentClass()->getName().' ';
			}
			$result.= '{'.PHP_EOL;

			// check for constants
			foreach( $refClass->getConstants() as $name=>$value ){
				if($refClass->getParentClass() !== false && $refClass->getParentClass()->getConstant($name) !== false){
					continue;
				}
				if( isset($comments) &&
					isset($comments["constants"]) &&
					isset($comments["constants"][$name])){
					$docLines = array();
					foreach($comments["constants"][$name] as $comment)
						$docLines[] = $comment;
					$result.= $this->generateDocBlock("\t", $docLines);
				}
				$result.= "\t".'const '.$name.' = '.var_export($value, true).';'.PHP_EOL;
			}

			// check for public attributes
			foreach( $refClass->getProperties() as $property ){
				if( $property->getDeclaringClass()->getName()!=$refClass->getName() ){
					continue;
				}
				if( $property->isPublic() OR $property->isProtected() ){
					if( isset($comments) &&
						isset($comments["properties"]) &&
						isset($comments["properties"][$property->getName()])){
						$docLines = array();
						foreach($comments["properties"][$property->getName()] as $comment)
							$docLines[] = $comment;
						$result.= $this->generateDocBlock("\t", $docLines);
					}
					$result.= "\t";
					if( $property->isPublic() ){
						$result.= 'public ';
					}else{
						$result.= 'protected ';
					}
					$result.= '$'.$property->getName().';'.PHP_EOL;
				}
			}

			// check for methods
			foreach( $refClass->getMethods() as $method ){
				$method = $refClass->getMethod($method->getName());
				if( $method->getDeclaringClass()->getName()!=$refClass->getName() ){
					continue;
				}
				if( $method->isPublic() OR $method->isProtected() ){

					$refParameters = $method->getParameters();
					$result.= "\t";
					if( isset($comments["methods"]) &&
						isset($comments["methods"][$method->getName()])){
						$docLines = array();
						foreach($comments["methods"][$method->getName()] as $comment)
							$docLines[] = $comment;
						$result.= $this->generateDocBlock("", $docLines);
					}
					if( $method->isPublic() ){
						$result.= 'public ';
					}else{
						$result.= 'protected ';
					}
					$result.= $this->buildFunction($method);
				}
			}

			$result.= '}'."\n";
		}else{
			trigger_error('Gasped: class '.$className.' not found!',E_USER_ERROR);
		}
		return $result;
	}

	/**
	 * TODO
	 */
	protected function addComments(&$result, $comments, $indent){
		$docLines = array();
		foreach($comments as $comment)
			$docLines[] = $comment;
		$result.= $this->generateDocBlock($indent, $docLines);
	}

	/**
	 * Build the php code of parameters
	 * @param $parameters ReflectionParameters the parameters
	 * @param string the code php of the parameters
	 */
	protected function buildParameters($parameters){
		$result = '';
		// check parameters
		$trenn = '';
		foreach( $parameters as $param) {
			$result.= $trenn;
		    $class = $param->getClass();
		    if( !empty($class) ){
			    $result.= $class->getName().' ';
		    }
	        if($param->isPassedByReference()) {
	            $result.= '&';
	        }
		    $result.= '$'.$param->getName();
	        if($param->isOptional()) {
	            if($param->isDefaultValueAvailable()) {
		            $result.= '='.var_export($param->getDefaultValue(), true);
	            }else{
	                $result.= '=""';
                }
	        }
		    $trenn = ', ';
	    }
		return $result;
	}

	/**
	 * Build the php code of an extension and can write it in files
	 * @param $extensionName string the name of the extension
	 * @param $comments array the data of the comments to generate with the extension (or null)
	 * @param $target string the path to the target where to save the generated files (or null)
	 * @return string the generated php code
	 */
	public function buildExtension($extensionName, $comments = NULL, $target = null) {
		$result = '<?php'.PHP_EOL;
		if(extension_loaded($extensionName)){
			$refExtension = new ReflectionExtension($extensionName);
			$header = $result;
			$header.= '/**'."\n";
			$header.= ' * '.$refExtension->getName().'-API v'.$refExtension->getVersion().' Docs build by Gasped ['.date('Y-m-d').']'."\n";
			$header.= ' * @package '.$refExtension->getName()."\n".' */'."\n";
			$header.= "\n";
			$result = $header;
			// constants
			foreach( $refExtension->getConstants() as $name=>$value ){
				if( isset($comments) &&
					isset($comments["constants"]) &&
					isset($comments["constants"][$name])){
					$docLines = array();
					foreach($comments["constants"][$name] as $comment)
						$docLines[] = $comment;
					$result.=$this->generateDocBlock("", $docLines);
				}
				$result.= 'define('.$name.' , '.var_export($value, true).');'.PHP_EOL;
			}
			// functions
			foreach( $refExtension->getFunctions() as $function){
				if( isset($comments) && 
					isset($comments["functions"]) &&
					isset($comments["functions"][$function->getName()])){
					$docLines = array();
					foreach( $comments["functions"][$function->getName()]  as $comment)
						$docLines[] = $comment;
					$result.= $this->generateDocBlock("", $docLines);
				}
				$result.= $this->buildFunction($function);
			}

			if($target != NULL){
				$file = fopen("{$target}/{$extensionName}_extension.php", "w");
				if($file)
					fwrite($file, $result);
			}

			// classes
			foreach( $refExtension->getClassNames() as $className ){
				$classResult = "";
				$classResult.= $this->buildClass($className, isset($comments) &&
															 isset($comments["classes"]) &&
															 isset($comments["classes"][$className]) ? 
															 $comments["classes"][$className] :
															 NULL);
				if($target != NULL){
					$file = fopen("{$target}/{$className}.class.php", "w");
					if($file)
						fwrite($file, $header.$classResult);
				}
				$result.= $classResult;
			}
		}else{
			trigger_error('Gasped: extension '.$extensionName.' not found!',E_USER_ERROR);
		}
		return $result;
	}
}
