var url = window.location.origin

async function files() {

  document.getElementById("response").innerHTML = "Index Files";

  fetch(url+"/FileNames")
  .then(response => {
    if (response.ok) {
      return response.text(); // Convertir la respuesta a texto
    } else {
      throw new Error('Error en la solicitud');
    }
  })
  .then(json_data => {

    var data = JSON.parse(json_data);
// Iterar sobre los elementos del vector y construir la celda de 2 columnas
    console.log(json_data);
    console.log(data);
    var table = document.createElement("table");
    data.forEach(function (element) {
        var row = document.createElement("tr");
        var column1 = document.createElement("td");
        var column2 = document.createElement("td");
        column1.textContent = element; // Primer elemento del array (string)
        column2.innerHTML = `<button type="button" class="btn btn-dark" onclick="download('${element}')">Descargar ${element}</button>`;
        row.appendChild(column1);
        row.appendChild(column2);
        table.appendChild(row);
      });

// Agregar la tabla al elemento HTML donde deseas mostrarla
var container = document.getElementById("response");
container.appendChild(table);
    
  })
  .catch(error => {
    // Manejo de errores en caso de que la solicitud falle
    console.log('Error:', error);
  });


}

async function download(x) {

  fetch(url+"/download", {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({ "resourse": x, "b": 2 })
})
  .then(response => response.blob())
  .then(blob => {
    // Crear un enlace para descargar el archivo
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = x;

    // Agregar el enlace al documento y hacer clic en él para iniciar la descarga
    document.body.appendChild(link);
    link.click();

    // Limpiar el enlace
    document.body.removeChild(link);
    URL.revokeObjectURL(url);
  })
  .catch(error => {
    console.error('Error:', error);
  });

}

