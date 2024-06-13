<template>
    <canvas id="main-canvas"/>
</template>

<script lang="ts">
    import { defineComponent } from "vue";
    import paper from "paper";
    import * as pdfMake from "pdfmake/build/pdfmake";


    import type { StageBaseObject } from "../stageObjects/StageBaseObject";
    import { StageCollection } from "@/stageObjects/StageCollection";

    export default defineComponent({
        data: () => ({ 
            drag_toggle: true,
            dragging: false,
            tool: new paper.Tool(),
            objects: new Map<number, StageCollection>(),
            lastId: 0
        }),

        methods: {
            updateCanvas() {
                paper.project.clear();

                for (const object of this.objects.values()) {   
                    paper.project.activeLayer.addChild(object.draw());
                }
            },

            addObject(object: StageBaseObject, dragable: boolean = true): number {
                const id = this.lastId++;
                this.objects.set(id, new StageCollection([0,0], [object], dragable && this.drag_toggle));
                return id;
            },

            getObject(id: number): StageCollection | undefined {
                return this.objects.get(id);
            },

            removeObject(id: number) {
                this.objects.delete(id);
            },

            clearObjects() {
                this.objects.clear();
            },

            enableDragging(option: boolean) {
                this.drag_toggle = option;
            },

            setMouseActions() {     
                const hitOptions = {
                    segments: true,
                    stroke: true,
                    fill: true,
                    tolerance: 5,
                };

                this.tool.onMouseDown = (event: any) => {
                    // only left mouse button
                    if (event.event.button != 0) return;
                    if (!this.drag_toggle) return;

                    // drag canvas only if no object is hit
                    const hitResult = paper.project.hitTest(event.point, hitOptions);
                    if (hitResult) return;

                    this.dragging = true;
                }

                this.tool.onMouseDrag = (event: any) => {
                    if (event.event.button != 0) return;
                    if(!this.dragging || !this.drag_toggle) return;

                    const delta = event.downPoint.subtract(event.point);
                    paper.view.translate(delta.multiply(-1));
                }

                this.tool.onMouseUp = (event: any) => {
                    if (event.event.button != 0) return;
                    if (!this.drag_toggle) return;

                    this.dragging = false;
                }

                // detect scroll inside the canvas (scroll down = zoom out)
                const canvas = document.getElementById("main-canvas") as HTMLDivElement;

                canvas.addEventListener("wheel", (event: any) => {
                    event.preventDefault();
                    const max = 5;
                    const min = 1/max;

                    // zoom in/out
                    const delta = -1*event.deltaY;
                    const zoom = 1 + delta/1000;

                    // clamp the zoom
                    const newZoom = Math.min(Math.max(paper.view.zoom*zoom, min), max);

                    if (newZoom == paper.view.zoom) return;

                    paper.view.zoom = newZoom;
                    
                    // move the view to the mouse position
                    const mouse = paper.view.getEventPoint(event);
                    const center = paper.view.center;
                    const offset = mouse.subtract(center);
                    paper.view.center = center.add(offset.multiply(zoom - 1));
                });
            },

            download(type: "svg" | "pdf" = "svg") {
                // add dark background with some padding
                const bounds = new paper.Rectangle(paper.project.activeLayer.strokeBounds).expand(20);
                
                const rect = new paper.Path.Rectangle(bounds);
                rect.fillColor = new paper.Color('#181818');
                rect.sendToBack();

                const svg = paper.project.exportSVG({ asString: true, bounds: bounds });
                if (typeof svg !== "string") return;
                    
                if (type == "svg") {
                    const blob = new Blob([svg], {type: "image/svg+xml;charset=utf-8"});
                    const link = document.createElement('a');
                    link.href = URL.createObjectURL(blob);
                    link.download = "scene.svg";
                    link.click();
                } else if (type == "pdf") {
                    const docDefinition = {
                        pageSize: {
                            width: bounds.width,
                            height: bounds.height
                        },
                        pageMargins: 0,
                        content: [
                            {
                                svg: svg,
                                width: bounds.width,
                                height: bounds.height,
                            }
                        ]
                    };
                    pdfMake.createPdf(docDefinition).download("scene.pdf");
                }

               rect.remove();
            }
        },

        mounted() {
            // resize the canvas to the size of the container
            const canvas = document.getElementById("main-canvas") as HTMLCanvasElement;
            canvas.width = parseInt(canvas.style.width);
            canvas.height = parseInt(canvas.style.height);

            paper.setup("main-canvas");
            
            this.setMouseActions();
            this.updateCanvas();
        },
    });
</script>

<style scoped>
    #main-canvas {
        width: inherit;
        height: inherit;
        background-color: #181818;
    }
</style>
