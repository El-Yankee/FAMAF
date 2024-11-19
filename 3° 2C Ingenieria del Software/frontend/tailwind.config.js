/** @type {import('tailwindcss').Config} */
export default {
  content: ["./index.html", "./src/**/*.{js,ts,jsx,tsx}"],
  theme: {
    extend: {
      colors: {
        rojoficha: "#FF5959",
        amarilloficha: "#FAD05A",
        verdeficha: "#4ade80",
        azulficha: "#45B3EB",
        negrofondo: "#121212",
        blancofondo: "#FAFAFA",
        cartafig: "#45B3EB",
        cartamov: "#EB7D45",
        resaltador: "#9DE3FA",
      },
    },
  },
  plugins: [],
};
