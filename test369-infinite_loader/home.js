(function() {
  'use strict';

  const throttleMilliseconds = 1000;

  $(function() {
    appendItems();

    // Loading contents just by scrolling to the bottom just feels wrong to me.
    // $(window).on('scroll', _.throttle(scroller.handleScroll, eventThrottleMilliseconds));

    $(window).on('mousewheel wheel', _.throttle(handleScroll, throttleMilliseconds));
    $(window).on('touchstart', _.throttle(handleScroll, throttleMilliseconds));
    $(window).on('touchmove', _.throttle(handleScroll, throttleMilliseconds));
    $(window).on('touchend', _.throttle(handleScroll, throttleMilliseconds));
  });

  var handlingScroll = false;

  function handleScroll() {
    if (!handlingScroll && isClientAtBottom()) {
      try {
        handlingScroll = true;
        appendItems();
      } finally {
        handlingScroll = false;
      }
    }
  }

  function isClientAtBottom() {
    // We need a fuzz because scrollTop() can get fractional values on some
    // environment (like smartphones).
    const fuzz = 1.0;

    return $(window).height() + $(window).scrollTop() > $(document).height() - fuzz;
  }

  function appendItems() {
    const items = $('#items');

    for (let i = 0; i < 10; i++) {
      items.append($('<li>').text('Lorem ipsum'));
    }
  }
})();
