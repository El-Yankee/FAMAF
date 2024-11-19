import React, { useEffect, useState } from "react";
import { Button, Modal } from "antd";
import { useNavigate } from "react-router-dom";
import TablePlayers from "../components/TablePlayers";
import LobbySquares from "../components/LobbySquares";
import {
  GameData,
  LeaveGame,
  StartGame,
  DeleteGame,
} from "../services/GameServices";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";

const WaitingRoom = ({
  initialGameName = "",
  initialIsCreator = false,
  initialNumberOfPlayers = 0,
}) => {
  const navigate = useNavigate();
  const [gameName, setGameName] = useState(initialGameName);
  const [gamestate, setGamestate] = useState();
  const [isCreator, setIsCreator] = useState(initialIsCreator);
  const [numberOfPlayers, setNumberOfPlayers] = useState(
    initialNumberOfPlayers
  );
  const [maxNumberOfPlayers, setMaxNumberOfPlayers] = useState();
  const [playersList, setPlayersList] = useState([]);
  const [socket, setSocket] = useState(null);
  const [isModalOpen, setIsModalOpen] = useState(false);
  // Obtener playerID desde el contexto
  const { playerID } = usePlayerContext();
  // Obtener game_id desde el contexto
  const { game_id } = useGameContext();

  const getGameInfo = async (game_id) => {
    console.log("Success");

    try {
      // Esperamos la resolución de la promesa de GameData
      const response = await GameData(game_id);

      if (response) {
        console.log("Game Info:", response);

        return response;
      }
    } catch (error) {
      console.error("Error getting game data", error);
    }
  };

  const quitRoom = async (game_id) => {
    if (playerID !== isCreator) {
      console.log("Success");
      try {
        // Esperamos la resolución de la promesa de LeaveGame
        const response = await LeaveGame(playerID, game_id);

        if (response) {
          console.log("New Game Info:", response);

          // Navegamos solo cuando la respuesta está lista
          navigate(`/lobby`);
        }
      } catch (error) {
        console.error("Error getting new game data", error);
      }
    } else if (playerID === isCreator) {
      console.log("Success");
      try {
        // Esperamos la resolución de la promesa de DeleteGame
        const response = await DeleteGame(game_id);

        if (response) {
          console.log("Game Deleted:", response);

          // Navegamos solo cuando la respuesta está lista
          navigate(`/lobby`);
        }
      } catch (error) {
        console.error("Error deleting game", error);
      }
    }
  };

  const start = async (game_id) => {
    console.log("Success");

    try {
      // Esperamos la resolución de la promesa de StartGame
      const response = await StartGame(playerID, game_id);

      if (response) {
        console.log("Info:", response);

        // Navegamos solo cuando la respuesta está lista
        navigate(`/game`);
      }
    } catch (error) {
      console.error("Error getting data", error);
    }
  };

  useEffect(() => {
    if (!gamestate) {
      getGameInfo(game_id).then((response) => {
        if (response) {
          setGameName(response.game_name);
          setGamestate(response.state);
          setIsCreator(response.host_id);
          setNumberOfPlayers(response.players);
          setMaxNumberOfPlayers(response.game_size);
        }
      });
    }
  }, []);

  useEffect(() => {
    // Crear la conexión WebSocket al backend
    const ws = new WebSocket(`http://127.0.0.1:8000/ws/game/${game_id}`);

    // Manejar la apertura de la conexión
    ws.onopen = () => {
      console.log("Conectado al WebSocket del lobby");
    };

    // Manejar los mensajes recibidos
    ws.onmessage = (event) => {
      const data = JSON.parse(event.data);

      console.log("Mensaje recibido:", data);
      console.log("Player_id:", playerID);

      if (data.state === "playing") {
        navigate(`/game`);
      }

      if (data.error === "Game not found") {
        setIsModalOpen(true);
      }

      setNumberOfPlayers(data.players);

      // Agregar la clave 'key' a cada objeto en data.player_details
      const playersWithKeys = data.player_details.map((player, index) => ({
        ...player,
        key: index,
      }));

      setPlayersList(playersWithKeys);
    };

    // Manejar el cierre de la conexión
    ws.onclose = () => {
      console.log("Conexión WebSocket cerrada");
    };

    // Guardar el WebSocket en el estado para usarlo después
    setSocket(ws);

    // Limpiar el WebSocket al desmontar el componente
    return () => {
      ws.close();
    };
  }, []);

  return (
    <div>
      <LobbySquares />
      <h1 className="text-blancofondo font-sans uppercase m-auto pt-40 text-center text-4xl">
        {gameName}
      </h1>
      <TablePlayers playersList={playersList} isCreator={isCreator} />
      <div className="flex flex-col">
        {playerID === isCreator && numberOfPlayers === maxNumberOfPlayers && (
          <Button
            className="flex m-auto my-3 text-blancofondo"
            type="primary"
            disabled={!isCreator}
            onClick={() => start(game_id)}
          >
            Iniciar Partida
          </Button>
        )}
        <Button
          className="flex m-auto my-3"
          danger
          ghost
          onClick={() => quitRoom(game_id)}
        >
          Abandonar
        </Button>
      </div>
      <div>
        <Modal
          title="Oops!"
          open={isModalOpen}
          footer={null}
          className="text-center"
          closable={false}
        >
          <p className="text-negrofondo text-lg ">El creador decidio irse</p>
          <Button
            className="mt-5 text-blancofondo"
            type="primary"
            onClick={() => navigate(`/lobby`)}
          >
            Volver al Lobby
          </Button>
        </Modal>
      </div>
    </div>
  );
};

export default WaitingRoom;
