<!---
This file uses markdown syntax, adhere when fiddling!
http://en.wikipedia.org/wiki/Markdown
-->

libGENEIAL - Coding Guidelines
===========================================


##1. Coding Guidelines

###1.1 Google C++ Styleguide
Any code regarding this library should follow the (Google C++ Styleguide)[http://google-styleguide.googlecode.com/svn/trunk/cppguide.html]

However, there are some derivations

###1.1 Derivations from Google C++ Styleguide

 * We fordbid the omittance of `{` and `}` respectively. 
 * Always declare any private members, before public members
 * We restrict the use of inline to functions containing less than 5 SLOC


### 1.2 A note on comments
 * We use the following syntax to indicate TODOs: 

    //TODO(abbr): Description of the TODO

   Whereas abbr is a shorthand symbol for the author of the TODO.

 * We advocate the same syntax as above for NOTE and IMPORTANT