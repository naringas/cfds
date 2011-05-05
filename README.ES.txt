cfd  - funcionalidad en común (PDF, etc)
cfde - cfd emision
cfdr - cfd recepcion

La diferencia entre los modulos cfde y cfdr es que el primero es para la
emision de facturas, por lo tanto incorpora controles de acceso a las facturas
(o nodos tipo cfd) basandose en el nombre de usuario (que se espera sea igual
al RFC del cliente).

El modulo cfdr es para la recepción de facturas, por lo tanto cualquier
usuario con acceso a este modulo puede ver todas las facturas emitidas.
Ademas el modulo cfdr permite buscar la facturas por serie, folio, fecha, rfc
emisor, rfc receptor, mientras que el modulo cfde solo permite busqueda por
fecha.

Sin embargo, la diferencia principla es que el modulo cfde guarda los XML
dentro de la base de datos, pensando en que se utilice en un servidor de una
pagina de internet publica.
Mientras que cfdr gurda una solo una referencia a la ubicacion del archivo XML
que debe existir en el sistema de archivos, pensando en que se utilice en un
servidor interno.

Otra diferencia es que cfde automaticamente borra FEs al rfc generico y purga
los nodos tipo CFD de mas de cinco semenas de antiguedad. Mientras que cfdr
mantiene todos los CFDs.
