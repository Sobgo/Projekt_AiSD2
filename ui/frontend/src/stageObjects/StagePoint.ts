import paper from 'paper';
import { StageBaseObject } from "./StageBaseObject";
import { StageLabel } from "./StageLabel";
import type { ShapeOptions, Subset } from './types';

export class StagePoint extends StageBaseObject {
    text: string;
    
    constructor(position: paper.PointLike, text: string, options?: Subset<ShapeOptions>) {
        super(position, options);
        this.text = text;
    }

    draw() {
        const group = new paper.Group();
        const path = new paper.Path.Circle(this.position, 3);

        path.fillColor = new paper.Color(this.drawOptions?.fill_color ?? "white");
        path.strokeColor = new paper.Color(this.drawOptions?.stroke_color ?? "white");

        const label = new StageLabel(
            this.position.add([0, 5]),
            this.text, 
            { font_size: 6, font_color: "white", align: "top", justification: "left" }
        );
    
        group.addChildren([path,  label.draw()]);
        return group;
    }
}