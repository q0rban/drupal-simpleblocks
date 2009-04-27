// $Id
$(function() {
  $('#edit-simpleblocks-title-enabled').change(function() {
    if (this.checked) {
      $('#block-admin-configure #edit-title').val('').removeAttr("disabled");
    }
    else {
      $('#block-admin-configure #edit-title').val('<none>').attr("disabled", true);
    }
  });

  $('#edit-simpleblocks-status').change(function() {
    if (this.checked) {
      $('#block-admin-configure input, #block-admin-configure textarea').removeAttr("disabled");
    }
    else {
      $('#block-admin-configure input, #block-admin-configure textarea').not('#edit-submit, #edit-simpleblocks-status').attr("disabled", true);
    }
  });

  // Simulate a change so we can disable the title field if need be.
  $('#edit-simpleblocks-title-enabled, #edit-simpleblocks-status').change();
});