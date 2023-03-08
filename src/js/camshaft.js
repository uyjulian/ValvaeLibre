function numCylSelected() {
  let cyl = document.getElementById("num-cyl").value

  if(cyl==0){
    alert("Please select the number of Cylinders.")
  } else if(cyl==1){
    //alert("1 Cylinders Selected")
    listFiringOrders(cyl)

  } else if(cyl==2){
    //alert("2 Cylinders Selected")
    listFiringOrders(cyl)

  } else if(cyl==4){
    //alert("4 Cylinders Selected")
    listFiringOrders(cyl)

  } else{ //if(cyl==8)
    //alert("8 Cylinders Selected")
    listFiringOrders(cyl)
  }
}

function listFiringOrders(cyl) {
  
  // Create Dropdown and Append it to Firing_Order Section
  let firing_order = document.getElementById("firing_order-inputs")
  if(firing_order.childElementCount == 2 ) {
    var selectList = document.getElementById("fireOrderSelect")
    firing_order.removeChild(selectList)
  }
  if( cyl == 1 )
  { 
    var opt_txt = ["1"]
    var opt_val = [[1]]

    var selectList = document.createElement("select")
    selectList.id = "fireOrderSelect"
    firing_order.appendChild(selectList)

    for (var i = 0; i < opt_txt.length; i++) {
        var option = document.createElement("option")
        option.value = opt_val[i]
        option.text = opt_txt[i]
        selectList.appendChild(option)
    }
  } else if( cyl == 4 )
  { 
    var opt_txt = ["1-3-4-2","1-3-2-4","1-4-3-2","1-2-4-3"]
    var opt_val = [[1,3,4,2],[1,3,2,4],[1,4,3,2],[1,2,4,3]]

    var selectList = document.createElement("select")
    selectList.id = "fireOrderSelect"
    firing_order.appendChild(selectList)

    for (var i = 0; i < opt_txt.length; i++) {
        var option = document.createElement("option")
        option.value = opt_val[i]
        option.text = opt_txt[i]
        selectList.appendChild(option)
    }
  } else if ( cyl == 6 ) 
  {
    var opt_txt = ["1-5-3-6-2-4","1-2-4-6-5-3","1-4-6-3-2"]
    var opt_val = [[1,5,3,6,2,4],[1,2,4,6,5,3],[1,4,6,3,2]]

    var selectList = document.createElement("select")
    selectList.id = "fireOrderSelect"
    firing_order.appendChild(selectList)

    for (var i = 0; i < opt_txt.length; i++) {
        var option = document.createElement("option")
        option.value = opt_val[i]
        option.text = opt_txt[i]
        selectList.appendChild(option)
    }
  } else if( cyl == 8 )
  { 
    var opt_txt = ["1-8-4-3-6-5-7-2","1-8-7-2-6-5-4-3","1-5-4-8-6-3-7-2"]
    var opt_val = [[1,8,4,3,6,5,7,2],[1,8,7,2,6,5,4,3],[1,5,4,8,6,3,7,2]]

    var selectList = document.createElement("select")
    selectList.id = "fireOrderSelect"
    firing_order.appendChild(selectList)

    for (var i = 0; i < opt_txt.length; i++) {
        var option = document.createElement("option")
        option.value = opt_val[i]
        option.text = opt_txt[i]
        selectList.appendChild(option)
    }
  }
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

function makeJSON(in_lift, ex_lift, in_open, ex_open, in_duration, ex_duration, num_cylinders, fire_order)
{
  var tmp = fire_order.split(',')

  let fire_order_list = Array()
  
  for(let i=0; i < tmp.length; i++) {
    fire_order_list.push(parseInt(tmp[i]))
  }

  var dict = {
    "intake_valve_lift": in_lift*1.0,
    "exhaust_valve_lift": ex_lift*1.0,
    "intake_valve_opens": +in_open,
    "exhaust_valve_opens": +ex_open,
    "intake_valve_duration": +in_duration,
    "exhaust_valve_duration": +ex_duration,
    "number_of_cylinders": +num_cylinders,
    "firing_order": fire_order_list
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

function storeFunc()
{
  
  valid = document.getElementById("f1").checkValidity();
  if (!valid) return;

  var num_cylinders = document.getElementById("num-cyl").value
  
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

  var open = [
    document.getElementById("in_open").value,
    document.getElementById("ex_open").value
  ];

  var close = [
    document.getElementById("in_close").value,
    document.getElementById("ex_close").value
  ];


  var fire_order = document.getElementById("fireOrderSelect").value;

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
/* 
  alert(lift_valve[intake_val])
  alert(lift_valve[exhaust_val])
  alert(true_open[intake_val])
  alert(true_open[exhaust_val])
  alert(duration[intake_val])
  alert(duration[exhaust_val])
  alert(num_cylinders)
  alert(fire_order)
*/  
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