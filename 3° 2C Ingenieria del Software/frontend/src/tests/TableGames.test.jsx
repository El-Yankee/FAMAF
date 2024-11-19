import React from "react";
import { render, screen, fireEvent, waitFor } from "@testing-library/react";
import { vi } from "vitest";
import TableGames from "../components/TableGames.jsx";
import { useNavigate } from "react-router-dom";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";
import { JoinGame } from "../services/LobbyServices";

vi.mock("react-router-dom", () => ({
  useNavigate: vi.fn(),
}));

vi.mock("../context/PlayerContext.jsx", () => ({
  usePlayerContext: vi.fn(),
}));

vi.mock("../context/GameContext.jsx", () => ({
  useGameContext: vi.fn(),
}));

vi.mock("../services/LobbyServices", () => ({
  JoinGame: vi.fn(),
}));

describe("TableGames Component", () => {
  const mockNavigate = vi.fn();
  const mockSetGameID = vi.fn();
  const mockPlayerID = "player1";

  beforeEach(() => {
    useNavigate.mockReturnValue(mockNavigate);
    usePlayerContext.mockReturnValue({ playerID: mockPlayerID });
    useGameContext.mockReturnValue({ setGameID: mockSetGameID });
  });

  afterEach(() => {
    vi.clearAllMocks();
  });

  const gamesList = [
    {
      key: "1",
      game_name: "Juego1",
      game_id: "1",
      game_size: "4",
      players: "3",
      state: "waiting",
      type: "Public",
      player_details: [],
    },
    {
      key: "2",
      game_name: "Juego2",
      game_id: "2",
      game_size: "2",
      players: "2",
      state: "playing",
      type: "Public",
      player_details: [],
    },
    {
      key: "3",
      game_name: "Juego3",
      game_id: "3",
      game_size: "4",
      players: "4",
      state: "waiting",
      type: "Public",
      player_details: [],
    },
  ];

  test("renders table with correct columns", () => {
    render(<TableGames gamesList={gamesList} />);

    expect(screen.getByText("Nombre Partida")).toBeInTheDocument();
    expect(screen.getByText("Jugadores")).toBeInTheDocument();
    expect(screen.getByText("Estado")).toBeInTheDocument();
  });

  test("renders correct game data", () => {
    render(<TableGames gamesList={gamesList} />);

    expect(screen.getByText("Juego1")).toBeInTheDocument();
    expect(screen.getByText("Juego2")).toBeInTheDocument();
    expect(screen.getByText("Juego3")).toBeInTheDocument();
  });

  test("join button triggers join function and navigates on success", async () => {
    JoinGame.mockResolvedValue({ success: true });

    render(<TableGames gamesList={gamesList} />);

    const joinButton = screen.getAllByText("Unirme")[0];
    fireEvent.click(joinButton);

    expect(JoinGame).toHaveBeenCalledWith(mockPlayerID, "1", "CAB");
    await waitFor(() => expect(mockSetGameID).toHaveBeenCalledWith("1"));
    expect(mockNavigate).toHaveBeenCalledWith("/waitingRoom");
  });

  test("displays error message on join failure", async () => {
    const consoleErrorSpy = vi
      .spyOn(console, "error")
      .mockImplementation(() => {});
    JoinGame.mockRejectedValue(new Error("Join failed"));

    render(<TableGames gamesList={gamesList} />);

    const joinButton = screen.getAllByText("Unirme")[0];
    fireEvent.click(joinButton);

    await waitFor(() =>
      expect(consoleErrorSpy).toHaveBeenCalledWith(
        "Error getting new game data",
        expect.any(Error)
      )
    );
    consoleErrorSpy.mockRestore();
  });

  test("displays correct number of players", () => {
    render(<TableGames gamesList={gamesList} />);

    expect(screen.getByText("3 / 4")).toBeInTheDocument();
    expect(screen.getByText("2 / 2")).toBeInTheDocument();
    expect(screen.getByText("4 / 4")).toBeInTheDocument();
  });

  test("renders correct game state for waiting games", () => {
    render(<TableGames gamesList={gamesList} />);

    expect(screen.getAllByText("Unirme").length).toBe(1);
  });

  test("renders correct game state for playing games", () => {
    render(<TableGames gamesList={gamesList} />);

    expect(screen.getByText("Jugando")).toBeInTheDocument();
  });

  test("renders correct game state for full games", () => {
    render(<TableGames gamesList={gamesList} />);

    expect(screen.getByText("Sala llena")).toBeInTheDocument();
  });

  test("join button is not rendered for full games", () => {
    render(<TableGames gamesList={gamesList} />);

    const joinButtons = screen.queryAllByText("Unirme");
    expect(joinButtons.length).toBe(1); // Only one join button should be rendered
  });

  test("join button is not rendered for games in progress", () => {
    render(<TableGames gamesList={gamesList} />);

    const joinButtons = screen.queryAllByText("Unirme");
    expect(joinButtons.length).toBe(1); // Only one join button should be rendered
  });

  test("pagination is rendered correctly", () => {
    render(<TableGames gamesList={gamesList} />);

    expect(screen.getByText("1")).toBeInTheDocument();
    expect(screen.getByTitle("Previous Page").closest("li")).toHaveClass(
      "ant-pagination-disabled"
    );
    expect(screen.getByTitle("Next Page").closest("li")).toHaveClass(
      "ant-pagination-disabled"
    );
  });

  test("renders correct number of rows", () => {
    render(<TableGames gamesList={gamesList} />);

    const rows = screen.getAllByRole("row");
    expect(rows.length).toBe(4); // 1 header row + 3 data rows
  });

  test("join button calls join function with correct game ID", async () => {
    JoinGame.mockResolvedValue({ success: true });

    render(<TableGames gamesList={gamesList} />);

    const joinButton = screen.getAllByText("Unirme")[0];
    fireEvent.click(joinButton);

    expect(JoinGame).toHaveBeenCalledWith(mockPlayerID, "1", "CAB");
  });

  // Additional tests
  test("renders correct game state for waiting games with less players", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego4",
        game_id: "4",
        game_size: "5",
        players: "2",
        state: "waiting",
        type: "Public",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    expect(screen.getByText("Juego4")).toBeInTheDocument();
    expect(screen.getByText("2 / 5")).toBeInTheDocument();
    expect(screen.getByText("Unirme")).toBeInTheDocument();
  });

  test("join button is not rendered for games with state other than waiting", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego5",
        game_id: "5",
        game_size: "3",
        players: "3",
        state: "playing",
        type: "Public",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    expect(screen.queryByText("Unirme")).not.toBeInTheDocument();
    expect(screen.getByText("Jugando")).toBeInTheDocument();
  });

  // New tests
  test("renders no games message when gamesList is empty", () => {
    render(<TableGames gamesList={[]} />);

    expect(screen.getByText("No data")).toBeInTheDocument();
  });

  test("renders correct game state for waiting games with maximum players", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego6",
        game_id: "6",
        game_size: "4",
        players: "4",
        state: "waiting",
        type: "Public",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    expect(screen.getByText("Juego6")).toBeInTheDocument();
    expect(screen.getByText("4 / 4")).toBeInTheDocument();
    expect(screen.getByText("Sala llena")).toBeInTheDocument();
  });

  test("join button is rendered for waiting games with less players", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego7",
        game_id: "7",
        game_size: "5",
        players: "3",
        state: "waiting",
        type: "Public",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    expect(screen.getByText("Juego7")).toBeInTheDocument();
    expect(screen.getByText("3 / 5")).toBeInTheDocument();
    expect(screen.getByText("Unirme")).toBeInTheDocument();
  });

  test("join button is not rendered for games with state other than waiting", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego8",
        game_id: "8",
        game_size: "3",
        players: "3",
        state: "playing",
        type: "Public",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    expect(screen.queryByText("Unirme")).not.toBeInTheDocument();
    expect(screen.getByText("Jugando")).toBeInTheDocument();
  });

  test("renders correct game state for waiting games with less players", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego9",
        game_id: "9",
        game_size: "5",
        players: "2",
        state: "waiting",
        type: "Public",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    expect(screen.getByText("Juego9")).toBeInTheDocument();
    expect(screen.getByText("2 / 5")).toBeInTheDocument();
    expect(screen.getByText("Unirme")).toBeInTheDocument();
  });

  test("join button is not rendered for games with state other than waiting", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego10",
        game_id: "10",
        game_size: "3",
        players: "3",
        state: "playing",
        type: "Public",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    expect(screen.queryByText("Unirme")).not.toBeInTheDocument();
    expect(screen.getByText("Jugando")).toBeInTheDocument();
  });

  // Tests for modal functionality
  test("shows modal when join button is clicked for private game", () => {
    const newGamesList = [
      {
        key: "1",
        game_name: "Juego11",
        game_id: "11",
        game_size: "4",
        players: "2",
        state: "waiting",
        type: "Private",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    const joinButton = screen.getByText("Unirme");
    fireEvent.click(joinButton);

    expect(screen.getByText("Ingresar contraseña")).toBeInTheDocument();
  });

  test("calls join function with correct password when modal is confirmed", async () => {
    JoinGame.mockResolvedValue({ success: true });

    const newGamesList = [
      {
        key: "1",
        game_name: "Juego12",
        game_id: "12",
        game_size: "4",
        players: "2",
        state: "waiting",
        type: "Private",
        player_details: [],
      },
    ];

    render(<TableGames gamesList={newGamesList} />);

    const joinButton = screen.getByText("Unirme");
    fireEvent.click(joinButton);

    const passwordInput = screen.getByPlaceholderText("");
    fireEvent.change(passwordInput, { target: { value: "password123" } });

    const okButton = screen.getByText("OK");
    fireEvent.click(okButton);

    await waitFor(() =>
      expect(JoinGame).toHaveBeenCalledWith(mockPlayerID, "12", "password123")
    );
  });
});
