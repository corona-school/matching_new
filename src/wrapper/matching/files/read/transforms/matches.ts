import { Match } from "../../../../types";
import { MatchOutputFormat } from "../formats";

export function matchTransformFromOutput(matchOutput: MatchOutputFormat): Match {
    return {
        helpee: {
            uuid: matchOutput["pupil uuid:"]
        },
        helper: {
            uuid: matchOutput["student uuid:"]
        }
    };
}
export function matchesTransformFromOutput(matchesOutput: MatchOutputFormat[]): Match[] {
    return matchesOutput.map(matchTransformFromOutput);
}
