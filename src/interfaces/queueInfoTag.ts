export type PartPositioning = "partOver" | "partUnder" | "partCentre";

/**
 * Represents a formatted <queueInfo> tag
 */
export interface QueueInfoTag {
  behaviour: {
    partPositioning: PartPositioning;
    partPitch: number;
    partRoll: number;
    partYaw: number;
  };
  direction: {
    dx: number;
    dy: number;
    dz: number;
  };
  position: {
    x: number;
    y: number;
    z: number;
  };
  queueParent: string;
}
