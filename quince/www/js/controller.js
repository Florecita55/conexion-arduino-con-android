angular.module("controller", [])

.controller("ctrl", function($scope, $timeout){
    //bluetoothSerial = bluetoothSerial == undefined? { discoverUnpaired: function(next, error){ next({name: "x-JHI"}); }}: bluetoothSerial;
    
    $scope.destinos = ["Catedral", "Museo de arte prehispanico Rufino Tamayo",
                      "Iglesia la Soledad", "Iglesia Santo Domingo", "Jardin Etnobotanico",
                      "Paseo Juarez el Llano", "Mercado 20 de noviembre"];
    
    $scope.data = {
        titulo: "Buscando dispositivos",
        estado: 1,
        buscando: false
    };
    var primera = true;
    
    $scope.destino = {
        
    };
    
    $scope.mensajes = ["Primer mensaje"];
    /*
    1 - Sin conectar
    2 - Conectado
    3 - Mapa cargado
    */
    $scope.dispositivos = false;
    
    
    $scope.load = function(){
    
        $scope.data.buscando = true;
        if(typeof bluetoothSerial === "undefined"){
            alert("No esta el puglin");
            bluetoothSerial = { discoverUnpaired: function(next, error){ next([{name: "x-JHI"}]); }}
        }else{
            //alert("Si esta");
        }

        bluetoothSerial.discoverUnpaired(function(dispositivos){
            // funcion cuando encuentra dispositivos
            $scope.$apply(function(){
                $scope.dispositivos = dispositivos;
                $scope.data.titulo = "Conectando"
            });
        }, function(err){
            // funcion si ocurre un error en la busqueda de dispositivos
            swal("Upps","Ocurrio un error: "+JSON.stringify(err),"error");
        });
    
    }
    
    $scope.volver = function(){
        bluetoothSerial.unsubscribe();
        primera = false;
        $scope.data = {
            titulo: "Buscando dispositivos",
            estado: 1,
            buscando: false
        };
        $scope.dispositivos = false;
        $scope.load();
    };
    
    
    $scope.conectar = function(dispositivo){
        bluetoothSerial.connect(dispositivo.address, function(){
            swal("Conectado", "La conexion fue exitosa", "success");
            
            //if(primera){
            bluetoothSerial.subscribe("\n", function(mensaje){
                var datos = mensaje.split(":");
                
                $scope.$apply(function(){
                    $scope.destino.clave = datos[0];
                    $scope.destino.id = parseInt(datos[1]);
                    $scope.destino.nombre = $scope.destinos[$scope.destino.id];
                    $scope.data.titulo = $scope.destino.nombre;
                    $scope.mensajes.push(mensaje);
                });
                
                setTimeout(function(){
                    bluetoothSerial.write("x", function(){
                        swal("Liberado", "La estacion ha sido liberada", "success");
                        $scope.$apply(function(){
                            $scope.mensajes.push("Liberando dispositivo");
                        });
                    }, function(){
                        swal("Error", "No pude liberar la estacion", "error");
                        $scope.$apply(function(){
                            $scope.mensajes.push("Hubo un error cuando le pedi el codigo");
                        });
                    });
                }, 3000);
                
            });
            //}
            // jjj
            $scope.$apply(function(){
                $scope.data.estado = 2;
            });
            bluetoothSerial.write("a", function(){
                $scope.$apply(function(){
                    $scope.mensajes.push("Ya le mande a pedir el codigo");
                });
            }, function(){
                $scope.$apply(function(){
                    $scope.mensajes.push("Hubo un error cuando le pedi el codigo");
                });
            });
        },function(){
            // funcion si ocurre un error en la busqueda de dispositivos
            swal("Upps","Ocurrio un error: "+JSON.stringify(err),"error");
        });
    };
});