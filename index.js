var addon = require("bindings")("dictionary");

addon.init("./dump.txt");

console.dir(addon.find("salom"));

console.dir(addon.startsWith("a", 100));
