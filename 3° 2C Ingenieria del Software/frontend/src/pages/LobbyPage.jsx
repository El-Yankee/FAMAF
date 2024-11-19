import React from "react";
import { useEffect, useState, useRef } from "react";
import { Button } from "antd";
import { useNavigate } from "react-router-dom";
import LobbySquares from "../components/LobbySquares";
import TableGames from "../components/TableGames";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import music from "../assets/sounds/musicaMenuAgeII.mp3";
import age from "../assets/images/iconoAge.jpeg";

const LobbyPage = () => {
  const navigate = useNavigate();
  const [socket, setSocket] = useState(null);
  const [partidas, setPartidas] = useState([]);
  // Obtener playerID desde el contexto
  const { playerID } = usePlayerContext();

  const audioRef = useRef(null); // Referencia para el elemento <audio>

  console.log("playerID", playerID);

  const handleClick = () => {
    navigate(`/creategame`);
  };

  useEffect(() => {
    // Crear la conexión WebSocket al backend
    const ws = new WebSocket(`http://127.0.0.1:8000/ws/${playerID}`);

    // Manejar la apertura de la conexión
    ws.onopen = () => {
      console.log("Conectado al WebSocket del lobby");
    };

    // Manejar los mensajes recibidos
    ws.onmessage = (event) => {
      const data = JSON.parse(event.data);

      console.log("Mensaje recibido:", data);

      // Agregar la clave 'key' a cada objeto en data
      const dataWithKeys = data.map((item, index) => ({
        ...item,
        key: index,
      }));

      // Actualizar la lista de partidas con los datos modificados
      setPartidas(dataWithKeys);
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
        Lobby
      </h1>
      <TableGames gamesList={partidas} />
      <Button
        className="flex m-auto my-3 text-blancofondo"
        type="primary"
        htmlType="submit"
        size="large"
        onClick={handleClick}
      >
        Crear Partida
      </Button>

      <audio ref={audioRef} src={music} loop />
      <div
        style={{
          position: "fixed",
          bottom: 0,
          left: 0,
          margin: "10px",
          opacity: 0,
          transition: "opacity 0.3s",
        }}
        onMouseEnter={(e) => (e.currentTarget.style.opacity = 1)}
        onMouseLeave={(e) => (e.currentTarget.style.opacity = 0)}
      >
        <Button
          className="bg-negrofondo"
          icon={<img src={age} alt="Age Icon" />}
          onClick={() => {
            if (audioRef.current.paused) {
              audioRef.current.play();
            } else {
              audioRef.current.pause();
            }
          }}
        ></Button>
        {!audioRef.current?.paused && (
          <div
            style={{ display: "flex", alignItems: "center", marginTop: "10px" }}
          >
            <span role="img" aria-label="music-note">
              🎵
            </span>
            <input
              type="range"
              min="0"
              max="1"
              step="0.01"
              defaultValue="1"
              onChange={(e) => {
                audioRef.current.volume = e.target.value;
              }}
              style={{ width: "100px", marginLeft: "10px" }}
            />
          </div>
        )}
      </div>
    </div>
  );
};
export default LobbyPage;
