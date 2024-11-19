import React, { createContext, useContext, useState, useEffect } from "react";

// Create the context
export const PlayerContext = createContext();

// Provider component
export const PlayerProvider = ({ children }) => {
  const [playerID, setPlayerID] = useState(() => {
    // Retrieve the player ID from session storage if it exists
    const savedPlayerID = sessionStorage.getItem("playerID");
    try {
      return savedPlayerID ? JSON.parse(savedPlayerID) : null;
    } catch (error) {
      console.error("Error parsing playerID from sessionStorage:", error);
      return null;
    }
  });

  useEffect(() => {
    // Save the player ID to session storage whenever it changes
    if (playerID !== null) {
      sessionStorage.setItem("playerID", JSON.stringify(playerID));
    }
  }, [playerID]);

  return (
    <PlayerContext.Provider value={{ playerID, setPlayerID }}>
      {children}
    </PlayerContext.Provider>
  );
};

// Custom hook to use the PlayerContext
export const usePlayerContext = () => {
  return useContext(PlayerContext);
};
