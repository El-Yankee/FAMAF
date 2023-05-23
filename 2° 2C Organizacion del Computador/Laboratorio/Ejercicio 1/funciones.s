.equ SCREEN_WIDTH, 		640
.equ SCREEN_HEIGH, 		480	

calcular_pixel:
	// 	Parametros:
	// 	x3 -> Pixel X.
	// 	x4 -> Pixel Y.
	// 	Return x0 -> Posición (x,y) en la imgen.

	mov x0, 640							// x0 = 640.
	mul x0, x0, x4						// x0 = 640 * y.		
	add x0, x0, x3						// x0 = (640 * y) + x.
	lsl x0, x0, 2						// x0 = ((640 * y) + x) * 4.
	add x0, x0, x20						// x0 = ((640 * y) + x) * 4 + A[0].
ret										// Seguimos con la siguiente instrucción. -> BR x30


dibujar_cuadrado:
	// 	Parametros:
	// 	w10 -> Color.
	//	x1 -> Ancho.
	//	x2 -> Alto.
	// 	x3 -> Pixel X.
	// 	x4 -> Pixel Y.

	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]					// Salvamos en el stack el registro de retorno (x30).
	BL calcular_pixel 			// Calculamos el pixel a dibujar con la función "calcular_pixel". Retorna x0.
	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
	ADD SP, SP, 8						// Dejamos el stack como antes.
	// Usamos los registros temporales: x9, x11, x12, x13
	mov x9, x2							// x9 = x2 --> A x9 le guardamos el alto de la imagen.
	mov x13, x0							// x13 = x0 --> A x13 le guardamos la posición de x0 calculada.	
	pintar_cuadrado:
		mov x11, x1						// x11 = x1 --> A x11 le asignamos el ancho de la fila.
		mov x12, x13					// x12 = x13 --> A x12 le guardamos x13 (En esta parte de la ejecucción a x12 se le guarda el pixel inicial de la fila).
		color_cuadrado:
			stur w10, [x13]				// Memory[x13] = w10 --> A x13 le asignamos en memoria el color que respresenta w10.
			add x13, x13, 4				// w13 = w13 + 4 --> x13 se mueve un pixel hacia la derecha.
			sub x11, x11, 1				// w11 = w11 - 1 --> x11 le restamos un pixel de ancho.
			cbnz x11, color_cuadrado	// Si x11 no es 0 (la fila no se termino de pintar), seguimos pintandola.
			mov x13, x12				// En esta parte, ya se termino de pintar la fila. x13 = x12. Volvemos al pixel de origen de la fila.
			add x13, x13, 2560			// x13 = x13 + 2560. La constante 2560 es el total de pixeles de una fila, entoces si lo sumamos es como dar un salto de linea.
			sub x9, x9, 1				// x9 = x9 - 1 --> Le restamos 1 al alto de la fila.
			cbnz x9, pintar_cuadrado	// Si el alto no es 0, es porque aún no se termino de pintar.
ret


dibujar_triangulo:
	// 	Parametros:
	// 	w10 -> Color.
	//	x1 -> Ancho.
	//  x2 -> Cantidad de filas a pintar antes de disminuir en 1 su valor (Altura = x1 * x2).
	// 	x3 -> Pixel X.
	// 	x4 -> Pixel Y.

	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]					// Salvamos en el stack el registro de retorno (x30).
	BL calcular_pixel 					// Calculamos el pixel a dibujar con la función "calcular_pixel". Retorna x0.
	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
	ADD SP, SP, 8						// Dejamos el stack como antes.
	
	// Usamos los registros temporales: x9, x11, x12, x13
	mov x13, x0							// x13 = x0 --> A x13 le guardamos la posición de x0 calculada.
	mov x14, x1							// x14 = x1 --> A x14 le asignamos el ancho de la fila.
	
	pintar_triangulo:
		mov x15, x2							// x15 = x2  --> A x15 le asignamos la cantidad de filas a pintar antes de disminuir el ancho de la fila actual.
		pintar_fila:
			mov x11, x14					// x11 = x14 --> A x11 le asignamos el ancho de la fila.
			mov x12, x13					// x12 = x13 --> A x12 le guardamos x13 (En esta parte de la ejecucción a x12 se le guarda el pixel inicial de la fila).
			
			color_triangulo:
				stur w10, [x13]				// Memory[x13] = w10 --> A x13 le asignamos en memoria el color que respresenta w10.
				add x13, x13, 4				// w13 = w13 + 4 --> x13 se mueve un pixel hacia la derecha.
				sub x11, x11, 1				// w11 = w11 - 1 --> x11 le restamos un pixel de ancho.
				cbnz x11, color_triangulo	// Si x11 <= 0 (la fila no se termino de pintar), seguimos pintandola.
				
			mov x13, x12					// En esta parte, ya se termino de pintar la fila. x13 = x12. Volvemos al pixel de origen de la fila.
			add x13, x13, 2560				// Pasamos a la siguiente fila.
			sub x15, x15, 1					// x15 = x15 - 1. Le restamos 1 a x15 para pintar al siguiente fila del mismo ancho que la anterior.
			cbnz x15, pintar_fila
			
			mov x13, x12				// En esta parte, ya se termino de pintar la fila. x13 = x12. Volvemos al pixel de origen de la fila.
			add x13, x13, 2564			// x13 = x13 + 2562. La constante 2560 es el total de pixeles de una fila, el numero 4 que se suma a 2560 sirve para movernos 
										//	1 pixel (4 posiciones) hacia la derecha. entonces si lo sumamos es como dar un salto de linea movido 1 pixeles a la derecha.
			sub x14, x14, 2				// x14 = x14 - 2. A x14 le restamos 2 para disminuir el ancho de la siguiente fila en 1 pixel.
			cbnz x14, pintar_triangulo
