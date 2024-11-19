import React, { act } from "react";
import { render, screen, fireEvent, waitFor } from "@testing-library/react";
import { BrowserRouter } from "react-router-dom";
import { describe, it, expect, vi, beforeEach } from "vitest";
import WaitingRoom from "./../pages/WaitingRoom";
import { GameData, LeaveGame, StartGame } from "../services/GameServices";

// Mockear los componentes LobbySquares y TablePlayers
vi.mock("../components/LobbySquares", () => ({
  default: () => <div>LobbySquares Mock</div>,
}));

vi.mock("../components/TablePlayers", () => ({
  default: () => <div>TablePlayers Mock</div>,
}));

// Mock WebSocket
const mockWebSocket = {
  onopen: vi.fn(),
  onmessage: vi.fn(),
  onclose: vi.fn(),
  close: vi.fn(),
};
global.WebSocket = vi.fn((url) => {
  if (!url.startsWith("http://")) {
    throw new Error(`Invalid WebSocket URL: ${url}`);
  }
  return mockWebSocket;
});

// Mock useNavigate from react-router-dom
const mockNavigate = vi.fn();
vi.mock("react-router-dom", async (importOriginal) => {
  const actual = await importOriginal();
  return {
    ...actual,
    useNavigate: () => mockNavigate,
  };
});

vi.mock("../services/GameServices", () => ({
  GameData: vi.fn(),
  LeaveGame: vi.fn(),
  StartGame: vi.fn(),
}));

// Mock usePlayerContext
const mockSetPlayerID = vi.fn();
vi.mock("../context/PlayerContext", () => ({
  usePlayerContext: () => ({
    setPlayerID: mockSetPlayerID,
    playerID: "1",
  }),
}));

// Mock useGameContext
const mockSetGameID = vi.fn();
vi.mock("../context/GameContext", () => ({
  useGameContext: () => ({
    setGameID: mockSetGameID,
    game_id: "1",
  }),
}));

const renderWithRouter = (ui, { route = "/" } = {}) => {
  window.history.pushState({}, "Test page", route);
  return render(ui, { wrapper: BrowserRouter });
};

