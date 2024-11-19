import React from "react";
import LobbySquares from "../components/LobbySquares";
import { Button, Avatar } from "antd";
import { useNavigate } from "react-router-dom";
import perfilRamos from "../assets/images/perfilRamos.jpeg";
import perfilAvalos from "../assets/images/perfilAvalos.png";
import perfilAfonso from "../assets/images/perfilAfonso.jpeg";
import perfilDiForte from "../assets/images/perfilDiForte.jpeg";
import perfilMurphy from "../assets/images/perfilMurphy.png";
import perfilAngeli from "../assets/images/perfilAngeli.png";
import perfilEspinosa from "../assets/images/perfilEspinosa.png";
import perfilUsaj from "../assets/images/perfilUsaj.jpeg";

const CreditsPage = () => {
  const navigate = useNavigate();

  return (
    <>
      <div className="relative h-screen overflow-hidden bg-negrofondo flex justify-center items-center">
        <div className="text-blancofondo font-sans text-xl text-center whitespace-pre-wrap animate-scrollCredits">
          <h1 className="text-3xl mb-4">¡Gracias por jugar!</h1>
          <h1 className="text-3xl mb-4">Esperamos que hayas disfrutado</h1>

          <div className="mb-8">
            <h2 className="text-2xl text-left mb-2">Equipo Backend:</h2>
            <ul className="text-center ml-5">
              <li className="mb-2 flex">
                <Avatar src={perfilAfonso} className="mr-2" />
                Afonso Santiago
              </li>
              <li className="mb-2 flex ">
                <Avatar src={perfilDiForte} className="mr-2" />
                Di Forte Federico
              </li>
              <li className="mb-2 flex ">
                <Avatar src={perfilMurphy} className="mr-2" />
                Murphy Ronnie
              </li>
            </ul>
            <h2 className="text-2xl text-left mb-2">Equipo Frontend:</h2>
            <ul className="text-center ml-5">
              <li className="mb-2 flex">
                <Avatar src={perfilAngeli} className="mr-2" />
                Angeli Mateo
              </li>
              <li className="mb-2 flex ">
                <Avatar src={perfilEspinosa} className="mr-2" />
                Espinosa Bruno
              </li>
              <li className="mb-2 flex ">
                <Avatar src={perfilUsaj} className="mr-2" />
                Usaj Santiago
              </li>
            </ul>
          </div>
          <div className="mb-8">
            <h2 className="text-2xl text-left mb-2">Profesores:</h2>
            <ul className="text-center ml-5">
              <li className="mb-2 flex ">
                <Avatar src={perfilRamos} className="mr-2" /> Ramos Leandro
              </li>
              <li className="mb-2 flex ">
                <Avatar src={perfilAvalos} className="mr-2" /> Avalos Santiago
              </li>
            </ul>
          </div>
          <Button
            className="flex m-auto"
            danger
            ghost
            onClick={() => navigate(`/lobby`)}
          >
            Volver al lobby
          </Button>
        </div>
      </div>

      <LobbySquares />
    </>
  );
};

export default CreditsPage;
