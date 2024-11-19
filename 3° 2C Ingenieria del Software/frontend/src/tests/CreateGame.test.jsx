import { render, screen, fireEvent, act } from "@testing-library/react";
import { describe, it, expect, vi, beforeEach } from "vitest";
import CreateGame from "./../pages/CreateGame";
import * as ReactRouterDom from "react-router-dom";
import { usePlayerContext } from "../context/PlayerContext";
import { useGameContext } from "../context/GameContext";
import { CreateAGame } from "../services/CreateGameServices";

vi.mock("react-router-dom", async () => {
  const actual = await vi.importActual("react-router-dom");
  return {
    ...actual,
    useNavigate: vi.fn(),
  };
});

vi.mock("../context/PlayerContext", () => ({
  usePlayerContext: vi.fn(),
}));

vi.mock("../context/GameContext", () => ({
  useGameContext: vi.fn(),
}));

vi.mock("../services/CreateGameServices", () => ({
  CreateAGame: vi.fn(),
}));

describe("CreateGame", () => {
  beforeEach(() => {
    usePlayerContext.mockReturnValue({ playerID: "test-player-id" });
    useGameContext.mockReturnValue({ setGameID: vi.fn() });
  });

  it("should render the title 'Crear partida'", () => {
    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );
    const title = screen.getByText(/Crear partida/i);
    expect(title).toBeInTheDocument();
  });

  it("should render the form with game name field, amount of players field, and submit button", () => {
    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const gameName = screen.getByPlaceholderText(/Ingresar nombre partida/i);
    expect(gameName).toBeInTheDocument();

    const playersPlaceholder = screen.getByText(
      /Selecciona cantidad jugadores/i,
      { selector: "span" }
    );
    expect(playersPlaceholder).toBeInTheDocument();

    const button = screen.getByRole("button", { name: /Crear/i });
    expect(button).toBeInTheDocument();
  });

  it("should display error when game name is missing", async () => {
    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const button = screen.getByRole("button", { name: /Crear/i });

    fireEvent.click(button);

    const errorMessage = await screen.findByText(
      /¡Ingresar el nombre es obligatorio!/i
    );
    expect(errorMessage).toBeInTheDocument();
  });

  it("should display error for invalid (non-alphanumeric) game name", async () => {
    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const gameName = screen.getByPlaceholderText(/Ingresar nombre partida/i);
    fireEvent.change(gameName, { target: { value: "game name!" } });

    const button = screen.getByRole("button", { name: /Crear/i });
    fireEvent.click(button);

    const errorMessage = await screen.findByText(
      /¡Solo caracteres alfanuméricos!/i
    );
    expect(errorMessage).toBeInTheDocument();
  });

  it("should display error when game name is longer than 20 characters", async () => {
    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const gameName = screen.getByPlaceholderText(/Ingresar nombre partida/i);
    fireEvent.change(gameName, {
      target: { value: "game12345678901234567890" },
    });

    const button = screen.getByRole("button", { name: /Crear/i });
    fireEvent.click(button);

    const errorMessage = await screen.findByText(/¡No más de 20 caracteres!/i);
    expect(errorMessage).toBeInTheDocument();
  });

  it("should display error when amount of players is missing", async () => {
    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const button = screen.getByRole("button", { name: /Crear/i });
    fireEvent.click(button);

    const errorMessage = await screen.findByText(
      /Por favor selecciona una opción/i
    );
    expect(errorMessage).toBeInTheDocument();
  });

  it("should navigate to /waitingRoom when form is submitted", async () => {
    const mockNavigate = vi.fn();
    ReactRouterDom.useNavigate.mockReturnValue(mockNavigate);
    CreateAGame.mockResolvedValue({ game_id: "test-game-id" });

    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const gameName = screen.getByPlaceholderText(/Ingresar nombre partida/i);
    fireEvent.change(gameName, { target: { value: "game123" } });

    const players = screen.getByText(/Selecciona cantidad jugadores/i);
    fireEvent.mouseDown(players);
    fireEvent.click(screen.getByText("2 jugadores"));

    const button = screen.getByRole("button", { name: /Crear/i });
    await act(async () => {
      fireEvent.click(button);
    });

    await new Promise((resolve) => setTimeout(resolve, 0));

    expect(mockNavigate).toHaveBeenCalledWith("/waitingRoom");
  });

  it("should log error when CreateAGame service fails", async () => {
    const mockNavigate = vi.fn();
    ReactRouterDom.useNavigate.mockReturnValue(mockNavigate);
    CreateAGame.mockRejectedValue(new Error("Failed to create game"));

    console.error = vi.fn();

    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const gameName = screen.getByPlaceholderText(/Ingresar nombre partida/i);
    fireEvent.change(gameName, { target: { value: "game123" } });

    const players = screen.getByText(/Selecciona cantidad jugadores/i);
    fireEvent.mouseDown(players);
    fireEvent.click(screen.getByText("2 jugadores"));

    const button = screen.getByRole("button", { name: /Crear/i });
    await act(async () => {
      fireEvent.click(button);
    });

    await new Promise((resolve) => setTimeout(resolve, 0));

    expect(console.error).toHaveBeenCalledWith(
      "Error joining lobby:",
      new Error("Failed to create game")
    );
  });

  it("should navigate to /lobby when 'Atras' button is clicked", () => {
    const mockNavigate = vi.fn();
    ReactRouterDom.useNavigate.mockReturnValue(mockNavigate);

    render(
      <ReactRouterDom.BrowserRouter>
        <CreateGame />
      </ReactRouterDom.BrowserRouter>
    );

    const backButton = screen.getByRole("button", { name: /Atras/i });
    fireEvent.click(backButton);

    expect(mockNavigate).toHaveBeenCalledWith("/lobby");
  });
});
