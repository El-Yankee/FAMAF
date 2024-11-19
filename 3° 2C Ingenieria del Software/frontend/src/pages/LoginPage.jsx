import React from "react";
import { Button, Checkbox, Form, Input } from "antd";
import { useNavigate } from "react-router-dom";
import LobbySquares from "../components/LobbySquares";
import { JoinLobby } from "../services/LobbyServices";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import MovingBG from "../components/MovingBG.jsx";

const LoginPage = () => {
  const navigate = useNavigate();
  // Obtener setPlayerID desde el contexto
  const { setPlayerID } = usePlayerContext();

  const onFinish = async (values) => {
    console.log("Success:", values);

    try {
      // Esperamos la resolución de la promesa de JoinLobby
      const response = await JoinLobby(values.username);

      if (response) {
        console.log("Lobby response:", response);
        // Actualizar el playerID en el contexto
        setPlayerID(response.player_id);

        // Navegamos solo cuando la respuesta está lista
        navigate("/lobby");
      }
    } catch (error) {
      console.error("Error joining lobby:", error);
    }
  };

  const onFinishFailed = (errorInfo) => {
    console.log("Failed:", errorInfo);
  };

  return (
    <div>
      <MovingBG />
      {/* <LobbySquares /> */}
      <h1 className="text-blancofondo font-sans uppercase m-auto pt-40 text-center text-4xl relative">
        El Switcher
      </h1>
      <Form
        className="bg-negrofondo p-2"
        name="Login"
        style={{
          maxWidth: 235,
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
          name="username"
          style={{ width: "100%" }}
          rules={[
            {
              validator: (_, value) => {
                if (!value) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      El nombre es obligatorio!
                    </span>
                  );
                }
                if (!/^[a-zA-Z0-9]+$/.test(value)) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>
                      Solo caracteres alfanuméricos!
                    </span>
                  );
                }
                if (value.length > 8) {
                  return Promise.reject(
                    <span style={{ fontSize: 13 }}>Menos de 8 caracteres!</span>
                  );
                }
                return Promise.resolve();
              },
            },
          ]}
        >
          <Input
            placeholder="Ingresar nombre jugador"
            className="bg-blancofondo"
          />
        </Form.Item>

        <Form.Item>
          <Button className="text-blancofondo" type="primary" htmlType="submit">
            Jugar
          </Button>
        </Form.Item>
      </Form>
    </div>
  );
};
export default LoginPage;
