import { BalancingCoefficients } from "../../../../types";
import { BalancingCoefficientsInputFormat } from "../formats";

export function balancingCoefficientsTransformToInput(
    balancingCoefficients: BalancingCoefficients
): BalancingCoefficientsInputFormat {
    return {
        BundeslandBonus: balancingCoefficients.state,
        FachUebereinstimmung: balancingCoefficients.subjectMatching,
        MatchingPriorityBonus: balancingCoefficients.matchingPriority,
        WartezeitBonus: balancingCoefficients.waitingTime
    };
}
