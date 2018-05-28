<?php
/**
 * pggi-API v1.0 Docs build by Gasped [2017-08-12]
 * @package pggi
 */

include_once("../Gasped/DocArray.class.php");

$GApplicationDoc = new ClassDocArray();
$GApplicationDoc->addMethod("__construct"	, array("Creates a new GApplication instance."			))
				->addMethod("on"			, array("connect a callback to a signal the can receive",
													'@param $type long the signal'					,
													'@param $callback long the callback to connect'	,
													'@param $param optional parameters'				))
				->addMethod("quit"			, array("Immediately quits the application.",
													"Upon return to the mainloop, run will return, calling only the 'shutdown' function before doing so."))
				->addMethod("run"			,array("Runs the application starting the main loop and callong the 'activate' function"));

