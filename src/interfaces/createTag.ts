import { QueueInfoTag } from "./queueInfoTag";
import { PathTag } from "./pathTag";
import { SurfaceTag } from "./surfaceTag";

/**
 * Represents a formatted \<create\> tag
 */
export interface CreateTag {
  time: number;
  instanceName: string;
  geometry?: string;
  /**
   * Conveyors are identified by the presence of a \<path\> sub tag in their \<create\> tags
   */
  path?: PathTag;
  queueInfo?: QueueInfoTag;
  surface?: SurfaceTag;
}
