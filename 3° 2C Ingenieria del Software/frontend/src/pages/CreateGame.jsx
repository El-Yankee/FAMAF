import React from "react";
import { Form, Button, Select, Input } from "antd";
import { useNavigate } from "react-router-dom";
import LobbySquares from "../components/LobbySquares";
import { CreateAGame } from "../services/CreateGameServices";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";

const CreateGame = () => {
  const navigate = useNavigate();
  // Obtener playerID desde el contexto
  const { playerID } = usePlayerContext();
  // Obtener game_id desde el contexto
  const { setGameID } = useGameContext();

  const onFinish = async (values) => {
    console.log("Success:", values);

    if (values.nombre.toLowerCase() === "becarefuleense") {
      navigate(`/credits`);
    } else {
      try {
        // Esperamos la resolución de la promesa de JoinLobby
        const response = await CreateAGame(
          playerID,
          values.nombre,
          values.jugadores,
          values.password
        );

        if (response) {
          console.log("Lobby response:", response);

          // Actualizar el gameID en el contexto
          setGameID(response.game_id);

          navigate(`/waitingRoom`);
        }
      } catch (error) {
        console.error("Error joining lobby:", error);
      }
    }
  };

  const goBack = () => {
    console.log("Success");

    navigate(`/lobby`);
  };

  const onFinishFailed = (errorInfo) => {
    console.log("Failed:", errorInfo);
  };

  return (
    <div>
      <LobbySquares />
      <h1 className="text-blancofondo font-sans uppercase m-auto pt-40 text-center text-4xl">
        Crear partida
      </h1>
      <Form
        className="bg-negrofondo p-2"
        name="Crear Partida"
        style={{
          maxWidth: 280,
          margin: "0 auto", // Center the form horizontally
          display: "flex",
          flexDirection: "column",
          alignItems: "center", // Center the form items horizontally
        }}
        onFinish={onFinish}
        onFinishFailed={onFinishFailed}
        autoComplete="off"
      >
        <Form.Item
          name="nombre"
          style={{ width: "100%" }}
          rules={[
            {
              validator: (_, value) => {
                if (!value) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      ¡Ingresar el nombre es obligatorio!
                    </span>
                  );
                }
                if (!/^[a-zA-Z0-9]+$/.test(value)) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      ¡Solo caracteres alfanuméricos!
                    </span>
                  );
                }
                if (value.length > 20) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      ¡No más de 20 caracteres!
                    </span>
                  );
                }
                return Promise.resolve();
              },
            },
          ]}
        >
          <Input
            className="bg-blancofondo"
            placeholder="Ingresar nombre partida"
          />
        </Form.Item>

        <Form.Item
          name="password"
          style={{ width: "100%" }}
          rules={[
            {
              validator: (_, value) => {
                if (value && !/^[a-zA-Z0-9]+$/.test(value)) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      ¡Solo caracteres alfanuméricos!
                    </span>
                  );
                }
                if (value && value.length < 4) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      ¡Deben ser por lo menos 4 caracteres!
                    </span>
                  );
                }
                if (value && value.length > 20) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      ¡No más de 20 caracteres!
                    </span>
                  );
                }
                return Promise.resolve();
              },
            },
          ]}
        >
          <Input
            className="bg-blancofondo"
            placeholder="Ingresar contraseña (opcional)"
          />
        </Form.Item>

        <Form.Item
          name="jugadores"
          style={{ width: "100%" }}
          rules={[
            { required: true, message: "Por favor selecciona una opción" },
          ]}
        >
          <Select placeholder="Selecciona cantidad jugadores">
            <Select.Option value="2">2 jugadores</Select.Option>
            <Select.Option value="3">3 jugadores</Select.Option>
            <Select.Option value="4">4 jugadores</Select.Option>
          </Select>
        </Form.Item>

        <Form.Item name="boton">
          <Button className="text-blancofondo" type="primary" htmlType="submit">
            Crear
          </Button>
        </Form.Item>
      </Form>
      <Button className="flex m-auto" danger ghost onClick={() => goBack()}>
        Atras
      </Button>
    </div>
  );
};

export default CreateGame;
