const express = require("express");

const server = express();

server.use(express.json());
server.use(express.urlencoded({ extended: true }));

server.get("/check", (req, res) => {
  res.status(200);
});

server.get("/suggest", (req, res) => {
  res.status(200);
});

server.listen(5566, () => {
  console.log("Server started");
});
