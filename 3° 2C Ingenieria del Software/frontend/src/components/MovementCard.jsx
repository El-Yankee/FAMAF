import React, { useEffect, useState } from "react";
import { Card, List } from "antd";
import { PlayerMovements } from "../services/GameServices";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";
import mov1 from "../assets/images/mov1.svg"; // Add your image path here
import mov2 from "../assets/images/mov2.svg"; // Add your image path here
import mov3 from "../assets/images/mov3.svg"; // Add your image path here
import mov4 from "../assets/images/mov4.svg"; // Add your image path here
import mov5 from "../assets/images/mov5.svg"; // Add your image path here
import mov6 from "../assets/images/mov6.svg"; // Add your image path here
import mov7 from "../assets/images/mov7.svg"; // Add your image path here

const MovementCard = ({ onSelectMovCard, updateboard, vertical }) => {
  const { playerID } = usePlayerContext();
  const { game_id } = useGameContext();
  const [data, setData] = useState([]);

  const getMoves = async () => {
    console.log("Success");

    try {
      // Esperamos la resolución de la promesa de GameData
      const response = await PlayerMovements(playerID, game_id);

      if (response) {
        console.log("Moves:", response);

        return response;
      }
    } catch (error) {
      console.error("Error getting game data", error);
    }
  };

  const handleCardClick = (movementid) => {
    console.log(`Card with title ${movementid} clicked`);
    onSelectMovCard(movementid);
  };

  const getImageForTitle = (movementid) => {
    switch (movementid) {
      case 1:
        return mov1;
      case 2:
        return mov2;
      case 3:
        return mov3;
      case 4:
        return mov4;
      case 5:
        return mov5;
      case 6:
        return mov6;
      case 7:
        return mov7;
      default:
        return mov1;
    }
  };

  useEffect(() => {
    getMoves().then((response) => {
      setData(response.ids_of_movement_charts);
      console.log("Data:", response);
    });
  }, [updateboard]);

  return (
    <List
      grid={{
        gutter: 10,
        column: vertical ? 1 : 3,
      }}
      dataSource={data}
      renderItem={(item) => (
        <List.Item style={{ display: "flex", justifyContent: "center" }}>
          <Card
            className="bg-negrofondo hover:bg-blancofondo"
            bordered={false}
            hoverable
            style={{
              width: 284 / 3,
              height: 425 / 3,
            }}
            cover={<img alt={item} src={getImageForTitle(item)} />}
            onClick={() => handleCardClick(item)}
          />
        </List.Item>
      )}
    />
  );
};

export default MovementCard;
