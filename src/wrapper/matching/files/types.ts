import { FileResult } from "tmp";

type File = FileResult;

export type InputFiles = {
    helpers: File;
    helpees: File;
    balancingCoefficients: File;
};

export type OutputFiles = {
    matches: File;
    stats: File;
};
