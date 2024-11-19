import React, { useEffect, useState, useRef } from "react";
import { Button, Modal } from "antd";
import { useNavigate } from "react-router-dom";
import MovementCard from "../components/MovementCard";
import FigureCard from "../components/FigureCard";
import {
  ChangeTurn,
  LeaveGame,
  GameData,
  DeleteGame,
  PossiblesMoves,
  SwapTiles,
  UndoMovement,
  UndoAllMovements,
  UseFigureCard,
  BlockFigureCard,
} from "../services/GameServices";
import confetti from "canvas-confetti";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";
import { UndoOutlined } from "@ant-design/icons";
import music from "../assets/sounds/musicaMario.mp3";
import mario from "../assets/images/iconoMario.png";

const GamePage = () => {
  const navigate = useNavigate();
  const audioRef = useRef(null); // Referencia para el elemento <audio>
  const [turn, setTurn] = useState(null);
  const [socket, setSocket] = useState(null);
  const [gamestate, setGamestate] = useState();
  const [playersList, setPlayersList] = useState([]);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [modalQuit, setModalQuit] = useState(false);
  const { playerID } = usePlayerContext();
  const { game_id } = useGameContext();
  const [board, setBoard] = useState([]);
  const [winnerPlayer, setWinnerPlayer] = useState(null);
  const [SelectMovCard, setSelectMovCard] = useState(null);
  const [SelectFigCard, setSelectFigCard] = useState(null);
  const [SelectFigCardId, setSelectFigCardId] = useState(null);
  const [SelectPlayer, setSelectPlayer] = useState(null);
  const [SelectFirstTitle, setSelectFirstTitle] = useState(null);
  const [SelectSecondTitle, setSelectSecondTitle] = useState(null);
  const [PossibleTiles1, setPossibleTiles1] = useState();
  const [PossibleTiles2, setPossibleTiles2] = useState();
  const [PossibleTiles3, setPossibleTiles3] = useState();
  const [PossibleTiles4, setPossibleTiles4] = useState();
  const [Player1, setPlayer1] = useState(null);
  const [Player2, setPlayer2] = useState(null);
  const [Player3, setPlayer3] = useState(null);
  const [Player4, setPlayer4] = useState(null);
  const [blockColor, setBlockColor] = useState();
  const [secondsLeft, setSecondsLeft] = useState(120);
  const [isRunning, setIsRunning] = useState(false);
  const [messages, setMessages] = useState([]);
  const [message, setMessage] = useState("");
  const [chat_socket, setChat_Socket] = useState(null);
  const chatContainerRef = useRef(null);

  const showModal = () => {
    setIsModalOpen(true);
  };

  const handleOk = () => {
    setModalQuit(false);
  };

  const handleCancel = () => {
    setModalQuit(false);
  };

  const winner = () => {
    var duration = 1 * 100;
    var end = Date.now() + duration;

    (function frame() {
      // launch a few confetti from the left edge
      confetti({
        particleCount: 6,
        angle: 60,
        spread: 55,
        origin: { x: 0 },
      });
      // and launch a few from the right edge
      confetti({
        particleCount: 6,
        angle: 120,
        spread: 55,
        origin: { x: 1 },
      });

      // keep going until we are out of time
      if (Date.now() < end) {
        requestAnimationFrame(frame);
      }
    })();
  };

  const formatTime = (sec) => {
    const minutes = Math.floor(sec / 60);
    const remainingSeconds = sec % 60;
    return `${String(minutes).padStart(2, "0")}:${String(
      remainingSeconds
    ).padStart(2, "0")}`;
  };

  const quitRoom = async (game_id) => {
    console.log("Success");

    try {
      console.log("Player ID:", playerID);
      console.log("Game ID:", game_id);

      if (playerID === turn) {
        await passTurn(game_id);
      }

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
  };

  const passTurn = async (game_id) => {
    console.log("Success");

    try {
      console.log("Player ID:", playerID);
      console.log("Game ID:", game_id);
      // Esperamos la resolución de la promesa de LeaveGame
      const response = await ChangeTurn(playerID, game_id);

      if (response) {
        console.log("New Game Info:", response);
        resetSelect();
      }
    } catch (error) {
      console.error("Error getting new game data", error);
    }
  };

  const undoMov = async (game_id) => {
    console.log("Success");

    try {
      // Esperamos la resolución de la promesa de UndoMovement
      const response = await UndoMovement(playerID, game_id);

      if (response) {
        console.log("Undo Mov:", response);
      }
    } catch (error) {
      console.error("Error getting new game data", error);
    }
  };

  const undoallMov = async (game_id) => {
    console.log("Success");

    try {
      // Esperamos la resolución de la promesa de UndoAllMovements
      const response = await UndoAllMovements(playerID, game_id);

      if (response) {
        console.log("Undo All Mov:", response);
      }
    } catch (error) {
      console.error("Error getting new game data", error);
    }
  };

  const finishGame = async (game_id) => {
    console.log("Success");

    try {
      console.log("Game ID:", game_id);

      if (playersList.length === 1) {
        // If the player is the last one in the game, delete the game
        const response = await DeleteGame(game_id);
        if (response) {
          console.log("Game Deleted:", response);
          navigate(`/lobby`);
        }
      } else {
        // Otherwise, just leave the game
        const response = await LeaveGame(playerID, game_id);
        if (response) {
          console.log("Left Game:", response);
          navigate(`/lobby`);
        }
      }
    } catch (error) {
      console.error("Error finishing the game", error);
    }
  };

  const handleSquareClick = (index) => {
    if (index === SelectFirstTitle) {
      resetSelect();
    } else if (SelectFirstTitle === null) {
      setSelectFirstTitle(index);
      console.log(`First square ${index} clicked`);
    } else {
      setSelectSecondTitle(index);
      console.log(`Second square ${index} clicked`);
    }
  };

  const resetSelect = () => {
    setSelectFirstTitle(null);
    setSelectSecondTitle(null);
    setSelectMovCard(null);
    setPossibleTiles1(null);
    setPossibleTiles2(null);
    setPossibleTiles3(null);
    setPossibleTiles4(null);
  };

  const invertBoard = (board, size) => {
    const rows = [];
    for (let i = 0; i < size; i++) {
      rows.push(board.slice(i * size, (i + 1) * size));
    }
    // Invertir el orden de las filas
    const invertedRows = rows.reverse();
    return invertedRows.flat();
  };

  const gameBoard = (board) => {
    const size = 6;
    const invertedBoard = invertBoard(board, size);

    return invertedBoard.map((item) => (
      <Button
        key={item.id}
        disabled={playerID !== turn}
        onClick={() => handleSquareClick(item.id)}
        style={{
          width: "60px",
          height: "60px",
          backgroundColor:
            item.color === "red"
              ? "#FF5959"
              : item.color === "blue"
              ? "#45B3EB"
              : item.color === "green"
              ? "#4ade80"
              : item.color === "yellow"
              ? "#FAD05A"
              : item.color,
          border:
            item.id === PossibleTiles1 ||
            item.id === PossibleTiles2 ||
            item.id === PossibleTiles3 ||
            item.id === PossibleTiles4
              ? "5px solid #FAFAFA"
              : item.highlight
              ? `5px solid ${
                  item.color === "red"
                    ? "#bf4343"
                    : item.color === "blue"
                    ? "#3486b0"
                    : item.color === "green"
                    ? "#38a660"
                    : item.color === "yellow"
                    ? "#bb9c44"
                    : "black"
                }`
              : "none",
          boxShadow:
            item.id === SelectFirstTitle ? "0 0 10px 5px #FAFAFA" : "none",
        }}
      ></Button>
    ));
  };

  const putSwap = async () => {
    console.log("Success");

    try {
      // Esperamos la resolución de la promesa de SwapTiles
      const response = await SwapTiles(
        playerID,
        game_id,
        SelectMovCard,
        SelectFirstTitle,
        SelectSecondTitle
      );

      if (response) {
        console.log("Swap:", response);

        return response;
      }
    } catch (error) {
      console.error("Error getting game data", error);
    }
  };

  const swap = () => {
    console.log("SelectFirstTitle:", SelectFirstTitle);
    console.log("SelectSecondTitle:", SelectSecondTitle);

    if (
      SelectSecondTitle === PossibleTiles1 ||
      SelectSecondTitle === PossibleTiles2 ||
      SelectSecondTitle === PossibleTiles3 ||
      SelectSecondTitle === PossibleTiles4
    ) {
      putSwap().then((response) => {
        if (response) {
          console.log("Swap response:", response);
          setSelectMovCard(null);
          setSelectFirstTitle(null);
          setSelectSecondTitle(null);
          setPossibleTiles1(null);
          setPossibleTiles2(null);
          setPossibleTiles3(null);
          setPossibleTiles4(null);
        }
      });
    }
  };

  const getPossibleMoves = async () => {
    console.log("Success");

    try {
      // Esperamos la resolución de la promesa de PossiblesMoves
      const response = await PossiblesMoves(
        playerID,
        game_id,
        SelectMovCard,
        SelectFirstTitle
      );

      if (response) {
        console.log("Possible Moves:", response);

        return response;
      }
    } catch (error) {
      console.error("Error getting game data", error);
    }
  };

  const startMove = () => {
    if (SelectMovCard && SelectFirstTitle && playerID === turn) {
      console.log("Carta de movimineto:", SelectMovCard);
      console.log("Ficha:", SelectFirstTitle);

      getPossibleMoves().then((response) => {
        if (response) {
          setPossibleTiles1(response.tile_1);
          setPossibleTiles2(response.tile_2);
          setPossibleTiles3(response.tile_3);
          setPossibleTiles4(response.tile_4);
        }
      });
    } else {
      alert("Selecciona ambos componentes primero");
    }
  };

  const useFigure = async () => {
    console.log("Success");

    if (SelectPlayer && SelectPlayer !== playerID) {
      try {
        // Esperamos la resolución de la promesa de BlockFigureCard
        const response = await BlockFigureCard(
          playerID,
          SelectPlayer,
          game_id,
          SelectFigCardId,
          SelectFirstTitle
        );

        if (response) {
          console.log("Figure block:", response);
          resetSelect();

          return response;
        }
      } catch (error) {
        console.error("Error getting game data", error);
      }
    } else {
      try {
        // Esperamos la resolución de la promesa de UseFigureCard
        const response = await UseFigureCard(
          playerID,
          game_id,
          SelectFigCard,
          SelectFirstTitle
        );

        if (response) {
          console.log("Figure use:", response);
          resetSelect();

          return response;
        }
      } catch (error) {
        console.error("Error getting game data", error);
      }
    }
  };

  const getGameInfo = async (game_id) => {
    console.log("LLame a GAMEINFO");

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

  const sendMessage = () => {
    if (message.trim() && chat_socket) {
      chat_socket.send(message); // Enviar el mensaje al servidor WebSocket
      setMessage("");
    }
  };

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
      const startTimestamp = parseInt(data.timestamp, 10); // Timestamp de inicio en segundos
      const currentTimestamp = Math.floor(Date.now() / 1000); // Timestamp actual en segundos
      const elapsed = currentTimestamp - startTimestamp; // Tiempo transcurrido desde el inicio

      console.log("Mensaje recibido:", data);

      setTurn(data.turn);
      setBoard(data.board);
      setPlayersList(data.player_details);
      setBlockColor(data.prohibited_color);

      const player1 = data.player_details.find(
        (player) => player.player_id === playerID
      );
      const otherPlayers = data.player_details.filter(
        (player) => player.player_id !== playerID
      );

      setPlayer1(player1);
      setPlayer2(otherPlayers[0]);
      setPlayer3(otherPlayers[1]);
      setPlayer4(otherPlayers[2]);

      if (data.players === 1) {
        setWinnerPlayer(data.player_details[0].player_name);
        showModal();
      }

      const winnerPlayer = data.player_details.find(
        (player) => player.number_of_figure_card === 0
      );

      if (winnerPlayer) {
        setWinnerPlayer(winnerPlayer.player_name);
        showModal();
      }

      if (elapsed < 120) {
        setSecondsLeft(120 - elapsed); // Calcula el tiempo restante
        setIsRunning(true);
      } else {
        setSecondsLeft(0); // Si ya pasaron 2 minutos, el temporizador llega a cero
        setIsRunning(false);
      }
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

  useEffect(() => {
    if (!gamestate) {
      getGameInfo(game_id).then((response) => {
        if (response) {
          setGamestate(response.state);
          setPlayersList(response.player_details);
        }
      });
    }
  }, []);

  useEffect(() => {
    if (SelectFirstTitle !== null && SelectMovCard !== null) {
      startMove();
    }

    if (SelectFirstTitle !== null && SelectSecondTitle !== null) {
      swap();
    }

    if (SelectFigCard !== null && SelectFirstTitle !== null) {
      useFigure();
    }
  }, [SelectFirstTitle, SelectSecondTitle, SelectMovCard, SelectFigCard]);

  useEffect(() => {
    if (isRunning && secondsLeft > 0) {
      const interval = setInterval(() => {
        setSecondsLeft((prevSeconds) => prevSeconds - 1);
      }, 1000);

      return () => clearInterval(interval); // Limpia el intervalo cuando el temporizador se detiene
    } else {
      if (turn != null) {
        passTurn(game_id).then(() => {
          console.log("Turno pasado");
        });
      }
    }
  }, [isRunning, secondsLeft]);

  useEffect(() => {
    // Crear la conexión WebSocket al backend
    const chat_ws = new WebSocket(
      `http://127.0.0.1:8000/ws/chat/${game_id}/${playerID}`
    );

    // Manejar la apertura de la conexión
    chat_ws.onopen = () => {
      console.log("Conectado al WebSocket del chat");
    };

    // Manejar los mensajes recibidos
    chat_ws.onmessage = (event) => {
      const data = JSON.parse(event.data);

      console.log("Chat:", data);

      // Formatear el mensaje en el formato deseado
      if (data.player_name === "sistema") {
        const formattedMessage = `${data.message.toUpperCase()}`;
        setMessages((prevMessages) => [...prevMessages, formattedMessage]);
      } else {
        const formattedMessage = `${data.player_name} : ${data.message}`;
        setMessages((prevMessages) => [...prevMessages, formattedMessage]);
      }
    };

    // Manejar el cierre de la conexión
    chat_ws.onclose = () => {
      console.log("Conexión WebSocket cerrada");
    };

    // Guardar el WebSocket en el estado para usarlo después
    setChat_Socket(chat_ws);

    // Limpiar el WebSocket al desmontar el componente
    return () => {
      chat_ws.close();
    };
  }, [game_id, playerID]);

  useEffect(() => {
    if (chatContainerRef.current) {
      chatContainerRef.current.scrollTop =
        chatContainerRef.current.scrollHeight;
    }
  }, [messages]);

  return (
    <div className="m-auto flex flex-col items-center justify-center min-h-screen">
      <div className="Cards_Top_Player  w-80">
        {Player2 && (
          <>
            <h2 className=" text-blancofondo text-center font-sans uppercase">
              {Player2.player_name}
            </h2>
            <h2 className=" text-blancofondo text-center font-sans uppercase">
              Cartas de Figuras Restantes: {Player2.number_of_figure_card}
            </h2>
            <FigureCard
              playersList={Player2}
              onSelectFigCard={(title) => setSelectFigCard(title)}
              onSelectFigCardId={(id) => setSelectFigCardId(id)}
              onSelectPlayer={(player) => setSelectPlayer(player)}
              updateboard={board}
            />
          </>
        )}
      </div>
      <div className="flex">
        <div className="Cards_Left_Player w-80 flex items-center">
          {Player3 && (
            <>
              <div>
                <h2 className=" text-blancofondo text-center font-sans uppercase m-4">
                  {Player3.player_name}
                </h2>
                <h2 className=" text-blancofondo text-center font-sans uppercase">
                  Cartas de Figuras Restantes: {Player3.number_of_figure_card}
                </h2>
              </div>
              <FigureCard
                playersList={Player3}
                onSelectFigCard={(title) => setSelectFigCard(title)}
                onSelectFigCardId={(id) => setSelectFigCardId(id)}
                onSelectPlayer={(player) => setSelectPlayer(player)}
                updateboard={board}
                vertical={true}
              />
            </>
          )}
        </div>
        <div
          className="container"
          style={{
            display: "grid",
            gridTemplateColumns: "repeat(6, 60px)", // Increased size
            gap: "10px", // Increased gap
            justifyContent: "center",
            marginBottom: "20px",
            marginTop: "20px",
          }}
        >
          {gameBoard(board)}
        </div>
        <div className="Cards_Right_Player w-80 flex items-center">
          {Player4 && (
            <>
              <FigureCard
                playersList={Player4}
                onSelectFigCard={(title) => setSelectFigCard(title)}
                onSelectFigCardId={(id) => setSelectFigCardId(id)}
                onSelectPlayer={(player) => setSelectPlayer(player)}
                updateboard={board}
                vertical={true}
              />
              <div>
                <h2 className=" text-blancofondo text-center font-sans uppercase m-4">
                  {Player4.player_name}
                </h2>
                <h2 className=" text-blancofondo text-center font-sans uppercase">
                  Cartas de Figuras Restantes: {Player4.number_of_figure_card}
                </h2>
              </div>
            </>
          )}
        </div>
      </div>
      <div className="Cards_Bottom_Player  w-80">
        {Player1 && (
          <>
            <h2 className=" text-blancofondo text-center font-sans uppercase">
              Cartas de Figuras Restantes: {Player1.number_of_figure_card}
            </h2>
            <FigureCard
              playersList={Player1}
              onSelectFigCard={(title) => setSelectFigCard(title)}
              onSelectFigCardId={(id) => setSelectFigCardId(id)}
              onSelectPlayer={(player) => setSelectPlayer(player)}
              updateboard={board}
            />
            <MovementCard
              onSelectMovCard={(title) => setSelectMovCard(title)}
              updateboard={board}
            />
          </>
        )}
      </div>

      <div className="botones flex flex-col gap-4 fixed bottom-20 right-20 ">
        <div className="turn text-blancofondo text-center font-sans uppercase">
          <h3>Turno de:</h3>
          {playersList.map((player) => (
            <div key={player.player_id}>
              {player.player_id === turn && <h2>{player.player_name}</h2>}
            </div>
          ))}
        </div>
        <div className="blockColor text-blancofondo text-center font-sans uppercase">
          <h3>Color Bloqueado:</h3>
          <div
            className="littleSquare h-8 rounded-xl"
            style={{
              backgroundColor:
                blockColor === "blue"
                  ? "#45B3EB"
                  : blockColor === "red"
                  ? "#FF5959"
                  : blockColor === "green"
                  ? "#4ade80"
                  : blockColor === "yellow"
                  ? "#FAD05A"
                  : "#FAFAFA",
            }}
          ></div>
        </div>
        <Button
          className="text-blancofondo"
          type="primary"
          disabled={playerID !== turn}
          style={{
            backgroundColor: playerID !== turn ? "#eeecec" : "#1677ff",
          }}
          onClick={() => undoMov(game_id)}
          icon={
            <UndoOutlined
              style={{
                fontSize: "24px",
                display: "block",
                margin: "0 auto",
              }}
            />
          }
        >
          Deshacer Movimiento
        </Button>
        <Button
          className="text-blancofondo"
          type="primary"
          disabled={playerID !== turn}
          style={{
            backgroundColor: playerID !== turn ? "#eeecec" : "#1677ff",
          }}
          onClick={() => undoallMov(game_id)}
          icon={
            <UndoOutlined
              style={{
                fontSize: "24px",
                display: "block",
                margin: "0 auto",
              }}
            />
          }
        >
          Deshacer todos los Movimientos
        </Button>
        <Button
          className="text-blancofondo"
          type="primary"
          disabled={playerID !== turn}
          style={{
            backgroundColor: playerID !== turn ? "#eeecec" : "#1677ff",
          }}
          onClick={() => passTurn(game_id)}
        >
          Terminar Turno
        </Button>

        <Button
          className="flex flex-col gap-4"
          danger
          ghost
          onClick={() => setModalQuit(true)}
        >
          Salir
        </Button>
      </div>

      <div className="chat flex flex-col gap-4 fixed bottom-20 left-20 text-blancofondo">
        <div className="App">
          <div
            ref={chatContainerRef}
            style={{
              maxHeight: "200px",
              overflowY: "auto",
              border: "background-color: #FAFAFA",
              padding: "10px",
            }}
          >
            {messages.map((msg, index) => (
              <div key={index}>{msg}</div>
            ))}
          </div>
          <input
            className="text-negrofondo p-3 rounded-xl"
            value={message}
            onChange={(e) => setMessage(e.target.value)}
            placeholder="Escribe tu mensaje..."
            onKeyUp={(e) => {
              if (e.key === "Enter") {
                sendMessage();
              }
            }}
          />
          <Button
            className="text-blancofondo ml-4"
            type="primary"
            onClick={sendMessage}
          >
            Enviar
          </Button>
        </div>
        <div>
          <h1>Temporizador</h1>
          <h2>{formatTime(secondsLeft)}</h2>
          {secondsLeft === 0 && <h3>¡Tiempo terminado!</h3>}
        </div>
      </div>

      <audio ref={audioRef} src={music} loop />
      <div
        style={{
          position: "fixed",
          bottom: 0,
          left: 0,
          margin: "10px",
        }}
      >
        <Button
          className="bg-negrofondo"
          icon={<img src={mario} alt="Age Icon" />}
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

      <div className="modal_confirmacion">
        <Modal
          title="Salir de la Partida"
          open={modalQuit}
          footer={null}
          className="text-center"
          onOk={handleOk}
          onCancel={handleCancel}
        >
          <p className="text-negrofondo text-lg ">
            Te gustaria volver al lobby o abandonar la partida?
          </p>
          <div className="flex gap-4 align-middle justify-center">
            <Button
              className="mt-5 text-blancofondo"
              type="primary"
              onClick={() => navigate(`/lobby`)}
            >
              Volver al Lobby
            </Button>
            <Button
              className="mt-5 text-blancofondo"
              type="primary"
              onClick={() => quitRoom(game_id)}
            >
              Abandonar
            </Button>
          </div>
        </Modal>
      </div>

      <div className="modal_victoria">
        <Modal
          title="¡Felicidades!"
          open={isModalOpen}
          footer={null}
          className="text-center"
          closable={false}
        >
          <p className="text-negrofondo text-lg ">
            {winnerPlayer} ha ganado la partida
          </p>
          <Button
            className="mt-5 text-blancofondo"
            type="primary"
            onClick={() => finishGame(game_id)}
          >
            Volver al Lobby
          </Button>
        </Modal>
        {isModalOpen && winner()}
      </div>
    </div>
  );
};
export default GamePage;
