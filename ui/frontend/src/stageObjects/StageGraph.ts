import paper from 'paper';
import { StageBaseObject } from "./StageBaseObject";
import type { Graph, GraphOptions, Subset } from "./types";
import { StageLabel } from './StageLabel';
import { StageCollection } from './StageCollection';
import { StageEllipse } from './StageEllipse';

export class StageGraph extends StageBaseObject {
    declare drawOptions: Subset<GraphOptions>;

    vertices: paper.Point[];
    edges: [number, number][];
    vertexValues?: string[];
    edgeValues?: string[];
    dragable: boolean;

    constructor(position: paper.PointLike, graph: Graph, options?: Subset<GraphOptions>) {
        super(position, options);
        this.vertices = graph.vertices.map(vertex => new paper.Point(vertex));
        this.edges = graph.edges;
        this.vertexValues = graph.vertexValues;
        this.edgeValues = graph.edgeValues;
        this.dragable = options?.dragable;
    }

    draw() {
        const graphGroup = new paper.Group();

        const drawEdge = (edge: [number, number], index: number) => {
            const edgeGroup = new paper.Group();

            const edgePath = new paper.Path.Line(
                this.vertices[edge[0]].add(this.position),
                this.vertices[edge[1]].add(this.position)
            );

            edgePath.strokeColor = new paper.Color(this.drawOptions.edge_color ?? "white");
            edgeGroup.addChild(edgePath);
            
            if (this.edgeValues) {
                const edgeLabel = new StageLabel(edgePath.position, this.edgeValues[index], {
                    font_color: this.drawOptions.edge_color ?? "white",
                    font_size: 12, 
                    align: "center" , 
                    justification: "center",
                    outline_color: "black",
                    outline_width: 2
                });
                edgeGroup.addChild(edgeLabel.draw());
            }
            
            return edgeGroup;
        };
            
        const edges = this.edges.map((edge, index) => drawEdge(edge, index));
        graphGroup.addChildren(edges);
        
        // draw vertices
        graphGroup.addChildren(this.vertices.map((vertexPosition, index) => {
            // collection to enable dragging
            const collection = new StageCollection(this.position, [], this.dragable);
            const radius = this.drawOptions.vertex_radius ?? 16;
            
            const node = new StageEllipse(vertexPosition.add([-radius/2, -radius/2]), radius, radius, {
                fill_color: this.drawOptions.vertex_fill_color ?? "white",
                stroke_color: this.drawOptions.vertex_stroke_color ?? "black"
            });

            node.update = (delta: any, idx = index) => {
                this.vertices[idx] = this.vertices[idx].add(delta);
                
                // update edges
                this.edges.forEach((edge, edgeIndex) => {
                    if (edge[0] === idx || edge[1] === idx) {
                        edges[edgeIndex].remove();
                        edges[edgeIndex] = drawEdge(edge, edgeIndex);
                        edges[edgeIndex].sendToBack();
                    }
                });
            };

            collection.add(node);

            // label
            if (this.vertexValues) {
                collection.add(new StageLabel(vertexPosition, this.vertexValues[index], {
                    font_color: this.drawOptions.vertex_label_color ?? "black",
                    font_size: 12, 
                    align: "center" , 
                    justification: "center",
                }));
            }

            return collection.draw();
        }));

        return graphGroup;
    }
}
