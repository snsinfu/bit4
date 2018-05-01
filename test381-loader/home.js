$(function() {
  'use strict';

  var main = $('#main');
  var ajaxStatusBox = $('<p>').appendTo(main);
  var uiStatusBox = $('<p>').appendTo(main);

  $(window).on('wheels:wheelstart', function(event) {
    uiStatusBox.text('wheelstart');
  });

  $(window).on('wheels:wheelend', function(event) {
    uiStatusBox.text('wheelend: ' + event.deltaY);
  });

  (function rebind() {
    $(window).one('overscroll', function(event) {
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

// custom event: wheelstart wheelend -------------------------------------------

$(function() {
  'use strict';

  var bottomFuzz = 1;

  $(window).on('wheels:wheelstart', function(event) {
    if (isViewportAtBottom() && event.deltaY > 0) {
      $(window).trigger('overscroll');
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
  var deltaY = 0;

  $(window).on('wheel', function(event) {
    if (!wheelTimeout) {
      $(window).trigger($.Event('wheels:wheelstart', event));
    } else {
      window.clearTimeout(wheelTimeout);
    }

    wheelTimeout = window.setTimeout(function() {
      $(window).trigger($.Event('wheels:wheelend', { deltaY: deltaY }));
      wheelTimeout = null;
      deltaY = 0;
    }, wheelTimeoutDuration)

    deltaY += event.deltaY;
  });
});
