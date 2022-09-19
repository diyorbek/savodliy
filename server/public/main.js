//#region Editor events
const Parchment = Quill.import("parchment");
const ErrorWord = new Parchment.Attributor.Attribute("error", "data-error", {
  scope: Parchment.Scope.INLINE,
});

Quill.register(ErrorWord);

const quill = new Quill("#editor", {
  placeholder: "Shu yerga yozing...",
  theme: "bubble",
  modules: {
    toolbar: false,
    history: {
      userOnly: true,
    },
  },
});

const markerContainer = document.getElementById("markers");

function styleMarker(element, params) {
  for (let param in params) {
    element.style[param] = params[param] + "px";
  }
}

function createMarker() {
  const element = document.createElement("div");
  element.className = "marker";
  return element;
}

function putMarkers(boundaries) {
  boundaries.forEach((boundary) => {
    const marker = createMarker();

    styleMarker(marker, boundary);
    markerContainer.appendChild(marker);
  });
}

function getBoundaries(positions) {
  return positions.map(([start, length]) => quill.getBounds(start, length));
}

function getPositions(text, word) {
  return [...text.matchAll(new RegExp(`${word}\\W`, "gi"))].map(({ index }) => [
    index,
    word.length,
  ]);
}

function getWordBoundaries(text, words) {
  const positions = words.map((word) => getPositions(text, word)).flat();
  return getBoundaries(positions);
}

function getWordPositions(text, words) {
  return words.map((word) => getPositions(text, word)).flat();
}

function markWords(words) {
  const content = quill.getText();
  const positions = getWordPositions(content, words);

  quill.formatText(0, content.length, "error", false);
  positions.map(([start, length]) =>
    quill.formatText(start, length, "error", true)
  );
}

function sendRequest(text) {
  const myHeaders = new Headers();
  myHeaders.append("Content-Type", "application/json");

  const requestOptions = {
    method: "POST",
    headers: myHeaders,
    body: JSON.stringify({ text }),
    redirect: "follow",
  };

  return fetch("/check-text", requestOptions);
}

async function checkWords() {
  const response = await sendRequest(quill.getText());
  const incorrectWords = await response.json();

  markWords(incorrectWords);
}

function debounce(cb, delay = 250) {
  let timeout;

  return (...args) => {
    clearTimeout(timeout);
    timeout = setTimeout(() => {
      cb(...args);
    }, delay);
  };
}

const debouncedCheckWords = debounce(checkWords, 1000);

quill.on("text-change", function (_delta, _oldDelta, source) {
  if (source != "api") debouncedCheckWords();
});

checkWords();
//#endregion

//#region Suggestions
const suggestionsPopup = document.getElementById("suggestions");

addEventListener("click", async (event) => {
  const errorAttr = event.target.getAttribute("data-error");

  if (!errorAttr) {
    if (event.target !== suggestionsPopup) {
      suggestionsPopup.replaceChildren();
      suggestionsPopup.classList.remove("show");
    }

    return;
  }

  const blot = Parchment.find(event.target);
  const start = blot.offset(quill.scroll);
  const text = event.target.innerText;
  const boundary = quill.getBounds(start, text.length);

  suggestionsPopup.replaceChildren();
  suggestionsPopup.classList.add("show");
  suggestionsPopup.style.top = boundary.bottom + "px";
  suggestionsPopup.style.left = boundary.left + "px";

  const response = await getSuggestionsRequest(text);
  const suggestions = await response.json();

  if (!suggestions.length) {
    const suggestionNode = document.createElement("p");
    suggestionNode.innerText = "- Topilmadi -";
    suggestionsPopup.appendChild(suggestionNode);

    return;
  }

  suggestionsPopup.setAttribute("data-start", start);
  suggestionsPopup.setAttribute("data-length", text.length);

  suggestions.forEach((suggestion) => {
    const suggestionNode = document.createElement("div");
    suggestionNode.className = "suggestion";
    suggestionNode.innerText = suggestion;

    suggestionsPopup.appendChild(suggestionNode);
  });
});

suggestionsPopup.addEventListener("click", (event) => {
  const target = event.target;
  const start = Number(suggestionsPopup.getAttribute("data-start"));
  const length = Number(suggestionsPopup.getAttribute("data-length"));

  if (!target.classList.contains("suggestion") || isNaN(start) || isNaN(length))
    return;

  const suggestion = target.innerText;
  quill.deleteText(start, length, "user");
  quill.insertText(start, suggestion, "user");
});

function getSuggestionsRequest(word) {
  return fetch(`/suggest?word=${word}`);
}

//#endregion