describe("WaitingRoom", () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it("should render game name after useEffect runs", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom />);

    // Esperar hasta que el nombre del juego sea renderizado
    const gameName = await screen.findByText("Test Game");
    expect(gameName).toBeInTheDocument();
  });

  it("should display 'Iniciar Partida' button if user is creator and there are 4 players", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom initialIsCreator={true} />);

    // Esperar hasta que el nombre del juego sea renderizado
    const gameName = await screen.findByText("Test Game");
    expect(gameName).toBeInTheDocument();

    // Verificar si el botón de "Iniciar Partida" está visible
    const startGameButton = screen.getByRole("button", {
      name: /Iniciar Partida/i,
    });
    expect(startGameButton).toBeInTheDocument();
  });

  it("should display 'Abandonar' button if user is not the creator", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "2",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom initialIsCreator={false} />);

    // Esperar hasta que el nombre del juego sea renderizado
    const gameName = await screen.findByText("Test Game");
    expect(gameName).toBeInTheDocument();

    // Simular que el usuario no es el creador
    const leaveButton = screen.getByRole("button", { name: /Abandonar/i });
    expect(leaveButton).toBeInTheDocument();
  });

  it("should navigate to /game when clicking 'Iniciar Partida' as creator with 4 players", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    StartGame.mockResolvedValueOnce({});

    renderWithRouter(<WaitingRoom initialIsCreator={true} />);

    // Esperar hasta que el nombre del juego sea renderizado
    const gameName = await screen.findByText("Test Game");
    expect(gameName).toBeInTheDocument();

    // Verificar si el botón de "Iniciar Partida" está visible
    const startGameButton = screen.getByRole("button", {
      name: /Iniciar Partida/i,
    });
    expect(startGameButton).toBeInTheDocument();

    // Simular el click en el botón de 'Iniciar Partida'
    fireEvent.click(startGameButton);
    await waitFor(() => {
      expect(mockNavigate).toHaveBeenCalledWith("/game");
    });
  });

  it("should navigate to /lobby when clicking 'Abandonar'", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "2",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    LeaveGame.mockResolvedValueOnce({});

    renderWithRouter(<WaitingRoom initialIsCreator={false} />);

    // Simular el click en el botón de 'Abandonar'
    const leaveButton = screen.getByRole("button", { name: /Abandonar/i });

    fireEvent.click(leaveButton);
    await waitFor(() => {
      expect(mockNavigate).toHaveBeenCalledWith("/lobby");
    });
  });

  it("should update players list when WebSocket message is received", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom />);

    // Simular el mensaje recibido
    const data = {
      players: 2,
      player_details: [
        {
          player_id: "1",
          player_name: "santi",
        },
        {
          player_id: "2",
          player_name: "juan",
        },
      ],
    };

    const event = { data: JSON.stringify(data) };

    await act(async () => {
      mockWebSocket.onmessage(event);
    });

    // Verificar si player_details fue modificado
    expect(data.player_details).toEqual([
      {
        player_id: "1",
        player_name: "santi",
      },
      {
        player_id: "2",
        player_name: "juan",
      },
    ]);
  });

  it("should navigate to /game when WebSocket message state is 'playing'", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom />);

    // Simular el mensaje recibido
    const data = {
      players: 4,
      player_details: [],
      state: "playing",
    };

    const event = { data: JSON.stringify(data) };

    act(() => mockWebSocket.onmessage(event));

    // Esperar hasta que la navegación ocurra
    await waitFor(() => {
      expect(mockNavigate).toHaveBeenCalledWith("/game");
    });
  });

  it("should call GameData and handle success response", async () => {
    const mockResponse = {
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    };

    GameData.mockResolvedValueOnce(mockResponse);

    renderWithRouter(<WaitingRoom />);

    await waitFor(() => {
      expect(GameData).toHaveBeenCalledWith("1");
    });

    expect(GameData).toHaveBeenCalledTimes(1);
    expect(screen.getByText("Test Game")).toBeInTheDocument();
  });

  it("should handle error when GameData fails", async () => {
    const consoleErrorSpy = vi
      .spyOn(console, "error")
      .mockImplementation(() => {});
    GameData.mockRejectedValueOnce(new Error("Error getting game data"));

    renderWithRouter(<WaitingRoom />);

    await waitFor(() => {
      expect(GameData).toHaveBeenCalledWith("1");
    });

    expect(GameData).toHaveBeenCalledTimes(1);
    expect(consoleErrorSpy).toHaveBeenCalledWith(
      "Error getting game data",
      expect.any(Error)
    );

    consoleErrorSpy.mockRestore();
  });

  it("should call LeaveGame and navigate to /lobby on quitRoom success", async () => {
    LeaveGame.mockResolvedValueOnce({});

    renderWithRouter(<WaitingRoom initialIsCreator={false} />);

    // Simular el click en el botón de 'Abandonar'
    const leaveButton = screen.getByRole("button", { name: /Abandonar/i });

    fireEvent.click(leaveButton);
    await waitFor(() => {
      expect(LeaveGame).toHaveBeenCalledWith("1", "1");
      expect(mockNavigate).toHaveBeenCalledWith("/lobby");
    });
  });

  it("should handle error when LeaveGame fails", async () => {
    const consoleErrorSpy = vi
      .spyOn(console, "error")
      .mockImplementation(() => {});
    LeaveGame.mockRejectedValueOnce(new Error("Error getting new game data"));

    renderWithRouter(<WaitingRoom initialIsCreator={false} />);

    // Simular el click en el botón de 'Abandonar'
    const leaveButton = screen.getByRole("button", { name: /Abandonar/i });

    fireEvent.click(leaveButton);
    await waitFor(() => {
      expect(LeaveGame).toHaveBeenCalledWith("1", "1");
      expect(consoleErrorSpy).toHaveBeenCalledWith(
        "Error getting new game data",
        expect.any(Error)
      );
    });

    consoleErrorSpy.mockRestore();
  });

  it("should call StartGame and navigate to /game on start success", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom initialIsCreator={true} />);

    StartGame.mockResolvedValueOnce({});

    // Esperar hasta que el nombre del juego sea renderizado
    const gameName = await screen.findByText("Test Game");
    expect(gameName).toBeInTheDocument();

    // Verificar si el botón de "Iniciar Partida" está visible
    const startGameButton = screen.getByRole("button", {
      name: /Iniciar Partida/i,
    });
    expect(startGameButton).toBeInTheDocument();

    fireEvent.click(startGameButton);
    await waitFor(() => {
      expect(StartGame).toHaveBeenCalledWith("1", "1");
      expect(mockNavigate).toHaveBeenCalledWith("/game");
    });
  });

  it("should handle error when StartGame fails", async () => {
    const consoleErrorSpy = vi
      .spyOn(console, "error")
      .mockImplementation(() => {});
    StartGame.mockRejectedValueOnce(new Error("Error getting data"));

    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom initialIsCreator={true} />);

    StartGame.mockResolvedValueOnce({});

    // Esperar hasta que el nombre del juego sea renderizado
    const gameName = await screen.findByText("Test Game");
    expect(gameName).toBeInTheDocument();

    // Verificar si el botón de "Iniciar Partida" está visible
    const startGameButton = screen.getByRole("button", {
      name: /Iniciar Partida/i,
    });
    expect(startGameButton).toBeInTheDocument();

    fireEvent.click(startGameButton);
    await waitFor(() => {
      expect(StartGame).toHaveBeenCalledWith("1", "1");
      expect(consoleErrorSpy).toHaveBeenCalledWith(
        "Error getting data",
        expect.any(Error)
      );
    });

    consoleErrorSpy.mockRestore();
  });

  it("should log 'Conectado al WebSocket del lobby' when WebSocket connection opens", async () => {
    const consoleLogSpy = vi.spyOn(console, "log").mockImplementation(() => {});

    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom initialIsCreator={true} />);

    // Simular la apertura de la conexión WebSocket
    await act(async () => {
      mockWebSocket.onopen();
    });

    expect(consoleLogSpy).toHaveBeenCalledWith(
      "Conectado al WebSocket del lobby"
    );

    consoleLogSpy.mockRestore();
  });

  it("should log 'Conexión WebSocket cerrada' when WebSocket connection closes", async () => {
    const consoleLogSpy = vi.spyOn(console, "log").mockImplementation(() => {});

    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom initialIsCreator={true} />);

    // Simular el cierre de la conexión WebSocket
    await act(async () => {
      mockWebSocket.onclose();
    });

    expect(consoleLogSpy).toHaveBeenCalledWith("Conexión WebSocket cerrada");

    consoleLogSpy.mockRestore();
  });

  it("should open modal when WebSocket message error is 'Game not found'", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom />);

    // Simular el mensaje recibido
    const data = {
      players: 4,
      player_details: [],
      error: "Game not found",
    };

    const event = { data: JSON.stringify(data) };

    await act(async () => {
      mockWebSocket.onmessage(event);
    });

    // Verificar si el modal está abierto
    const modalText = await screen.findByText("El creador decidio irse");
    expect(modalText).toBeInTheDocument();
  });

  it("should navigate to /lobby when clicking 'Volver al Lobby' in the modal", async () => {
    GameData.mockResolvedValueOnce({
      game_name: "Test Game",
      state: "waiting",
      host_id: "1",
      players: 4,
      game_size: 4,
      player_details: [],
    });

    renderWithRouter(<WaitingRoom />);

    // Simular el mensaje recibido que abre el modal
    const data = {
      players: 4,
      player_details: [],
      error: "Game not found",
    };

    const event = { data: JSON.stringify(data) };

    await act(async () => {
      mockWebSocket.onmessage(event);
    });

    // Verificar si el modal está abierto
    const modalText = await screen.findByText("El creador decidio irse");
    expect(modalText).toBeInTheDocument();

    // Simular el click en el botón de 'Volver al Lobby'
    const returnToLobbyButton = screen.getByRole("button", {
      name: /Volver al Lobby/i,
    });

    fireEvent.click(returnToLobbyButton);
    await waitFor(() => {
      expect(mockNavigate).toHaveBeenCalledWith("/lobby");
    });
  });
});
