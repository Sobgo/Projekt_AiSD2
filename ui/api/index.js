import express from "express";
import { execFile } from "node:child_process";
import cors from "cors";

const app = express();

app.use(cors({ origin: "http://localhost:5173" }));

// type ResType = {
// 	execCode: number | null;
// 	stdout: string;
// 	stderr: string;
// };

app.get("/fence_logistics", (req, res, next) => {
        const cmd = './../../build/bin/fence_logistics';
        const { input } = req.query;

        const response = {
            execCode: 0,
            stdout: '',
            stderr: '',
        };

        const childProcess = execFile(cmd, ['--', '--'], (_err, stdout, stderr) => {
            response.stderr = stderr;
            response.stdout = stdout;
            res.json(response);
        });
    
        if (childProcess.stdin) {
            if (input)
                childProcess.stdin.write(input);
    
            childProcess.stdin.end();
        }
    
        childProcess.on('exit', (code) => {
            response.execCode = code;
        });
});

app.listen("8081", () => {
    console.log(`Backend server is running on port: 8081`);
});
