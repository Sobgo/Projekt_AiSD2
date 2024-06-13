import paper from "paper";

export type Subset<T> = {
    [P in keyof T]?: T[P];
};

export type Graph = {
    vertices: paper.PointLike[];
    edges: [number, number, boolean, string][];
    vertexValues?: any[];
    edgeValues?: any[];
}

export type ShapeOptions = {
    stroke_color: string;
    fill_color: string;
    stroke_width: number;
};

export type LabelOptions = {
    font_size: number;
    font_color: string;
    justification: string;
    align: "top" | "center" | "bottom" | string;
    outline_color: string;
    outline_width: number;
};

export type ContainerOptions = {
    wrap_count: number;
    size: number;
    aspect_ratio: "fit" | "square" | "longest" | string;
};

export type GraphOptions = {
    vertex_radius: number;
    vertex_fill_color: string;
    vertex_stroke_color: string;
    vertex_label_color: string;
    edge_color: string;
    dragable: boolean;
};
