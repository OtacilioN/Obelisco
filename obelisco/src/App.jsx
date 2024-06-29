import { useEffect, useState } from "react";
import "./App.css";
import { onValue, ref, set } from "firebase/database";
import db from "./firebase";

function updateRemotePosition(pos) {
  const remotePosRef = ref(db, "test/position");
  set(remotePosRef, pos)
    .then(() => console.log("Remote position updated to 42."))
    .catch((error) =>
      console.error("Failed to update remote position:", error)
    );
}

function App() {
  const [position, setPosition] = useState(0);

  useEffect(() => {
    const query = ref(db, "test/position");
    onValue(query, (snapshot) => {
      setPosition(snapshot.val());
    });
  }, []);

  return (
    <div>
      <h1>O motor de Obelisco está na posição {position}</h1>

      <h1>
        Isto é{" "}
        {position > 140 ? "Fechado" : position < 70 ? "Aberto" : "Semi aberto"}
      </h1>
      <button onClick={() => updateRemotePosition(65)}> Abrir </button>
      <button onClick={() => updateRemotePosition(149)}> Fechar </button>
    </div>
  );
}

export default App;
