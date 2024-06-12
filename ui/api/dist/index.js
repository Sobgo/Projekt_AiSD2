import express from "express";
import { execSync } from "node:child_process";
import cors from "cors";
const app = express();
app.use(cors({ origin: "http://localhost:5173" }));
app.post("/fence_logistics", (req, res, next) => {
    try {
        const input = JSON.stringify(req.body);
        console.log(input);
        const output = execSync('./../../build/bin/fence_logistics', {
            input: input,
            encoding: 'utf-8'
        });
        res.json({ output });
    }
    catch (error) {
        next(error);
    }
});
app.listen("8081", () => {
    console.log(`Backend server is running on port: 8081`);
});
