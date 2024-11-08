import { SurfaceTag } from "./surfaceTag";

/**
 * Represents a formatted <create> tag
 */
export interface CreateTag {
  time: number;
  instanceName: string;
  geometry?: string;
  surface?: SurfaceTag;
}
