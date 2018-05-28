When you begin to learn a programming language, the first program you often write is a 'hello world' program. So, just to fit in with everyone else, the first tutorial in this manual just happens to be a 'hello world' tutorial!

Throughout the tutorials we expect a reasonable grasp of PHP itself. The tutorials are designed to give the user an idea of how to use PGGI, and the ideas and techniques behind it.

In this tutorial we will create a simple window with the text "Hello World!" in it.

We will start by listing the program and will then explain each line of the program, giving an overview of a very basic PGGI application.

## Example A simple Hello World script

```
<?php
namespace PGGI;
function activate($app, $data){
	$label = new GLabel("Just wanted to say\n'Hello World!'");
	$win = new GWindow($app, "Hello World !");
	$win->add($label);
	$win->showAll();
}

$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, "PGGI\\activate");
$a->run();

```

### GApplication
```
$a = new GApplication();
$a->on(SIGNAL_GAPPLICATION_ACTIVATE, "PGGI\\activate");
$a->run();
```

The first class you will have use is GApplication. This class take care of running the main loop of the application.
The run() function launch the application and start the main loop. At this moment a 'activate' signal will be sent.
In order to fill the application, you have to create a callback function that will be linked to the activate signal handler.
The on function link a Signal with a callback function.


