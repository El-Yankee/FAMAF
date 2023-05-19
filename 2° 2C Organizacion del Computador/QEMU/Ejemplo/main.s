.data
f: .dword 1
g: .dword 2
h: .dword 3
i: .dword 4
j: .dword 5

.text
LDR X0, f
LDR X1, g
LDR X2, h
LDR X3, i
LDR X4, j
ADD X0, X1, X2
ADD X0, X0, X3
ADD X0, X0, X4

infloop: B infloop
