const lib = require("../lib");
const { join } = require("path");
const { existsSync } = require("fs");

const DUMP_FILE_NAME = "dump.txt";
const WORD_LIST_FILE_NAME = "word_list.txt";
const DUMP_FILE_PATH = join(__dirname, `./${DUMP_FILE_NAME}`);
const WORD_LIST_FILE_PATH = join(__dirname, `./${WORD_LIST_FILE_NAME}`);

if (existsSync(DUMP_FILE_PATH)) {
  // if there is dump file, read from it
  lib.init();
  lib.readFromDumpFile(DUMP_FILE_PATH);
} else if (existsSync(WORD_LIST_FILE_PATH)) {
  // if there is no dump file, try word list
  lib.init(WORD_LIST_FILE_PATH);
  lib.dumpToFile(DUMP_FILE_PATH);
} else {
  lib.init();
}

module.exports.dictionary = {
  find: lib.find,
  startsWith: lib.startsWith,
  insert: lib.insert,
  dumpToFile: () => lib.dumpToFile(DUMP_FILE_PATH),
};
