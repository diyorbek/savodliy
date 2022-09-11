/// <reference types="node"/>

declare const dictionary: {
  init(wordListFilePath: string): void;
  readFromDumpFile(filePath: string): void;
  dumpToFile(filePath: string): void;
  insert(word: string): void;
  find(word: string): boolean;
  startsWith(word: string): string[];
};

export = dictionary;
