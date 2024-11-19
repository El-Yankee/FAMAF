import React, { act } from "react";
import { render, screen, fireEvent, waitFor } from "@testing-library/react";
import { BrowserRouter } from "react-router-dom";
import { describe, it, expect, vi, beforeEach } from "vitest";
import LobbyPage from "../pages/LobbyPage";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";

// Mock useNavigate from react-router-dom
const mockNavigate = vi.fn();
vi.mock("react-router-dom", async () => {
  const actual = await vi.importActual("react-router-dom");
  return {
    ...actual,
    useNavigate: () => mockNavigate,
  };
});

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

// Mock usePlayerContext
vi.mock("../context/PlayerContext.jsx", () => ({
  usePlayerContext: vi.fn(),
}));

// Mock useGameContext
vi.mock("../context/GameContext.jsx", () => ({
  useGameContext: vi.fn(),
}));

describe("LobbyPage", () => {
  beforeEach(() => {
    vi.clearAllMocks();
    usePlayerContext.mockReturnValue({ playerID: "testPlayer" });
    useGameContext.mockReturnValue({ setGameID: vi.fn() });
    render(
      <BrowserRouter>
        <LobbyPage />
      </BrowserRouter>
    );
  });

  it("should render the title 'LobbyPage'", () => {
    const title = screen.getByText(/Lobby/i);
    expect(title).toBeInTheDocument();
  });

  it("should render the 'Crear Partida' button", () => {
    const button = screen.getByRole("button", { name: /Crear Partida/i });
    expect(button).toBeInTheDocument();
  });

  it("should navigate to create game page on button click", () => {
    const button = screen.getByRole("button", { name: /Crear Partida/i });
    fireEvent.click(button);
    expect(mockNavigate).toHaveBeenCalledWith("/creategame");
  });

  it("should establish WebSocket connection on mount", () => {
    expect(global.WebSocket).toHaveBeenCalledWith(
      "http://127.0.0.1:8000/ws/testPlayer"
    );
  });

  it("should log WebSocket connection open", () => {
    const consoleSpy = vi.spyOn(console, "log");
    mockWebSocket.onopen();
    expect(consoleSpy).toHaveBeenCalledWith("Conectado al WebSocket del lobby");
  });

  it("should log WebSocket connection close", () => {
    const consoleSpy = vi.spyOn(console, "log");
    mockWebSocket.onclose({
      wasClean: true,
      code: 1000,
      reason: "Normal closure",
    });
    expect(consoleSpy).toHaveBeenCalledWith("Conexión WebSocket cerrada");
  });

  it("should log playerID from context", () => {
    const consoleSpy = vi.spyOn(console, "log");
    render(
      <BrowserRouter>
        <LobbyPage />
      </BrowserRouter>
    );
    expect(consoleSpy).toHaveBeenCalledWith("playerID", "testPlayer");
  });

  it("should clean up WebSocket on unmount", () => {
    const { unmount } = render(
      <BrowserRouter>
        <LobbyPage />
      </BrowserRouter>
    );
    unmount();
    expect(mockWebSocket.close).toHaveBeenCalled();
  });

  it("should update partidas state on WebSocket message", async () => {
    const data = [
      { game_name: "Test Game 1", state: "waiting", host_id: "1" },
      { game_name: "Test Game 2", state: "waiting", host_id: "2" },
    ];
    const event = { data: JSON.stringify(data) };

    act(() => mockWebSocket.onmessage(event));

    await waitFor(() => {
      expect(screen.getByText("Test Game 1")).toBeInTheDocument();
      expect(screen.getByText("Test Game 2")).toBeInTheDocument();
    });
  });
});
