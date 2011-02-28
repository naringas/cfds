INTRODUCIÓN
-----------

La intención de éste modulo es la distribución de facturas electrónicas
emitidas. Por eso el nombre cfde = comprobante fiscal digital emitido.

INFORMACIÓN
-----------

El modulo necesita que los usuarios se registren con su RFC como nombre de
usuario. Esto permite que se les pueda dar acceso únicamente a sus CFDs o FEs.
El usuario administrador (uid 1) tiene acceso a todas las facturas.
La sección NOTAS tiene información sobre como validar los nombres de usuario.

Este modulo utiliza el modulo 'Rules' para enviar notificaciones a los usuarios
cuando se crea una nueva Factura asignada al cliente. El modulo provee una
regla predeterminada y una acción de rules. El envio por correo depende del
modulo mimemail http://drupal.org/project/mimemail

El modulo no contempla Comprobantes Fiscales Digitales por Internet.

INSTALACIÓN
-----------

Los requerimientos son:
 El modulo 'cfd'
 el modulo 'Libraries': http://drupal.org/project/libraries
 y el modulo 'Date API': http://drupal.org/project/date

Como parte de la instalación del modulo 'cfd' son necesario algunos archivos
 del SAT (Servicio de Administración Tributaria).

El modulo requiere que TCPDF (http://www.tcpdf.org/) esté
 en 'sites/all/libraries' (queda como: 'sites/all/libraries/tcpdf')

Para el caldendario emergente es necesario descargar datepick (NO datepickER)
 datepick: http://keith-wood.name/datepick.html
 datepick debe ser colocado en 'sites/all/libraries' y debe quedar como
 'sites/all/libraries/datepick' 
Para usar datepick es necesario jquery 1.3 (jquery_update)
 jquery_update: http://drupal.org/project/jquery_update

NOTAS
-----

Las expresiones regulares para validar el RFC usando el modulo
 'Custom Username Validation'
 (http://drupal.org/project/custom_username_validation) son:

  - Todo en mayúsculas:
/^(([A-Z]){3,4})([0-9]{6})((([A-Z]|[0-9]){3}))$/
  
  - O permitiendo minúsculas:
/^(([A-Z]|[a-z]){3,4})([0-9]{6})((([A-Z]|[a-z]|[0-9]){3}))$/

 Como los nombres usuario no distingen entre mayúsculas y minúsculas esto no es
 muy importante.