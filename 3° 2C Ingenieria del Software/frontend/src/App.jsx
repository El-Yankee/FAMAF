import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import { PlayerProvider } from "./context/PlayerContext.jsx";
import { GameProvider } from "./context/GameContext.jsx";
import { PlayerColorProvider } from "./context/PlayerColorContext.jsx";
import LoginPage from "./pages/LoginPage";
import WaitingRoom from "./pages/WaitingRoom";
import LobbyPage from "./pages/LobbyPage";
import GamePage from "./pages/GamePage";
import CreateGame from "./pages/CreateGame";
import CreditsPage from "./pages/CreditsPage.jsx";
import "./styles/Styles.css";

function App() {
  return (
    <div>
      <PlayerProvider>
        <GameProvider>
          <PlayerColorProvider>
            <Router>
              <Routes>
                <Route path="/" element={<LoginPage />} />
                <Route path="/lobby" element={<LobbyPage />} />
                <Route path="/waitingRoom" element={<WaitingRoom />} />
                <Route path="/creategame" element={<CreateGame />} />
                <Route path="/game" element={<GamePage />} />
                <Route path="/credits" element={<CreditsPage />} />
              </Routes>
            </Router>
          </PlayerColorProvider>
        </GameProvider>
      </PlayerProvider>
    </div>
  );
}

export default App;
