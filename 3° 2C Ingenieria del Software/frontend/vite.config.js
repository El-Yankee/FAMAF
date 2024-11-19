import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],
  test: {
    environment: "jsdom",
    setupFiles: "./src/setupTests.js",
    globals: true,
    coverage: {
      provider: "istanbul", // Puedes usar 'v8' o 'istanbul' (recomendado para más opciones de reporte)
      reporter: ["text", "json", "html"], // Formatos de reporte
      include: ["src/**/*.{js,jsx,ts,tsx}"], // Archivos para los que deseas cobertura
      all: true, // Para asegurarse de que todos los archivos sean incluidos, no solo los que tienen pruebas
    },
  },
});
