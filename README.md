# Edward

Edward is a Lisp. I am building this from the ground up myself in C and it is an ongoing project. The current status can be inferred easily from the source in evaluation.c. 
I am developing this with a goal of ruby-esque syntax and method functionality combined with the powerful structure of functional programming. 


To Compile:

    cc -std=c99 -Wall evaluation.c mpc.c -ledit -lm -o evaluation

It is also very basic and being developed by a noob. Pull requests are welcome. 

### TODOS
- min and max functions
- lexical scoping
- user defined types
- macros
- garbage collection
- tail call optimization
- static typing
