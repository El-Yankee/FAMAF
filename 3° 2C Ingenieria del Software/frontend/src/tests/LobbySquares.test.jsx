import React from "react";
import { render, screen } from "@testing-library/react";
import LobbySquares from "../components/LobbySquares";

describe("LobbySquares Component", () => {
  test("renders LobbySquares component", () => {
    render(<LobbySquares />);
    const lobbySquaresElement = screen.getByTestId("lobby-squares");
    expect(lobbySquaresElement).toBeInTheDocument();
  });

  test("renders all squares with correct classes", () => {
    render(<LobbySquares />);
    const squares = screen.getAllByTestId("square");
    expect(squares.length).toBe(12);

    const classNames = [
      "bg-rojoficha absolute left-0 top-0 size-36 rounded-lg",
      "bg-verdeficha absolute left-40 top-0 size-36 rounded-lg",
      "bg-azulficha absolute left-80 top-0 size-36 rounded-lg",
      "bg-amarilloficha absolute left-0 top-40 size-36 rounded-lg",
      "bg-rojoficha absolute left-40 top-40 size-36 rounded-lg",
      "bg-verdeficha absolute left-0 top-80 size-36 rounded-lg",
      "bg-rojoficha absolute bottom-0 right-0 size-36 rounded-lg",
      "bg-verdeficha absolute bottom-0 right-40 size-36 rounded-lg",
      "bg-azulficha absolute bottom-0 right-80 size-36 rounded-lg",
      "bg-amarilloficha absolute bottom-40 right-0 size-36 rounded-lg",
      "bg-rojoficha absolute bottom-40 right-40 size-36 rounded-lg",
      "bg-verdeficha absolute bottom-80 right-40 size-36 rounded-lg",
    ];

    squares.forEach((square, index) => {
      expect(square).toHaveClass(classNames[index]);
    });
  });
});
