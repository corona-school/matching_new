import { MatchingResult } from "../..";
import { OutputFiles } from "../types";
import { createTemporaryFile, readTemporaryFile } from "../utils";
import { MatchesOutputFormat, StatsOutputFormat } from "./formats";
import { matchesTransformFromOutput, statsTransformFromOutput } from "./transforms";

export function createOutputFiles(): OutputFiles {
    return {
        matches: createTemporaryFile(),
        stats: createTemporaryFile()
    };
}

type ReadOutput = {
    matches: MatchesOutputFormat;
    stats: StatsOutputFormat;
};

function readOutput(outputFiles: OutputFiles): ReadOutput {
    return {
        matches: readTemporaryFile(outputFiles.matches) ?? [], //use empty array as default, because it looks like a possible algorithm's outcome is "null"
        stats: readTemporaryFile(outputFiles.stats) ?? []
    };
}

export function readOutputFiles(outputFiles: OutputFiles): MatchingResult {
    const output = readOutput(outputFiles);
    return {
        matches: matchesTransformFromOutput(output.matches),
        stats: statsTransformFromOutput(output.stats)
    };
}
