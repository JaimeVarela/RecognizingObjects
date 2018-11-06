# Reconocimiento de objetos por histograma

Este programa se ha creado empleando las librerías OpenCV. Inicialmente se utiliza para comprobar si dos cartones de dos imágenes diferentes resultan ser el mismo cartón, aunque puede emplearse con otras clases de objetos:

![compare](https://user-images.githubusercontent.com/44776831/48085016-791f4980-e1f9-11e8-9202-5aef3d725e38.png)

En el menú principal se pueden elegir las siguientes opciones: guardar una imagen, consultar las imágenes guardadas, borrar la información guardada y salir de la aplicación:

![elige](https://user-images.githubusercontent.com/44776831/48085167-d9ae8680-e1f9-11e8-8764-abb99ea9bed0.png)

1. Guardar una imagen:

1.1. Se introduce la imagen que se quiere guardar (junto con su ruta).

1.2. Se llama a la función CannyThreshold, que se encarga de detectar los bordes dentro de la imagen, creando una máscara con el resultado (el cartón):

![canny](https://user-images.githubusercontent.com/44776831/48085418-7a04ab00-e1fa-11e8-9cce-a50fb1720bcb.png)

1.3. A continuación se halla el histograma de la máscara creada:

![histograma](https://user-images.githubusercontent.com/44776831/48085585-fc8d6a80-e1fa-11e8-944a-eb754d215e90.png)

1.4. Finalmente se compara el histograma de la máscara con los histogramas que el programa tenga guardados empleando el método Bhattacharyya. Si la distancia entre el histograma de la nueva imagen es lo suficientemente lejana de los histogramas guardados, se concluye que el programa no contiene una imagen del cartón, por lo que guardamos la imagen. En caso de que se acerque lo suficiente, se considera que el programa ya contiene una imagen del cartón, por lo que no guarda la nueva imagen:

![compara](https://user-images.githubusercontent.com/44776831/48086006-12e7f600-e1fc-11e8-8d7e-470cecbf3c6a.png)
