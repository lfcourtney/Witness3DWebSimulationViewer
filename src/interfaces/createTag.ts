import { QueueInfoTag } from "./queueInfoTag";
import { SurfaceTag } from "./surfaceTag";

/**
 * Represents a formatted <create> tag
 */
export interface CreateTag {
  time: number;
  instanceName: string;
  queueInfo?: QueueInfoTag;
  geometry?: string;
  surface?: SurfaceTag;
}
