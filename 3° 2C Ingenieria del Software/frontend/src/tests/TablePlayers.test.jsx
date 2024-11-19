import { describe, it, expect, vi, beforeEach } from "vitest";
import { render, screen } from "@testing-library/react";
import TablePlayers from "../components/TablePlayers.jsx";
import { usePlayerColor } from "../context/PlayerColorContext.jsx";

vi.mock("../context/PlayerColorContext.jsx", () => ({
  usePlayerColor: vi.fn(),
}));

describe("TablePlayers", () => {
  const mockAssignColorToPlayer = vi.fn();
  const mockPlayerColors = {
    1: "volcano",
    2: "blue",
    3: "green",
    4: "purple",
  };

  beforeEach(() => {
    usePlayerColor.mockReturnValue({
      playerColors: mockPlayerColors,
      assignColorToPlayer: mockAssignColorToPlayer,
    });
  });

  const playersList = [
    { player_id: 1, player_name: "Santi Usaj", key: 1 },
    { player_id: 2, player_name: "Ferrari", key: 2 },
    { player_id: 3, player_name: "Mateo Angeli", key: 3 },
    { player_id: 4, player_name: "Fede Di Forte", key: 4 },
  ];

  it("debe renderizar el componente correctamente", () => {
    render(<TablePlayers playersList={playersList} isCreator={1} />);

    // Verificar que el componente existe
    expect(screen.getByRole("table")).toBeInTheDocument();
  });

  it("debe renderizar la tabla con los datos correctos", () => {
    render(<TablePlayers playersList={playersList} isCreator={1} />);

    // Verificar que los nombres de los jugadores aparecen en la tabla
    expect(screen.getByText("Santi Usaj")).toBeInTheDocument();
    expect(screen.getByText("Ferrari")).toBeInTheDocument();
    expect(screen.getByText("Mateo Angeli")).toBeInTheDocument();
    expect(screen.getByText("Fede Di Forte")).toBeInTheDocument();
  });

  it("debe mostrar las etiquetas con las clases correctas", () => {
    render(<TablePlayers playersList={playersList} isCreator={1} />);

    // Verificar que la etiqueta 'Creador' está presente con la clase de color correcto (volcano)
    const creatorTag = screen.getByText("CREADOR");
    expect(creatorTag).toBeInTheDocument();
    expect(creatorTag).toHaveClass("ant-tag-volcano");

    // Verificar que las etiquetas 'Jugador' están presentes con las clases de color correctas
    const playerTags = screen.getAllByText("JUGADOR");
    playerTags.forEach((tag, index) => {
      expect(tag).toBeInTheDocument();
      const playerId = playersList[index + 1].player_id; // Ajustar el índice para omitir el creador
      const expectedClass = `ant-tag-${mockPlayerColors[playerId]}`;
      expect(tag).toHaveClass(expectedClass);
    });
  });

  it("debe asignar colores a los jugadores al montar el componente", () => {
    render(<TablePlayers playersList={playersList} isCreator={1} />);

    // Verificar que assignColorToPlayer fue llamado para cada jugador
    playersList.forEach((player) => {
      expect(mockAssignColorToPlayer).toHaveBeenCalledWith(player.player_id);
    });
  });
});
