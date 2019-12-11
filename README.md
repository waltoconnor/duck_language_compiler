Walt O'Connor
Type Checker/Compiler

TO BUILD:
Run make in the root directory, compiles with no warning under g++ 7.4.0 under Ubuntu 18.04
There is likely a linker error with Mac, I do not have a Mac so I can't do a lot about this.
I think someone has a solution on Piazza

The Dockerfile should work fine, but my Docker install is comically broken right now so I can't verify this.

`quack.sh [FILENAME]` will compile the file, output it to `./ir_out/[FILENAME ].qk.c`
and then compile that to `./a.out`

`quack_do_examples.sh` will compile, build and run a number of quack programs from working_samples.
Running `./bin/parser ./[PATH TO FILE] will generate a [FILENAME ].qk.c` file in `./ir_out/`
use `gcc [FILENAME ].qk.c Builtins.o -o [FILENAME ]`



# Overall:
- Typecase not implemented
- Greater than is not implemented

# Type Checker:
- Assign before use checked, catches it most times.
- Works for type inference on all AST Nodes.
- Maintains tables for local and instance vars.
- Correctly LCA-updates types for relevant tables while operating.
- (instance vars can only have their types updated in the constructor).
- Sometimes accepts programs that break the code generator.
- Sometimes rejects valid programs.
- Doesn't check to ensure that a methods return type actually matches the object being returned.
- If and While both correctly use temporary tables for types.
- If will find local variables that were declared in both branches of an If statement, and LCA them, and add them to the method local table (this was a lot of work).
- Correctly LCAs variables declared in if branch with local table.
- A subclass trying to use a parent instance variable will fail compilation, though a subclass will compile if it doesn't assign the parents instance vars but also doesn't use them.
- The type checker is extremely strict about variables being the same type throughout the entirety of a block, if a type gets upcasted after using a method specific to the child class, the program is rejected. (mammals.qk is rejected)

# Code Generator:
- Generates valid C code for almost all programs.
- Class generation works correctly.
- Method generation works correctly.
- Constructor generation works correctly.
- Accessing local variables works.
- Accessing instance variables works.
- Accessing instance variables of other classes works.
- Call works.
- Construct works.
- Assign works.
- AssignDeclare works.
- Load works.
- Return works.
- If works (using goto).
- While works (using goto).
- And works.
- Or is broken despite being line for line identical to And.
- BinOp is not implemented
- Not is not implemented.
- Greater is not implemented.
- Dot works.
- Dispatch tables work correctly (see Submarine.qk)
- Register like allocation is done correctly (no expressions inlined anywhere).
- Not not implemented
- Complex inheritance works correctly (see GoodWalk)

# Final Comments
- Overall it's a little fiddly, but there is a subset of the language it works really well for, and it can handle a lot of the very complex example cases without substantial issues.
- Dispatch tables work perfectly, which was the big one.
- Most of the structural components (Classes/Methods) work without problems.
- Assigning and calling seem really solid.
- If/While both also work well enough.
- Some language features don't work too well but almost all of the hard stuff is done, the rest of it is just incomplete due to time constraints.




