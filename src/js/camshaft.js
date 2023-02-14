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
  let style_span = document.createElement("span")
  let new_dropdown = document.createElement("select")
  style_span.className = "dropdown-style2"
  new_dropdown.name = "fire-order"
  style_span.append(new_dropdown)
  firing_order.append(style_span)

  // Add options to the dropdown
  
}