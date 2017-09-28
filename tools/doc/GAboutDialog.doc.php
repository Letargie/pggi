<?php
/**
 * pggi-API v1.0 Docs build by Gasped [2017-08-12]
 * @package pggi
 */

include_once("../Gasped/DocArray.class.php");

$GAboutDialogDoc = new ClassDocArray();
$GAboutDialogDoc->setHeader(array("Display information about an application"))
				->addConstant("GABOUT_DIALOG_LICENSE_UNKNOWN"		, array("@var No license specified"))
				->addConstant("GABOUT_DIALOG_LICENSE_CUSTOM"		, array("@var A license text is going to be specified by the developer"))
				->addConstant("GABOUT_DIALOG_LICENSE_GPL_2_0"		, array("@var The GNU General Public License, version 2.0 or later"))
				->addConstant("GABOUT_DIALOG_LICENSE_GPL_3_0"		, array("@var The GNU General Public License, version 3.0 or later"))
				->addConstant("GABOUT_DIALOG_LICENSE_LGPL_2_1"		, array("@var The GNU Lesser General Public License, version 2.1 or later"))
				->addConstant("GABOUT_DIALOG_LICENSE_LGPL_3_0"		, array("@var The GNU Lesser General Public License, version 3.0 or later"))
				->addConstant("GABOUT_DIALOG_LICENSE_BSD"			, array("@var The BSD standard license"))
				->addConstant("GABOUT_DIALOG_LICENSE_MIT_X11"		, array("@var The MIT/X11 standard license"))
				->addConstant("GABOUT_DIALOG_LICENSE_ARTISTIC"		, array("@var The Artistic License, version 2.0"))
				->addConstant("GABOUT_DIALOG_LICENSE_GPL_2_0_ONLY"	, array("@var The GNU General Public License, version 2.0 only. Since 3.12."))
				->addConstant("GABOUT_DIALOG_LICENSE_GPL_3_0_ONLY"	, array("@var The GNU General Public License, version 3.0 only. Since 3.12."))
				->addConstant("GABOUT_DIALOG_LICENSE_LGPL_2_1_ONLY"	, array("@var The GNU Lesser General Public License, version 2.1 only. Since 3.12."))
				->addConstant("GABOUT_DIALOG_LICENSE_LGPL_3_0_ONLY"	, array("@var The GNU Lesser General Public License, version 3.0 only. Since 3.12."))
				->addProperty("programName"							, array("@var The name of the program. If this is not set, it defaults to the name of the application"))
				->addProperty("version"								, array("@var The version of the program."))
				->addProperty("copyright"							, array("@var Copyright information for the program."))
				->addProperty("comments"							, array("@var Comments about the program. This string is displayed in a label in the main dialog, thus it should be a short explanation of the main purpose of the program, not a detailed list of features."))
				->addProperty("license"								, array("@var The license of the program. This string is displayed in a text view in a secondary dialog, therefore it is fine to use a long multi-paragraph text. Note that the text is only wrapped in the text view if the 'wrapLicense' property is set to TRUE; otherwise the text itself must contain the intended linebreaks. When setting this property to a non-NULL value, the 'licenseType' property is set to GABOUT_DIALOG_LICENSE_CUSTOM as a side effect."))
				->addProperty("wrapLicense"							, array("@var Whether to wrap the text in the license dialog."))
				->addProperty("licenseType"							, array("@var The license of the program, as a value of GABOUT_DIALOG_LICENSE_X. The GAboutDialog will automatically fill out a standard disclaimer and link the user to the appropriate online resource for the license text. If GABOUT_DIALOG_LICENSE_UNKNOWN is used, the link used will be the same specified in the 'website' property. If GABOUT_DIALOG_LICENSE_CUSTOM is used, the current contents of the 'license' property are used. For any other GABOUT_DIALOG_LICENSE_X value, the contents of the 'license' property are also set by this property as a side effect."))
				->addProperty("website"								, array("@var The URL for the link to the website of the program. This should be a string starting with 'http://."))
				->addProperty("websiteLabel"						, array("@var The label for the link to the website of the program."))
				->addProperty("translatorCredits"					, array("@var Credits to the translators. This string should be marked as translatable. The string may contain email addresses and URLs, which will be displayed as links, see the introduction for more details."))
				->addProperty("logoIconName"						, array("@var A named icon to use as the logo for the about box. This property overrides the 'logo' property."))
				->addMethod  ("__construct"							, array("Creates a new GAboutDialog."));

