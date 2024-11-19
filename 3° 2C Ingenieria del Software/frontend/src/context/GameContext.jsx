import React, { createContext, useContext, useState, useEffect } from "react";

// Create the context
export const GameContext = createContext();

// Provider component
export const GameProvider = ({ children }) => {
  const [game_id, setGameID] = useState(() => {
    // Retrieve the game ID from session storage if it exists
    const savedGameID = sessionStorage.getItem("game_id");
    try {
      return savedGameID ? JSON.parse(savedGameID) : null;
    } catch (error) {
      console.error("Error parsing game_id from sessionStorage:", error);
      return null;
    }
  });

  useEffect(() => {
    // Save the game ID to session storage whenever it changes
    if (game_id !== null) {
      sessionStorage.setItem("game_id", JSON.stringify(game_id));
    }
  }, [game_id]);

  return (
    <GameContext.Provider value={{ game_id, setGameID }}>
      {children}
    </GameContext.Provider>
  );
};

// Custom hook to use the GameContext
export const useGameContext = () => {
  return useContext(GameContext);
};
