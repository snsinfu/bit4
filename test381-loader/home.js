$(function() {
  'use strict';

  var main = $('#main');
  var ajaxStatusBox = $('<p>').appendTo(main);
  var uiStatusBox = $('<p>').appendTo(main);
  var wheelCount = 0;
  var wheelThreshold = 20;

  $(window).on('wheels:wheelstart', function(event) {
    uiStatusBox.text('wheelstart');
    wheelCount = 0;
  });

  $(window).on('wheels:wheelend', function(event) {
    uiStatusBox.text('wheelend: ' + event.deltaY);
  });

  $(window).on('wheels:overscroll', function(event) {
    uiStatusBox.text('overscroll: ' + event.deltaY + ' [' + wheelCount + ']');
    if (++wheelCount >= wheelThreshold) {
      $(window).trigger('app:load-more');
      wheelCount = 0;
    }
  });

  (function rebind() {
    $(window).one('app:load-more', function(event) {
      ajaxStatusBox.addClass('loading').text('Loading...');
      loadMore()
        .done(function() {
          ajaxStatusBox.text('OK');
        })
        .always(function() {
          ajaxStatusBox.removeClass('loading');
          rebind();
        });
    });
  })();

  function loadMore() {
    return delay(1500, function() {});
  }

  function delay(timeout, func) {
    var deferred = $.Deferred().notify();
    window.setTimeout(function() {
      try {
        deferred.resolve(func());
      } catch (error) {
        deferred.reject(error);
      }
    }, timeout);
    return deferred.promise();
  }
});

// custom event: wheels:overscroll ---------------------------------------------

$(function() {
  'use strict';

  var bottomFuzz = 1;
  var overscrolling = false;
  var deltaSum = 0;

  $(window).on('wheels:wheelstart', function(event) {
    if (isViewportAtBottom() && event.deltaY > 0) {
      overscrolling = true;
      deltaSum = 0;
    }
  });

  $(window).on('wheels:wheelend', function(event) {
    overscrolling = false;
  });

  $(window).on('wheels:wheelmove', function(event) {
    if (!overscrolling) {
      return;
    }

    deltaSum += event.deltaY;

    if (deltaSum > 0) {
      $(window).trigger($.Event('wheels:overscroll', { deltaY: deltaSum }));
    }
  });

  function isViewportAtBottom() {
    return $(window).height() + $(window).scrollTop() + bottomFuzz > $(document).height();
  }
});

// custom event: wheelstart/end ------------------------------------------------

$(function() {
  'use strict';

  var wheelTimeoutDuration = 500;
  var wheelTimeout = null;
  var deltaSum = 0;

  $(window).on('wheel', function(event) {
    if (!wheelTimeout) {
      $(window).trigger($.Event('wheels:wheelstart', event));
    } else {
      window.clearTimeout(wheelTimeout);
    }

    wheelTimeout = window.setTimeout(function() {
      $(window).trigger($.Event('wheels:wheelend', { deltaY: deltaSum }));
      wheelTimeout = null;
      deltaSum = 0;
    }, wheelTimeoutDuration)

    deltaSum += event.deltaY;

    $(window).trigger($.Event('wheels:wheelmove', event));
  });
});
