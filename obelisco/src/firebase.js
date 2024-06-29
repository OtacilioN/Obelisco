import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyC39oJyAeH6gSAf-PreJEs0wDpbXLtnQpU",
  authDomain: "excellent-hue-425111-b0.firebaseapp.com",
  databaseURL: "https://excellent-hue-425111-b0-default-rtdb.firebaseio.com",
  projectId: "excellent-hue-425111-b0",
  storageBucket: "excellent-hue-425111-b0.appspot.com",
  messagingSenderId: "665758919696",
  appId: "1:665758919696:web:ddd2de556aa528e120ae95",
  measurementId: "G-5H6VMTJGTH",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);
export default db;
