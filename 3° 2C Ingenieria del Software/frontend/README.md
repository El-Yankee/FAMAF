# frontend
Repositorio utilizado para desarrollar el frontend del laboratorio

# Proyecto React con Vite

Este proyecto está creado utilizando **Vite** para el desarrollo de una aplicación en **React**.

## Requisitos previos

Antes de comenzar, asegúrate de tener instalado lo siguiente en tu sistema:

- [Node.js](https://nodejs.org) (versión 14 o superior)
- npm (que se instala junto con Node.js)

Puedes verificar que tienes Node.js y npm ejecutando los siguientes comandos en tu terminal:
```bash
node -v
npm -v
```

## Crear un nuevo proyecto

Sigue estos pasos para iniciar un proyecto de React con Vite:

1. **Crear el proyecto**:

   Ejecuta el siguiente comando en tu terminal para crear un nuevo proyecto con Vite:
   ```bash
   npm create vite@latest nombre-del-proyecto
   ```

2. **Selecciona React**:

   Cuando se te pida elegir un framework, selecciona **React**. También puedes optar por JavaScript o TypeScript según tus necesidades.

3. **Instala las dependencias**:

   Ingresa al directorio del proyecto y ejecuta el siguiente comando para instalar todas las dependencias necesarias:
   ```bash
   cd nombre-del-proyecto
   npm install
   ```

## Ejecutar el servidor de desarrollo

Una vez instaladas las dependencias, puedes iniciar el servidor de desarrollo ejecutando el siguiente comando:
```bash
npm run dev
```

Esto abrirá tu aplicación en el navegador en la URL [http://localhost:5173](http://localhost:5173). Cualquier cambio que realices en el código se reflejará automáticamente gracias a Vite.

## Crear una versión de producción

Cuando estés listo para crear una versión de producción optimizada, ejecuta:
```bash
npm run build
```

Esto generará una carpeta `dist` que contendrá todos los archivos listos para desplegar.

## Scripts disponibles

Además de los comandos mencionados anteriormente, aquí tienes algunos scripts adicionales disponibles en el proyecto:

- **`npm run dev`**: Inicia el servidor de desarrollo.
- **`npm run build`**: Crea una versión de producción optimizada.
- **`npm run preview`**: Previsualiza localmente la versión de producción generada por `build`.

## Estructura del proyecto

- **`src/`**: Contiene todos los archivos de código fuente de React.
- **`public/`**: Archivos públicos que no requieren procesamiento (como imágenes o `index.html`).
- **`vite.config.js`**: Configuración de Vite.
- **`node_modules/`**: Dependencias instaladas por npm.

## Más información

Para obtener más información sobre cómo usar Vite, visita la [documentación oficial de Vite](https://vitejs.dev).
