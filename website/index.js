const express = require("express");
const { exec } = require("child_process");

const app = express();
const port = 3000;
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static("public"));

app.get('/', async (req, res) => {
    res.sendFile("views/jisho.html", { root: __dirname });
})

app.post('/search', async(req, res) => {
    try {
        console.log(req.body);
        const proc = exec(`./public/GengoOmnitool ${req.body.search}`, (error, stdout, stderr) => console.log(stdout));
        proc.on("close", () => {
            // Waits to send response after the program has finished loading. That way, no timeout is needed to wait for the JSON generation to finish on the webpage.
            return res.send(true);
        });
    } catch (e) {
        console.error(e);
        res.send(false)
    }
})

app.listen(port, () => {
  return console.log(`Listening at http://localhost:${port}`);
})
