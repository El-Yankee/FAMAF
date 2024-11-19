// src/setupTests.js
import "@testing-library/jest-dom";

Object.defineProperty(window, "matchMedia", {
  writable: true,
  value: (query) => ({
    matches: false,
    media: query,
    onchange: null,
    addListener: vi.fn(), // Para Vitest
    removeListener: vi.fn(), // Para Vitest
    addEventListener: vi.fn(),
    removeEventListener: vi.fn(),
    dispatchEvent: vi.fn(),
  }),
});

beforeAll(() => {
  window.getComputedStyle = () => ({
    getPropertyValue: () => "", // Retorna un valor vacío o el que necesites para la propiedad CSS
  });
});
