$(function() {
  'use strict';

  var main = $('#main');
  var statusNote = $('<p>').appendTo(main);
  var loadCount = 0;

  (function rebind() {
    $(window).one('overscroll', function(event) {
      loadMore()
        .progress(function() {
          statusNote.addClass('loading').text(loadCount + ' → Loading...');
        })
        .done(function() {
          statusNote.text(loadCount);
        })
        .fail(function(error) {
          statusNote.text(loadCount + ' → Load failed');
          console.error(error);
        })
        .always(function() {
          statusNote.removeClass('loading');
          rebind();
        });
    });
  })();

  function loadMore() {
    return delay(2000, function() {
      if (Math.random() > 0.5) {
        throw new Error();
      }
      loadCount++;
    });
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

// custom event: overscroll ----------------------------------------------------

$(function() {
  'use strict';

  var bottomFuzz = 1;
  var eventName = 'overscroll';

  $(window).on('swipeUp', function(event) {
    $(window).trigger(eventName);
  });

  $(window).on('wheel', function(event) {
    if (isViewportAtBottom() && event.deltaY > 0) {
      $(window).trigger(eventName);
    }
  });

  function isViewportAtBottom(position) {
    return $(window).height() + $(window).scrollTop() + bottomFuzz > $(document).height();
  }
});
