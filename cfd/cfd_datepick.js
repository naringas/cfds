/** @file
 * adds datepick
 */

$(document).ready(function() {
  $("#edit-start, #edit-end").datepick({
    minDate: '-10w',
    maxDate: '+0',
    pickerClass: 'noPrevNext',
    showOtherMonths: true,
    selectOtherMonths: true,
    dateFormat: $.datepick.ISO_8601,
    showAnim: 'fadeIn',
    showSpeed: 'fast',
    showOnFocus: true,
    onSelect: function (dates) {
      if (this.id == 'edit-start') {
        $('#edit-end').datepick('option', 'minDate', dates[0] || null);
      }
      else {
        $('#edit-start').datepick('option', 'maxDate', dates[0] || null);
      }
    }
  });
  //onSelect like functinality on load (for when filter is in $_SESSION)
  var start = $("#edit-start").datepick('getDate')[0];
  var end = $("#edit-end").datepick('getDate')[0];
  if (start != undefined) {
    $('#edit-end').datepick('option', 'minDate', start);
  }
  if (end != undefined) {
    $('#edit-start').datepick('option', 'maxDate', end);
  }
});
