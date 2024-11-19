import React from "react";

const LobbySquares = () => {
  return (
    <>
      <div
        className="square bg-rojoficha absolute left-0 top-0 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-verdeficha absolute left-40 top-0 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-azulficha absolute left-80 top-0 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-amarilloficha absolute left-0 top-40 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-rojoficha absolute left-40 top-40 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-verdeficha absolute left-0 top-80 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-rojoficha absolute bottom-0 right-0 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-verdeficha absolute bottom-0 right-40 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-azulficha absolute bottom-0 right-80 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-amarilloficha absolute bottom-40 right-0 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-rojoficha absolute bottom-40 right-40 size-36 rounded-lg"
        data-testid="square"
      />
      <div
        className="square bg-verdeficha absolute bottom-80 right-40 size-36 rounded-lg"
        data-testid="square"
      />
      <div data-testid="lobby-squares"></div>
    </>
  );
};
export default LobbySquares;
