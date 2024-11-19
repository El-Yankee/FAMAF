import React from "react";
import { render, screen, fireEvent } from "@testing-library/react";
import { vi } from "vitest";
import MovementCard from "../components/MovementCard";
import { usePlayerContext } from "../context/PlayerContext.jsx";
import { useGameContext } from "../context/GameContext.jsx";
import { PlayerMovements } from "../services/GameServices";

// Mock the contexts and services
vi.mock("../context/PlayerContext.jsx", () => ({
  usePlayerContext: vi.fn(),
}));
vi.mock("../context/GameContext.jsx", () => ({
  useGameContext: vi.fn(),
}));
vi.mock("../services/GameServices", () => ({
  PlayerMovements: vi.fn(),
}));

describe("MovementCard Component", () => {
  const mockPlayerID = "player1";
  const mockGameID = "game1";
  const mockMovements = {
    ids_of_movement_charts: [1, 2, 3],
  };

  beforeEach(() => {
    usePlayerContext.mockReturnValue({ playerID: mockPlayerID });
    useGameContext.mockReturnValue({ game_id: mockGameID });
    PlayerMovements.mockResolvedValue(mockMovements);
  });

  it("renders without crashing", async () => {
    render(<MovementCard onSelectMovCard={vi.fn()} updateboard={false} />);
    expect(await screen.findByAltText("1")).toBeInTheDocument();
    expect(await screen.findByAltText("2")).toBeInTheDocument();
    expect(await screen.findByAltText("3")).toBeInTheDocument();
  });

  it("calls onSelectMovCard when a card is clicked", async () => {
    const mockOnSelectMovCard = vi.fn();
    render(
      <MovementCard onSelectMovCard={mockOnSelectMovCard} updateboard={false} />
    );

    const card = await screen.findByAltText("1");
    fireEvent.click(card);

    expect(mockOnSelectMovCard).toHaveBeenCalledWith(1);
  });

  it("displays the correct images for each movement", async () => {
    render(<MovementCard onSelectMovCard={vi.fn()} updateboard={false} />);

    const card1 = await screen.findByAltText("1");
    const card2 = await screen.findByAltText("2");
    const card3 = await screen.findByAltText("3");

    expect(card1).toHaveAttribute("src", expect.stringContaining("mov1.svg"));
    expect(card2).toHaveAttribute("src", expect.stringContaining("mov2.svg"));
    expect(card3).toHaveAttribute("src", expect.stringContaining("mov3.svg"));
  });

  it("updates the data when updateboard changes", async () => {
    const { rerender } = render(
      <MovementCard onSelectMovCard={vi.fn()} updateboard={false} />
    );
    expect(await screen.findByAltText("1")).toBeInTheDocument();

    const newMockMovements = {
      ids_of_movement_charts: ["4", "5", "6"],
    };
    PlayerMovements.mockResolvedValueOnce(newMockMovements);

    rerender(<MovementCard onSelectMovCard={vi.fn()} updateboard={true} />);
    expect(await screen.findByAltText("4")).toBeInTheDocument();
    expect(await screen.findByAltText("5")).toBeInTheDocument();
    expect(await screen.findByAltText("6")).toBeInTheDocument();
  });

  it("updates the data when updateboard changes", async () => {
    const { rerender } = render(
      <MovementCard onSelectMovCard={vi.fn()} updateboard={false} />
    );
    expect(await screen.findByAltText("1")).toBeInTheDocument();

    const newMockMovements = {
      ids_of_movement_charts: ["7"],
    };
    PlayerMovements.mockResolvedValueOnce(newMockMovements);

    rerender(<MovementCard onSelectMovCard={vi.fn()} updateboard={true} />);
    expect(await screen.findByAltText("7")).toBeInTheDocument();
  });
});
