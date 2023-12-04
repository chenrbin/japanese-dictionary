const loadJson = () => {
    $(".result-list").empty();
    $.getJSON("result_data.json", (data) => {
        $.each(data.results, (index, res) => {
            const resultTerm = "<div class='result'><span class='result-term'></span></div><hr>";
            $(".result-list").append(resultTerm);
            const lastResTermAppended = $(".result-term").length - 1;
            const lastResAppended = $(".result").length - 1;
            const ruby_term = ("<ruby class='result-term-reading'>" + res.term + "</ruby>");
            const ruby_yomi = ("<ruby>" + ", " + res.yomikata+ "</ruby>");
            $(".result-term").eq(lastResTermAppended).append(ruby_term);
            $(".result-term").eq(lastResTermAppended).append(ruby_yomi);
            const resultDefList = "<ul class='result-def-list'></ul>";
            $(".result").eq(lastResAppended).append(resultDefList);
            $.each(res.definitions, (index, def) => {
                const lastUlAppended = $(".result-def-list").length - 1;
                const li = ("<li>" + def + "</li>");
                $(".result-def-list").eq(lastUlAppended).append(li);
            })
        })
    });
}

const runOmnitool = async () => {
    const searchQuery = "{\"search\": \"" + $("#textinput").val() + "\"}";
    res = await fetch("http://localhost:3000/search", {
        method: "post",
        headers: {
            "Accept": "application/json",
            "Content-Type": "application/json"
        },
        body: searchQuery
    });
    return res;
}

const getResults = async (e) => {
    e.preventDefault(); // Prevent page from reloading.
    res = await runOmnitool();
    loadJson();
}

$(document).ready(() => {
    $("#textinput").on('keydown', (e) => {
        if(e.keyCode == 13) {
            getResults(e);
        }
        // Runs when enter is pressed.
    });

    $("#go").on('click', (e) => {
        getResults(e);
    });
});
