Orkestra
========

An IDE for Symfony2.

Intro
-----

Hello all, I'm Vinorcola, I'm french (so maybe excuse me for the eventual spelling mistakes). I was tired to write my Symfony2 apps with geany, notepad++ or equivalent tools. I gave a try to NetBeans and Eclipse, but those IDE are just a nightmare to handle and furthermore, they are just so so so (...) slow. Damn! My PC is 5 year-old and the interface freeze at quite everything you do. No. No, I was not going to change my PC to write code. Mine is powerful enough for that, even if it is 5 year-old. So `rm -R NetBeans/ Eclipse/`.

Now, let's thinks 2 seconds. I like PHP Symfony2. I like C++ and Qt. C++ also come with the best IDE ever (OK, I've never tried Microsoft Visual Studio): QtCreator. And belive me, QtCreator does a lot of things. It can be compared to NetBeans and Eclipse, only 100 000 (more) times faster. So why don't I write an IDE for Symfony2 in C++ with Qt? Well, there you go.

Orkestra is meant to be an IDE dedicated to Symfony2. An IDE fast and easy to use / configure. My aim is to get inspired from QtCreator and do the same thing for Symfony2. Here is a list of features in my mind:

* Manage several projects at the time;
* Jump from a controller/action to the view with a single click (don't waste your time in the file hierarchie);
* The same way, jump to a repository, entity, form, etc. just by clicking on their name in the code;
* Create a service easily (an UI that change itself the configuration files in XML or YAML);
* Integrates the console manipulation into the IDE;
* Display the official doc and the API doc into the IDE (just put your mouse on a class or an object, press F1, and there you go: on the API doc page related to the class);
* git integration;
* ... some more that will come.

Tools
-----

Orkestra is developed in C++11 with Qt5 and QScintilla. The tool will be available under GPL licence. For the moment, I'm alone and the project is at the very begining. Feel free to contact me if you'r also interested in the project.
