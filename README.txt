

Source code related to the "Service Queue" assignment.
Important files:

    sq.h : this specifies the functions to be implemented for the
            service queue ADT.  Banner comments specify behavioral
            and runtime requirements.  Also specified is a typedef
            for SQ as an incompletely specified type: the specification
            of the corresponding struct is to be hidden in a .c file
            implementing the ADT.

    sq_slow.c : an implementation of the service queue ADT which 
            is behaviorally correct, but does NOT meet the runtime
            requirements.

    driver.c : a simple interactive program which creates a service
            queue instance and lets the user execute operations on 
            the resulting service queue.  There is basically a 1-to-1
            correspondence between the user commands and the functions
            specified in sq.h

    Makefile : a simple makefile to create sq_slow.o and the executable
            called driver.  You will modify the makefile to have additional
            targets for sq.o (your "fast" implementation of the SQ ADT in
            your file sq.c) and fdriver which is the result of compiling 
            driver.c and linking it with sq.o (rather than sq_slow.o to 
            which the target "driver" links).

    list.h and llist.c : implementation of a list ADT used by sq_slow.c


COMMENTS:  You probably shouldn't approach your task of implementing
    sq.c as a "modification" of sq_slow.c (and list.h / llist.c).  It 
    won't really make your life any easier!  Take a "clean slate" 
    approach.  This principle applies to the given linked list code
    as well: although you will almost certainly have linked lists
    as part of your implementation, trying to tweak the given linked
    list code to meet your requirements is more trouble than it is
    worth IMO. 
    task from a "clean slate"
