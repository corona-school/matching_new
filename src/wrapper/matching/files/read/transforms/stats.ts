import { Stats } from "../../../../types";
import { StatsOutputFormat } from "../formats";

export function statsTransformFromOutput(statsOutput: StatsOutputFormat): Stats {
    return {} as Stats; //todo: correctly parse stats
}
