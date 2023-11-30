express = require("express");

const app = express();
const port = 3000;
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static("public"));

app.get('/', async (req, res) => {
    res.sendFile("views/jisho.html", { root: __dirname });
})

app.listen(port, () => {
  return console.log(`Listening at http://localhost:${port}`);
})
