1 - Run ```cmake .``` in the directory of the repo directory

2 - Run ```make`` in the same repo directory

3 - finally to excute the program run ```./interpreter <command> <test file>```

4 - There are several commands to choose from that each do their own thing but build off of each other:

  > tokenize - lexically analyzes your code. (skips over comment blocks and normal comments)

  > parse - parses the expressions in your code.

  > evaluate - evaluates the expressions in your code to individual literals.

  TODO: run - runs through each statement in your code and evaluates them.
  
OPTIONAL:- to clean the project run ```make clean``` all while in the same repo directory

### The program operates as expected in a local environment; however, it encounters issues when deployed on GitHub Workspace.
