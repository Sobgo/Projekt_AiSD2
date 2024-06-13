import paper from 'paper';
import type { LabelOptions, Subset } from "./types";
import { StageBaseObject } from "./StageBaseObject";

export class StageLabel extends StageBaseObject {
    declare drawOptions: Subset<LabelOptions>;
    text: string;

    constructor(position: paper.PointLike, text: string, options?: Subset<LabelOptions>) {
        super(position, options);
        this.text = text;
    }

    draw() {
        const group = new paper.Group();

        const font_size = this.drawOptions?.font_size ?? 16;
        let position = this.position;
        if (this.drawOptions?.align === "center") {
            position = new paper.Point(position.x, position.y + font_size * 3/8);
        } else if (this.drawOptions?.align === "top") {
            position = new paper.Point(position.x, position.y + font_size * 3/4);
        }

        const path = new paper.PointText(position);

        path.content = this.text;
        path.fontFamily = "monospace"
        path.fontSize = font_size;

        if (this.drawOptions?.font_color) path.fillColor = new paper.Color(this.drawOptions.font_color);
        if (this.drawOptions?.justification) path.justification = this.drawOptions.justification;
        
        group.addChild(path);


        // outline
        if (this.drawOptions?.outline_color) {
            const outline = path.clone({insert: false});
            outline.strokeColor = new paper.Color(this.drawOptions.outline_color);
            outline.strokeWidth = this.drawOptions.outline_width ?? 1;
            group.addChild(outline);
            outline.insertBelow(path);
        }

        return group;
    }
}
