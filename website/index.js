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

app.post('/translate', async(req, res) => {
    try {
        console.log(req.body);
        exec(`./public/GengoOmnitool ${req.body.search}`, (error, stdout, stderr) => console.log(stdout));
        await new Promise(temp => setTimeout(temp, 10)); // send response after waiting 10ms
        res.send(true);
    } catch (e) {
        console.error(e);
        res.send(false)
    }
})

app.listen(port, () => {
  return console.log(`Listening at http://localhost:${port}`);
})
