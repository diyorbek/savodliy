const pureWordExp = /[a-zЀ-ӿ]+(\S*[a-zЀ-ӿ]+)?/gi;
const wordCharsExp = /[ʻʼ’'`´‘a-z\u0400-\u04FF\-]+/gi;

function getCleanWords(text) {
  const words = text.match(pureWordExp) || [];

  return words;
}

module.exports = {
  getCleanWords,
};
