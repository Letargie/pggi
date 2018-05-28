## Signals

As discussed before, it is essential for any application to respond to some action that is triggered either by the user or automatically. Signals help the program in knowing when a particular action has taken place so that you can act on it.

Every widget has a set of signals associated with it. This means that, the widget is capable of emitting any of those signals depending on the action it receives. Also remember that signals are inherited across classes, and hence the child class always derives all of the signals of its parent and may add a few of its own.

An important concept to know here is that it is possible to emit a signal without the user performing an action at all. Most widgets have functions that will emit a particular signal when you use them, irrespective of whether the user has actually performed an action or not. An example of such a function is the clicked() method of GButton, which will emit the clicked signal for the button. 
