function makeJSON(in_lift, ex_lift, in_open, ex_open, in_duration, ex_duration, num_cylinders, fire_order)
{
    var dict = {
        "intake_valve_lift": in_lift*1.0,
        "exhaust_valve_lift": ex_lift*1.0,
        "intake_valve_opens": +in_open,
        "exhaust_valve_opens": +ex_open,
        "intake_valve_duration": +in_duration,
        "exhaust_valve_duration": +ex_duration,
        "number_of_cylinders": +num_cylinders,
        "firing_order": fire_order
    };
    
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

    fileName = "table.json";
    saveData(dict, fileName);
    return 0;
}

function true_angle_conversion(angle, specifier, valve_type)
{
    var BTDC = "btdc";
    var BBDC = "bbdc";
    var ATDC = "atdc";
    var ABDC = "abdc";
    var new_angle;

    if (specifier == BTDC && valve_type == 0) {
        new_angle = (720 - angle);
        return new_angle;
    } else if (specifier == BBDC && valve_type == 1) {
        new_angle = (540 - angle);
        return new_angle;
    } else if (specifier == ATDC && valve_type == 1) {
        new_angle = (0 + -(-angle));
        return new_angle;
    } else if (specifier == ABDC && valve_type == 0) {
        new_angle = (180 + -(-angle));
        return new_angle;
    }
}

function storeFunc()
{
    valid = document.getElementById("f1").checkValidity();
    if (!valid) return;

    var intake_val = 0;
    var exhaust_val = 1;

    var duration = [
        document.getElementById("in_duration").value,
        document.getElementById("ex_duration").value
    ];

    var lift_valve = [
        document.getElementById("in_lift_valve").value,
        document.getElementById("ex_lift_valve").value
    ];

    var lift_cam = [
        document.getElementById("in_lift_cam").value,
        document.getElementById("ex_lift_cam").value
    ];

    var open = [
        document.getElementById("in_open").value,
        document.getElementById("ex_open").value
    ];

    var close = [
        document.getElementById("in_close").value,
        document.getElementById("ex_close").value
    ];

    var ra_ratio = document.getElementById("ra_ratio").value;

    var fire_order_str = document.getElementById("fire_order").value;
    var fire_order = [
        parseInt(fire_order_str.split(",")[0]),
        parseInt(fire_order_str.split(",")[1]),
        parseInt(fire_order_str.split(",")[2]),
        parseInt(fire_order_str.split(",")[3])
    ]

    var num_cylinders;
    var r1 = document.getElementById("radio_1").checked;
    var r2 = document.getElementById("radio_2").checked;
    var r3 = document.getElementById("radio_3").checked;
    if (r1) num_cylinders = 4;
    else if (r2) num_cylinders = 6;
    else if (r3) num_cylinders = 8;
    
    var spec1 = document.getElementById("specifier1").value;
    var spec2 = document.getElementById("specifier2").value;
    var spec3 = document.getElementById("specifier3").value;
    var spec4 = document.getElementById("specifier4").value;

    true_open = [
        true_angle_conversion(open[intake_val], spec1, intake_val),
        true_angle_conversion(open[exhaust_val], spec3, exhaust_val)
    ];
    true_close = [
        true_angle_conversion(close[intake_val], spec2, intake_val),
        true_angle_conversion(close[exhaust_val], spec4, exhaust_val)
    ];
    
    calculated_intake_duration = - true_open[intake_val] + true_close[intake_val] + 720;
    calculated_exhaust_duration = - true_open[exhaust_val] + true_close[exhaust_val] + 720;
    
    makeJSON(
        lift_valve[intake_val],
        lift_valve[exhaust_val],
        true_open[intake_val],
        true_open[exhaust_val],
        duration[intake_val],
        duration[exhaust_val],
        num_cylinders,
        fire_order);

    return 0;
}
