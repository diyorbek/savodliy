function getCleanWords(text) {
  return text.match(/[ʻʼ’'`´‘a-z\u0400-\u04FF\-]+/gi) || [];
}

module.exports = {
  getCleanWords,
};