ret


cohete:
	SUB SP, SP, 8 						// Apuntamos en el stack.
		STUR X30, [SP, 0]

		// Dibujo cohete
		movz w10, 0x5B, lsl 16
		movk w10, 0x5B5B, lsl 0
		mov x1, 130
		mov x2, 250
		mov x3, 140
		mov x4, 100
		BL dibujar_cuadrado

		LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


cabeza_cohete:
	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]

	// Cabeza cohete
	movz w10, 0xFE, lsl 16
	movk w10, 0x0000, lsl 0
	mov x1, 113
	mov x2, 20
	mov x3, 150
	mov x4, 80
	BL dibujar_cuadrado

	movz w10, 0xFE, lsl 16
	movk w10, 0x0000, lsl 0
	mov x1, 90
	mov x2, 20
	mov x3, 162
	mov x4, 60
	BL dibujar_cuadrado

	movz w10, 0xFE, lsl 16
	movk w10, 0x0000, lsl 0
	mov x1, 60
	mov x2, 20
	mov x3, 177
	mov x4, 40
	BL dibujar_cuadrado

	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


luces:
	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]


	// Luz arriba derecha
	movz w10, 0xEA, lsl 16
	movk w10, 0xEA00, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 250
	mov x4, 175
	BL dibujar_cuadrado

	movz w10, 0xEA, lsl 16
	movk w10, 0x0000, lsl 0
	mov x1, 5
	mov x2, 5
	mov x3, 255
	mov x4, 175
	BL dibujar_cuadrado

	
	// Luz abajo derecha
	movz w10, 0xEA, lsl 16
	movk w10, 0xEA00, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 250
	mov x4, 250
	BL dibujar_cuadrado

	movz w10, 0xEA, lsl 16
	movk w10, 0x0000, lsl 0
	mov x1, 5
	mov x2, 5
	mov x3, 255
	mov x4, 250
	BL dibujar_cuadrado

	// Luz abajo izquierda
	movz w10, 0xEA, lsl 16
	movk w10, 0xEA00, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 150
	mov x4, 250
	BL dibujar_cuadrado

	movz w10, 0xEA, lsl 16
	movk w10, 0x0000, lsl 0
	mov x1, 5
	mov x2, 5
	mov x3, 150
	mov x4, 250
	BL dibujar_cuadrado

	// Luz arriba izquierda
	movz w10, 0xEA, lsl 16
	movk w10, 0xEA00, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 150
	mov x4, 175
	BL dibujar_cuadrado

	movz w10, 0xEA, lsl 16
	movk w10, 0x0000, lsl 0
	mov x1, 5
	mov x2, 5
	mov x3, 150
	mov x4, 175
	BL dibujar_cuadrado


	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


