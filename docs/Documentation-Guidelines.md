<!---
This file uses markdown syntax, adhere when fiddling!
http://en.wikipedia.org/wiki/Markdown
-->

libGENEIAL - Documentation Guidelines
=====================================

We stipulate the use of JavaDoc Doxygen syntax as depicted in the following example from the (Doxygen Manual)[http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html#cppblock].

Note: Whilst Doxygen support other documentation syntax formats (e.g., QT style) , we strongly discourage their use in this projects!

    /**
     test class. A more elaborate class description.
     */
    class Test
    {
      public:
        /** 
         * An enum.
         * More detailed enum description.
         */
        enum TEnum { 
              TVal1, /**< enum value TVal1. */  
              TVal2, /**< enum value TVal2. */  
              TVal3  /**< enum value TVal3. */  
             } 
           *enumPtr, /**< enum pointer. Details. */
           enumVar;  /**< enum variable. Details. */
           
          /**
           * A constructor.
           * A more elaborate description of the constructor.
           */
          Test();
          /**
           * A destructor.
           * A more elaborate description of the destructor.
           */
         ~Test();
        
          /**
           * a normal member taking two arguments and returning an integer value.
           * @param a an integer argument.
           * @param s a constant character pointer.
           * @see Test()
           * @see ~Test()
           * @see testMeToo()
           * @see publicVar()
           * @return The test results
           */
           int testMe(int a,const char *s);
           
          /**
           * A pure virtual member.
           * @see testMe()
           * @param c1 the first argument.
           * @param c2 the second argument.
           */
           virtual void testMeToo(char c1,char c2) = 0;
       
          /** 
           * a public variable.
           * Details.
           */
           int publicVar;
           
          /**
           * a function variable.
           * Details.
           */
           int (*handler)(int a,int b);
    };
