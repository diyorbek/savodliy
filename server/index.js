const express = require("express");
const { dictionary } = require("./dictionary");

const { json, urlencoded } = express;
const server = express();

server.use(json());
server.use(urlencoded({ extended: true }));

server.get("/check", (req, res) => {
  const { word } = req.query;

  if (!word || !word.length) {
    res.status(400);
    res.json("Invalid word");
    return;
  }

  if (dictionary.find(word)) {
    res.status(200);
    res.json("correct");
  } else {
    res.status(404);
    res.json("not correct");
  }
});

const defaultSuggestionLimit = 5;

server.get("/suggest", (req, res) => {
  const { word, limit: limitParam } = req.query;
  const limit =
    limitParam != undefined ? Number(limitParam) : defaultSuggestionLimit;

  if (!word || !word.length) {
    res.status(400);
    res.json("Invalid word");
    return;
  }

  if (limit < 1 || limit > 30) {
    res.status(400);
    res.json("Limit should be in range [1, 30]");
    return;
  }

  const suggestions = dictionary.startsWith(word, limit);

  res.status(200);
  res.json(suggestions);
});

server.post("/suggest", (req, res) => {
  const { word } = req.query;

  if (!word || !word.length) {
    res.status(400);
    res.json("Invalid word");
    return;
  }

  try {
    dictionary.insert(word);
    dictionary.dumpToFile();

    res.status(201);
    res.json("success");
  } catch (error) {
    console.log(error);

    res.status(500);
    res.json("Server error");
  }
});

server.listen(5566, () => {
  console.log("Server started");
});
