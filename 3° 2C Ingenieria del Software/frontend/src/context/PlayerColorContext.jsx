import React, { createContext, useState, useContext } from "react";

// Crear el contexto
const PlayerColorContext = createContext();

// Proveedor del contexto
export const PlayerColorProvider = ({ children }) => {
  const [playerColors, setPlayerColors] = useState(() => {
    // Recuperar los colores de sessionStorage cuando se cargue la página
    const savedColors = sessionStorage.getItem("playerColors");
    return savedColors ? JSON.parse(savedColors) : {};
  });

  const getRandomColor = () => {
    const colors = ["#FF5959", "#45B3EB", "#4ade80", "#FAD05A"];
    return colors[Math.floor(Math.random() * colors.length)];
  };

  const assignColorToPlayer = (playerId) => {
    setPlayerColors((prevColors) => {
      // Si el jugador ya tiene un color, no asignar uno nuevo
      if (prevColors[playerId]) {
        return prevColors;
      }

      const newColors = { ...prevColors, [playerId]: getRandomColor() };

      // Guardar los colores actualizados en sessionStorage
      sessionStorage.setItem("playerColors", JSON.stringify(newColors));

      return newColors;
    });
  };

  return (
    <PlayerColorContext.Provider value={{ playerColors, assignColorToPlayer }}>
      {children}
    </PlayerColorContext.Provider>
  );
};

// Custom hook para usar el contexto más fácilmente
export const usePlayerColor = () => useContext(PlayerColorContext);
