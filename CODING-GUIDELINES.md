libGENEIAL - Coding Guidelines {#coding}
==============================


##1. Coding Guidelines

###1.1 Google C++ Styleguide
Any code regarding this library should follow the (Google C++ Styleguide)[http://google-styleguide.googlecode.com/svn/trunk/cppguide.html]

However, there are some derivations

###1.1 Derivations from Google C++ Styleguide

 * We fordbid the omittance of `{` and `}`, respectively. 
 * Always declare any private members before public members
 * We restrict the use of inline to functions containing less than 5 SLOC


## 1.2 A note on comments
 * We use the following syntax to indicate TODOs: 

    //TODO (abbr): Description of the TODO

   Whereas abbr is a shorthand symbol for the author of the TODO.

### 1.2.1 A note on notes
 Commentaries notes follow the TODO Pattern
    
   //NOTE (abbr): Your very important note.

   Whereas abbr is a shorthand symbol for the author of the note.
   
### 1.2.2 A note on important notes

   //IMPORTANT (abbr): Your very important note.

   Whereas abbr is a shorthand symbol for the author of the very important note.

## 1.3 A note on namespaces

 * namespaces must consist of lowercase characters only, especially namespaces and classes must not share the same name.
   e.g., forbidden

    namespace Population{ class Population {};}

    better:

    namespace population{ class Population {};}

### 1.4 A note on tabs vs. spaces
1 tab = 4 spaces, expand tabs, do not use the tab character, its from acient times where printers where still a thing. Every clever douchebag has a different customize tabwidth, there is no ocd sanity here...
each time a tab is not expanded 4 happy kittens will certainly die, do not be a mass murderer, configure your ide to protect kittens
