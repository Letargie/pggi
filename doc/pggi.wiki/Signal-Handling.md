## Introduction

Signal handling is one of most fundamental concepts of PGGI, indeed it's what make the application interact.
The purpose of PGGI is to make event-driven applications. Every time an action is performed or when a user interacts with the application a signal is launched. This signal will inform the different component of PGGI of the event that occurred so they can react accordingly.

Signal handling basically consists of connecting a signal to its signal handler, and of course, writing the signal handler itself. The job of generating the signal whenever an action occurs is taken care by PGGI.
Although signal generation does not guarantee that the user has performed some action. It is possible to manually generate a signal, and PGGI generates signals internally all the time.

