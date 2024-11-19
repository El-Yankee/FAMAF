

# Backend
Repositorio utilizado para desarrollar el backend del laboratorio

## Requisitos previos

Antes de comenzar, asegúrate de estar en un entorno virtual.
Puedes utilizar el siguiente.
- [venv](https://docs.python.org/es/3/library/venv.html)

Crear entorno virtual:
```bash
python3 -m venv .venv
```

Activa tu entorno virtual:
```bash
source .venv/bin/activate
```
una vez activado el entorno virtual puedes verificar que estan en uno con el siguiente comando
```bash
which python
/home/user/code/awesome-project/.venv/bin/python
```
para salir del entorno virtual utiliza 
```bash
deactivate
```
## Dependencias del proyecto

Ejecuta los siguientes comandos para instalar las dependencias del proyecto:

```bash
pip install fastapi==0.108.0
pip install uvicorn
pip install 'uvicorn[standard]'
```

```bash
pip install SQLalchemy
```

Asegúrate de que el directorio src esté en el PYTHONPATH para que los módulos puedan ser encontrados correctamente. Puedes hacerlo temporalmente en tu terminal antes de ejecutar pytest:

```bash
export PYTHONPATH=$PYTHONPATH:/tu/ruta/al/proyecto/backend/src
```

y para ejecutar nuestro servidor iremos al path de nuestro archivo a corre y ejecutaremos el comando

```bash
uvicorn app:app --reload
```

Ahora para ver la documentacion del mismo con ir a la pagina
*http://127.0.0.1:8000/docs* podremos ver todos los endpoints de nuestra api.

## Runear los test

Primero hay que descargar las dependencias.

```bash
pip install httpx
pip install pytest-asyncio
pip install pytest
pip install coverage
pip install factory_boy
```

Hay que estar en la dirección `/backend/src` y ahí se ejecuta el comando:

```bash
coverage run -m pytest -v
```

Despues para ver el respuen del test se ejecuta:

```bash
coverage report -m
```

## Organizacion de archivos

**`src/`**: Carpeta principal para el código fuente.

**`models/`**: Contiene las clases y sus metodos.

**`routes/`**: Contiene todos los endpoints de cada clase.

**`test/`** Contiene todos los test.

**`app.py`** Contiene el manejo principal de la API.
