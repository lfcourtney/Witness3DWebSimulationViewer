import { TranslateTag } from "./translateTag";
import { ScaleTag } from "./scaleTag";
/**
 * Represents a formatted <update> tag
 */
export interface UpdateTag {
  time: number;
  instanceName: string;
  translate?: TranslateTag;
  scale?: ScaleTag;
}
