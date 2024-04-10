# encoding: utf-8
# Revisión 2019 (a Python 3 y base64): Pablo Ventura
# Copyright 2014 Carlos Bederián
# $Id: connection.py 455 2011-05-01 00:32:09Z carlos $

import socket
from constants import *
from base64 import b64encode
import os

class Connection(object):
    """
    Conexión punto a punto entre el servidor y un cliente.
    Se encarga de satisfacer los pedidos del cliente hasta
    que termina la conexión.
    """

    def __init__(self, socket: socket.socket, directory):
        self.s = socket
        self.dir = directory
        self.server_status = True
        self.buffer = ""

    def quit(self):
        """
        Termina la conexión
        """
        # Enviamos mensaje de exito
        self.error_mesg(CODE_OK)

        # Cambiamos el estado para terminar la conexion
        self.server_status = False

        print("Closing connection...")
        
    def get_file_listing(self):
        """
        Obtener la lista de archivos que están actualmente disponibles en el directorio
        """
        # Enviamos mensaje de exito
        self.error_mesg(CODE_OK)

        file_list = ""

        # Vemos todos los archivos disponibles en el directorio
        for files in os.listdir(self.dir):
            file_list += files + EOL

        # Enviamos por mensaje todos los archivos disponibles
        self.send_message(file_list)

    def get_metadata (self, filename: str):
        """
        Devuelve el tamaño del archivo pedido
        """  
        invalid_chars = set(filename) - VALID_CHARS
        test = len(invalid_chars) == 0

        # Tenemos que ver si el archivo solicitado existe en el directorio
        if not os.path.isfile(os.path.join(self.dir, filename)):
            self.error_mesg(FILE_NOT_FOUND)

        # Vemos si el archivo tiene caracteres invalidos
        elif not test:
            self.error_mesg(INVALID_ARGUMENTS)
        
        else:
            # Enviamos mensaje de exito
            self.error_mesg(CODE_OK)
            # Obtenemos el tamaño del archivo
            size = os.path.getsize(os.path.join(self.dir, filename))
            # Enviamos el tamaño como string
            self.send_message(str(size))

    def get_slice (self, filename: str, offset: int, size: int):
        """
        Devuelve un slice o parte del archivo pedido
        """ 
        invalid_chars = set(filename) - VALID_CHARS
        test = len(invalid_chars) == 0

        # Tenemos que ver si el archivo solicitado existe
        if not os.path.isfile(os.path.join(self.dir, filename)):
            self.error_mesg(FILE_NOT_FOUND)

        # Vemos si el archivo tiene caracteres invalidos
        elif not test:
            self.error_mesg(INVALID_ARGUMENTS)    

        else:
            file_size = os.path.getsize(os.path.join(self.dir, filename))

            # Vemos si los el offset y size son validos
            if file_size < offset + size or offset < 0 or size < 0:
                self.error_mesg(BAD_OFFSET)

            else:
               # Enviamos mensaje de exito
                self.error_mesg(CODE_OK)
                # Obtenemos el slice o parte del archivo
                path = os.path.join(self.dir, filename)
                # Enviamos el slice codificado en b64
                with open(path, "rb") as archive:
                    archive.seek(offset)
                    data = archive.read(size)
                    self.send_message(data, codif="b64encode")

    def send_message(self, message: bytes | str, codif="ascii"):
        """
        Envia un mensaje a través del socket      
        """
        try:
            # Aplicamos la codifiacion que necesitamos
            if codif == "ascii":
                message = message.encode("ascii")

            elif codif == "b64encode":
                message = b64encode(message)

            else:
                raise ValueError(f"send: codificación inválida '{codif}'")
            
            # Enviamos el mensaje codificado
            while message:
                bytes_sent = self.s.send(message)
                assert bytes_sent > 0
                message = message[bytes_sent:]
            
            # Enviamos el fin de línea
            self.s.send(EOL.encode("ascii"))

        # En caso de encontrar estos errores, cerramos la conexion
        except BrokenPipeError or ConnectionResetError:
            self.server_status = False

    def command(self, line: str):
        """
        Selecciona el comando a ejecutar según la línea recibida               
        """
        # Separamos el comando de sus argumentos
        arguments = line.split(" ")

        print(f"Request: {line}")

        if len(arguments) == 0:
            self.error_mesg(INTERNAL_ERROR)
            self.quit 
        
        # Veo cual de todas las funciones es el primer elemento de la lista
        if arguments[0] == "get_file_listing":
            # Verificamos que la funcion venga con la cantidad correcta de argumentos
            if len(arguments) == 1:
                self.get_file_listing()
            else:
                self.error_mesg(INVALID_ARGUMENTS)

        elif arguments[0] == "get_metadata":
            # Verificamos que la funcion venga con la cantidad correcta de argumentos
            if len(arguments) == 2:
                self.get_metadata(arguments[1])
            else:
                self.error_mesg(INVALID_ARGUMENTS)

        elif arguments[0] == "get_slice": 
            # Verificamos que la funcion venga con la cantidad correcta de argumentos
            if len(arguments) == 4 and arguments[2].isdecimal() and arguments[3].isdecimal():
                self.get_slice(arguments[1], int(arguments[2]), int(arguments[3]))
            else:
                self.error_mesg(INVALID_ARGUMENTS)

        elif arguments[0] == "quit":
            # Verificamos que la funcion venga con la cantidad correcta de argumentos
            if len(arguments) == 1:
                self.quit()
            else:
                self.error_mesg(INVALID_ARGUMENTS)

        else:
            # Si no es ningun comando entonces enviamos error
            self.error_mesg(INVALID_COMMAND)

    def read_line(self):
        """
        Devuelve la línea, eliminando el terminador
        """
        # Mientras que la conexion este abierta y no se haya terminado de escribir el comando, seguimos recibiendo la entrada
        while not EOL in self.buffer and self.server_status:
            try:
                data = self.s.recv(4096).decode("ascii")
                self.buffer += data

                if len(data) == 0:
                    self.quit()

            except UnicodeError:
                self.error_mesg(BAD_REQUEST)
            except ConnectionResetError or BrokenPipeError:
                self.server_status = False

        # Eliminamos el simbolo de fin de comando
        if EOL in self.buffer:
            request, self.buffer = self.buffer.split(EOL, 1)
            return request.strip()

    def error_mesg(self, error_num: int):
        """
        Vemos que tipo de error es y lo enviamos
        """
        # Revisamos si el numero de error corresponde a uno fatal
        if fatal_status(error_num):
            self.send_message(f"{error_num} {error_messages[error_num]}")
            # Si es fatal entonces cerramos la conexion
            self.quit()
        else:
            # Si no lo es simplemente enviamos el error
            self.send_message(f"{error_num} {error_messages[error_num]}")
    
    def handle(self):
        """
        Atiende eventos de la conexión hasta que termina.
        """
        line = self.read_line()
        
        # Mantenemos la conexion abierta hasta que se cierre
        while self.server_status:

            if NEWLINE in line:
                # Si no es valida tiramos error
                self.error_mesg(BAD_EOL)

            elif len(line) > 0:
                # Si es valida entonces llamamos a la funcion
                self.command(line)
           
            line = self.read_line()

        # Cerramos la conexion
        self.s.close() 
