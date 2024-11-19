import React, { useRef, useState } from "react";
import { Space, Table, Button, Input, Modal, message } from "antd";
import { SearchOutlined, LockOutlined } from "@ant-design/icons";
import Highlighter from "react-highlight-words";
import { JoinGame } from "../services/LobbyServices";
import { useNavigate } from "react-router-dom";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";

const { Column } = Table;

const TableGames = ({ gamesList }) => {
  const navigate = useNavigate();
  const { playerID } = usePlayerContext();
  const { setGameID } = useGameContext();
  const [searchText, setSearchText] = useState("");
  const [searchedColumn, setSearchedColumn] = useState("");
  const searchInput = useRef(null);
  const [isModalVisible, setIsModalVisible] = useState(false);
  const [selectedGameId, setSelectedGameId] = useState(null);
  const [password, setPassword] = useState("");

  const showModal = (game_id) => {
    setSelectedGameId(game_id);
    setIsModalVisible(true);
  };

  const handleOk = () => {
    join(selectedGameId, password);
    setPassword("");
  };

  const handleCancel = () => {
    setIsModalVisible(false);
    setPassword("");
  };

  const handleSearch = (selectedKeys, confirm, dataIndex) => {
    confirm();
    setSearchText(selectedKeys[0]);
    setSearchedColumn(dataIndex);
  };

  const handleReset = (clearFilters) => {
    clearFilters();
    setSearchText("");
  };

  const getColumnSearchProps = (dataIndex) => ({
    filterDropdown: ({
      setSelectedKeys,
      selectedKeys,
      confirm,
      clearFilters,
      close,
    }) => (
      <div
        style={{
          padding: 8,
        }}
        onKeyDown={(e) => e.stopPropagation()}
      >
        <Input
          ref={searchInput}
          placeholder={`Buscar ${dataIndex}`}
          value={selectedKeys[0]}
          onChange={(e) =>
            setSelectedKeys(e.target.value ? [e.target.value] : [])
          }
          onPressEnter={() => handleSearch(selectedKeys, confirm, dataIndex)}
          style={{
            marginBottom: 8,
            display: "block",
          }}
        />
        <Space>
          <Button
            type="primary"
            onClick={() => handleSearch(selectedKeys, confirm, dataIndex)}
            icon={<SearchOutlined />}
            size="small"
            style={{
              width: 90,
            }}
          >
            Buscar
          </Button>
          <Button
            onClick={() => clearFilters && handleReset(clearFilters)}
            size="small"
            style={{
              width: 90,
            }}
          >
            Limpiar
          </Button>
          <Button
            type="link"
            size="small"
            onClick={() => {
              confirm({
                closeDropdown: false,
              });
              setSearchText(selectedKeys[0]);
              setSearchedColumn(dataIndex);
            }}
          >
            Filtrar
          </Button>
          <Button
            type="link"
            size="small"
            onClick={() => {
              close();
            }}
          >
            Cerrar
          </Button>
        </Space>
      </div>
    ),
    filterIcon: (filtered) => (
      <SearchOutlined
        style={{
          color: filtered ? "#1677ff" : undefined,
        }}
      />
    ),
    onFilter: (value, record) =>
      record[dataIndex].toString().toLowerCase().includes(value.toLowerCase()),
    onFilterDropdownOpenChange: (visible) => {
      if (visible) {
        setTimeout(() => searchInput.current?.select(), 100);
      }
    },
    render: (text) =>
      searchedColumn === dataIndex ? (
        <Highlighter
          highlightStyle={{
            backgroundColor: "#ffc069",
            padding: 0,
          }}
          searchWords={[searchText]}
          autoEscape
          textToHighlight={text ? text.toString() : ""}
        />
      ) : (
        text
      ),
  });

  const join = async (game_id, game_password) => {
    console.log("Success");

    try {
      const response = await JoinGame(playerID, game_id, game_password);

      if (response) {
        console.log("New Game Info:", response);
        setIsModalVisible(false);
        setGameID(game_id);
        navigate(`/waitingRoom`);
      } else {
        message.error("Incorrect password. Please try again.");
      }
    } catch (error) {
      console.error("Error getting new game data", error);
      message.error("Incorrect password. Please try again.");
    }
  };

  const rejoin = async (game_id) => {
    console.log("Success");
    setGameID(game_id);
    navigate(`/game`);
  };

  return (
    <>
      <Table
        className="w-1/4 m-auto my-2 rounded-lg"
        dataSource={gamesList}
        pagination={{ pageSize: 5, size: "small" }}
        style={{ backgroundColor: "#FAFAFA" }}
      >
        <Column
          title={<div style={{ textAlign: "center" }}>Nombre Partida</div>}
          dataIndex="game_name"
          key="game_name"
          {...getColumnSearchProps("game_name")}
          render={(text, record) => (
            <span>
              {record.type === "Private" && (
                <LockOutlined style={{ marginRight: 8 }} />
              )}
              {text}
            </span>
          )}
        />
        <Column
          title={<div style={{ textAlign: "center" }}>Jugadores</div>}
          dataIndex="players"
          key="players"
          {...getColumnSearchProps("players")}
          render={(_, record) =>
            record.state === "playing"
              ? `${record.game_size} / ${record.game_size}`
              : `${record.players} / ${record.game_size}`
          }
        />
        <Column
          title={<div style={{ textAlign: "center" }}>Estado</div>}
          dataIndex="game_id"
          key="game_id"
          align="center"
          render={(_, record) => (
            <Space size="middle">
              {record.type === "Private" ? (
                record.player_details.some(
                  (player) => player.player_id === playerID
                ) ? (
                  <button
                    className="bg-green-500 hover:bg-green-700 text-blancofondo font-bold py-2 px-2 rounded"
                    onClick={() => rejoin(record.game_id)}
                  >
                    Reunirme
                  </button>
                ) : (
                  <button
                    className="bg-blue-500 hover:bg-blue-700 text-blancofondo font-bold py-2 px-2 rounded"
                    onClick={() => showModal(record.game_id)}
                  >
                    Unirme
                  </button>
                )
              ) : record.state === "waiting" &&
                record.players < record.game_size ? (
                <button
                  className="bg-blue-500 hover:bg-blue-700 text-blancofondo font-bold py-2 px-2 rounded"
                  onClick={() => join(record.game_id, "CAB")}
                >
                  Unirme
                </button>
              ) : record.state === "playing" ? (
                record.player_details.some(
                  (player) => player.player_id === playerID
                ) ? (
                  <button
                    className="bg-green-500 hover:bg-green-700 text-blancofondo font-bold py-2 px-2 rounded"
                    onClick={() => rejoin(record.game_id)}
                  >
                    Reunirme
                  </button>
                ) : (
                  <span className="text-verdeficha font-bold">Jugando</span>
                )
              ) : (
                <span className="text-rojoficha font-bold">Sala llena</span>
              )}
            </Space>
          )}
        />
      </Table>
      <Modal
        title="Ingresar contraseña"
        open={isModalVisible}
        onOk={handleOk}
        onCancel={handleCancel}
      >
        <Input.Password
          placeholder=""
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
      </Modal>
    </>
  );
};

export default TableGames;
