import paper from 'paper';
import { StageBaseObject } from "./StageBaseObject";
import { StageLabel } from "./StageLabel";
import { StageRect } from "./StageRect";
import type { ContainerOptions, Subset } from "./types";

export class StageArray extends StageBaseObject {
    declare drawOptions: Subset<ContainerOptions>;
    data: any[];

    constructor(position: paper.PointLike, data: any[], options?: Subset<ContainerOptions>) {
        super(position, options);
        this.data = data;
    }

    draw() {
        const group = new paper.Group();

        const origin = new paper.Point(this.position);
        const wrap_count = this.drawOptions?.wrap_count ?? this.data.length;
        const height = this.drawOptions?.size ?? 40;
        const font_size = height / 3;
        const margin = font_size;
       
        const aspect = this.drawOptions?.aspect_ratio ?? "fit";

        let width = height;
        if (aspect == "longest") {
            const longest = this.data.reduce((a, b) => a.toString().length > b.toString().length ? a : b);
            width = longest.toString().length * (font_size - 4) + margin * 2;
        }

        const textOptions = {
            font_size: font_size, 
            font_color: "white",
            justification: "center"
        };

        let totalRowLenght = 0;

        this.data.map((item, index) => {
            const text = item.toString();

            if (aspect == "fit") {
                width = Math.max(height, text.length * (font_size - 4) + margin * 2);
            }

            const x = origin.x + totalRowLenght;
            const y = origin.y + Math.floor(index / wrap_count) * (height + font_size * 1.6);

            const cell = new StageRect({x, y}, width, height, {
                stroke_color: "white" 
            });
            
            const value = new StageLabel({ 
                x: x + width/2, 
                y: y + height/2,
            }, text, {...textOptions, align: "center"});

            const label = new StageLabel({
                x: x + width/2, 
                y: y + height + 4,
            }, index.toString(), {...textOptions, align: "top"});

            totalRowLenght += width;
            if (index % wrap_count == wrap_count - 1) totalRowLenght = 0;

            group.addChildren([cell.draw(), value.draw(), label.draw()]);
        });

        // add invisible rectangle to make dragging easier
        const rect = new paper.Path.Rectangle(group.strokeBounds);
        rect.fillColor = new paper.Color("white");
        rect.opacity = 0;
        group.addChild(rect);

        return group;
    }
}
