import axios from "axios";

export const GameData = async (game_id) => {
  try {
    const response = await axios.get(`http://127.0.0.1:8000/game/${game_id}`);

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const LeaveGame = async (player_id, game_id) => {
  try {
    console.log("Player ID:", player_id);
    console.log("Game ID:", game_id);
    const response = await axios.put(
      `http://127.0.0.1:8000/leave_game/${player_id}/${game_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const StartGame = async (player_id, game_id) => {
  try {
    const response = await axios.put(
      `http://127.0.0.1:8000/start_game/${player_id}/${game_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const ChangeTurn = async (player_id, game_id) => {
  try {
    console.log("Player ID:", player_id);
    console.log("Game ID:", game_id);
    const response = await axios.put(
      `http://127.0.0.1:8000/next_turn/${player_id}/${game_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const DeleteGame = async (game_id) => {
  try {
    const response = await axios.delete(
      `http://127.0.0.1:8000/delete_game/${game_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const PossiblesMoves = async (
  player_id,
  game_id,
  movement_id,
  tile_id
) => {
  try {
    const response = await axios.get(
      `http://127.0.0.1:8000/possible_movements/${player_id}/${game_id}/${movement_id}/${tile_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const SwapTiles = async (
  player_id,
  game_id,
  movement_id,
  tile_id1,
  tile_id2
) => {
  try {
    const response = await axios.put(
      `http://127.0.0.1:8000/swap_tiles/${player_id}/${game_id}/${movement_id}/${tile_id1}/${tile_id2}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const PlayerMovements = async (player_id, game_id) => {
  try {
    const response = await axios.get(
      `http://127.0.0.1:8000/player_movement_charts/${player_id}/${game_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const UndoMovement = async (player_id, game_id) => {
  try {
    const response = await axios.put(
      `http://127.0.0.1:8000/undo_a_movement/${player_id}/${game_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const UndoAllMovements = async (player_id, game_id) => {
  try {
    const response = await axios.put(
      `http://127.0.0.1:8000/undo_all_movements/${player_id}/${game_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const UseFigureCard = async (player_id, game_id, figure_id, tile_id) => {
  try {
    const response = await axios.post(
      `http://127.0.0.1:8000/use_figure_chart/${player_id}/${game_id}/${figure_id}/${tile_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};

export const BlockFigureCard = async (player_id, targeted_player_id, game_id, figure_id, tile_id) => {
  try {
    const response = await axios.post(
      `http://127.0.0.1:8000/block_figure_chart/${player_id}/${targeted_player_id}/${game_id}/${figure_id}/${tile_id}`
    );

    console.log(response.data);

    return response.data;
  } catch (error) {
    console.error("Error al obtener datos:", error);
  }
};