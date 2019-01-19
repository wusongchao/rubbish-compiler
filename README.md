# Some bullshit

Context-free grammar patterns:
```
<prog> → program <id>；<block>
<block> → [<condecl>][<vardecl>][<proc>]<body>
<condecl> → const <const>{,<const>}
<const> → <id>:=<integer>
<vardecl> → var <id>{,<id>}
<proc> → procedure <id>([<id>{,<id>}]);<block>{;<proc>}
<body> → begin <statement>{;<statement>}end

<statement> → <id> := <exp>               
              |if <bool> then <statement>[else <statement>]
              |while <bool> do <statement>
              |call <id>[(<exp>{,<exp>})]
              |<body>
              |read (<id>{，<id>})
              |write (<exp>{,<exp>})

<bool> → <bool> || <join> | <join>
<join> → <join> && <equality> | <equality>
<equality> → <equality> == <rel> | <equality> != <rel> | <rel>
<rel> → <expr> < <expr> | <expr> <= <expr> | <expr> >= <expr> | <expr> > <expr> | <expr>
<expr> → [+|-]<term>{<aop><term>}
<term> → <unary>{<mop><unary>}
<unary> → ! <unary> | - <unary> | <factor>
<factor> → <id>| <integer> | true | false | (<bool>)
<lop> → =|<>|<|<=|>|>=
<aop> → +|-
<mop> → *|/
<id> → l{l|d}   
<integer> → d{d} 

l:letter
d:digit
```

The program can accept input from any ```istream(cin, ifstream, istringstream, ...)```, the ```IRGenerator``` and ```ObjectCodeGenerator``` can output result to any ```ostream(cout, ofstream, ostringstream)``` by calling the instance method ```generator.output(ostream&)```.

```IRGenerator``` generate quad IR. The object machine is a stack base machine.

## TODO
1. change some grammar, for example: replace ```begin end``` with ```{}```
2. more clearly nested block rule(nested function declaration is allowed, the enclosure reference behaviour is similar to value capturing)
3. construct type, ```T[]``` or ```struct```
4. allow initialzation in variable declaration
5. change the IR format
6. add keyword ```break```, ```continue```, ```return```
7. fix the bug in the ir generataion phase, like the ```jmp``` in procedure visit phase, the ```if <bool>``` when ```<bool>``` is not a bool expression
8. type check (and implicit conversion)
9. function overload

## NOTE

transfer the type check work from parser to IRGenerator, except the reference to undeclared reference(since the check need sequential information, if we do this in the IRGenerator, we may need an extra field(offset) to record the sequential position)

each block corresponds to some symbol tables(```Env```), which means the ```Env``` is attached to the block node.

when a statement in a block try to reference a identifier, it will look up through the "top" Env(recorded by parser)'s prev list.

```
nullptr
  ^prev
  block - func a:
    ^prev
    block - func b:
      ^prev
      block - func c:
```

a can call b(declaration in func a's field), a cannot call c, but c can call a