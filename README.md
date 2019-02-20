# Abstract-VM

42 Abstract VM project in the C++/OOP branch

Interpreter for low level assembly-like language for a stack based calculator

## Installation

```bash
make
```

## Usage

```bash
./avm file
```

Reads contents of file if given as argument-- otherwise reads from stdin

Instructions are define thusly:

```
s := INSTR [SEP INSTR]* #

INSTR :=
	  push VALUE
	  | pop
	  | dump
	  | assert VALUE
	  | add
	  | sub
	  | mul
	  | div
	  | mod
	  | print
	  | exit

VALUE :=
	  int8(N)
	  | int16(N)
	  | int32(N)
	  | float(Z)
	  | double(Z)

N := [-]?[0..9]+

Z := [-]?[0..9]+.[0..9]+

SEP := '\n'+
```

| Command | Description                                                              | Example				     |
|---------|--------------------------------------------------------------------------|---------------------------|
| push    | pushes argument of type v to top of stack                                | ```push int8(42)```       |
| pop     | pops and deletes top of stack                                            | ```pop```                 |
| dump    | prints every value on the stack                                          | ```dump```			     |
| assert  | asserts that the top value is equal to the one given                     | ```assert float(42.42)``` |
| add     | unstacks and adds the top two elements and stacks result                 | ```add```                 |
| sub     | unstacks and subtracts the top two elements and stacks result            | ```sub```                 |
| mul     | unstacks and multiplies the top two elements and stacks result           | ```mul```                 |
| div     | unstacks and divides the top two elements and stacks result              | ```div```                 |
| mod     | unstacks and finds the modulus of the top two elements and stacks result | ```mod```                 |
| print   | asserts that top element is int8 and prints value to standard out        | ```print```               |
| exit    | terminates execution.  Every program must have an 'exit' command         | ```exit```                |
| ;;      | EOF for stdin.  Every program reading from stdin must end with this      | ```;;```                  |
