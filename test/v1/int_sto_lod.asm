INT 4

LIT 111 STO 0
LIT 222 STO 1
LIT 333 STO 2
LIT 444 STO 3

LOD 1 LIT 222 EXT AST
LIT 333 LOD 2 EXT AST
LOD 0 LIT 111 EXT AST
LIT 444 LOD 3 EXT AST

LIT 666 STO 1
LIT 777 STO 2

LOD 0 LIT 111 EXT AST
LOD 1 LIT 666 EXT AST
LOD 2 LIT 777 EXT AST
LOD 3 LIT 444 EXT AST

OPR HLT