ventanillas:
	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]

	// Ventanilla 1
	movz w10, 0x00, lsl 16
	movk w10, 0xEAE0, lsl 0
	mov x1, 30
	mov x2, 40
	mov x3, 190
	mov x4, 120
	BL dibujar_cuadrado

	movz w10, 0x00, lsl 16
	movk w10, 0x32EA, lsl 0
	mov x1, 20
	mov x2, 20
	mov x3, 190
	mov x4, 120
	BL dibujar_cuadrado

	// Ventanilla 2 
	movz w10, 0x00, lsl 16
	movk w10, 0xEAE0, lsl 0
	mov x1, 30
	mov x2, 40
	mov x3, 190
	mov x4, 200
	BL dibujar_cuadrado

	movz w10, 0x00, lsl 16
	movk w10, 0x32EA, lsl 0
	mov x1, 20
	mov x2, 20
	mov x3, 200
	mov x4, 200
	BL dibujar_cuadrado

	// Ventanilla 3
	movz w10, 0x00, lsl 16
	movk w10, 0xEAE0, lsl 0
	mov x1, 30
	mov x2, 40
	mov x3, 190
	mov x4, 280
	BL dibujar_cuadrado

	movz w10, 0x00, lsl 16
	movk w10, 0x32EA, lsl 0
	mov x1, 20
	mov x2, 20
	mov x3, 190
	mov x4, 300
	BL dibujar_cuadrado


	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


propulsores:
	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]

	// Propulsor 1
	movz w10, 0xEA, lsl 16
	movk w10, 0xE000, lsl 00
	mov x1, 30
	mov x2, 3
	mov x3, 150
	mov x4, 350
	BL dibujar_triangulo

	movz w10, 0xFC, lsl 16
	movk w10, 0x0000, lsl 00
	mov x1, 30
	mov x2, 2
	mov x3, 150
	mov x4, 350
	BL dibujar_triangulo


	// Propulsor 2
	movz w10, 0xEA, lsl 16
	movk w10, 0xE000, lsl 00
	mov x1, 30
	mov x2, 3
	mov x3, 190
	mov x4, 350
	BL dibujar_triangulo

	movz w10, 0xFC, lsl 16
	movk w10, 0x0000, lsl 00
	mov x1, 30
	mov x2, 2
	mov x3, 190
	mov x4, 350
	BL dibujar_triangulo

	// Propulsor 3
	movz w10, 0xEA, lsl 16
	movk w10, 0xE000, lsl 00
	mov x1, 30
	mov x2, 3
	mov x3, 230
	mov x4, 350
	BL dibujar_triangulo
	
	movz w10, 0xFC, lsl 16
	movk w10, 0x0000, lsl 00
	mov x1, 30
	mov x2, 2
	mov x3, 230
	mov x4, 350
	BL dibujar_triangulo
	

	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


estrellas:
	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 340
	mov x4, 400
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 370
	mov x4, 370
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 400
	mov x4, 320
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 10
	mov x4, 400
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 10
	mov x4, 90
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 75
	mov x4, 120
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 550
	mov x4, 120
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 600
	mov x4, 470
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 500
	mov x4, 50
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 560
	mov x4, 230
	BL dibujar_cuadrado


	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 430
	mov x4, 60
	BL dibujar_cuadrado

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 450
	mov x4, 100
	BL dibujar_cuadrado


	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 10
	mov x2, 10
	mov x3, 380
	mov x4, 210
	BL dibujar_cuadrado


	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


luna:
	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]

	movz w10, 0xFF, lsl 16
	movk w10, 0xFFFF, lsl 0
	mov x1, 70
	mov x2, 70
	mov x3, 500
	mov x4, 120
	BL dibujar_cuadrado


	movz w10, 0x95, lsl 16
	movk w10, 0x9595, lsl 0
	mov x1, 15
	mov x2, 15
	mov x3, 510
	mov x4, 155
	BL dibujar_cuadrado

	movz w10, 0x95, lsl 16
	movk w10, 0x9595, lsl 0
	mov x1, 20
	mov x2, 20
	mov x3, 540
	mov x4, 125
	BL dibujar_cuadrado

	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


pintar_fondo:
	SUB SP, SP, 8 						// Apuntamos en el stack.
	STUR X30, [SP, 0]
	movz w10, 0x16, lsl 16
	movk w10, 0x16, lsl 00
	mov x1, SCREEN_WIDTH
	mov x2, SCREEN_HEIGH
	mov x3, 0
	mov x4, 0
	BL dibujar_cuadrado

	LDR X30, [SP, 0]					// Le asignamos x30 su posición de retorno desde el stack. (Anteriormente fue pisada al llamar calcular_pixel). 			
		ADD SP, SP, 8	
	ret


