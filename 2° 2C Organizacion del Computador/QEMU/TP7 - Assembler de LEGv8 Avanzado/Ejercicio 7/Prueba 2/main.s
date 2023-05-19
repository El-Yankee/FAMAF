.data
        N: .dword 3 
        A: .dword 1,7,2,44,3,21,1,2,3 // A[N][N]
.text
        ldr X0, =A            // x0 =&A[0][0]
        ldr X9, N             // N=3
        add X1, XZR, XZR   // s=0
    	mul X9, X9, X9    // X9= N * N
	lsl x9, x9, 3        // X9= N * N * 8
	add X9, x9, X0    // finalAddr = &A[0][0] + (N*N*8)    
oLoop:  cmp X0,X9           // if(i==finalAddr)
        b.GE oEnd             // goto oEnd;
        ldur X11, [X0,#0]    // X11=A[i]    
        add X1, X1, X11        // s+=A[i]
       	add X0, X0, #8        // i++
        b oLoop
oEnd:



