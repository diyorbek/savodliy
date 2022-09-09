var addon = require("bindings")("dictionary");

addon.init("./dump.txt");

addon.dumpToFile("./out.txt");
addon.readFromDumpFile("./out.txt");
console.dir(addon.find("salom"));
