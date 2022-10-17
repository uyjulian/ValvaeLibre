function storeFunc() {
    var ra1 = document.getElementById("ra_ratio1").value;
    var ra2 = document.getElementById("ra_ratio2").value;

    var duration = [document.getElementById("dura_in").value,
        document.getElementById("dura_ex").value];

    var lift = [document.getElementById("lift_in").value,
        document.getElementById("lift_ex").value];

    var open = [document.getElementById("open_in").value,
        document.getElementById("open_ex").value];

    var close = [document.getElementById("close_in").value,
    document.getElementById("close_ex").value];

    var lobe_cl = [document.getElementById("lcl_in").value,
        document.getElementById("lcl_ex").value];

    var lobe_sep = document.getElementById("lobe_sep").value;

    document.getElementById("disp1").innerHTML =
        "Rocker Arm Ratio: " + ra1 + ":" + ra2 + "\n";
    document.getElementById("disp2").innerHTML =
        "Lobe Separation: " + lobe_sep + "\n";

    // Row 1
    document.getElementById("r1c1").innerHTML = duration[0];
    document.getElementById("r1c2").innerHTML = lift[0];
    document.getElementById("r1c3").innerHTML = open[0];
    document.getElementById("r1c4").innerHTML = close[0];
    document.getElementById("r1c5").innerHTML = lobe_cl[0];
    // Row 2
    document.getElementById("r2c1").innerHTML = duration[1];
    document.getElementById("r2c2").innerHTML = lift[1];
    document.getElementById("r2c3").innerHTML = open[1];
    document.getElementById("r2c4").innerHTML = close[1];
    document.getElementById("r2c5").innerHTML = lobe_cl[1];

    makeJSON(ra1, ra2, duration, lift, open, close, lobe_cl, lobe_sep);
}

function makeJSON(ra1, ra2, duration, lift, open, close, lobe_cl, lobe_sep) {
    var dict = {
        "ra1": +ra1,
        "ra2": +ra2,
        "lobe_sep": +lobe_sep,
        "duration": duration.reduce((results, x) => { results.push(+x); return results; }, []),
        "lift": lift.reduce((results, x) => { results.push(+x); return results; }, []),
        "open": open.reduce((results, x) => { results.push(+x); return results; }, []),
        "close": close.reduce((results, x) => { results.push(+x); return results; }, []),
        "lobe_cl": lobe_cl.reduce((results, x) => { results.push(+x); return results; }, [])
    };

    //var dictstring = JSON.stringify(dict);

    var saveData = (function () {
        var a = document.getElementById("link");
        a.style = "display: none";
        return function (data, fileName) {
            var json = JSON.stringify(data),
                blob = new Blob([json], { type: "octet/stream" }),
                url = window.URL.createObjectURL(blob);
            a.href = url;
            a.download = fileName;
            a.click();
            window.URL.revokeObjectURL(url);
        };
    }());

    fileName = "cam_card.json";

    saveData(dict, fileName);
    
}