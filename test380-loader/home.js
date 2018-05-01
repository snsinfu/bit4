$(function() {
  'use strict';

  var main = $('#main');
  var itemList = $('<ul>').appendTo(main);
  var statusNote = $('<p>').appendTo(main);
  var currentEnd = 0;

  (function rebind() {
    $(window).one('overscroll', function(event) {
      statusNote.removeClass('error').addClass('loading').text('Loading...');
      loadMore(currentEnd)
        .done(function(items) {
          statusNote.text('OK');
          if (items.length === 0) {
            return;
          }
          items.forEach(function(item) {
            itemList.append($('<li>').text(item.index + ': ' + item.title));
          });
          currentEnd = items[items.length - 1].index + 1;
        })
        .fail(function(xhr) {
          statusNote.addClass('error').text('Load failed: ' + xhr.statusText);
        })
        .always(function() {
          statusNote.removeClass('loading');
          delay(1000, rebind);
        });
    });
  })();

  function loadMore(begin) {
    return $.getJSON('/items?begin=' + begin);
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

  var eventName = 'overscroll';
  var bottomFuzz = 1;
  var counter = 0;
  var counterThreshold = 5;

  $(window).on('swipeUp', function(event) {
    $(window).trigger(eventName);
  });

  $(window).on('wheel', function(event) {
    if (isViewportAtBottom() && event.deltaY > 0) {
      counter++;
      if (counter > counterThreshold) {
        $(window).trigger(eventName);
        counter = 0;
      }
    }
  });

  function isViewportAtBottom(position) {
    return $(window).height() + $(window).scrollTop() + bottomFuzz > $(document).height();
  }
});
