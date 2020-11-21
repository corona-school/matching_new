import { readFileSync, writeSync } from "fs";
import { FileResult, fileSync } from "tmp";

export type TmpFile = FileResult;

export function createTemporaryFile(): TmpFile {
    return fileSync();
}

export function deleteTemporaryFiles(files: TmpFile[]) {
    files.forEach(f => f.removeCallback());
}

export function writeToTemporaryFile<X>(objects: X): TmpFile {
    const json = JSON.stringify(objects);

    const tmpFile = createTemporaryFile();

    writeSync(tmpFile.fd, json);

    return tmpFile;
}

export function readTemporaryFile<X>(file: TmpFile): X {
    const buffer = readFileSync(file.fd);

    const contents = JSON.parse(buffer.toString()) as X;

    return contents;
}
